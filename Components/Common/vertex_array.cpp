#include "vertex_array.h"

namespace Hub
{
	SPVertexArray VertexArray::create()
	{
		return SPVertexArray(new VertexArray());
	}

	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &_obj);
	}

	VertexArray::operator GLuint() const
	{
		return _obj;
	}

	void VertexArray::bindAttribute(const Atrribute& atrribute, const VertexBuffer& buffer, Type::type_t type, uint count, uint stride, intptr_t offset)
	{
		glBindVertexArray(_obj);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glEnableVertexAttribArray(atrribute);
		glVertexAttribPointer(atrribute, count, type, GL_FALSE, stride, (GLvoid*)(offset));
	}

	VertexArray::VertexArray()
	{
		glGenVertexArrays(1, &_obj);
	}

}