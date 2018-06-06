
#ifndef SCOPED_THREAD_INCLUDED
#define SCOPED_THREAD_INCLUDED

#include <thread>
#include <utility>

class scoped_thread_t
{
public:
	enum class action { join, detach };

	scoped_thread_t(std::thread t, action act)
		: t_(std::move(t)), act_(act)
	{
		if(!t_.joinable())
		{
			throw std::logic_error("no thread");
		}
	}

	~scoped_thread_t()
	{
		if(action::join == act_)
		{
			t_.join();
		}
		else
		{
			t_.detach();
		}
	}

	scoped_thread_t(scoped_thread_t&) = delete;
	scoped_thread_t& operator=(const scoped_thread_t&) = delete;

private:
	std::thread t_;
	action act_;
};

#endif /* SCOPED_THREAD_INCLUDED */
