#pragma  once
// popular single header image loading library 
#define STB_IMAGE_IMPLEMENTATION // must: only contains the relevant definition source code
#include "stb/stb_image.h"

class Image
{
public:
	Image(const char* filePath) 
	{
		_imageIns = stbi_load(filePath, &_width, &_height, &_channels, 0);
	}
	~Image() 
	{
		stbi_image_free(_imageIns);
	}
	int getWidth()const { return _width; }
	int getHeight()const { return _height; }
	const unsigned char* getRawIns() const { return _imageIns; }
	bool loadSuccessfully() { return _imageIns != nullptr; }

private:
	int _width;
	int _height;
	int _channels;
	unsigned char* _imageIns;
};