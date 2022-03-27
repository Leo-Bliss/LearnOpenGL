#pragma once
#include "utils.h"
#include "buffer.h"
#include <memory>

namespace Hub
{
	class VertexBuffer;
	using SPVertexBuffer = std::shared_ptr<VertexBuffer>;

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