#include "vertex_buffer.h"

namespace Hub
{

	SPVertexBuffer VertexBuffer::create()
	{
		return SPVertexBuffer(new VertexBuffer());
	}

	VertexBuffer::~VertexBuffer()
	{

	}

	VertexBuffer::operator GLuint() const
	{
		return _obj;
	}

	VertexBuffer::VertexBuffer()
	{
		glGenBuffers(1, &_obj);
	}

}