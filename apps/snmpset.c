/*
 * snmpset.c - send snmp SET requests to a network entity.
 *
 */
/***********************************************************************
	Copyright 1988, 1989, 1991, 1992 by Carnegie Mellon University

                      All Rights Reserved

Permission to use, copy, modify, and distribute this software and its 
documentation for any purpose and without fee is hereby granted, 
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in 
supporting documentation, and that the name of CMU not be
used in advertising or publicity pertaining to distribution of the
software without specific, written prior permission.  

CMU DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
CMU BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
SOFTWARE.
******************************************************************/
#include <config.h>

#if HAVE_STDLIB_H
#include <stdlib.h>
#endif
#if HAVE_UNISTD_H
#include <unistd.h>
#endif
#if HAVE_STRING_H
#include <string.h>
#else
#include <strings.h>
#endif
#include <sys/types.h>
#if HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif
#include <stdio.h>
#include <ctype.h>
#if TIME_WITH_SYS_TIME
# ifdef WIN32
#  include <sys/timeb.h>
# else
#  include <sys/time.h>
# endif
# include <time.h>
#else
# if HAVE_SYS_TIME_H
#  include <sys/time.h>
# else
#  include <time.h>
# endif
#endif
#if HAVE_SYS_SELECT_H
#include <sys/select.h>
#endif
#if HAVE_WINSOCK_H
#include <winsock.h>
#endif
#if HAVE_NETDB_H
#include <netdb.h>
#endif
#if HAVE_ARPA_INET_H
#include <arpa/inet.h>
#endif

#include "asn1.h"
#include "snmp_api.h"
#include "snmp_impl.h"
#include "snmp_client.h"
#include "mib.h"
#include "snmp.h"
#include "system.h"
#include "snmp_parse_args.h"
#include "int64.h"

int failures = 0;

void usage(void)
{
  fprintf(stderr,"Usage: snmpset ");
  snmp_parse_args_usage(stderr);
  fprintf(stderr," [<objectID> <type> <value> ...]\n\n");
  snmp_parse_args_descriptions(stderr);
  fprintf(stderr, 
    "  type - one of i, u, t, a, o, s, x, d, b, n\n");
  fprintf(stderr,
    "    i: INTEGER, u: unsigned INTEGER, t: TIMETICKS, a: IPADDRESS\n");
  fprintf(stderr,
    "    o: OBJID, s: STRING, x: HEX STRING, d: DECIMAL STRING, b: BITS\n");
#ifdef OPAQUE_SPECIAL_TYPES
  fprintf(stderr,
    "    U: unsigned int64, I: signed int64, F: float, D: double\n");
#endif /* OPAQUE_SPECIAL_TYPES */

}

int main(int argc, char *argv[])
{
    struct snmp_session session, *ss;
    struct snmp_pdu *pdu, *response = NULL;
    struct variable_list *vars;
    int arg;
    int count;
    int current_name = 0;
    int current_type = 0;
    int current_value = 0;
    char *names[128];
    char types[128];
    char *values[128];
    oid name[MAX_OID_LEN];
    size_t name_length;
    int status;
    int exitval = 0;

    putenv(strdup("POSIXLY_CORRECT=1"));

    /* get the common command line arguments */
    switch (arg = snmp_parse_args(argc, argv, &session, NULL, NULL)) {
    case -2:
    	exit(0);
    case -1:
        usage();
        exit(1);
    default:
        break;
    }

    if (arg >= argc) {
      fprintf(stderr, "Missing object name\n");
      usage();
      exit(1);
    }

    /* get object names, types, and values */
    for(; arg < argc; arg++){
      names[current_name++] = argv[arg++];
      if (arg < argc) {
        switch(*argv[arg]){
	case '=':
        case 'i':
        case 'u':
        case 't':
        case 'a':
        case 'o':
        case 's':
        case 'x':
        case 'd':
	case 'b':
#ifdef OPAQUE_SPECIAL_TYPES
        case 'I':
        case 'U':
        case 'F':
        case 'D':
#endif /* OPAQUE_SPECIAL_TYPES */
          types[current_type++] = *argv[arg++];
          break;
        default:
          fprintf(stderr, "%s: Bad object type: %c\n", argv[arg-1], *argv[arg]);
          exit(1);
        }
      } else {
        fprintf(stderr, "%s: Needs type and value\n", argv[arg-1]);
        exit(1);
      }
      if (arg < argc)
        values[current_value++] = argv[arg];
      else {
        fprintf(stderr, "%s: Needs value\n", argv[arg-2]);
        exit(1);
      }
    }

    SOCK_STARTUP;

    /* open an SNMP session */
    ss = snmp_open(&session);
    if (ss == NULL){
      /* diagnose snmp_open errors with the input struct snmp_session pointer */
      snmp_sess_perror("snmpset", &session);
	  SOCK_CLEANUP;
      exit(1);
    }

    /* create PDU for SET request and add object names and values to request */
    pdu = snmp_pdu_create(SNMP_MSG_SET);
    for(count = 0; count < current_name; count++){
      name_length = MAX_OID_LEN;
      if (snmp_parse_oid(names[count], name, &name_length) == NULL) {
        snmp_perror(names[count]);
        failures++;
      } else
        if (snmp_add_var(pdu, name, name_length, types[count], values[count])) {
          snmp_perror(names[count]);
          failures++;
        }
    }

    if (failures) {
      SOCK_CLEANUP;
      exit(1);
    }

    /* do the request */
    status = snmp_synch_response(ss, pdu, &response);
    if (status == STAT_SUCCESS){
      if (response->errstat == SNMP_ERR_NOERROR){
        for(vars = response->variables; vars; vars = vars->next_variable)
          print_variable(vars->name, vars->name_length, vars);
        } else {
          fprintf(stderr, "Error in packet.\nReason: %s\n",
                 snmp_errstring(response->errstat));
          if (response->errindex != 0){
            fprintf(stderr, "Failed object: ");
            for(count = 1, vars = response->variables;
                  vars && (count != response->errindex);
                  vars = vars->next_variable, count++)
              ;
            if (vars)
              fprint_objid(stderr, vars->name, vars->name_length);
            fprintf(stderr, "\n");
          }
	  exitval = 2;
        }
      } else if (status == STAT_TIMEOUT){
        fprintf(stderr,"Timeout: No Response from %s\n", session.peername);
	exitval = 1;
      } else {    /* status == STAT_ERROR */
        snmp_sess_perror("snmpset", ss);
	exitval = 1;
    }

    if (response)
      snmp_free_pdu(response);
    snmp_close(ss);
    SOCK_CLEANUP;
    return exitval;
}
