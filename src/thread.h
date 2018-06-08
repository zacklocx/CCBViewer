
#ifndef THREAD_INCLUDED
#define THREAD_INCLUDED

#include <thread>
#include <utility>

enum class thread_exec_t { join, detach };

template<thread_exec_t>
class thread_t
{
public:
	template<class Func, class... Args>
	thread_t(Func&& func, Args&&... args)
		: thread_(std::forward<Func>(func), std::forward<Args>(args)...)
	{}

	thread_t(const thread_t&) = delete;
	thread_t(thread_t&&) = default;

	~thread_t()
	{
		if(thread_.joinable())
		{
			join_or_detach();
		}
	}

private:
	std::thread thread_;

	void join_or_detach();
};

template<>
inline void thread_t<thread_exec_t::join>::join_or_detach()
{
	thread_.join();
}

template<>
inline void thread_t<thread_exec_t::detach>::join_or_detach()
{
	thread_.detach();
}

#endif /* THREAD_INCLUDED */
