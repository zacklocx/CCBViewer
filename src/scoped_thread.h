
#ifndef SCOPED_THREAD_INCLUDED
#define SCOPED_THREAD_INCLUDED

#include <thread>
#include <utility>

class scoped_thread_t
{
public:
	explicit scoped_thread_t(std::thread t)
		: t_(std::move(t))
	{
		if(!t_.joinable())
		{
			throw std::logic_error("no thread");
		}
	}

	~scoped_thread_t()
	{
		t_.join();
	}

	scoped_thread_t(scoped_thread_t&) = delete;
	scoped_thread_t& operator=(const scoped_thread_t&) = delete;

private:
	std::thread t_;
};

#endif /* SCOPED_THREAD_INCLUDED */
