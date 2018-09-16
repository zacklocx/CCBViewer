
#ifndef TEXTURE_INCLUDED
#define TEXTURE_INCLUDED

#include <string>
#include <memory>

class texture_t
{
public:
	texture_t();
	~texture_t();

	unsigned int id() const;

	int width() const;
	int height() const;

	bool load(const std::string& path);
	void draw(float x, float y, float width, float height) const;

private:
	struct data_t
	{
		data_t();
		~data_t();

		unsigned int id_;
		int width_, height_;
	};

	std::shared_ptr<texture_t::data_t> data_;
};

#endif /* TEXTURE_INCLUDED */
