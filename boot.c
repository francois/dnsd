#include "dnsd.h"
#include <sys/socket.h>

server_state* new_dnsd(int argc, char** argv) {
	int i;
	DLOG("Received %d arguments:\n", argc);
	for(i = 0; i < argc; i++) {
		DLOG2("\t%i: %s\n", i, argv[i]);
	}

	server_state* sstate;
	sstate = calloc(1, sizeof(server_state));
	if( !sstate ) return sstate;

	// assign defaults
	sstate->port = 2353;

	// parse options and assign values

	return sstate;
}

void init(server_state* sstate) {
	assert( sstate );

	// read zone file / data to serve

	LOG("Opening socket on port %d\n", sstate->port);
	sstate->socket = socket( PF_INET, SOCK_DGRAM, -1 );
	if( !sstate->socket ) {
		LOG2("Failed to open socket on port %d (errno: %d)\n", sstate->port, errno);
		free_dnsd(sstate);
		exit(2);
	}
}

void free_dnsd(server_state* sstate) {
	assert( sstate );

	close(sstate->socket);
	free(sstate);
}
