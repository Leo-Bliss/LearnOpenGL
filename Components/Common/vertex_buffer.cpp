#include "vertex_buffer.h"

namespace Hub
{

	SPVertexBuffer VertexBuffer::create()
	{
		return SPVertexBuffer(new VertexBuffer());
	}

	SPVertexBuffer VertexBuffer::create(const void* data, size_t length, BufferUsage::buffer_usage_t usage)
	{
		return SPVertexBuffer(new VertexBuffer(data, length, usage));
	}

	VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1, &_obj);
	}

	VertexBuffer::operator GLuint() const
	{
		return _obj;
	}

	void VertexBuffer::data(const void* data, size_t length, BufferUsage::buffer_usage_t usage)
	{
		glBindBuffer(GL_ARRAY_BUFFER, _obj);
		glBufferData(GL_ARRAY_BUFFER, length, data, usage);
	}

	void VertexBuffer::subData(const void* data, size_t offset, size_t length)
	{
		glBindBuffer(GL_ARRAY_BUFFER, _obj);
		glBufferSubData(GL_ARRAY_BUFFER, offset, length, data);
	}

	void VertexBuffer::getSubData(void* data, size_t offset, size_t length)
	{
		glBindBuffer(GL_ARRAY_BUFFER, _obj);
		glGetBufferSubData(GL_ARRAY_BUFFER, offset, length, data);
	}

	VertexBuffer::VertexBuffer()
	{
		glGenBuffers(1, &_obj);
	}

	VertexBuffer::VertexBuffer(const void* data, size_t length, BufferUsage::buffer_usage_t usage):VertexBuffer()
	{
		this->data(data, length, usage);
	}

}