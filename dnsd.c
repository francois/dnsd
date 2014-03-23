#include "dnsd.h"

int main(int argc, char** argv) {
	server_state* sstate;

	LOG("Booting dnsd v%s...\n", VERSION);

	sstate = new_dnsd(argc, argv);
	if( ! sstate ) {
		LOG("Failed to allocate memory; cannot continue (errno: %d)\n", ENOMEM);
		exit(2);
	}

	init(sstate);
	run(sstate);
	free_dnsd(sstate);

	exit(0);
}
