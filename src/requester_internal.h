#ifndef __REQUESTER_INTERNAL_H__
#define __REQUESTER_INTERNAL_H__

#include <stdint.h>

#include <smart/requester.h>
#include <smart/defines.h>

struct requester_st {
  uint8_t mid;
  uint8_t pid;
  int fd;
  uint8_t buf[MAX_BUF_LEN];
};

#endif /* __INVERTER_INTERNAL_H__ */
