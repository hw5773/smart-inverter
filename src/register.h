#ifndef __REGISTER_H__ 
#define __REGISTER_H__

#define OFFSET 1000

typedef struct reg_st {
  short reg[23];
} reg_t;

reg_t *init_registers(void);
void free_registers(reg_t *reg);
void set_register_value_by_addr(reg_t *reg, int addr, short value);
short get_register_value_by_addr(reg_t *reg, int addr);

#endif /* __REGISTER_H__ */
