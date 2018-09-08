
#ifndef UTILS_INCLUDED
#define UTILS_INCLUDED

#include <cstdint>

#include <string>

uint64_t timestamp_s();
uint64_t timestamp_ms();

std::string now();
std::string what_time(uint64_t ts);

std::string md5(const std::string& s);
std::string urlencode(const std::string& s);

#endif // UTILS_INCLUDED
