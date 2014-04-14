#include "dnsd.h"

char* opcode_to_string(OPCODE opcode){
	static char* query   = "query";
	static char* iquery  = "iquery";
	static char* status  = "status";
	static char* unknown = "unknown";

	switch(opcode){
		case QUERY:  return query;
		case IQUERY: return iquery;
		case STATUS: return status;
		default:     return unknown;
	}
}

char* rcode_to_string(RCODE rcode){
	static char* no_error              = "no error";
	static char* format_error          = "format error";
	static char* server_failure        = "server failure";
	static char* name_error            = "name error";
	static char* not_implemented_error = "not implemented error";
	static char* refused               = "refused";
	static char* unknown               = "unknown";

	switch(rcode){
		case NO_ERROR:              return no_error;
		case FORMAT_ERROR:          return format_error;
		case SERVER_FAILURE:        return server_failure;
		case NAME_ERROR:            return name_error;
		case NOT_IMPLEMENTED_ERROR: return not_implemented_error;
		case REFUSED:               return refused;
		default:                    return unknown;
	}
}
