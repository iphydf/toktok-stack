#pragma once

#include "../sys/socket.h"

struct ifreq {
    struct sockaddr ifr_broadaddr;
};

struct ifconf {
    char *ifc_buf;
    int ifc_len;
};

#define SIOCGIFCONF 0
#define SIOCGIFBRDADDR 1
