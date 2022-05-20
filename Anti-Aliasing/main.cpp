#include "window.h"
#include "shader.h"
#include "camera.h"
#include "vertex_array.h"
#include "vertex_buffer.h"
#include "uniform_buffer.h"
#include "texture.h"


namespace Hub
{
	int windowWidth = 800;
	int windowHeight = 600;
	float deltaTime = 0.0f; // 存储上一帧渲染所用时间（当前时间戳-上一帧时间戳）
	float lastFrame = 0.0f; // 上一帧时间戳

	Camera camera;

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
		camera.processMouseScroll(static_cast<float>(yoffset));
	}

	float cubeVertices[] = {
		// positions         
		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,

		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,

		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,

		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
	};

	float quadVertices[] = {   // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
	   // positions   // texCoords
	   -1.0f,  1.0f,  0.0f, 1.0f,
	   -1.0f, -1.0f,  0.0f, 0.0f,
		1.0f, -1.0f,  1.0f, 0.0f,

	   -1.0f,  1.0f,  0.0f, 1.0f,
		1.0f, -1.0f,  1.0f, 0.0f,
		1.0f,  1.0f,  1.0f, 1.0f
	};

	void test()
	{
		glfwWindowHint(GLFW_SAMPLES, 4);
		Window hWindow(windowWidth, windowHeight);
		auto window = hWindow.getGLWindowIns();
		glfwSetCursorPosCallback(window, mouse_callback);
		glfwSetScrollCallback(window, scroll_callback);
		Shader shader("./shader/shader.vs", "./shader/shader.fs");

		auto VAO = VertexArray::create();
		auto VBO = VertexBuffer::create(cubeVertices, sizeof(cubeVertices), BufferUsage::StaticDraw);
		VAO->bindAttribute(0, 3, *VBO, Type::Float, 3 * sizeof(float), 0);


		glEnable(GL_DEPTH_TEST);
		glEnable(GL_MULTISAMPLE);

		while (!hWindow.shouldClose())
		{
			float currentFrame = static_cast<float>(glfwGetTime());
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			glfwPollEvents();
			processInput(window);

			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			auto projection = camera.getProjectionMatrix(windowWidth / windowHeight * 1.0f);
			auto view = camera.getViewMatrix();
	

			glBindVertexArray(*VAO);

			shader.use();
			shader.setMatirx4("projection", projection);
			shader.setMatirx4("view", view);
			auto model = glm::mat4(1.0);
			//model = glm::scale(model, glm::vec3(2, 2, 2));
			shader.setMatirx4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);

			glBindVertexArray(0);
			glfwSwapBuffers(window);
		}
		glfwTerminate();
	}

	void test2()
	{
		Window hWindow(windowWidth, windowHeight);
		auto window = hWindow.getGLWindowIns();
		glfwSetCursorPosCallback(window, mouse_callback);
		glfwSetScrollCallback(window, scroll_callback);
		Shader shader("./shader/shader.vs", "./shader/shader.fs");
		Shader screenShader("./shader/screenShader.vs", "./shader/screenShader.fs");
		
		// cube VAO
		auto cubeVAO = VertexArray::create();
		auto cubeVBO = VertexBuffer::create(cubeVertices, sizeof(cubeVertices), BufferUsage::StaticDraw);
		cubeVAO->bindAttribute(0, 3, *cubeVBO, Type::Float, 3 * sizeof(float), 0);
		
		// screen VAO
		auto quaVAO = VertexArray::create();
		auto quaVBO = VertexBuffer::create(quadVertices, sizeof(quadVertices),BufferUsage::StaticDraw);
		quaVAO->bindAttribute(0, 3, *quaVBO, Type::Float, 5 * sizeof(float), 0);
		quaVAO->bindAttribute(1, 2, *quaVBO, Type::Float, 5 * sizeof(float), 3 * sizeof(float));
		
		// cfg MSAA framebuffer
		unsigned int intermediateFBO;
		glGenFramebuffers(1, &intermediateFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, intermediateFBO);

		// create a color attachment texture
		auto screenTexture = Texture::create();
		screenTexture->image2D(nullptr, Format::RGB, windowWidth, windowHeight, Type::UnsignedByte);

		screenTexture->setFilter(Filter::operator_t::Min, Filter::filter_t::Linear);
		screenTexture->setFilter(Filter::operator_t::Mag, Filter::filter_t::Linear);
		// we only need a clolr buffer
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *screenTexture, 0);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cout << "ERROR::FRAMEBUFFER:: Intermediate framebuffer is not complete!" << std::endl;
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glEnable(GL_DEPTH_TEST);

		// shader cfg
		shader.use();
		screenShader.setInt("screenTexture", 0);

		while (!hWindow.shouldClose())
		{
			float currentFrame = static_cast<float>(glfwGetTime());
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			glfwPollEvents();
			processInput(window);

			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			auto projection = camera.getProjectionMatrix(windowWidth / windowHeight * 1.0f);
			auto view = camera.getViewMatrix();


			glBindVertexArray(*cubeVAO);

			shader.use();
			shader.setMatirx4("projection", projection);
			shader.setMatirx4("view", view);
			auto model = glm::mat4(1.0);
			shader.setMatirx4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);

			glBindVertexArray(0);
			glfwSwapBuffers(window);
		}
		glfwTerminate();
	}

}




int main()
{
	Hub::test2();
	return 0;
}