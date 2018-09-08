
#include "json.h"

#include <vector>
#include <sstream>
#include <exception>
#include <iostream>

bool jparse(const std::string& jstr, Json::Value& jval, bool jexc /* = false */)
{
	Json::CharReaderBuilder builder;
	builder["collectComments"] = false;

	JSONCPP_STRING jerr;
	Json::CharReader* reader = builder.newCharReader();

	bool ret = reader->parse(jstr.data(), jstr.data() + jstr.size(), &jval, &jerr);

	if(jexc && jerr.length() > 0)
	{
		throw std::runtime_error(jerr);
	}

	return ret;
}

Json::Value jquery(const Json::Value& jval, const std::string& query)
{
	std::string s;
	std::vector<std::string> sv;
	std::istringstream ss(query);

	while(std::getline(ss, s, '.'))
	{
		sv.push_back(s);
	}

	const Json::Value* ret = &jval;

	for(const auto& it : sv)
	{
		if(ret->isArray())
		{
			ret = &(*ret)[std::stoi(it)];
		}
		else
		{
			ret = &(*ret)[it];
		}

		if(ret->isNull())
		{
			break;
		}
	}

	return *ret;
}

bool jtob(const Json::Value& jval)
{
	bool ret = false;

	if(jval.isBool())
	{
		ret = jval.asBool();
	}
	else if(jval.isIntegral())
	{
		ret = jval.asInt() != 0;
	}

	return ret;
}

std::string jtos(const Json::Value& jval)
{
	std::string ret = "";

	if(jval.isString())
	{
		ret = jval.asString();
	}
	else if(jval.isInt())
	{
		ret = std::to_string(jval.asInt64());
	}
	else if(jval.isUInt())
	{
		ret = std::to_string(jval.asUInt64());
	}
	else if(jval.isDouble())
	{
		ret = std::to_string(jval.asDouble());
	}

	return ret;
}

int jtoi(const Json::Value& jval)
{
	int ret = 0;

	if(jval.isString())
	{
		ret = std::stoi(jval.asString());
	}
	else if(jval.isInt())
	{
		ret = jval.asInt();
	}

	return ret;
}

unsigned int jtou(const Json::Value& jval)
{
	unsigned int ret = 0;

	if(jval.isString())
	{
		ret = std::stoul(jval.asString());
	}
	else if(jval.isUInt())
	{
		ret = jval.asUInt();
	}

	return ret;
}

int64_t jtoi64(const Json::Value& jval)
{
	int64_t ret = 0;

	if(jval.isString())
	{
		ret = std::stoll(jval.asString());
	}
	else if(jval.isInt())
	{
		ret = jval.asInt64();
	}

	return ret;
}

uint64_t jtou64(const Json::Value& jval)
{
	uint64_t ret = 0;

	if(jval.isString())
	{
		ret = std::stoull(jval.asString());
	}
	else if(jval.isUInt())
	{
		ret = jval.asUInt64();
	}

	return ret;
}

float jtof(const Json::Value& jval)
{
	float ret = 0.0f;

	if(jval.isString())
	{
		ret = std::stof(jval.asString());
	}
	else if(jval.isDouble())
	{
		ret = jval.asFloat();
	}

	return ret;
}

double jtod(const Json::Value& jval)
{
	double ret = 0.0;

	if(jval.isString())
	{
		ret = std::stod(jval.asString());
	}
	else if(jval.isDouble())
	{
		ret = jval.asDouble();
	}

	return ret;
}
