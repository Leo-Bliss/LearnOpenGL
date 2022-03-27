#pragma once
#include "utils.h"
#include "define.h"
#include "buffer.h"

namespace Hub
{
	SharedPtrDefine(ElementBuffer);

	class ElementBuffer
	{
	public:
		static SPElementBuffer create();
		static SPElementBuffer create(const void* data, size_t length, BufferUsage::buffer_usage_t usage);

		~ElementBuffer();
		operator GLuint() const;
		void data(const void* data, size_t length, BufferUsage::buffer_usage_t usage);

	private:
		ElementBuffer();
		ElementBuffer(const void* data, size_t length, BufferUsage::buffer_usage_t usage);
		GLuint _obj;
	};
}