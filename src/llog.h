
#ifndef LLOG_INCLUDED
#define LLOG_INCLUDED

#include <string>
#include <iostream>

#ifdef APP_DEBUG
#include <mutex>
#include <chrono>
#include <sstream>
#include <iomanip>
#endif

class llog_t
{
public:
	llog_t(const std::string& tag = "", std::ostream& out = std::cout)
	#ifdef APP_DEBUG
		: tag_(tag), out_(out)
	#endif
	{}

	~llog_t()
	{
	#ifdef APP_DEBUG
		stream_ << "\n";

		auto now = std::chrono::system_clock::now();
		auto time = std::chrono::system_clock::to_time_t(now);
		auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count() % 1000;

		static std::mutex mutex;
		{
			std::lock_guard<std::mutex> lock(mutex);

			out_ << "[" << std::put_time(std::localtime(&time), "%F %T") << "." << std::setfill('0') << std::setw(3) << ms << "] ";

			if(tag_ != "")
			{
				out_ << "<" << tag_ << "> ";
			}

			out_ << stream_.str();
			out_.flush();
		}
	#endif
	}

	template<typename T>
	llog_t& operator <<(const T& t)
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

#define LLOG llog_t

#endif /* LLOG_INCLUDED */
