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
	public:
		Window(int width = 800, int height = 600, std::string name = "Window", unsigned int majorVersion = 3, unsigned int minorVersion = 3);
		
		Status::status_t init();
		GLFWwindow* getGLWindowIns() const;

		int _width;
		int _height;
		std::string _name;
		int _majorVersion;
		int _minorVersion;

		bool shouldClose();
		void setShouldClose(bool val);
		int getKey(int key);

		//// temp: not memeber???
		//void swapBuffers();
		//void processInput();
		//void listenEvents();
		//void terminate();
		//// todo fix

		void setFramebufferSizeCallback(GLFWframebuffersizefun callBack);
		void setKeyCallback(GLFWkeyfun callBack);

	private:
		GLFWwindow* _glWindow;
	};
}