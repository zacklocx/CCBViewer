
#ifndef TIMER_INCLUDED
#define TIMER_INCLUDED

#include <cstdint>

#include <memory>
#include <functional>

#include <boost/asio.hpp>
#include <boost/asio/steady_timer.hpp>

class timer_t
{
public:
	typedef std::function<void(int, uint64_t)> handler_type;

	timer_t(boost::asio::io_service& service, int period_ms, handler_type handler);

	int period() const;
	uint64_t count() const;
	uint64_t limit() const;
	bool is_running() const;

	void run(uint64_t limit = 0);
	void stop();

private:
	int period_ms_;
	uint64_t count_, limit_;
	bool is_running_;

	handler_type handler_;

	std::shared_ptr<boost::asio::steady_timer> timer_;

	void handle_wait(const boost::system::error_code& ec);
};

#endif /* TIMER_INCLUDED */
