#include "application.h"

namespace Hub
{
	void Application::init()
	{
		// init opengl
		createContext();
		// init data
		initData();
	}

	void Application::update()
	{
		// object update
		
		// input update
	}

	void Application::render()
	{
		// mesh render
	}

	void Application::run()
	{
		init();
		while (!currentWindow->shouldClose())
		{
			update();
			render();

			// double buffer
			currentWindow->swapBuffer();
			// events
			currentWindow->pollEvents();
		}
	}

	void Application::createContext()
	{
		// 
		currentWindow = std::make_unique<Window>();
	}

}


