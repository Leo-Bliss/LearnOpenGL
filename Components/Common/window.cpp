#pragma once
#include "window.h"
#include <iostream>

namespace Hub
{
	void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) // 输入ESC关闭窗口
		{
			glfwSetWindowShouldClose(window, true);
		}
	}

	Window::Window(int width, int height, std::string name,
		unsigned int majorVersion, unsigned int minorVersion) :
		_width(width), _height(height), _name(name),
		_majorVersion(majorVersion), _minorVersion(minorVersion)
	{
		init();
	}

	Window::~Window()
	{
		this->destroy();
	}

	Status::status_t Window::init()
	{
		glfwInit();

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, _majorVersion); // 主版本
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, _minorVersion); // 次版本
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		_window = glfwCreateWindow(_width, _height, _name.c_str(), nullptr, nullptr);

		if (!_window)
		{
			std::cerr << "Failed to create a GLFW window" << std::endl;
			glfwTerminate();
			return Status::status_t::FAILED;
		}

		glfwMakeContextCurrent(_window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cerr << "Failed to init GLAD" << std::endl;
			return Status::status_t::FAILED;
		}

		// Viewport: 告诉OpenGL渲染窗口的尺寸大小：视口
		glViewport(0, 0, _width, _height);

		// 注册调整窗口大小的回调事件: 视口 随 窗口大小 变化
		glfwSetFramebufferSizeCallback(_window, framebuffer_size_callback);
		// 键盘输入回调
		glfwSetKeyCallback(_window, key_callback);

		return Status::status_t::SUCCESS;
	}

	Window::WindowHandle Window::getNativeHandle() const
	{
		return _window;
	}

	bool Window::shouldClose() const
	{
		return glfwWindowShouldClose(_window);
	}

	void Window::setShouldClose(bool val)
	{
		if (shouldClose() == val) return;
		glfwSetWindowShouldClose(_window, val);
	}

	int Window::getKey(int key)
	{
		return glfwGetKey(_window, key);
	}

	void Window::setFramebufferSizeCallback(GLFWframebuffersizefun callBackFunc)
	{
		glfwSetFramebufferSizeCallback(_window, callBackFunc);
	}

	void Window::setKeyCallback(GLFWkeyfun callBackFunc)
	{
		glfwSetKeyCallback(_window, callBackFunc);
	}

	void Window::destroy()
	{
		if (this->_window != nullptr)
		{
			setShouldClose(true);
			glfwDestroyWindow(this->_window);
		}
	}
}
