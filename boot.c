#include "dnsd.h"
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <fcntl.h>

server_state* new_dnsd(int argc, char** argv) {
	int i;
	LOG("Received %d arguments:\n", argc);
	for(i = 0; i < argc; i++) {
		LOG2("\t%i: %s\n", i, argv[i]);
	}

	server_state* sstate;
	sstate = calloc(1, sizeof(server_state));
	if( !sstate ) return sstate;

	// assign defaults
	sstate->port = 2353;
	sstate->bind_address = "0.0.0.0";

	// parse options and assign values

	return sstate;
}

void init(server_state* sstate) {
	assert( sstate );

	// read zone file / data to serve

	LOG("Opening socket on port %d\n", sstate->port);
	sstate->socket = socket( PF_INET, SOCK_DGRAM, 0 );
	if( !sstate->socket ) {
		LOG("Failed to create socket (errno: %d)\n", errno);
		free_dnsd(sstate);
		exit(2);
	}

	struct sockaddr_in address;
	memset( &address, 0, sizeof(address) );
	address.sin_family = PF_INET;
	address.sin_port = htons(sstate->port);
	if ( ! inet_aton(sstate->bind_address, &address.sin_addr) ) {
		LOG2("Failed to parse [%s] (errno: %d)\n", sstate->bind_address, errno);
		free_dnsd(sstate);
		exit(2);
	}

	if( -1 == bind( sstate->socket, (struct sockaddr *)&address, sizeof(address) ) ) {
		LOG2("Failed to bind socket on port %d (errno: %d)\n", sstate->port, errno);
		free_dnsd(sstate);
		exit(2);
	}

	LOG2("DNSD ready to accept requests on %s:%d\n", sstate->bind_address, sstate->port);
}

void free_dnsd(server_state* sstate) {
	assert( sstate );

	LOG("Closing down socket on port %d\n", sstate->port);
	close(sstate->socket);

	LOG("DNSD on port %d is down\n", sstate->port);
	free(sstate);
}
