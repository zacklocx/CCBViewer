
#ifndef LOG_INCLUDED
#define LOG_INCLUDED

#include <mutex>
#include <chrono>
#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>

class log_t
{
public:
	log_t(const std::string& tag = "", std::ostream& out = std::cout)
		: tag_(tag), out_(out)
	{}

	~log_t()
	{
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
	}

	template<typename T>
	log_t& operator<<(const T& t)
	{
		stream_ << t;
		return *this;
	}

private:
	std::string tag_;
	std::ostream& out_;
	std::ostringstream stream_;
};

#define LOG log_t

#endif /* LOG_INCLUDED */
