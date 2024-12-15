#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <unistd.h>

#include <smart/debug.h>

#include "common.h"

int open_serial_port(const char *serial)
{
  assert(serial != NULL);
  fstart("serial: %s", serial);
  
  int ret;
  struct termios tty;

  ret = open(serial, O_RDWR);
  if (ret < 0) goto err;

  if (tcgetattr(ret, &tty))
  {
    emsg("Error %i from tcgetattr: %s", errno, strerror(errno));
    goto err;
  }

  tty.c_cflag &= ~PARENB;
  tty.c_cflag &= ~CSTOPB;
  tty.c_cflag &= ~CSIZE;
  tty.c_cflag |= CS8;
  tty.c_cflag &= ~CRTSCTS;
  tty.c_cflag |= CREAD | CLOCAL;

  tty.c_lflag &= ~ICANON;
  tty.c_lflag &= ~ECHO;
  tty.c_lflag &= ~ECHOE;
  tty.c_lflag &= ~ECHONL;
  tty.c_lflag &= ~ISIG;
  tty.c_iflag &= ~(IXON | IXOFF | IXANY);
  tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL);

  tty.c_oflag &= ~OPOST;
  tty.c_oflag &= ~ONLCR;

  tty.c_cc[VTIME] = 10;
  tty.c_cc[VMIN] = 0;

  if (tcsetattr(ret, TCSANOW, &tty))
  {
    emsg("Error %i from tcsetattr: %s", errno, strerror(errno));
    goto err;
  }

err:
  ffinish("ret: %p", ret);
  return ret;

}
