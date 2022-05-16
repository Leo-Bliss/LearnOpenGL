#pragma once
#pragma once
#include "utils.h"
#include "buffer.h"
#include <memory>

namespace Hub
{
	class UniformBuffer;
	using SPUniformBuffer = std::shared_ptr<UniformBuffer>;

	class UniformBuffer
	{
	public:
		static SPUniformBuffer create();
		static SPUniformBuffer create(const void* data, size_t length, BufferUsage::buffer_usage_t usage);

		~UniformBuffer();
		operator GLuint() const;
		void data(const void* data, size_t length, BufferUsage::buffer_usage_t usage);
		void subData(const void* data, size_t offset, size_t length);
		void bindBufferRange(unsigned int point, unsigned int offset, unsigned int size);
	private:
		UniformBuffer();
		UniformBuffer(const void* data, size_t length, BufferUsage::buffer_usage_t usage);
		GLuint _obj;
	};
}