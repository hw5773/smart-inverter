#ifndef __INVERTER_INTERNAL_H__
#define __INVERTER_INTERNAL_H__

#include <smart/inverter.h>
#include <smart/defines.h>

struct inverter_st {
  int id;
  int fd;
  reg_t *reg;
  uint8_t buf[MAX_BUF_LEN];
};

#endif /* __INVERTER_INTERNAL_H__ */
