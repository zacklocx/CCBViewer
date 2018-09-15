
#include "json.h"

#include <vector>
#include <memory>
#include <sstream>
#include <fstream>
#include <iostream>

namespace
{
	bool jread(std::istream& in, jvalue_t& val)
	{
		Json::CharReaderBuilder builder;
		builder["collectComments"] = false;

		JSONCPP_STRING err;
		return parseFromStream(builder, in, &val, &err);
	}

	bool jwrite(std::ostream& out, const jvalue_t& val)
	{
		Json::StreamWriterBuilder builder;
		builder["commentStyle"] = "None";
		builder["indentation"] = "\t";

		std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
		return writer->write(val, &out);
	}
}

bool jload(const std::string& path, jvalue_t& val)
{
	std::ifstream f(path);
	return jread(f, val);
}

bool jsave(const std::string& path, const jvalue_t& val)
{
	std::ofstream f(path);
	return jwrite(f, val);
}

jvalue_t jparse(const std::string& str)
{
	std::istringstream ss(str);
	jvalue_t val;
	jread(ss, val);

	return val;
}

std::string jdump(const jvalue_t& val)
{
	std::ostringstream ss;
	jwrite(ss, val);

	return ss.str();
}

bool jquery(jvalue_t& val, const std::string& query, jvalue_t*& target)
{
	bool ret = true;
	target = &val;

	std::string s;
	std::vector<std::string> sv;
	std::istringstream ss(query);

	while(std::getline(ss, s, '.'))
	{
		if(s.length() > 0)
		{
			sv.push_back(s);
		}
	}

	for(const auto& key : sv)
	{
		if(target->isObject())
		{
			target = (ret = target->isMember(key))? &(*target)[key] : nullptr;
		}
		else if(target->isArray())
		{
			Json::ArrayIndex index = std::stoi(key);
			target = (ret = target->isValidIndex(index))? &(*target)[index] : nullptr;
		}
		else
		{
			ret = false;
			target = nullptr;
		}

		if(!ret)
		{
			break;
		}
	}

	return ret;
}

jvalue_t jget(const jvalue_t& val, const std::string& query)
{
	jvalue_t* target;
	return jquery(const_cast<jvalue_t&>(val), query, target)? *target : jnull;
}

void jset(jvalue_t& val, const std::string& query, const jvalue_t& v)
{
	jvalue_t* target;

	if(jquery(val, query, target))
	{
		*target = v;
	}
}

bool jtob(const jvalue_t& val)
{
	bool ret = false;

	if(val.isBool())
	{
		ret = val.asBool();
	}
	else if(val.isIntegral())
	{
		ret = val.asInt() != 0;
	}

	return ret;
}

std::string jtos(const jvalue_t& val)
{
	std::string ret = "";

	if(val.isBool() || val.isString())
	{
		ret = val.asString();
	}
	else if(val.isInt())
	{
		ret = std::to_string(val.asInt64());
	}
	else if(val.isUInt())
	{
		ret = std::to_string(val.asUInt64());
	}
	else if(val.isDouble())
	{
		ret = std::to_string(val.asDouble());
	}

	return ret;
}

int jtoi(const jvalue_t& val)
{
	int ret = 0;

	if(val.isString())
	{
		ret = std::stoi(val.asString());
	}
	else if(val.isInt())
	{
		ret = val.asInt();
	}

	return ret;
}

unsigned int jtou(const jvalue_t& val)
{
	unsigned int ret = 0;

	if(val.isString())
	{
		ret = std::stoul(val.asString());
	}
	else if(val.isUInt())
	{
		ret = val.asUInt();
	}

	return ret;
}

int64_t jtoi64(const jvalue_t& val)
{
	int64_t ret = 0;

	if(val.isString())
	{
		ret = std::stoll(val.asString());
	}
	else if(val.isInt())
	{
		ret = val.asInt64();
	}

	return ret;
}

uint64_t jtou64(const jvalue_t& val)
{
	uint64_t ret = 0;

	if(val.isString())
	{
		ret = std::stoull(val.asString());
	}
	else if(val.isUInt())
	{
		ret = val.asUInt64();
	}

	return ret;
}

float jtof(const jvalue_t& val)
{
	float ret = 0.0f;

	if(val.isString())
	{
		ret = std::stof(val.asString());
	}
	else if(val.isDouble())
	{
		ret = val.asFloat();
	}

	return ret;
}

double jtod(const jvalue_t& val)
{
	double ret = 0.0;

	if(val.isString())
	{
		ret = std::stod(val.asString());
	}
	else if(val.isDouble())
	{
		ret = val.asDouble();
	}

	return ret;
}
