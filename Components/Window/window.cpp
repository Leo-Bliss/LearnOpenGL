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
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) // ����ESC�رմ���
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

	Status::status_t Window::init()
	{
		glfwInit();

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, _majorVersion); // ���汾
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, _minorVersion); // �ΰ汾
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		_glWindow = glfwCreateWindow(_width, _height, _name.c_str(), nullptr, nullptr);

		if (!_glWindow)
		{
			std::cerr << "Failed to create a GLFW window" << std::endl;
			glfwTerminate();
			return Status::status_t::FAILED;
		}

		glfwMakeContextCurrent(_glWindow);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cerr << "Failed to init GLAD" << std::endl;
			return Status::status_t::FAILED;
		}

		// Viewport: ����OpenGL��Ⱦ���ڵĳߴ��С���ӿ�
		glViewport(0, 0, _width, _height);

		// ע��������ڴ�С�Ļص��¼�: �ӿ� �� ���ڴ�С �仯
		glfwSetFramebufferSizeCallback(_glWindow, framebuffer_size_callback);
		// ��������ص�
		glfwSetKeyCallback(_glWindow, key_callback);

		return Status::status_t::SUCCESS;
	}

	GLFWwindow* Window::getNativeHandle() const
	{
		return _glWindow;
	}

	bool Window::shouldClose()
	{
		return glfwWindowShouldClose(_glWindow);
	}

	void Window::setShouldClose(bool val)
	{
		if (shouldClose() == val) return;
		glfwSetWindowShouldClose(_glWindow, val);
	}

	int Window::getKey(int key)
	{
		return glfwGetKey(_glWindow, key);
	}

	/*void Window::swapBuffers()
	{
		glfwSwapBuffers(_glWindow);
	}

	void Window::processInput()
	{

	}

	void Window::listenEvents()
	{
		glfwPollEvents();
	}

	void Window::terminate()
	{
		glfwTerminate();
	}*/


	void Window::setFramebufferSizeCallback(GLFWframebuffersizefun callBackFunc)
	{
		glfwSetFramebufferSizeCallback(_glWindow, callBackFunc);
	}

	void Window::setKeyCallback(GLFWkeyfun callBackFunc)
	{
		glfwSetKeyCallback(_glWindow, callBackFunc);
	}

}
