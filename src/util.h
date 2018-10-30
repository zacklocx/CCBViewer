
#ifndef UTIL_INCLUDED
#define UTIL_INCLUDED

#include <cstdint>

#include <string>

uint64_t timestamp_s();
uint64_t timestamp_ms();

std::string now();

std::string ttos(uint64_t ts);
uint64_t stot(const std::string& s);

int rand_int(int from, int to);
double rand_real(double from, double to);

std::string md5(const std::string& s);
std::string urlencode(const std::string& s);

#endif // UTIL_INCLUDED
