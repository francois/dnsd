#include "dnsd.h"
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/time.h>

static volatile int done = 0;

static void
initiate_shutdown(int sig) {
	done = 1;
}

static void
log_received_query( ssize_t received, char* buffer, struct sockaddr_in* sender ) {
	int i, j;

	LOG3("Received %ld bytes from %s:%d\n", received, inet_ntoa(sender->sin_addr), ntohs(sender->sin_port));
	for(i = 0; i <= (received >> 3); i++) {
		int maxj = (received - (i << 3)) > 8 ? 8 : (received - (i << 3));

		printf("%04x\t", i << 3);

		for(j = 0; j < maxj; j++) {
			printf("%02x ", 0x000000ff & buffer[(i << 3) + j]);
		}

		if( maxj < 4 ) putc('\t', stdout);
		if( maxj < 8 ) putc('\t', stdout);
		putc('\t', stdout);
		for(j = 0; j < maxj; j++) {
			char c = buffer[(i << 3) + j];
			if(! ( c >= 32 && c < 127 ) ) c = '.';
			printf("%c", c);
		}

		putc('\n', stdout);
	}

	fflush(stdout);

	/* NOTE: The packet is in network-byte order, of big-endian.
	 * Intel uses little-endian, so we have to convert.
	 * This is what the calls to ntohs do here.
	 *
	 * @see https://en.wikipedia.org/wiki/Network_byte_order#Endianness_in_networking
	 */
	HEADER* header = (HEADER*)buffer;
	LOG1("id:\t%u\n", ntohs(header->id));
	LOG1("qr:\t%s\n", header->qr ? "question" : "answer");
	LOG1("opcode:\t%s\n", opcode_to_string(header->opcode));
	LOG1("aa:\t%s\n", header->aa ? "true" : "false");
	LOG1("tc:\t%s\n", header->tc ? "true" : "false");
	LOG1("rd:\t%s\n", header->rd ? "true" : "false");
	LOG1("ra:\t%s\n", header->ra ? "true" : "false");
	LOG1("z:\t%d\n", header->z);
	LOG1("rcode:\t%s\n", rcode_to_string(header->rcode));
	LOG1("qdcount:\t%d\n", ntohs(header->qdcount));
	LOG1("ancount:\t%d\n", ntohs(header->ancount));
	LOG1("nscount:\t%d\n", ntohs(header->nscount));
	LOG1("arcount:\t%d\n", ntohs(header->arcount));
}

void process_request(server_state* sstate) {
	assert( sstate );

	LOG1("Accepting request on port %d\n", sstate->port);

	ssize_t received;
	char buffer[MAX_PACKET_SIZE];
	struct sockaddr_in sender;
	socklen_t socklen = sizeof( struct sockaddr_in );

	received = recvfrom( sstate->socket, buffer, sizeof(buffer), 0 /* flags */, (struct sockaddr*)&sender, &socklen );
	if( received == -1 ) {
		LOG2("Failed to read from socket %d (errno: %d)\n", sstate->port, errno);
		sstate->nerrors += 1;
		return;
	}

	// Reset number of errors counter, to indicate we should continue
	sstate->nerrors = 0;

	log_received_query( received, buffer, &sender );
	HEADER* header = (HEADER*)buffer;

	char outbuf[MAX_PACKET_SIZE];
	memset(outbuf, 0, MAX_PACKET_SIZE); // initialize all to zero
	HEADER* out = (HEADER*)outbuf;
	out->id = header->id;
	out->qr = 1; // answer
	out->opcode = QUERY;
	out->aa = 0;
	out->tc = 0;
	out->rd = 0;
	out->ra = 0;
	out->z = 0;
	out->rcode = FORMAT_ERROR;
	out->qdcount = 0;
	out->ancount = 0;
	out->nscount = 0;
	out->arcount = 0;

	int rval;
	int len = (((char*)&out->arcount) + 2) - outbuf;
	rval = sendto( sstate->socket, outbuf, len, 0, (struct sockaddr*)&sender, socklen );
	if( rval == -1 ) {
		LOG2("Failed to send response to %s: (errno %d)\n", inet_ntoa(sender.sin_addr), errno);
	} else if( len == rval ) {
		LOG2("Sent %d bytes back to %s\n", len, inet_ntoa(sender.sin_addr));
	} else {
		LOG3("Expected to send %d bytes to %s; actually sent %d\n", len, inet_ntoa(sender.sin_addr), rval);
	}
}


void run(server_state* sstate) {
	assert( sstate );

	// Handle SIGTERM, SIGINT and SIGQUIT (Ctrl+C) to initiate shutdown
	sig_t prevterm, prevint, prevquit;
	prevterm = signal(SIGTERM, &initiate_shutdown);
	prevint  = signal(SIGINT, &initiate_shutdown);
	prevquit = signal(SIGQUIT, &initiate_shutdown);

	while( ! done ) {
		fd_set readable;
		fd_set errored;

		FD_ZERO(&readable);
		FD_ZERO(&errored);

		FD_SET( sstate->socket, &readable );
		FD_SET( sstate->socket, &errored );

		struct timeval timeout = { 1 /* second */, 0 };
		int ready;
		ready = select( 1 + (int)sstate->socket, &readable, NULL, &errored, &timeout );
		if( ready == 0 ) {
#if DEBUG
			LOG1("Nothing ready on port %d\n", sstate->port);
#endif
			continue;
		}

		if( FD_ISSET( sstate->socket, &errored ) ) {
			// We have an error on this socket - should we continue?
			LOG1("DNSD on port %d failed\n", sstate->port);
			free_dnsd( sstate );
			exit(1);
		}

		if( FD_ISSET( sstate->socket, &readable ) ) {
			process_request( sstate );
		}

		if( sstate->nerrors > 5 ) {
			LOG2("%d errors in a row on port %d: aborting...\n", sstate->nerrors, sstate->port);
			// TODO: close socket, flag as unopened
			// TODO: On each loop init, check if socket is absent and open if it is
			free_dnsd( sstate );
			exit(1);
		}
	}

	// Reinstall initial handlers
	signal(SIGQUIT, prevquit);
	signal(SIGINT, prevint);
	signal(SIGTERM, prevterm);
}
