#ifndef __REQUESTER_INTERNAL_H__
#define __REQUESTER_INTERNAL_H__

#include <stdint.h>

#include <smart/requester.h>
#include <smart/defines.h>

struct requester_st {
  int id;
  int fd;
  uint8_t buf[MAX_BUF_LEN];
};

#endif /* __INVERTER_INTERNAL_H__ */
