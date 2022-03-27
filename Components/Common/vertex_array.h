#pragma once
#include "utils.h"
#include "define.h"
#include "vertex_buffer.h"
#include "element_buffer.h"


namespace Hub
{
	SharedPtrDefine(VertexArray);

	class VertexArray
	{
	public:
		static SPVertexArray create();
		~VertexArray();
		operator GLuint()const;
		void bindAttribute(const Atrribute& atrribute, uint count, const VertexBuffer& buffer,
			Type::type_t type, uint stride, intptr_t offset);
		void bindElements(const ElementBuffer& element);
		void bindTransformFeedback(uint index, const VertexBuffer& buffer);

	private:
		VertexArray();
		GLuint _obj;
		
	};
}