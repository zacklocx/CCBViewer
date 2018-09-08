
#ifndef JSON_INCLUDED
#define JSON_INCLUDED

#include <cstdint>

#include <string>

#include <json/json.h>

bool jparse(const std::string& jstr, Json::Value& jval, bool jexc = false);
Json::Value jquery(const Json::Value& jval, const std::string& query);

bool jtob(const Json::Value& jval);
std::string jtos(const Json::Value& jval);
int jtoi(const Json::Value& jval);
unsigned int jtou(const Json::Value& jval);
int64_t jtoi64(const Json::Value& jval);
uint64_t jtou64(const Json::Value& jval);
float jtof(const Json::Value& jval);
double jtod(const Json::Value& jval);

#endif /* JSON_INCLUDED */
