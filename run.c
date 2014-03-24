#include "dnsd.h"
#include <sys/socket.h>

static int done = 0;

static void
initiate_shutdown(int sig) {
	done = 1;
}

void run(server_state* sstate) {
	assert( sstate );

	// Handle SIGTERM (Ctrl+C) to initiate a shutdown
	sig_t prev;
	prev = signal(SIGTERM, &initiate_shutdown);

	// struct sockaddr address;
	// while( accept( sstate->socket, &address, sizeof(struct sockaddr) ) ) {
	// }

	// while accept
	//	serve

	while( ! done ) { }

	// Reinstall initial handler
	signal(SIGTERM, prev);
}
