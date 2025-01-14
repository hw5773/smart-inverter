#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <resolv.h>
#include <netdb.h>
#include <errno.h>
#include <limits.h>
#include <getopt.h>
#include <assert.h>

#include <sys/time.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <smart/debug.h>
#include <smart/types.h>
#include <smart/requester.h>
#include "check.h"

int usage(const char *pname)
{
  emsg(">> Usage: %s [options]", pname);
  emsg("Options");
  emsg("  -b, --baudrate  Baud Rate");
  emsg("  -c, --code      Function Code");
  emsg("  -m, --mid       my ID");
  emsg("  -p, --pid       peer ID");
  emsg("  -i, --interval  Interval");
  emsg("  -n, --number    Number");
  emsg("  -s, --serial    Serial");
  exit(1);
}

int dtype;
int main(int argc, char *argv[])
{   
  const char *pname, *opt, *serial;
	int c, rc, running, code, mid, pid, interval, num, baud_rate, loop, len;
  time_t start, prev, curr;
  requester_t *requester;

  dtype = SMART_DEBUG_REQUESTER;
  pname = argv[0];

  serial = NULL;
  mid = -1;
  pid = -1;
  baud_rate = -1;

  while (1)
  {
    int opt_idx = 0;
    static struct option long_options[] = {
      {"baudrate", required_argument, 0, 'b'},
      {"code", required_argument, 0, 'c'},
      {"mid", required_argument, 0, 'm'},
      {"pid", required_argument, 0, 'p'},
      {"interval", required_argument, 0, 'i'},
      {"number", required_argument, 0, 'n'},
      {"serial", required_argument, 0, 's'},
      {0, 0, 0, 0}
    };

    opt = "b:c:i:m:n:p:s:0";

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

      case 'c':
        code = atoi(optarg);
        rc = check_code(code);
        if (rc < 0)
          usage(pname);
        break;

      case 'm':
        mid = atoi(optarg);
        rc = check_id(mid);
        if (rc < 0)
          usage(pname);
        break;

      case 'p':
        pid = atoi(optarg);
        rc = check_id(pid);
        if (rc < 0)
          usage(pname);
        break;

      case 'i':
        interval = atoi(optarg);
        rc = check_interval(interval);
        if (rc < 0)
          usage(pname);
        break;

      case 'n':
        num = atoi(optarg);
        rc = check_number(num);
        if (rc < 0)
          usage(pname);
        break;

      case 's':
        serial = optarg;
        rc = check_serial(serial);
        if (rc < 0)
          usage(pname);
        break;

      default:
        usage(pname);
    }
  }



  imsg(SMART_DEBUG_REQUESTER, "Start the requester application (ID: %d)", mid);
  imsg(SMART_DEBUG_REQUESTER, "Communication at %s with %d baud rate", serial, baud_rate);

  requester = init_requester(serial);
  set_requester_my_id(requester, mid);
  set_requester_peer_id(requester, pid);
  set_requester_baud_rate(requester, baud_rate);

  loop = 0;
  running = 1;
  start = time(NULL);
  prev = start;
  while (running)
  {
    curr = time(NULL);
    if (curr > prev + interval)
    {
      len = send_request(requester);
      if (len > 0)
      {
        sleep(0.5);
        len = receive_response(requester);
        if (len > 0)
          loop += 1;
      }
      prev = curr;
    }

    if (loop == num)
      running = 0;
  }

	return 0;
}
