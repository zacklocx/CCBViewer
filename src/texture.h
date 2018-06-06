
#ifndef TEXTURE_INCLUDED
#define TEXTURE_INCLUDED

#include <string>

class texture_t
{
public:
	texture_t();

	int id() const;

	int width() const;
	int height() const;

	std::string path() const;

private:
	int id_, width_, height_;
	std::string path_;
};

#endif /* TEXTURE_INCLUDED */
