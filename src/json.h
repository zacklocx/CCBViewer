
#ifndef JSON_INCLUDED
#define JSON_INCLUDED

#include <cstdint>

#include <string>

#include <json/json.h>

using jvalue = Json::Value;

bool jload(const std::string& path, jvalue& jval);
bool jsave(const std::string& path, const jvalue& jval);

jvalue jparse(const std::string& str);
std::string jdump(const jvalue& jval, bool indent);

jvalue jquery(const jvalue& jval, const std::string& query);
jvalue jupdate(jvalue& jval, const std::string& query, const jvalue& new_jval);

bool jtob(const jvalue& jval);
std::string jtos(const jvalue& jval);
int jtoi(const jvalue& jval);
unsigned int jtou(const jvalue& jval);
int64_t jtoi64(const jvalue& jval);
uint64_t jtou64(const jvalue& jval);
float jtof(const jvalue& jval);
double jtod(const jvalue& jval);

#endif /* JSON_INCLUDED */
