#pragma once
#include "utils.h"
#include "vertex_buffer.h"
#include <memory>


namespace Hub
{
	class VertexArray;
	using SPVertexArray = std::shared_ptr<VertexArray>;

	class VertexArray
	{
	public:
		static SPVertexArray create();
		~VertexArray();
		operator GLuint()const;
		void bindAttribute(const Atrribute& atrribute, const VertexBuffer& buffer,
			Type::type_t type, uint count, uint stride, intptr_t offset);

	private:
		VertexArray();
		GLuint _obj;
		
	};
}