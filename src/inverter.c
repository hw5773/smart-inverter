#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <errno.h>
#include <getopt.h>
#include <assert.h>

#include <sys/time.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <smart/debug.h>
#include <smart/inverter.h>
#include "inverter_internal.h"

int usage(const char *pname)
{
  emsg(">> Usage: %s [options]", pname);
  emsg("Options");
  emsg("  -b, --baudrate    Baud rate");
  emsg("  -d, --device      Serial port");
  emsg("  -i, --id          Inverter ID");
  exit(1);
}

int dtype;
int main(int argc, char *argv[])
{   
  const char *device;
	int c, fd;
  int id, baud_rate;
  inverter_t *inverter;

  dtype = SMART_DEBUG_INVERTER;
  pname = argv[0];
  device = NULL;
  id = -1;
  baud_rate = -1;

  while (1)
  {
    int opt_idx = 0;
    static struct option long_options[] = {
      {"baudrate", required_argument, 0, 'b'},
      {"device", required_argument, 0, 'd'},
      {"id", required_argument, 0, 'i'},
      {0, 0, 0, 0}
    };

    opt = "d:i:0";

    c = getopt_long(argc, argv, opt, long_options, &opt_idx);

    if (c == -1)
      break;

    switch (c)
    {
      case 'b':
        baud_rate = atoi(optarg);
        check_baud_rate(baud_rate);
        break;
 
      case 'd':
        device = atoi(optarg);
        check_device(device);
        break;
 
      case 'i':
        id = atoi(optarg);
        check_id(id);
        break;

      default:
        usage(pname);
    }
  }

  imsg(SMART_DEBUG_INVERTER, "Start the inverter application (ID: %d)", id);
  imsg(SMART_DEBUG_INVERTER, "Communication at %s with %d baud rate", device, baud_rate);

  

  while (1)
  {
    len = receive_request(fd, buf, MAX_BUF_LEN);
    if (len > 0)
      process_request(fd, buf, len);
  }

	return 0;
}


