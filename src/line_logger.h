
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
	line_logger_t(bool enable = true, bool show_time = true, std::ostream& out = std::cout) :
		enable_(enable), show_time_(show_time), out_(out)
	{}

	~line_logger_t()
	{
#ifdef APP_DEBUG
		if(enable_)
		{
			stream_ << "\n";

			if(show_time_)
			{
				auto now = std::chrono::system_clock::now();
				auto time = std::chrono::system_clock::to_time_t(now);
				auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count() % 1000;

				out_ << "[ " << std::put_time(std::localtime(&time), "%F %T") << "." << std::setfill('0') << std::setw(3) << ms << " ] ";
			}

			out_ << stream_.str();
			out_.flush();
		}
#endif
	}

	template<typename T>
	line_logger_t& operator <<(const T& t)
	{
#ifdef APP_DEBUG
		if(enable_)
		{
			stream_ << t;
		}
#endif

		return *this;
	}

private:
	bool enable_, show_time_;

	std::ostream& out_;
	std::ostringstream stream_;
};

#define LLOG line_logger_t

#endif /* DUMP_INCLUDED */
