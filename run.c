#include "dnsd.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/time.h>

static volatile int done = 0;

static void
initiate_shutdown(int sig) {
	done = 1;
}

void process_request(server_state* sstate) {
	assert( sstate );

	LOG("Accepting request on port %d\n", sstate->port);

	ssize_t received;
	void* buffer[1];
	struct sockaddr_in sender;
	socklen_t socklen;

	received = recvfrom( sstate->socket, buffer, sizeof(buffer), 0 /* flags */, (struct sockaddr*)&sender, &socklen );
	if( received == -1 ) {
		LOG2("Failed to read from socket %d (errno: %d)\n", sstate->port, errno);
		sstate->nerrors += 1;
		return;
	}

	// Reset number of errors counter, to indicate we should continue
	sstate->nerrors = 0;

	LOG3("Received %u bytes from %s:%d\n", (unsigned int)received, inet_ntoa(sender.sin_addr), ntohs(sender.sin_port));
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
			LOG("Nothing ready on port %d\n", sstate->port);
			continue;
		}

		if( FD_ISSET( sstate->socket, &errored ) ) {
			// We have an error on this socket - should we continue?
			LOG("DNSD on port %d failed\n", sstate->port);
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
