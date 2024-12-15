#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stddef.h>
#include <stdlib.h>
#include <termios.h>

#include <smart/debug.h>

#include "requester.h"
#include "requester_internal.h"
#include "common.h"

requester_t *init_requester(const char *serial)
{
  assert(serial != NULL);
  fstart("serial: %s", serial);

  requester_t *ret;
  ret = (requester_t *)calloc(1, sizeof(requester_t));
  if (!ret) goto err;

  ret->fd = open_serial_port(serial);
  if (ret->fd < 0) goto err;

err:
  ffinish("ret: %p", ret);
  return ret;
}

void free_requester(requester_t *requester)
{
  assert(requester != NULL);
  fstart("requester: %p", requester);

  if (requester)
  {
    close(requester->fd);
    free(requester);
  }

  ffinish();
}

void set_requester_id(requester_t *requester, int id)
{
  assert(requester != NULL);
  fstart("requester: %p, id: %d", requester, id);

  requester->id = id;

  ffinish();
}

void set_requester_baud_rate(requester_t *requester, int baud_rate)
{
  assert(requester != NULL);
  assert(baud_rate > 0);
  fstart("requester: %p, baud_rate: %d", requester, baud_rate);

  struct termios tty;
  if (tcgetattr(requester->fd, &tty))
  {
    emsg("Error %i from tcgetattr: %s", errno, strerror(errno));
    goto err;
  }

  cfsetispeed(&tty, baud_rate);
  cfsetospeed(&tty, baud_rate);

  if (tcsetattr(requester->fd, TCSANOW, &tty))
  {
    emsg("Error %i from tcsetattr: %s", errno, strerror(errno));
    goto err;
  }

err:
  ffinish();
}
