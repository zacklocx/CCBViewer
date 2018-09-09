
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

	const jvalue_t& jquery(const jvalue_t& val, const std::string& query)
	{
		std::string s;
		std::vector<std::string> sv;
		std::istringstream ss(query);

		while(std::getline(ss, s, '.'))
		{
			sv.push_back(s);
		}

		const jvalue_t* target = &val;

		for(const auto& it : sv)
		{
			if(target->isArray())
			{
				target = &(*target)[std::stoi(it)];
			}
			else
			{
				target = &(*target)[it];
			}

			if(target->isNull())
			{
				break;
			}
		}

		return *target;
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

jvalue_t jget(const jvalue_t& val, const std::string& query)
{
	return jquery(val, query);
}

void jset(jvalue_t& val, const std::string& query, const jvalue_t& v)
{
	jvalue_t& target = const_cast<jvalue_t&>(jquery(val, query));

	if(!target.isNull())
	{
		target = v;
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
		ret = std::stol(val.asString());
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
