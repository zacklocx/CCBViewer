
#include "timer.h"

#include <chrono>
#include <utility>

#include <boost/bind.hpp>

timer_t::timer_t(boost::asio::io_service& service, int period_ms, handler_type handler)
		: period_ms_(period_ms), count_(0), limit_(0), is_running_(false)
		, handler_(std::move(handler))
		, timer_(std::make_shared<boost::asio::steady_timer>(std::ref(service), std::chrono::milliseconds(period_ms)))
{}

int timer_t::period_ms() const
{
	return period_ms_;
}

uint64_t timer_t::count() const
{
	return count_;
}

uint64_t timer_t::limit() const
{
	return limit_;
}

bool timer_t::is_running() const
{
	return is_running_;
}

void timer_t::run(uint64_t limit /* = 0 */)
{
	if(!is_running_)
	{
		count_ = 0;
		limit_ = limit;
		is_running_ = true;

		timer_->async_wait(boost::bind(&timer_t::operator(), this, boost::asio::placeholders::error));
	}
}

void timer_t::stop()
{
	if(is_running_)
	{
		is_running_ = false;
		timer_->cancel();
	}
}

void timer_t::operator()(const boost::system::error_code& ec)
{
	if(!ec)
	{
		++count_;

		if(handler_)
		{
			handler_(period_ms_, count_);
		}

		if(is_running_)
		{
			if(0 == limit_ || count_ < limit_)
			{
				timer_->expires_at(timer_->expires_at() + std::chrono::milliseconds(period_ms_));
				timer_->async_wait(boost::bind(&timer_t::operator(), this, boost::asio::placeholders::error));
			}
			else
			{
				stop();
			}
		}
	}
}
