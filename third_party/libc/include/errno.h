#pragma once

#include "types.h"

enum {
  EBADF = 9,
  EAGAIN = 11,
  EWOULDBLOCK = EAGAIN,
  EINPROGRESS = 115,
};

extern int errno;

const char *strerror_r(int errnum, char *buf, __size_t buflen);
