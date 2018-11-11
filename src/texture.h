
#ifndef TEXTURE_INCLUDED
#define TEXTURE_INCLUDED

#include <string>
#include <memory>
#include <unordered_map>

class texture_t
{
public:
	explicit texture_t(const std::string& path = "");
	~texture_t();

	bool load(const std::string& path);
	void unload();

	unsigned int id() const;

	int width() const;
	int height() const;

	void draw(float x, float y, float w, float h, float r) const;
	void draw(float x, float y, float w, float h,
		float tx, float ty, float tw, float th,
		float r) const;

	static void clear();

private:
	struct info_t
	{
		unsigned int id_;
		int width_, height_;
	};

	std::shared_ptr<const info_t> info_;

	static std::unordered_map<std::string, std::shared_ptr<const info_t>> cache_;
};

#endif /* TEXTURE_INCLUDED */
