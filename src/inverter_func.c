#include <assert.h>
#include <unistd.h>
#include <stdlib.h>
#include <termios.h>
#include <errno.h>
#include <string.h>

#include <smart/debug.h>
#include <smart/inverter.h>

#include "inverter_internal.h"
#include "check.h"
#include "common.h"
#include "command.h"
#include "ptype.h"
#include "function_code.h"

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

  switch (baud_rate)
  {
    case 9600:
      cfsetispeed(&tty, B9600);
      cfsetospeed(&tty, B9600);
      break;

    case 38400:
      cfsetispeed(&tty, B38400);
      cfsetospeed(&tty, B38400);
      break;

    case 115200:
      cfsetispeed(&tty, B115200);
      cfsetospeed(&tty, B115200);
      break;

    default:
      cfsetispeed(&tty, B115200);
      cfsetospeed(&tty, B115200);
  }

  if (tcsetattr(inverter->fd, TCSANOW, &tty))
  {
    emsg("Error %i from tcsetattr: %s", errno, strerror(errno));
    goto err;
  }

err:
  ffinish();
}

int receive_request(inverter_t *inverter)
{
  assert(inverter != NULL);
  fstart("inverter: %p", inverter);
  int rcvd;
  rcvd = read(inverter->fd, &(inverter->buf), MAX_BUF_LEN);
  if (rcvd > 0)
  {
    iprint(SMART_DEBUG_INVERTER, "request message", (inverter->buf), 0, rcvd, 16);
    imsg(SMART_DEBUG_INVERTER, "received %d bytes at %d", rcvd, inverter->fd);

    if (verify_checksum(inverter->buf, rcvd))
    {
      imsg(SMART_DEBUG_INVERTER, "No checksum error");
    }
    else
    {
      emsg("Checksum error");
      rcvd = -1;
    }
  }

  ffinish("rcvd: %d", rcvd);
  return rcvd;
}

int send_response(inverter_t *inverter, int cmd)
{
  assert(inverter != NULL);
  fstart("inverter: %p", inverter);
  int sent, len, v;
  uint8_t *p;

  p = inverter->buf;
  *(p++) = inverter->id;
  *(p++) = FC_READ_HOLDING_REGISTERS;

  *(p++) = 0x2c;

  // 1001 - 1002
  v = 0;
  VAR_TO_PTR_4BYTES(v, p);

  // 1003 - 1004
  v = 0;
  VAR_TO_PTR_4BYTES(v, p);

  // 1005 - 1006
  v = 0;
  VAR_TO_PTR_4BYTES(v, p);

  // 1007 - 1008
  v = 3764;
  if (cmd == SMART_COMMAND_INVALID_VAL)
  {
    v = 0;
  }
  VAR_TO_PTR_4BYTES(v, p);

  // 1009 - 1010
  v = 3775;
  VAR_TO_PTR_4BYTES(v, p);

  // 1011 - 1012
  v = 3776;
  VAR_TO_PTR_4BYTES(v, p);

  // 1013 - 1014
  v = 0;
  VAR_TO_PTR_4BYTES(v, p);

  // 1015 - 1016
  v = 0;
  VAR_TO_PTR_4BYTES(v, p);

  // 1017 - 1018
  v = 0;
  VAR_TO_PTR_4BYTES(v, p);

  // 1019 - 1020
  *(p++) = 0x00;
  *(p++) = 0x00;
  *(p++) = 0x02;
  *(p++) = 0x58;

  // 1021 - 1022
  *(p++) = 0x00;
  *(p++) = 0x00;
  *(p++) = 0x00;
  *(p++) = 0x22;

  len = p - inverter->buf;
  len += 2;
  if (cmd == SMART_COMMAND_NORMAL)
    insert_checksum(inverter->buf, len);
  sent = write(inverter->fd, inverter->buf, len);
  iprint(SMART_DEBUG_INVERTER, "response message", (inverter->buf), 0, sent, 16);
  imsg(SMART_DEBUG_INVERTER, "sent %d bytes at %d", sent, inverter->fd);

  ffinish("sent: %d", sent);
  return sent;
}
