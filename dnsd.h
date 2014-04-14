#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <assert.h>

#ifndef DNSD_H
#define DNSD_H 1

#define VERSION "0.0.1"

#define LOG(fmt, var0) fprintf(stderr, ("%s:%d\t" fmt), __FILE__, __LINE__, (var0))
#define LOG1(fmt, var0) fprintf(stderr, ("%s:%d\t" fmt), __FILE__, __LINE__, (var0))
#define LOG2(fmt, var0, var1) fprintf(stderr, ("%s:%d\t" fmt), __FILE__, __LINE__, (var0), (var1))
#define LOG3(fmt, var0, var1, var2) fprintf(stderr, ("%s:%d\t" fmt), __FILE__, __LINE__, (var0), (var1), (var2))

#define INET_ATON_SUCCESS 1

// See http://www.faqs.org/rfcs/rfc1035.html
// Section 2.3.4. Size limits
#define MAX_PACKET_SIZE 512

typedef struct {
  // configuration values
  int port;
  char* bind_address;

  // runtime values
  int socket;

  /**
   * The number of read errors on the socket.
   * When we reach a threshold, the socket shall be closed and reopened.
   */
  unsigned int nerrors;
} server_state;

server_state* new_dnsd(int argc, char** argv);
void init(server_state* sstate);
void run(server_state* sstate);
void free_dnsd(server_state* sstate);

#endif
