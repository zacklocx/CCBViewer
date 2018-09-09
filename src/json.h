
#ifndef JSON_INCLUDED
#define JSON_INCLUDED

#include <cstdint>

#include <string>

#include <json/json.h>

using jvalue_t = Json::Value;

bool jload(const std::string& path, jvalue_t& val);
bool jsave(const std::string& path, const jvalue_t& val);

jvalue_t jparse(const std::string& str);
std::string jdump(const jvalue_t& val);

jvalue_t jget(const jvalue_t& val, const std::string& query);
void jset(jvalue_t& val, const std::string& query, const jvalue_t& v);

bool jtob(const jvalue_t& val);
std::string jtos(const jvalue_t& val);
int jtoi(const jvalue_t& val);
unsigned int jtou(const jvalue_t& val);
int64_t jtoi64(const jvalue_t& val);
uint64_t jtou64(const jvalue_t& val);
float jtof(const jvalue_t& val);
double jtod(const jvalue_t& val);

#endif /* JSON_INCLUDED */
