#ifndef __INVERTER_H__
#define __INVERTER_H__

#include <smart/types.h>

inverter_t *init_inverter(const char *serial);
void free_inverter(inverter_t *inverter);

int receive_request(inverter_t *inverter);
int send_response(inverter_t *inverter);

#endif /* __INVERTER_H__ */
