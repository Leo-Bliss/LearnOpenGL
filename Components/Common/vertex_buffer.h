#pragma once
#include "utils.h"
#include <memory>

namespace Hub
{
	class VertexBuffer;
	using SPVertexBuffer = std::shared_ptr<VertexBuffer>;

	class VertexBuffer
	{
	public:
		static SPVertexBuffer create();
		~VertexBuffer();
		operator GLuint() const;

	private:
		VertexBuffer();
		GLuint _obj;
	};
}