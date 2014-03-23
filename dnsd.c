#include <stdio.h>
#include <stdlib.h>

#define VERSION "0.0.1"

#define LOG(fmt, var) fprintf(stderr, (fmt), (var))
#define DLOG(fmt, var0) fprintf(stderr, (fmt), (var0))
#define DLOG2(fmt, var0, var1) fprintf(stderr, (fmt), (var0), (var1))

typedef struct {
} server_state;

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
}

void run(server_state* sstate) {
}

void free_dnsd(server_state* sstate) {
	free(sstate);
}

int main(int argc, char** argv) {
	server_state* sstate;

	LOG("Booting dnsd v%s...\n", VERSION);

	sstate = new_dnsd(argc, argv);
	init(sstate);
	run(sstate);
	free_dnsd(sstate);

	exit(0);
}
