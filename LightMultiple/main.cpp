#include "window.h"
#include "image.h"
#include "texture.h"
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

	void test()
	{
		Window hWindow(windowWidth, windowHeight);
		auto window = hWindow.getGLWindowIns();

		Shader lightShader("./shader/light.vs", "./shader/light.fs");
		Shader lampShader("./shader/lamp.vs", "./shader/lamp.fs");

		//cube
		float vertices[] = {
			// positions          // normals           // texture coords
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
		};

		const char* filePath = "../Asset/container2.png";
		auto image1 = Image::create(filePath);
		auto diffuseMap = Texture::create(image1);
		diffuseMap->setWrapping(Wrapping::axis_t::S, Wrapping::Repeat);
		diffuseMap->setWrapping(Wrapping::axis_t::T, Wrapping::Repeat);
		diffuseMap->setFilter(Filter::operator_t::Min, Filter::filter_t::LinearMipmapLinear);
		diffuseMap->setFilter(Filter::operator_t::Mag, Filter::filter_t::Linear);

		filePath = "../Asset/container2_specular.png";
		auto image2 = Image::create(filePath);
		auto specularMap = Texture::create(image2);
		specularMap->setWrapping(Wrapping::axis_t::S, Wrapping::wrapping_t::Repeat);
		specularMap->setWrapping(Wrapping::axis_t::T, Wrapping::wrapping_t::Repeat);
		specularMap->setFilter(Filter::operator_t::Min, Filter::filter_t::LinearMipmapLinear);
		specularMap->setFilter(Filter::operator_t::Mag, Filter::filter_t::Linear);

		auto VBO = VertexBuffer::create(vertices, sizeof(vertices), BufferUsage::StaticDraw);
		auto cubeVAO = VertexArray::create();
		cubeVAO->bindAttribute(0, 3, *VBO, Type::Float, 8 * sizeof(float), 0 * sizeof(float));
		cubeVAO->bindAttribute(1, 3, *VBO, Type::Float, 8 * sizeof(float), 3 * sizeof(float));
		cubeVAO->bindAttribute(2, 2, *VBO, Type::Float, 8 * sizeof(float), 6 * sizeof(float));

		auto lightVAO = VertexArray::create();
		lightVAO->bindAttribute(0, 3, *VBO, Type::Float, 8 * sizeof(float), 0 * sizeof(float));

		glEnable(GL_DEPTH_TEST);
		glfwSetCursorPosCallback(window, mouse_callback);
		glfwSetScrollCallback(window, scroll_callback);

		// positions all containers
		std::vector<glm::vec3> cubePositions = {
			glm::vec3(0.0f,  0.0f,  0.0f),
			glm::vec3(2.0f,  5.0f, -15.0f),
			glm::vec3(-1.5f, -2.2f, -2.5f),
			glm::vec3(-3.8f, -2.0f, -12.3f),
			glm::vec3(2.4f, -0.4f, -3.5f),
			glm::vec3(-1.7f,  3.0f, -7.5f),
			glm::vec3(1.3f, -2.0f, -2.5f),
			glm::vec3(1.5f,  2.0f, -2.5f),
			glm::vec3(1.5f,  0.2f, -1.5f),
			glm::vec3(-1.3f,  1.0f, -1.5f)
		};
		// positions of the point lights
		std::vector<glm::vec3> pointLightPositions = {
			glm::vec3(0.7f,  0.2f,  2.0f),
			glm::vec3(2.3f, -3.3f, -4.0f),
			glm::vec3(-4.0f,  2.0f, -12.0f),
			glm::vec3(0.0f,  0.0f, -3.0f)
		};

		while (!hWindow.shouldClose())
		{
			GLfloat currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			glfwPollEvents();
			processInput(window);

			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			auto viewPos = camera.getPosition();
			lightShader.setVec3("viewPos", viewPos);


			lightShader.use();
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, *diffuseMap);
			lightShader.setInt("material.diffuse", 0);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, *specularMap);
			lightShader.setInt("material.specular", 1);

			lightShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
			lightShader.setFloat("material.shininess", 64.f);

			// directional light
			lightShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
			lightShader.setVec3("dirLight.ambient", 0.2f, 0.2f, 0.2f);
			lightShader.setVec3("dirLight.diffuse", 0.5f, 0.5f, 0.5f);
			lightShader.setVec3("dirLight.specular", 1.0f, 1.0f, 1.0f);

			// point lights
			for (auto i = 0; i < 4; ++i)
			{
				auto id = std::to_string(i);
				auto aim = "pointLights[" + id + "].";
				lightShader.setVec3((aim + "position").c_str(), pointLightPositions[i]);

				lightShader.setVec3((aim + "ambient").c_str(), 0.05f, 0.05f, 0.05f);
				lightShader.setVec3((aim + "diffuse").c_str(), 0.8f, 0.8f, 0.8f);
				lightShader.setVec3((aim + "specular").c_str(), 1.0f, 1.0f, 1.0f);

				lightShader.setFloat((aim + "constant").c_str(), 1.0f);
				lightShader.setFloat((aim + "linear").c_str(), 0.09f);
				lightShader.setFloat((aim + "quadratic").c_str(), 0.032f);
			}

			/*lightPos.x = 2.0f * sin(glfwGetTime());
			lightPos.y = 0.0f;
			lightPos.z = 1.5f * cos(glfwGetTime());*/
			// spotLight
			lightShader.setVec3("spotLight.position", viewPos);
			auto spotDir = camera.getFront();
			lightShader.setVec3("spotLight.direction", spotDir);

			lightShader.setVec3("spotLight.ambient", 0.05f, 0.05f, 0.05f);
			lightShader.setVec3("spotLight.diffuse", 0.8f, 0.8f, 0.8f);
			lightShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);

			lightShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
			lightShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(17.5f)));

			lightShader.setFloat("spotLight.constant", 1.0f);
			lightShader.setFloat("spotLight.linear", 0.022f);
			lightShader.setFloat("spotLight.quadratic", 0.019f);
			

			auto model = glm::mat4(1.0f);
			auto view = camera.getViewMatrix();
			auto projection = camera.getProjectionMatrix(windowWidth / windowHeight * 1.0f);

			lightShader.setMatirx4("view", view);
			lightShader.setMatirx4("projection", projection);
			glBindVertexArray(*cubeVAO);
			for (size_t i = 0; i < cubePositions.size(); ++i)
			{
				glm::mat4 model(1.0f);
				model = glm::translate(model, cubePositions[i]);
				float angle = 20.0f * i;
				model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
				lightShader.setMatirx4("model", model);
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
			glBindVertexArray(0);

			lampShader.use();
			lampShader.setMatirx4("view", view);
			lampShader.setMatirx4("projection", projection);
			glBindVertexArray(*lightVAO);
			for (size_t i = 0; i < 4; ++i)
			{
				model = glm::mat4(1.0f);
				model = glm::translate(model, pointLightPositions[i]);
				model = glm::scale(model, glm::vec3(0.2f));
				lampShader.setMatirx4("model", model);
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
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