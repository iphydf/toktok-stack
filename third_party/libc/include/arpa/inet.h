#pragma once

#include "../types.h"

const char *inet_ntop(int af, const void *__restrict src, char *__restrict dst,
                      __socklen_t size);
int inet_pton(int af, const char *__restrict src, void *__restrict dst);

__uint32_t htonl(__uint32_t hostlong);
__uint16_t htons(__uint16_t hostshort);

__uint32_t ntohl(__uint32_t netlong);
__uint16_t ntohs(__uint16_t netshort);
