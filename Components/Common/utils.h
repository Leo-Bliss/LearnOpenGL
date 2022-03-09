#pragma once
#include <glad/glad.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"


namespace Hub
{
	namespace Type
	{
		enum type_t
		{
			Byte = GL_BYTE,
			UnsignedByte = GL_UNSIGNED_BYTE,
			Short = GL_SHORT,
			UnsignedShort = GL_UNSIGNED_SHORT,
			Int = GL_INT,
			UnsignedInt = GL_UNSIGNED_INT,
			Float = GL_FLOAT,
			Double = GL_DOUBLE,
		};

	}
	using uint = unsigned int;

	using Atrribute = GLint;
	using Uniform = GLint;
	using Color = glm::vec4;
	inline const float* color_ptr(const Color& color)
	{
		return glm::value_ptr(color);
	}
}
