
#ifndef THREAD_INCLUDED
#define THREAD_INCLUDED

#include <thread>
#include <utility>

enum class thread_exec_t { join, detach };

template<thread_exec_t exec>
class thread_t
{
public:
	template<class F, class... Args>
	thread_t(F&& f, Args&&... args)
		: t_(std::forward<F>(f), std::forward<Args>(args)...)
	{}

	thread_t(const thread_t&) = delete;
	thread_t(thread_t&&) = default;

	~thread_t()
	{
		if(t_.joinable())
		{
			join_or_detach();
		}
	}

private:
	std::thread t_;

	void join_or_detach();
};

template<>
inline void thread_t<thread_exec_t::join>::join_or_detach()
{
	t_.join();
}

template<>
inline void thread_t<thread_exec_t::detach>::join_or_detach()
{
	t_.detach();
}

#endif /* THREAD_INCLUDED */
