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
#include "function_code.h"
#include "common.h"
#include "ptype.h"
#include "check.h"

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

void set_requester_my_id(requester_t *requester, int id)
{
  assert(requester != NULL);
  fstart("requester: %p, id: %d", requester, id);

  requester->mid = (uint8_t) id;

  ffinish();
}

void set_requester_peer_id(requester_t *requester, int id)
{
  assert(requester != NULL);
  fstart("requester: %p, id: %d", requester, id);

  requester->pid = (uint8_t) id;

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

int send_request(requester_t *requester)
{
  assert(requester != NULL);
  fstart("requester: %p", requester);
  int sent, len, v;
  uint8_t *p;

  p = requester->buf;
  *(p++) = (uint8_t) requester->pid;
  *(p++) = FC_READ_HOLDING_REGISTERS;
  v = 1000;
  VAR_TO_PTR_2BYTES(v, p);
  v = 22;
  VAR_TO_PTR_2BYTES(v, p);

  len = p - requester->buf + 2;
  insert_checksum(requester->buf, len);
  iprint(SMART_DEBUG_REQUESTER, "request message", (requester->buf), 0, len, 16);
  sent = write(requester->fd, requester->buf, len);
  imsg(SMART_DEBUG_REQUESTER, "sent %d bytes for the requester message at %d", sent, requester->fd);

  ffinish("sent: %d", sent);
  return sent;
}

int receive_response(requester_t *requester)
{
  assert(requester != NULL);
  fstart("requester: %p", requester);
  int rcvd;
  rcvd = read(requester->fd, &(requester->buf), MAX_BUF_LEN);

  ffinish("rcvd: %d", rcvd);
  return rcvd;
}
