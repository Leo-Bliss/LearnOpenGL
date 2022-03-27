#pragma once
#include "utils.h"
#include "define.h"

namespace Hub
{
	namespace BufferUsage
	{
		enum buffer_usage_t
		{
			StreamDraw = GL_STREAM_DRAW,
			StreamRead = GL_STREAM_READ,
			StreamCopy = GL_STREAM_COPY,
			StaticDraw = GL_STATIC_DRAW,
			StaticRead = GL_STATIC_READ,
			StaticCopy = GL_STREAM_COPY,
			DynamicDraw = GL_DYNAMIC_DRAW,
			DynamicRead = GL_DYNAMIC_READ,
			DynamicCopy = GL_DYNAMIC_COPY
		};
	}

	SharedPtrDefine(VertexBuffer);

	class VertexBuffer
	{
	public:
		static SPVertexBuffer create();
		static SPVertexBuffer create(const void* data, size_t length, BufferUsage::buffer_usage_t usage);

		~VertexBuffer();
		operator GLuint() const;
		void data(const void* data, size_t length, BufferUsage::buffer_usage_t usage);
		void subData(const void* data, size_t offset, size_t length);
		void getSubData(void* data, size_t offset, size_t length);

	private:
		VertexBuffer();
		VertexBuffer(const void* data, size_t length, BufferUsage::buffer_usage_t usage);
		GLuint _obj;
	};
}