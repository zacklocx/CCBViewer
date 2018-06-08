
#ifndef DEMO_INCLUDE
#define DEMO_INCLUDE

class demo_t
{
public:
	demo_t() : x_(0.0f), y_(0.0f) {}

	void set_x(float x) { x_ = x; }
	void set_y(float y) { y_ = y; }

	float get_x() const { return x_; }
	float get_y() const { return y_; }

private:
	float x_, y_;
};

void render(const demo_t&);

#endif /* DEMO_INCLUDED */
