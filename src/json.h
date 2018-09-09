
#ifndef JSON_INCLUDED
#define JSON_INCLUDED

#include <cstdint>

#include <json/json.h>

#include "type.h"

using jtype = Json::Value;

using jref = jtype&;
using cjref = const jtype&;

bool jload(csref path, jref jval);
bool jsave(csref path, cjref jval);

jtype jparse(csref str);
stype jdump(cjref jval, bool indent);

jtype jquery(cjref jval, csref query);
jtype jupdate(jref jval, csref query, cjref new_jval);

bool jtob(cjref jval);
stype jtos(cjref jval);
int jtoi(cjref jval);
unsigned int jtou(cjref jval);
int64_t jtoi64(cjref jval);
uint64_t jtou64(cjref jval);
float jtof(cjref jval);
double jtod(cjref jval);

#endif /* JSON_INCLUDED */
