
#include "texture.h"

#include <GL/gl.h>
#include <GL/glu.h>

#include <FreeImage.h>

#include "llog.h"

texture_t::data_t::data_t()
	: id_(0), width_(0), height_(0)
{}

texture_t::data_t::~data_t()
{
	if(glIsTexture(id_))
	{
		LLOG("~data");
		glDeleteTextures(1, &id_);
		id_ = 0;
	}
}

texture_t::texture_t()
	: data_(std::make_shared<texture_t::data_t>())
{
	// LLOG("texture") << data_.use_count();
}

texture_t::~texture_t()
{
	//LLOG("~texture") << data_.use_count();
}

unsigned int texture_t::id() const
{
	return data_->id_;
}

int texture_t::width() const
{
	return data_->width_;
}

int texture_t::height() const
{
	return data_->height_;
}

bool texture_t::load(const std::string& path)
{
	bool ret = false;

	FIBITMAP* dib;
	FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(path.c_str());

	if(FIF_UNKNOWN != fif && FreeImage_FIFSupportsReading(fif))
	{
		dib = FreeImage_Load(fif, path.c_str());
	}

	if(dib)
	{
		ret = true;

		data_->width_ = FreeImage_GetWidth(dib);
		data_->height_ = FreeImage_GetHeight(dib);

		GLenum internalformat, format;

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
		else if(32 == bpp)
		{
			internalformat = GL_RGBA8;
			format = GL_BGRA;
		}
		else
		{
			dib = FreeImage_ConvertTo32Bits(dib);

			internalformat = GL_RGBA8;
			format = GL_BGRA;
		}

		glGenTextures(1, &data_->id_);
		glBindTexture(GL_TEXTURE_2D, data_->id_);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexImage2D(GL_TEXTURE_2D, 0, internalformat, data_->width_, data_->height_, 0, format, GL_UNSIGNED_BYTE, FreeImage_GetBits(dib));

		FreeImage_Unload(dib);
	}

	return ret;
}

void texture_t::draw(float x, float y, float width, float height) const
{
	glEnable(GL_TEXTURE_2D);

	glPushMatrix();
	glTranslatef(x, y, 0.0f);

	glBindTexture(GL_TEXTURE_2D, data_->id_);

	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex2f(-width * 0.5f, -height * 0.5f);

		glTexCoord2f(1.0f, 0.0f);
		glVertex2f(width * 0.5f, -height * 0.5f);

		glTexCoord2f(1.0f, 1.0f);
		glVertex2f(width * 0.5f, height * 0.5f);

		glTexCoord2f(0.0f, 1.0f);
		glVertex2f(-width * 0.5f, height * 0.5f);
	glEnd();

	glPopMatrix();
}
