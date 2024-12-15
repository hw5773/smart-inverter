#include <assert.h>
#include <unistd.h>
#include <stdlib.h>
#include <termios.h>
#include <errno.h>
#include <string.h>

#include <smart/debug.h>
#include <smart/inverter.h>

#include "inverter_internal.h"
#include "common.h"

inverter_t *init_inverter(const char *serial)
{
  assert(serial != NULL);
  fstart("serial: %s", serial);
  
  inverter_t *ret;
  ret = (inverter_t *)calloc(1, sizeof(inverter_t));
  if (!ret) goto err;

  ret->fd = open_serial_port(serial);
  if (ret->fd < 0) goto err;

err:
  ffinish("ret: %p", ret);
  return ret;
}

void free_inverter(inverter_t *inverter)
{
  assert(inverter != NULL);
  fstart("inverter: %p", inverter);

  if (inverter)
  {
    close(inverter->fd);
    free(inverter);
  }

  ffinish();
}

void set_inverter_id(inverter_t *inverter, int id)
{
  assert(inverter != NULL);
  fstart("inverter: %p, id: %d", inverter, id);

  inverter->id = id;

  ffinish();
}

void set_inverter_baud_rate(inverter_t *inverter, int baud_rate)
{
  assert(inverter != NULL);
  assert(baud_rate > 0);
  fstart("inverter: %p, baud_rate: %d", inverter, baud_rate);

  struct termios tty;
  if (tcgetattr(inverter->fd, &tty))
  {
    emsg("Error %i from tcgetattr: %s", errno, strerror(errno));
    goto err;
  }

  cfsetispeed(&tty, baud_rate);
  cfsetospeed(&tty, baud_rate);

  if (tcsetattr(inverter->fd, TCSANOW, &tty))
  {
    emsg("Error %i from tcsetattr: %s", errno, strerror(errno));
    goto err;
  }

err:
  ffinish();
}
