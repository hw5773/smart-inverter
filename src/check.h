#ifndef __CHECK_H__
#define __CHECK_H__

#include <stdint.h>

int check_code(int code);
int check_interval(int interval);
int check_number(int num);

int check_baud_rate(int baud_rate);
int check_serial(const char *serial);
int check_id(int id);
int check_command(int cmd);

void insert_checksum(uint8_t *buf, int len);
int verify_checksum(uint8_t *buf, int len);

#endif /* __CHECK_H__ */
