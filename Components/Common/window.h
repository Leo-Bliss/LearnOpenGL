#pragma once
#include <glad/glad.h>
#include <glfw/glfw3.h>


#include <memory>
#include <string>
#include <functional>

namespace Hub
{
	namespace Status
	{
		enum class status_t
		{
			FAILED = -1,
			SUCCESS = 0
		};
	}

	class Window
	{
		using WindowHandle = GLFWwindow*;
	public:
		Window() = default;
		Window(int width = 800, int height = 600, std::string name = "Window", unsigned int majorVersion = 3, unsigned int minorVersion = 3);
		~Window();
		Status::status_t init();
		WindowHandle getNativeHandle() const;

		bool shouldClose() const;
		void setShouldClose(bool val);
		int getKey(int key);

		void setFramebufferSizeCallback(GLFWframebuffersizefun callBack);
		void setKeyCallback(GLFWkeyfun callBack);

	private:
		int _width;
		int _height;
		std::string _name;
		GLFWwindow* _window = nullptr;

		int _majorVersion; // TODO: 不作为成员
		int _minorVersion;

		void destroy();
	};
}