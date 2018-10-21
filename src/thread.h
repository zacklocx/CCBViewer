
#ifndef THREAD_INCLUDED
#define THREAD_INCLUDED

#include <thread>
#include <utility>

enum { JOIN, DETACH };

template<int>
class thread_t
{
public:
	template<typename F, typename... Args>
	thread_t(F&& f, Args&&... args)
		: thread_(std::forward<F>(f), std::forward<Args>(args)...)
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
inline void thread_t<JOIN>::join_or_detach()
{
	thread_.join();
}

template<>
inline void thread_t<DETACH>::join_or_detach()
{
	thread_.detach();
}

#endif /* THREAD_INCLUDED */
