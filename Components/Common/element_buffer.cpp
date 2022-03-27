#include "element_buffer.h"

namespace Hub
{
	SPElementBuffer ElementBuffer::create()
	{
		return SPElementBuffer(new ElementBuffer());
	}

	SPElementBuffer ElementBuffer::create(const void* data, size_t length, BufferUsage::buffer_usage_t usage)
	{
		return SPElementBuffer(new ElementBuffer(data, length, usage));
	}

	ElementBuffer::~ElementBuffer()
	{
		glDeleteBuffers(1, &_obj);
	}

	ElementBuffer::operator GLuint() const
	{
		return _obj;
	}

	void ElementBuffer::data(const void* data, size_t length, BufferUsage::buffer_usage_t usage)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _obj);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, length, data, usage);
	}

	ElementBuffer::ElementBuffer()
	{
		glGenBuffers(1, &_obj);
	}

	ElementBuffer::ElementBuffer(const void* data, size_t length, BufferUsage::buffer_usage_t usage):ElementBuffer()
	{
		this->data(data, length, usage);
	}

}