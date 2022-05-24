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

	bool blinn = false;
	bool blinnKeyPressed = false;

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
		if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS && !blinnKeyPressed)
		{
			blinn = !blinn;
			blinnKeyPressed = true;
		}
		if (glfwGetKey(window, GLFW_KEY_B) == GLFW_RELEASE)
		{
			blinnKeyPressed = false;
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

	void test()
	{
		Window hWindow(windowWidth, windowHeight);
		auto window = hWindow.getGLWindowIns();
		glfwSetCursorPosCallback(window, mouse_callback);
		glfwSetScrollCallback(window, scroll_callback);

		Shader shader("./shader/point.vs", "./shader/point.fs");

		float planeVertices[] = {
			// positions            // normals         // texcoords
			 10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
			-10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
			-10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,

			 10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
			-10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,
			 10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,  10.0f, 10.0f
		};

		auto VAO = VertexArray::create();
		auto VBO = VertexBuffer::create(planeVertices, sizeof(planeVertices), BufferUsage::StaticDraw);

		VAO->bindAttribute(0, 3, *VBO, Type::Float, 8 * sizeof(float), 0);
		VAO->bindAttribute(1, 3, *VBO, Type::Float, 8 * sizeof(float), 3 * sizeof(float));
		VAO->bindAttribute(2, 2, *VBO, Type::Float, 8 * sizeof(float), 6 * sizeof(float));

		const char* filePath = "../Asset/wood.jpg";
		auto floorTexture = Texture::create(filePath);
		floorTexture->setWrapping(Wrapping::axis_t::S, Wrapping::wrapping_t::ClampEdge);
		floorTexture->setWrapping(Wrapping::axis_t::T, Wrapping::wrapping_t::ClampEdge);
		floorTexture->setFilter(Filter::operator_t::Min, Filter::filter_t::LinearMipmapLinear);
		floorTexture->setFilter(Filter::operator_t::Mag, Filter::filter_t::Linear);

		glEnable(GL_DEPTH_TEST);
		
		shader.use();
		shader.setInt("floorTexture", 0);

		glm::vec3 lightPos(0.0f, 0.0f, 0.0f);

		while (!hWindow.shouldClose())
		{
			float currentFrame = static_cast<float>(glfwGetTime());
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			glfwPollEvents();
			processInput(window);

			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			shader.use();
			auto model = glm::mat4(1.0);
			auto view = camera.getViewMatrix();
			auto projection = camera.getProjectionMatrix(windowWidth / windowHeight * 1.0f);
			shader.setMatirx4("model", model);
			shader.setMatirx4("view", view);
			shader.setMatirx4("projection", projection);

			shader.setVec3("viewPos", camera.getPosition());
			shader.setVec3("lightPos", lightPos);
			shader.setInt("blinn", blinn);
			glBindVertexArray(*VAO);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, *floorTexture);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			std::cout << (blinn ? "Blinn-Phong" : "Phong") << std::endl;
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