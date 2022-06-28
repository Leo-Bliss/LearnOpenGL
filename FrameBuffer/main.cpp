#include "window.h"
#include "shader.h"
#include "camera.h"
#include "vertex_array.h"
#include "vertex_buffer.h"
#include "frame_buffer.h"
#include "render_buffer.h"
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
		Shader screenShader("./shader/framebuffer_screen.vs", "./shader/framebuffer_screen.fs");

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

		float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
		};

		auto cubeVAO = VertexArray::create();
		auto cubeVBO = VertexBuffer::create(cubeVertices, sizeof(cubeVertices), BufferUsage::StaticDraw);
		cubeVAO->bindAttribute(0, 3, *cubeVBO, Type::Float, 5 * sizeof(float), 0 * sizeof(float));
		cubeVAO->bindAttribute(1, 2, *cubeVBO, Type::Float, 5 * sizeof(float), 3 * sizeof(float));

		auto planeVAO = VertexArray::create();
		auto planeVBO = VertexBuffer::create(planeVertices, sizeof(planeVertices), BufferUsage::StaticDraw);
		planeVAO->bindAttribute(0, 3, *planeVBO, Type::Float, 5 * sizeof(float), 0 * sizeof(float));
		planeVAO->bindAttribute(1, 2, *planeVBO, Type::Float, 5 * sizeof(float), 3 * sizeof(float));
		

		auto quadVAO = VertexArray::create();
		auto quadVBO = VertexBuffer::create(quadVertices, sizeof(quadVertices), BufferUsage::StaticDraw);
		quadVAO->bindAttribute(0, 2, *quadVBO, Type::Float, 4 * sizeof(float), 0 * sizeof(float));
		quadVAO->bindAttribute(1, 2, *quadVBO, Type::Float, 4 * sizeof(float), 2 * sizeof(float));

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

		shader.use();
		shader.setInt("texture1", 0);
		screenShader.use();
		screenShader.setInt("screenTexture", 0);
		
		// framebuffer config
		auto FBO = FrameBuffer::create();
		glBindFramebuffer(GL_FRAMEBUFFER, *FBO);
		// create a color attachment texture
		auto textureColorbuffer = Texture::create();
		textureColorbuffer->image2D(nullptr, Format::format_t::RGBA, windowWidth, windowHeight, Type::UnsignedByte);
		textureColorbuffer->setFilter(Filter::operator_t::Min, Filter::filter_t::Linear);
		textureColorbuffer->setFilter(Filter::operator_t::Mag, Filter::filter_t::Linear);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *textureColorbuffer, 0);
		// create a render object for depth and stencil attachment(we don't sampling there)
		auto RBO = RenderBuffer::create();
		glBindRenderbuffer(GL_RENDERBUFFER, *RBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, windowWidth, windowHeight);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, *RBO);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cout << "ERROR::FRAMEBUFFER:: framebuffer is not complete!" << std::endl;
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glfwSetCursorPosCallback(window, mouse_callback);
		glfwSetScrollCallback(window, scroll_callback);

		while (!hWindow.shouldClose())
		{
			GLfloat currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			glfwPollEvents();
			processInput(window);
			// bind to framebuffer and draw scene as we normally would to color texture
			glBindFramebuffer(GL_FRAMEBUFFER, *FBO);

			glEnable(GL_DEPTH_TEST); //enable depth testing(is disabled for rendering screen-space quad)
			// make sure we clear the framebuffer's content
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
			model = glm::translate(model, glm::vec3(1.0f, 0.0f, -3.0f));
			shader.setMatirx4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);

			// floor
			glBindVertexArray(*planeVAO);
			glBindTexture(GL_TEXTURE_2D, *planeTexture);
			shader.setMatirx4("model", glm::mat4(1.0f));
			glDrawArrays(GL_TRIANGLES, 0, 6);

			// now bind back to default framebuffer and draw a quad plane with attached framebuffer color texture
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glDisable(GL_DEPTH_TEST);
			glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			screenShader.use();
			//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glBindVertexArray(*quadVAO);
			glBindTexture(GL_TEXTURE_2D, *textureColorbuffer); // use the color attachment as the texture of the quad plane
			glDrawArrays(GL_TRIANGLES, 0, 6);

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