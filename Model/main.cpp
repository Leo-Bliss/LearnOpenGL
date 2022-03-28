#include "window.h"
#include "model.h"
#include "shader.h"
#include "camera.h"
#include "image.h"
#include <iostream>

namespace Hub
{
	int windowWidth = 800;
	int windowHeight = 600;
	float deltaTime = 0.0f; // 存储上一帧渲染所用时间（当前时间戳-上一帧时间戳）
	float lastFrame = 0.0f; // 上一帧时间戳

	Camera camera;
	glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

	// 实现键盘移动摄像机
	void processInput(GLFWwindow* window)
	{
		const float cameraSpeed = 5.0f * deltaTime; // 保证摄像机移动速度和硬件无关

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			camera.processKeyBoard(CameraMovement::FORWORD, deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			camera.processKeyBoard(CameraMovement::BACKWORD, deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			camera.processKeyBoard(CameraMovement::LEFT, deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			camera.processKeyBoard(CameraMovement::RIGHT, deltaTime);
		}
	}

	// 鼠标控制摄像机
	bool firstMouse = true;
	glm::vec2 lastPos(windowWidth / 2.0f, windowHeight / 2.0f); // window center

	// strangely: todo fix
	void mouse_callback(GLFWwindow* window, double xpos, double ypos)
	{
		if (!(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS))
		{
			return;
		}
		glm::vec2 pos(xpos, ypos);
		if (firstMouse)
		{
			lastPos = pos;
			firstMouse = false;
		}
		GLfloat xoffset = pos.x - lastPos.x;
		GLfloat yoffset = lastPos.y - pos.y; // 由于这里y坐标范围从下往上
		lastPos = pos;
		camera.processMouseMovement(xoffset, yoffset);
	}

	// 滚轮调整摄像机fov, 实现缩放效果
	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		camera.processMouseScroll(yoffset);
	}
	void test()
	{
		Hub::Window hWindow(800, 600);
		auto window = hWindow.getGLWindowIns();

		Image::filpVerticallyOnLoadEnable(true);

		// shader
		Shader ourShader("./shader/shader.vs", "./shader/shader.fs");

		const char* filePath = "../Asset/backpack/backpack.obj";
		Model ourModel(filePath);
		
		glEnable(GL_DEPTH_TEST);
		glfwSetCursorPosCallback(window, mouse_callback);
		glfwSetScrollCallback(window, scroll_callback);

		while (!glfwWindowShouldClose(window))
		{
			GLfloat currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;
			glfwPollEvents();
			processInput(window);

			glClearColor(0.2f, 0.3f, 0.3f, 1.0f); 
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

			// Draw
			ourShader.use(); 
			auto projection = camera.getProjectionMatrix(windowWidth / windowHeight * 1.0f);;
			auto view = camera.getViewMatrix();
			auto model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.f, 0.f, 0.f));
			model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
			ourShader.setMatirx4("projection", projection);
			ourShader.setMatirx4("view", view);
			ourShader.setMatirx4("model", model);

			ourModel.draw(ourShader);

			// swap the screen buffers
			glfwSwapBuffers(window);

		}
		glfwTerminate();
	}
}
int main()
{
	Hub::test();
	return 0;
}



