
#ifndef LINE_LOGGER_INCLUDED
#define LINE_LOGGER_INCLUDED

#include <chrono>
#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>

class line_logger_t
{
public:
	line_logger_t(const std::string& tag = "", std::ostream& out = std::cout)
#ifdef APP_DEBUG
		: tag_(tag), out_(out)
#endif
	{}

	~line_logger_t()
	{
#ifdef APP_DEBUG
		stream_ << "\n";

		auto now = std::chrono::system_clock::now();
		auto time = std::chrono::system_clock::to_time_t(now);
		auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count() % 1000;

		out_ << "[" << std::put_time(std::localtime(&time), "%F %T") << "." << std::setfill('0') << std::setw(3) << ms << "] ";

		if(tag_ != "")
		{
			out_ << "<" << tag_ << "> ";
		}

		out_ << stream_.str();
		out_.flush();
#endif
	}

	template<typename T>
	line_logger_t& operator <<(const T& t)
	{
#ifdef APP_DEBUG
		stream_ << t;
#endif

		return *this;
	}

private:
#ifdef APP_DEBUG
	std::string tag_;
	std::ostream& out_;
	std::ostringstream stream_;
#endif
};

#define LLOG line_logger_t

#endif /* LINE_LOGGER_INCLUDED */
