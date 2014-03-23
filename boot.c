#include "dnsd.h"

server_state* new_dnsd(int argc, char** argv) {
	int i;
	DLOG("Received %d arguments:\n", argc);
	for(i = 0; i < argc; i++) {
		DLOG2("\t%i: %s\n", i, argv[i]);
	}

	server_state* sstate;
	sstate = calloc(1, sizeof(server_state));
	return sstate;
}

void init(server_state* sstate) {
	// read zone file / data to serve
	// open socket
}

void free_dnsd(server_state* sstate) {
	free(sstate);
}
