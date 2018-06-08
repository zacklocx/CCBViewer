
#ifndef A_INCLUDED
#define A_INCLUDED

namespace a_ns
{
	class a_t
	{
	public:
		a_t() { i_ = 999; }

		int get() const { return i_; }

	private:
		int i_;
	};

	void render(const a_ns::a_t&);
}

#endif /* A_INCLUDED */
