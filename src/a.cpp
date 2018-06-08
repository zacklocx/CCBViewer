
#include "a.h"

#include <iostream>

namespace a_ns
{
	void render(const a_ns::a_t& a)
	{
		std::cout << a.get() << std::endl;
	}
}
