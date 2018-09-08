
#include "utils.h"

#include <cstdio>

#include <chrono>
#include <sstream>
#include <iomanip>

#include <openssl/md5.h>

uint64_t timestamp_s()
{
	auto ts = std::chrono::system_clock::now().time_since_epoch();
	return std::chrono::duration_cast<std::chrono::seconds>(ts).count();
}

uint64_t timestamp_ms()
{
	auto ts = std::chrono::system_clock::now().time_since_epoch();
	return std::chrono::duration_cast<std::chrono::milliseconds>(ts).count();
}

std::string now()
{
	std::ostringstream ret;

	auto now = std::chrono::system_clock::now();
	auto time = std::chrono::system_clock::to_time_t(now);

	ret << std::put_time(std::localtime(&time), "%F %T");

	return ret.str();
}

std::string what_time(uint64_t ts)
{
	std::ostringstream ret;

	auto epoch = std::chrono::time_point<std::chrono::system_clock>();
	auto time = std::chrono::system_clock::to_time_t(epoch + std::chrono::seconds(ts));

	ret << std::put_time(std::localtime(&time), "%F %T");

	return ret.str();
}

std::string md5(const std::string& s)
{
	unsigned char digest[MD5_DIGEST_LENGTH];
	MD5((unsigned char*)s.c_str(), s.length(), digest);

	char ret[MD5_DIGEST_LENGTH * 2 + 1];

	for(int i = 0; i < MD5_DIGEST_LENGTH; ++i)
	{
		sprintf(ret + i * 2, "%02x", digest[i]);
	}

	return ret;
}

std::string urlencode(const std::string& s)
{
	const char lookup[]= "0123456789abcdef";

	std::ostringstream ret;

	for(int i = 0, len = s.length(); i < len; ++i)
	{
		const char& c = s[i];

		if((c >= 48 && c <= 57) || // 0-9
			(c >= 65 && c <= 90) || // A-Z
			(c >= 97 && c <= 122) || // a-z
			(c == '-' || c == '_' || c == '.' || c == '~'))
		{
			ret << c;
		}
		else
		{
			ret << '%';
			ret << lookup[(c & 0xF0) >> 4];
			ret << lookup[(c & 0x0F)];
		}
	}

	return ret.str();
}
