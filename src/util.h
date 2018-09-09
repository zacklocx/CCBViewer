
#ifndef UTIL_INCLUDED
#define UTIL_INCLUDED

#include <cstdint>

#include "type.h"

uint64_t timestamp_s();
uint64_t timestamp_ms();

std::string now();
std::string what_time(uint64_t ts);

std::string md5(csref s);
std::string urlencode(csref s);

#endif // UTIL_INCLUDED
