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

	void test()
	{
		Window hWindow(windowWidth, windowHeight);
		auto window = hWindow.getGLWindowIns();
		glfwSetCursorPosCallback(window, mouse_callback);
		glfwSetScrollCallback(window, scroll_callback);

		Shader pointShader("./shader/point.vs", "./shader/point.fs");

		float position[] = {
			 0.5f, -0.5f, -10.0f,  // 右下
			-0.5f, -0.5f, -5.0f,  // 左下
			-0.5f, 0.5f,  -1.0f,  // 左上

			//-0.5f, 0.5f,  0.0f,  // 左上
			 0.5f,  0.5f, 0.2f,  // 右上
			 //0.5f, -0.5f, 0.0f,  // 右下
			
		};

		float color[] = {
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 1.0f,

			//0.0f, 0.0f, 1.0f,
			1.0f, 1.0f, 0.3f,
			//1.0f, 0.0f, 0.0f,
			
		};


		auto VAO = VertexArray::create();
		auto VBO = VertexBuffer::create(nullptr, sizeof(position) + sizeof(color), BufferUsage::StaticDraw);
		glCheckError();
		// need to allocate memory by calling glBufferData before setSubData 
		VBO->subData(position, 0, sizeof(position));
		VBO->subData(color, sizeof(position), sizeof(color));
		glCheckError();

		VAO->bindAttribute(0, 3, *VBO, Type::Float, 3 * sizeof(float), 0);
		VAO->bindAttribute(1, 3, *VBO, Type::Float, 3 * sizeof(float), sizeof(position));

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_PROGRAM_POINT_SIZE);

		while (!hWindow.shouldClose())
		{
			float currentFrame = static_cast<float>(glfwGetTime());
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			glfwPollEvents();
			processInput(window);

			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			pointShader.use();
			auto model = glm::mat4(1.0);
			auto view = camera.getViewMatrix();
			auto projection = camera.getProjectionMatrix(windowWidth / windowHeight * 1.0f);
			pointShader.setMatirx4("model", model);
			pointShader.setMatirx4("view", view);
			pointShader.setMatirx4("projection", projection);

			glBindVertexArray(*VAO);
			glDrawArrays(GL_POINTS, 0, 4);

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
		//cube
		float cubeVertices[] = {
			// positions          // texture Coords
			// Back face
		   -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Bottom-left
			0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
			0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // bottom-right         
			0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
		   -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom-left
		   -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
		   // Front face
		   -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
			0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
			0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
			0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
		   -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // top-left
		   -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
		   // Left face
		   -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
		   -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-left
		   -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
		   -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
		   -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
		   -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
		   // Right face
			0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
			0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
			0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right         
			0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
			0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
			0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left     
		   // Bottom face
		   -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
			0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // top-left
			0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
			0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
		   -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
		   -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
		   // Top face
		   -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
			0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
			0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right     
			0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
		   -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
		   -0.5f,  0.5f,  0.5f,  0.0f, 0.0f  // bottom-left     
		};

		auto VAO = VertexArray::create();
		auto VBO = VertexBuffer::create(cubeVertices, sizeof(cubeVertices), BufferUsage::StaticDraw);
		VAO->bindAttribute(0, 3, *VBO, Type::Float, 5 * sizeof(float), 0);
		VAO->bindAttribute(1, 2, *VBO, Type::Float, 5 * sizeof(float), 3 * sizeof(float));
		
		const char* frontTexFile = "../Asset/container2.png";
		const char* backTexFile = "../Asset/container.jpg";
		auto frontTex = Texture::create(frontTexFile);
		auto backTex = Texture::create(backTexFile);
		auto setTextureConfig = [](SPTexture& t)
		{
			t->setWrapping(Wrapping::axis_t::S, Wrapping::wrapping_t::Repeat);
			t->setWrapping(Wrapping::axis_t::T, Wrapping::wrapping_t::Repeat);
			t->setFilter(Filter::operator_t::Min, Filter::filter_t::LinearMipmapLinear);
			t->setFilter(Filter::operator_t::Mag, Filter::filter_t::Linear);
		};
		setTextureConfig(frontTex);
		setTextureConfig(backTex);

		shader.use();
		shader.setInt("frontTexture", 0);
		shader.setInt("backTexture", 1);

		glEnable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);

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

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, *frontTex);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, *backTex);
			glBindVertexArray(*VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);

			glBindVertexArray(0);
			glfwSwapBuffers(window);
		}
		glfwTerminate();
	}

	void test3()
	{
		Window hWindow(windowWidth, windowHeight);
		auto window = hWindow.getGLWindowIns();
		glfwSetCursorPosCallback(window, mouse_callback);
		glfwSetScrollCallback(window, scroll_callback);
		Shader redShader("./shader/common.vs", "./shader/red.fs");
		Shader greenShader("./shader/common.vs", "./shader/green.fs");
		Shader blueShader("./shader/common.vs", "./shader/blue.fs");
		Shader yellowShader("./shader/common.vs", "./shader/yellow.fs");

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
		auto VAO = VertexArray::create();
		auto VBO = VertexBuffer::create(cubeVertices, sizeof(cubeVertices), BufferUsage::StaticDraw);
		VAO->bindAttribute(0, 3, *VBO, Type::Float, 3 * sizeof(float), 0);


		// link each shader's uniform block to this uniform binding point
		redShader.bindUniformBlock("Matrices", 0);
		greenShader.bindUniformBlock("Matrices", 0);
		blueShader.bindUniformBlock("Matrices", 0);
		yellowShader.bindUniformBlock("Matrices", 0);

		// create uniform buffer
		auto UBO = UniformBuffer::create(nullptr, 2 * sizeof(glm::mat4), BufferUsage::StaticDraw);
		// define the range of buffer that links to a uniform binding point
		UBO->bindBufferRange(0, 0, 2 * sizeof(glm::mat4));

		glEnable(GL_DEPTH_TEST);

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
			UBO->subData(glm::value_ptr(projection), 0, sizeof(glm::mat4));
			auto view = camera.getViewMatrix();
			UBO->subData(glm::value_ptr(view), sizeof(glm::mat4), sizeof(glm::mat4));

			glBindVertexArray(*VAO);

			redShader.use();
			auto model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-0.75f, 0.75f, 0.0f));//top left
			redShader.setMatirx4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);

			greenShader.use();
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(0.75f, 0.75f, 0.0f)); // top right
			greenShader.setMatirx4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);

			blueShader.use();
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-0.75f, -0.75f, 0.0f)); // bottom left
			blueShader.setMatirx4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);

			yellowShader.use();
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(0.75f, -0.75f, 0.0f)); // bottom right
			yellowShader.setMatirx4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);

			glBindVertexArray(0);
			glfwSwapBuffers(window);
		}
		glfwTerminate();
	}

}




int main()
{
	Hub::test3();
	return 0;
}