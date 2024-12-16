#ifndef __REQUESTER_H__
#define __REQUESTER_H__

#include <smart/types.h>

requester_t *init_requester(const char *serial);
void free_requester(requester_t *requester);
void set_requester_my_id(requester_t *requester, int id);
void set_requester_peer_id(requester_t *requester, int id);
void set_requester_baud_rate(requester_t *requester, int baud_rate);

int send_request(requester_t *requester);
int receive_response(requester_t *requester);

#endif /* __REQUESTER_H__ */
