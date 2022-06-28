#include "window.h"
#include "shader.h"
#include "camera.h"
#include "vertex_array.h"
#include "vertex_buffer.h"


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

	void testMaterial()
	{
		Window hWindow(windowWidth, windowHeight);
		auto window = hWindow.getNativeHandle();

		Shader lightShader("./shader/light.vs", "./shader/light.fs");
		Shader lampShader("./shader/lamp.vs", "./shader/lamp.fs");

		//cube
		float vertices[] = {
			// 顶点               // 法线
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
			};

			auto VBO = VertexBuffer::create(vertices, sizeof(vertices), BufferUsage::StaticDraw);
			auto cubeVAO = VertexArray::create();
			cubeVAO->bindAttribute(0, 3, *VBO, Type::Float, 6 * sizeof(float), 0 * sizeof(GLfloat));
			cubeVAO->bindAttribute(1, 3, *VBO, Type::Float, 6 * sizeof(float), 3 * sizeof(GLfloat));

			auto lightVAO = VertexArray::create();
			lightVAO->bindAttribute(0, 3, *VBO, Type::Float, 6 * sizeof(float), 0 * sizeof(GLfloat));

			glEnable(GL_DEPTH_TEST);
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

				lightShader.use();
				auto objColor = glm::vec3(1.0f, 0.5f, 0.31f);
				auto lightColor = glm::vec3(1.0f, 1.0f, 1.0f);


				lightColor.x = static_cast<float>(sin(glfwGetTime() * 2.0f));
				lightColor.y = static_cast<float>(sin(glfwGetTime() * 0.7f));
				lightColor.z = static_cast<float>(sin(glfwGetTime() * 1.3f));

				auto diffuseColor = lightColor * glm::vec3(0.5f);
				auto ambientColor = diffuseColor * glm::vec3(0.2f);
				lightShader.setVec3("light.ambient", ambientColor);
				lightShader.setVec3("light.diffuse", diffuseColor);
				lightShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

				lightPos.x = static_cast<float>(2.0f * sin(glfwGetTime()));
				lightPos.y = 0.0f;
				lightPos.z = static_cast<float>(1.5f * cos(glfwGetTime()));
				lightShader.setVec3("light.position", lightPos);
				auto viewPos = camera.getPosition();
				lightShader.setVec3("viewPos", viewPos);

				lightShader.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
				lightShader.setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
				lightShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
				lightShader.setFloat("material.shininess", 32.f);


				auto model = glm::mat4(1.0f);
				auto view = camera.getViewMatrix();
				auto projection = camera.getProjectionMatrix(windowWidth / windowHeight * 1.0f);

				lightShader.setMatirx4("model", model);
				lightShader.setMatirx4("view", view);
				lightShader.setMatirx4("projection", projection);

				glBindVertexArray(*cubeVAO);
				glDrawArrays(GL_TRIANGLES, 0, 36);
				glBindVertexArray(0);

				lampShader.use();
				lampShader.setMatirx4("view", view);
				lampShader.setMatirx4("projection", projection);
				model = glm::mat4(1.0f);
				model = glm::translate(model, lightPos);
				model = glm::scale(model, glm::vec3(0.2f));
				lampShader.setMatirx4("model", model);
				glBindVertexArray(*lightVAO);
				glDrawArrays(GL_TRIANGLES, 0, 36);
				glBindVertexArray(0);

				glfwSwapBuffers(window);
			}
			glfwTerminate();
	}
}

int main()
{
	Hub::testMaterial();
	return 0;
}