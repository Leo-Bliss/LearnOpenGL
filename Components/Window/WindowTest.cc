#include <iostream>
#include "window.h"

int main() {
	std::cout << "hello world" << std::endl;
	using Hub::Window;
	Window window(800, 600, "hello");
	while (!window.shouldClose())
	{
		glfwPollEvents();
		// äÖÈ¾Ö¸Áî
		glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window.getNativeHandle());
	}
	glfwTerminate();
	return 0;
}