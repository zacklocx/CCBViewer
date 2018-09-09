
#include "json.h"

#include <vector>
#include <memory>
#include <sstream>
#include <fstream>

bool jload(const std::string& path, jvalue& jval)
{
	std::ifstream file(path);

	bool ret = !file.fail();

	if(ret)
	{
		Json::CharReaderBuilder builder;
		builder["collectComments"] = false;

		JSONCPP_STRING err;
		ret = parseFromStream(builder, file, &jval, &err);
	}

	return ret;
}

bool jsave(const std::string& path, const jvalue& jval)
{
	std::ofstream file(path);

	bool ret = !file.fail();

	if(ret)
	{
		Json::StreamWriterBuilder builder;
		builder["commentStyle"] = "None";
		builder["indentation"] = "";

		std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
		ret = writer->write(jval, &file);
	}

	return ret;
}

jvalue jparse(const std::string& str)
{
	jvalue ret;

	Json::CharReaderBuilder builder;
	builder["collectComments"] = false;

	JSONCPP_STRING err;
	std::unique_ptr<Json::CharReader> const reader(builder.newCharReader());
	reader->parse(str.data(), str.data() + str.size(), &ret, &err);

	return ret;
}

std::string jdump(const jvalue& jval, bool indent)
{
	std::ostringstream ss;

	Json::StreamWriterBuilder builder;
	builder["commentStyle"] = "None";
	builder["indentation"] = indent? "\t" : "";

	std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
	writer->write(jval, &ss);

	return ss.str();
}

jvalue jquery(const jvalue& jval, const std::string& query)
{
	std::string s;
	std::vector<std::string> sv;
	std::istringstream ss(query);

	while(std::getline(ss, s, '.'))
	{
		sv.push_back(s);
	}

	const jvalue* ret = &jval;

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

jvalue jupdate(jvalue& jval, const std::string& query, const jvalue& new_jval)
{
	std::string s;
	std::vector<std::string> sv;
	std::istringstream ss(query);

	while(std::getline(ss, s, '.'))
	{
		sv.push_back(s);
	}

	jvalue* old_jval = &jval;

	for(const auto& it : sv)
	{
		if(old_jval->isArray())
		{
			old_jval = &(*old_jval)[std::stoi(it)];
		}
		else
		{
			old_jval = &(*old_jval)[it];
		}

		if(old_jval->isNull())
		{
			break;
		}
	}

	jvalue ret = *old_jval;

	if(!old_jval->isNull())
	{
		*old_jval = new_jval;
	}

	return ret;
}

bool jtob(const jvalue& jval)
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

std::string jtos(const jvalue& jval)
{
	std::string ret = "";

	if(jval.isBool() || jval.isString())
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

int jtoi(const jvalue& jval)
{
	int ret = 0;

	if(jval.isString())
	{
		ret = std::stol(jval.asString());
	}
	else if(jval.isInt())
	{
		ret = jval.asInt();
	}

	return ret;
}

unsigned int jtou(const jvalue& jval)
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

int64_t jtoi64(const jvalue& jval)
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

uint64_t jtou64(const jvalue& jval)
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

float jtof(const jvalue& jval)
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

double jtod(const jvalue& jval)
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
