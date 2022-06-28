#include "window.h"
#include "shader.h"
#include "camera.h"
#include "vertex_array.h"
#include "vertex_buffer.h"
#include <array>


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
		Window hWindow(windowWidth, windowHeight);
		auto window = hWindow.getNativeHandle();

		Shader shader("./shader/shader.vs", "./shader/shader.fs");

		// quad
		float quadVertices[] = {
			// positions     // colors
			-0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
			 0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
			-0.05f, -0.05f,  0.0f, 0.0f, 1.0f,

			-0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
			 0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
			 0.05f,  0.05f,  0.0f, 1.0f, 1.0f
		};

		auto VAO = VertexArray::create();
		auto VBO = VertexBuffer::create(quadVertices, sizeof(quadVertices), BufferUsage::StaticDraw);
		VAO->bindAttribute(0, 2, *VBO, Type::Float, 5 * sizeof(float), 0);
		VAO->bindAttribute(1, 3, *VBO, Type::Float, 5 * sizeof(float), 2 * sizeof(float));

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		// get offsetList
		std::array<glm::vec2, 100> offsets;
		unsigned int index = 0;
		float offset = 0.1f;
		for (int x = -10; x < 10; x += 2)
		{
			for (int y = -10; y < 10; y += 2)
			{
				glm::vec2 t;
				t.x = static_cast<float>(x / 10.0f + offset);
				t.y = static_cast<float>(y / 10.0f + offset);
				offsets[index++] = t;
			}
		}
		
		auto instanceVBO = VertexBuffer::create(&offsets[0], offsets.size() * sizeof(glm::vec2), BufferUsage::StaticDraw );
		VAO->bindAttribute(2, 2, *instanceVBO, Type::Float, sizeof(glm::vec2), 0);
		glVertexAttribDivisor(2, 1);

		glfwSetCursorPosCallback(window, mouse_callback);
		glfwSetScrollCallback(window, scroll_callback);

		while (!hWindow.shouldClose())
		{
			GLfloat currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			glfwPollEvents();
			processInput(window);

			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			shader.use();
			glBindVertexArray(*VAO);
			glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 100);


			glBindVertexArray(0);
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