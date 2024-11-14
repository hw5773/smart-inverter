#ifndef __REQUESTER_H__
#define __REQUESTER_H__

#include <smart/types.h>

requester_t *init_requester(const char *serial);
void free_requester(requester_t *requester);

int send_request(requester_t *requester);
int receive_response(requester_t *requester);

#endif /* __REQUESTER_H__ */
