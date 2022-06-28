#include "window.h"
#include "shader.h"
#include "camera.h"
#include "vertex_array.h"
#include "vertex_buffer.h"
#include "texture.h"


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

		Shader shader("./shader/depth.vs", "./shader/depth.fs");

		//cube
		float cubeVertices[] = {
		 // positions          // texture Coords
		 -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		  0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		  0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		  0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		 -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		  0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		  0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		  0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		 -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		 -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		  0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		  0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		  0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		  0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		  0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		  0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		  0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		  0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		  0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		 -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		  0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		  0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		  0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		 -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
		};
		// plane
		float planeVertices[] = {
		// positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
		 5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
		-5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
		-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

		 5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
		-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
		 5.0f, -0.5f, -5.0f,  2.0f, 2.0f
		};

		auto cubeVAO = VertexArray::create();
		auto cubeVBO = VertexBuffer::create(cubeVertices, sizeof(cubeVertices), BufferUsage::StaticDraw);
		cubeVAO->bindAttribute(0, 3, *cubeVBO, Type::Float, 5 * sizeof(float), 0 * sizeof(float));
		cubeVAO->bindAttribute(1, 2, *cubeVBO, Type::Float, 5 * sizeof(float), 3 * sizeof(float));

		auto planeVAO = VertexArray::create();
		auto planeVBO = VertexBuffer::create(planeVertices, sizeof(planeVertices), BufferUsage::StaticDraw);
		planeVAO->bindAttribute(0, 3, *planeVBO, Type::Float, 5 * sizeof(float), 0 * sizeof(float));
		planeVAO->bindAttribute(1, 2, *planeVBO, Type::Float, 5 * sizeof(float), 3 * sizeof(float));
		
		const char* filePath = "../Asset/container2.png";	
		auto cubeTexture = Texture::create(filePath);
		filePath = "../Asset/container.jpg";
		auto planeTexture = Texture::create(filePath);
		auto setTextureConfig = [](SPTexture& t)
		{
			t->setWrapping(Wrapping::axis_t::S, Wrapping::wrapping_t::Repeat);
			t->setWrapping(Wrapping::axis_t::T, Wrapping::wrapping_t::Repeat);
			t->setFilter(Filter::operator_t::Min, Filter::filter_t::LinearMipmapLinear);
			t->setFilter(Filter::operator_t::Mag, Filter::filter_t::Linear);
		};
		setTextureConfig(cubeTexture);
		setTextureConfig(planeTexture);


		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		shader.use();
		shader.setInt("texture1", 0);
		

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
			auto view = camera.getViewMatrix();
			auto projection = camera.getProjectionMatrix(windowWidth / windowHeight * 1.0f);
			shader.setMatirx4("view", view);
			shader.setMatirx4("projection", projection);

			// cube
			glBindVertexArray(*cubeVAO);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, *cubeTexture);
			auto model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
			shader.setMatirx4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(-0.5f, 0.0f, -3.0f));
			shader.setMatirx4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);

			// floor
			glBindVertexArray(*planeVAO);
			glBindTexture(GL_TEXTURE_2D, *planeTexture);
			shader.setMatirx4("model", glm::mat4(1.0f));
			glDrawArrays(GL_TRIANGLES, 0, 6);


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