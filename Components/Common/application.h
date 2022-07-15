#pragma once
#include "window.h"
#include <memory>

namespace Hub
{
	class Application
	{
	public:
		Application() = default;
		virtual ~Application() { glfwTerminate(); }
		void init();

		virtual void initData() {};

		virtual void update(); // logic tick
		virtual void render(); // render tick
		void run(); // main tick

	private:
		std::unique_ptr<Window> currentWindow = nullptr;

		void createContext(); // include init window

	};
}