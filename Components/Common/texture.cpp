#include "texture.h"


namespace Hub
{
	Texture::~Texture()
	{
		glDeleteTextures(1, &_obj);
	}

	Texture::operator GLuint() const
	{
		return _obj;
	}

	Hub::SPTexture Texture::create()
	{
		return SPTexture(new Texture());
	}

	SPTexture Texture::create(const SPImage image)
	{
		return SPTexture(new Texture(image));
	}

	Hub::SPTexture Texture::create(const char* filePath)
	{
		auto image = Image::create(filePath);
		return SPTexture(new Texture(image));
	}

	void Texture::setWrapping(Wrapping::axis_t axis, Wrapping::wrapping_t wrapping)
	{
		glBindTexture(GL_TEXTURE_2D, _obj);
		glTexParameteri(GL_TEXTURE_2D, axis, wrapping);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Texture::setFilter(Filter::operator_t op, Filter::filter_t flt)
	{
		glBindTexture(GL_TEXTURE_2D, _obj);
		glTexParameteri(GL_TEXTURE_2D, op, flt);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Texture::setBorderColor(const Color& color)
	{
		glBindTexture(GL_TEXTURE_2D, _obj);
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color_ptr(color));
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Texture::image2D(const void* data, Format::format_t format, int width, int height, Type::type_t dataType)
	{
		glBindTexture(GL_TEXTURE_2D, _obj);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, dataType, data);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Texture::generateMipMap()
	{
		glBindTexture(GL_TEXTURE_2D, _obj);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	Texture::Texture()
	{
		glGenTextures(1, &_obj);
	}

	Texture::Texture(const SPImage image):Texture()
	{
		Format::format_t format = getDefaultFormat(image->getChannels());
		image2D(image->getData(), format, image->getWidth(), image->getHeight(), Type::UnsignedByte);
		generateMipMap();
	}

	Format::format_t Texture::getDefaultFormat(int channelCount)
	{
		switch (channelCount)
		{
		case 1:
			return Format::Red;
			break;
		case 3:
			return Format::RGB;
			break;
		case 4:
			return Format::RGBA;
			break;
		}
		return Format::RGBA;
	}

}