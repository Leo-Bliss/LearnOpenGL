#include "uniform_buffer.h"

namespace Hub
{

	SPUniformBuffer Hub::UniformBuffer::create()
	{
		return SPUniformBuffer(new UniformBuffer());
	}

	SPUniformBuffer Hub::UniformBuffer::create(const void* data, size_t length, BufferUsage::buffer_usage_t usage)
	{
		return SPUniformBuffer(new UniformBuffer(data, length, usage));
	}

	Hub::UniformBuffer::~UniformBuffer()
	{
		glDeleteBuffers(1, &_obj);
	}

	Hub::UniformBuffer::operator GLuint() const
	{
		return _obj;
	}

	void Hub::UniformBuffer::data(const void* data, size_t length, BufferUsage::buffer_usage_t usage)
	{
		glBindBuffer(GL_UNIFORM_BUFFER, _obj);
		glBufferData(GL_UNIFORM_BUFFER, length, data, usage);
	}

	void UniformBuffer::subData(const void* data, size_t offset, size_t length)
	{
		glBindBuffer(GL_UNIFORM_BUFFER, _obj);
		glBufferSubData(GL_UNIFORM_BUFFER, offset, length, data);
	}

	void UniformBuffer::bindBufferRange(unsigned int point, unsigned int offset, unsigned int size)
	{
		glBindBufferRange(GL_UNIFORM_BUFFER, point, _obj, offset, size);
	}

	UniformBuffer::UniformBuffer()
	{
		glGenBuffers(1, &_obj);
	}

	UniformBuffer::UniformBuffer(const void* data, size_t length, BufferUsage::buffer_usage_t usage)
	{
		this->data(data, length, usage);
	}

}