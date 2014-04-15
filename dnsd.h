#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <assert.h>

#ifndef DNSD_H
#define DNSD_H 1

#define VERSION "0.0.1"

#define LOG(fmt) fprintf(stderr, ("%s:%d\t" fmt), __FILE__, __LINE__)
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

typedef enum {
  A     = 1,   // a host address
  NS    = 2,   // an authoritative name server
  MD    = 3,   // a mail destination (Obsolete - use MX)
  MF    = 4,   // a mail forwarder (Obsolete - use MX)
  CNAME = 5,   // the canonical name for an alias
  SOA   = 6,   // marks the start of a zone of authority
  MB    = 7,   // a mailbox domain name (EXPERIMENTAL)
  MG    = 8,   // a mail group member (EXPERIMENTAL)
  MR    = 9,   // a mail rename domain name (EXPERIMENTAL)
  /* NULL  = 10,  // a null RR (EXPERIMENTAL) */
  WKS   = 11,  // a well known service description
  PTR   = 12,  // a domain name pointer
  HINFO = 13,  // host information
  MINFO = 14,  // mailbox or mail list information
  MX    = 15,  // mail exchange
  TXT   = 16,  // text strings
  XFR   = 252, // A request for a transfer of an entire zone
  MAILB = 253, // A request for mailbox-related records (MB, MG or MR)
  MAILA = 254, // A request for mail agent RRs (Obsolete - see MX)

  QTYPE_ANY = 255, // A request for all records
} QTYPE;

typedef enum {
  IN  = 1,   // the Internet
  CS  = 2,   // the CSNET class (Obsolete - used only for examples in
  CH  = 3,   // the CHAOS class
  HS  = 4,   // Hesiod [Dyer 87]

  CLASS_ANY = 255, // any class
} CLASS;

typedef enum {
  QUERY  = 0, // a standard query
  IQUERY = 1, // an inverse query
  STATUS = 2, // a server status request
  // 3 to 15 are reserved for future use
} OPCODE;

typedef enum {
  NO_ERROR              =  0,
  FORMAT_ERROR          =  1,
  SERVER_FAILURE        =  2,
  NAME_ERROR            =  3,
  NOT_IMPLEMENTED_ERROR =  4,
  REFUSED               =  5,
  // 6-15: Reserved for future use.
} RCODE;

typedef struct {
  // octets 0 and 1
  unsigned short id;       // query id; copied from query to answer

  // octets 2 and 3
  unsigned short qr:1;     // 0 == query, 1 == response
  unsigned short opcode:4; // operation code, see OPCODE
  unsigned short aa:1;     // authoritative answer (bool)
  unsigned short tc:1;     // truncation (bool)
  unsigned short rd:1;     // recursion desired (bool)
  unsigned short ra:1;     // recursion available (bool)
  unsigned short z:3;      // reserved for future use (must be zero)
  unsigned short rcode:4;  // response code, see RCODE

  // octets 3 and 4
  unsigned short qdcount;  // number of entries in the question section

  // octets 5 and 6
  unsigned short ancount;  // number of entries in the answer section

  // octets 7 and 8
  unsigned short nscount;  // number of entries in the name server section

  // octets 9 and 10
  unsigned short arcount;  // number of entries in the additional records section
} HEADER;

char* opcode_to_string(OPCODE);
char* rcode_to_string(RCODE);

#endif
