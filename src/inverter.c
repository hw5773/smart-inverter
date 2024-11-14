#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <errno.h>
#include <getopt.h>
#include <assert.h>
#include <stdint.h>

#include <sys/time.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <smart/debug.h>
#include <smart/inverter.h>
#include "inverter_internal.h"
#include "check.h"

int usage(const char *pname)
{
  emsg(">> Usage: %s [options]", pname);
  emsg("Options");
  emsg("  -b, --baudrate    Baud rate");
  emsg("  -s, --serial      Serial port");
  emsg("  -i, --id          Inverter ID");
  exit(1);
}

int dtype;
int main(int argc, char *argv[])
{   
  const char *pname, *serial, *opt;
	int c, rc;
  int id, len, baud_rate;
  inverter_t *inverter;

  dtype = SMART_DEBUG_INVERTER;
  pname = argv[0];
  serial = NULL;
  id = -1;
  baud_rate = -1;

  while (1)
  {
    int opt_idx = 0;
    static struct option long_options[] = {
      {"baudrate", required_argument, 0, 'b'},
      {"serial", required_argument, 0, 's'},
      {"id", required_argument, 0, 'i'},
      {0, 0, 0, 0}
    };

    opt = "s:d:i:0";

    c = getopt_long(argc, argv, opt, long_options, &opt_idx);

    if (c == -1)
      break;

    switch (c)
    {
      case 'b':
        baud_rate = atoi(optarg);
        rc = check_baud_rate(baud_rate);
        if (rc < 0)
          usage(pname);
        break;
 
      case 's':
        serial = optarg;
        rc = check_serial(serial);
        if (rc < 0)
          usage(pname);
        break;
 
      case 'i':
        id = atoi(optarg);
        rc = check_id(id);
        if (rc < 0)
          usage(pname);
        break;

      default:
        usage(pname);
    }
  }

  imsg(SMART_DEBUG_INVERTER, "Start the inverter application (ID: %d)", id);
  imsg(SMART_DEBUG_INVERTER, "Communication at %s with %d baud rate", serial, baud_rate);

  inverter = init_inverter(serial);

  while (1)
  {
    len = receive_request(inverter);
    if (len > 0)
      send_response(inverter);
  }

	return 0;
}


