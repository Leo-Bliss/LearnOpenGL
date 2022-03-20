#pragma once
#include "utils.h"
#include "image.h"


#include <memory>

namespace Hub
{
	//// Internal texture formats
	//namespace InternalFormat 
	//{
	//	enum internal_format_t
	//	{
	//		Red = GL_RED,
	//		RGB = GL_RGB,
	//		RGBA = GL_RGBA,
 //           // ...

	//	};
	//}

	// Texture data formats == Internal texture formats
	namespace Format
	{
		enum format_t
		{
			Red = GL_RED,
			RGB = GL_RGB,
			BGR = GL_BGR,
			RGBA = GL_RGBA,
			BGRA = GL_BGRA
		};
	}

    // // Texture data types == untils: Type
    //namespace DataType
    //{
    //    enum data_type_t
    //    {
    //        Byte = GL_BYTE,
    //        UnsignedByte = GL_UNSIGNED_BYTE,
    //        Short = GL_SHORT,
    //        UnsignedShort = GL_UNSIGNED_SHORT,
    //        Int = GL_INT,
    //        UnsignedInt = GL_UNSIGNED_INT,
    //        Float = GL_FLOAT,
    //        Double = GL_DOUBLE,
    //        // ...
    //    };
    //}

    // Wrapping types
    namespace Wrapping
    {
        enum wrapping_t
        {
            ClampEdge = GL_CLAMP_TO_EDGE,
            ClampBorder = GL_CLAMP_TO_BORDER,
            Repeat = GL_REPEAT,
            MirroredRepeat = GL_MIRRORED_REPEAT
        };
        enum axis_t
        {
            S = GL_TEXTURE_WRAP_S,
            T = GL_TEXTURE_WRAP_T,
            R = GL_TEXTURE_WRAP_R,
        };
    }
    

     // Filter types
    namespace Filter
    {
        enum filter_t
        {
            Nearest = GL_NEAREST,
            Linear = GL_LINEAR,
            NearestMipmapNearest = GL_NEAREST_MIPMAP_NEAREST,
            LinearMipmapNearest = GL_LINEAR_MIPMAP_NEAREST,
            NearestMipmapLinear = GL_NEAREST_MIPMAP_LINEAR,
            LinearMipmapLinear = GL_LINEAR_MIPMAP_LINEAR
        };
        enum operator_t
        {
            Mag = GL_TEXTURE_MAG_FILTER,
            Min = GL_TEXTURE_MIN_FILTER,
        };
    }

	class Texture;
	using SPTexture = std::shared_ptr<Texture>;

	class Texture
	{
	public:
		~Texture();
        operator GLuint() const;

		static SPTexture create();
		static SPTexture create(const SPImage image);
        void setWrapping(Wrapping::axis_t axis, Wrapping::wrapping_t wrapping);
        void setFilter(Filter::operator_t op, Filter::filter_t flt);
        void setBorderColor(const Color& color);
        void image2D(const void* data, Format::format_t format, int width, int height, Type::type_t dataType);
        void generateMipMap();

	private:
		Texture();
        Texture(const SPImage image);
        Format::format_t getDefaultFormat(int channelCount = 4);
		GLuint _obj;
	};
}