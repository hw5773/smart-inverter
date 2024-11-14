#ifndef __INVERTER_INTERNAL_H__
#define __INVERTER_INTERNAL_H__

#include <smart/types.h>

struct inverter_st {
  int id;
  int fd;
  reg_t *reg;
};

#endif /* __INVERTER_INTERNAL_H__ */
