#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#ifndef DNSD_H
#define DNSD_H 1

#define VERSION "0.0.1"

#define LOG(fmt, var) fprintf(stderr, (fmt), (var))
#define DLOG(fmt, var0) fprintf(stderr, (fmt), (var0))
#define DLOG2(fmt, var0, var1) fprintf(stderr, (fmt), (var0), (var1))

typedef struct {
} server_state;

server_state* new_dnsd(int argc, char** argv);
void init(server_state* sstate);
void run(server_state* sstate);
void free_dnsd(server_state* sstate);

#endif
