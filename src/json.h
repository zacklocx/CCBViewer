
#ifndef JSON_INCLUDED
#define JSON_INCLUDED

#include <cstdint>

#include <string>

#include <json/json.h>

#define jnull Json::nullValue
#define jint Json::intValue
#define juint Json::uintValue
#define jreal Json::realValue
#define jstr Json::stringValue
#define jbool Json::booleanValue
#define jarr Json::arrayValue
#define jobj Json::objectValue

#define jnull_e Json::ValueType::nullValue
#define jint_e Json::ValueType::intValue
#define juint_e Json::ValueType::uintValue
#define jreal_e Json::ValueType::realValue
#define jstr_e Json::ValueType::stringValue
#define jbool_e Json::ValueType::booleanValue
#define jarr_e Json::ValueType::arrayValue
#define jobj_e Json::ValueType::objectValue

using jvalue_t = Json::Value;

bool jload(const std::string& path, jvalue_t& val);
bool jsave(const std::string& path, const jvalue_t& val);

jvalue_t jparse(const std::string& str);
std::string jdump(const jvalue_t& val);

bool jquery(jvalue_t& val, const std::string& query, jvalue_t*& target);

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
