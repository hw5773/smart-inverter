#include "register.h"

reg_t *init_registers()
{
  reg_t *ret;
  ret = (reg_t *)calloc(1, sizeof(reg_t));

  return ret;
}

void free_registers(reg_t *reg)
{
  if (reg)
  {
    free(reg);
  }
}

void set_register_value_by_addr(reg_t *reg, int addr, short value)
{
  assert(reg != NULL);
  assert(addr > 1000 && addr < 1023);
  fstart("reg: %p, addr: %d, value: %d", reg, addr, value);
  int a;
  a = addr - OFFSET;
  reg->reg[a] = value;
  ffinish();
}

short get_register_value_by_addr(reg_t *reg, int addr)
{
  assert(reg != NULL);
  assert(addr > 1000 && addr < 1023);
  fstart("reg: %p, addr: %d", reg, addr);
  int a;
  short ret;
  a = addr - OFFSET;
  ret = reg->reg[a];
  ffinish("ret: %d", ret);
  return ret;
}
