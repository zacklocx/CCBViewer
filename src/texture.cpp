
#include "texture.h"

#include <GL/gl.h>

#include <FreeImage.h>

std::unordered_map<std::string, std::shared_ptr<const texture_t::info_t>> texture_t::cache_;

texture_t::texture_t(const std::string& path /* = "" */)
{
	load(path);
}

texture_t::~texture_t()
{
	unload();
}

bool texture_t::load(const std::string& path)
{
	if(0 == path.length())
	{
		return false;
	}

	if(auto it = cache_.find(path); it != cache_.end())
	{
		if(info_ && info_->id_ != it->second->id_)
		{
			unload();
		}

		if(!info_)
		{
			info_ = it->second;
		}

		return true;
	}

	FIBITMAP* dib = nullptr;
	FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(path.c_str());

	if(FIF_UNKNOWN != fif && FreeImage_FIFSupportsReading(fif))
	{
		dib = FreeImage_Load(fif, path.c_str());
	}

	if(!dib)
	{
		return false;
	}

	unload();

	GLenum internalformat = GL_RGBA8, format = GL_BGRA;

	int bpp = FreeImage_GetBPP(dib);

	if(8 == bpp)
	{
		internalformat = GL_LUMINANCE8;
		format = GL_LUMINANCE;
	}
	else if(24 == bpp)
	{
		internalformat = GL_RGB8;
		format = GL_BGR;
	}
	else if(bpp != 32)
	{
		dib = FreeImage_ConvertTo32Bits(dib);
	}

	unsigned int id = 0;

	int width = FreeImage_GetWidth(dib);
	int height = FreeImage_GetHeight(dib);

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, internalformat, width, height, 0, format, GL_UNSIGNED_BYTE, FreeImage_GetBits(dib));

	FreeImage_Unload(dib);

	info_ = std::make_shared<const info_t>(info_t{ id, width, height });
	cache_[path] = info_;

	return true;
}

void texture_t::unload()
{
	if(!info_)
	{
		return;
	}

	unsigned int id = info_->id_;

	info_.reset();

	if(1 == info_.use_count())
	{
		for(const auto& [path, info] : cache_)
		{
			if(id == info->id_)
			{
				glDeleteTextures(1, &id);
				cache_.erase(path);

				break;
			}
		}
	}
}

unsigned int texture_t::id() const
{
	return info_? info_->id_ : 0u;
}

int texture_t::width() const
{
	return info_? info_->width_ : 0;
}

int texture_t::height() const
{
	return info_? info_->height_ : 0;
}

void texture_t::draw(float x, float y, float w, float h, float r) const
{
	unsigned int id = info_? info_->id_ : 0u;

	float vx = w * 0.5f, vy = h * 0.5f;

	glPushMatrix();

	glTranslatef(x, y, 0.0f);
	glRotatef(r, 0.0f, 0.0f, 1.0f);

	glBindTexture(GL_TEXTURE_2D, id);

	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex2f(-vx, -vy);

		glTexCoord2f(1.0f, 0.0f);
		glVertex2f(vx, -vy);

		glTexCoord2f(1.0f, 1.0f);
		glVertex2f(vx, vy);

		glTexCoord2f(0.0f, 1.0f);
		glVertex2f(-vx, vy);
	glEnd();

	glPopMatrix();
}

void texture_t::clear()
{
	for(const auto& [_, info] : cache_)
	{
		glDeleteTextures(1, &info->id_);
	}

	cache_.clear();
}
