#include "window.h"
#include "image.h"
#include "texture.h"
#include "shader.h"
#include "camera.h"
#include <iostream>
#include <vector>


namespace Hub
{
	int windowWidth = 800;
	int windowHight = 600;
	float deltaTime = 0.0f; // 存储上一帧渲染所用时间（当前时间戳-上一帧时间戳）
	float lastFrame = 0.0f; // 上一帧时间戳

	Camera camera;


	// 实现键盘移动摄像机
	void processInput(GLFWwindow* window)
	{
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
	glm::vec2 lastPos(windowWidth / 2.0f, windowHight / 2.0f); // window center

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
		Hub::Window hWindow(windowWidth, windowHight);
		auto window = hWindow.getNativeHandle();

		// shader
		Shader ourShader("./shader/shader.vs", "./shader/shader.fs");

		// 三角形三个顶点的标准化设备坐标
		// cube
		float vertices[] = {
			// 顶点               // 纹理
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

		GLuint VBO, VAO; // VBO:作为顶点缓冲对象ID; VAO:顶点数组对象ID

		glGenVertexArrays(1, &VAO); // 生成顶点数组对象,包含了绘制某种图形所需要的所有状态
		glGenBuffers(1, &VBO); // 生成顶点缓冲对象

		// 绑定VAO
		glBindVertexArray(VAO);

		// 绑定缓冲类型
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		// 复制顶点数据到缓冲
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// 位置属性
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		// 纹理属性
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLfloat*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);

		// unbind
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// 解绑VAO
		glBindVertexArray(0);

		const char* filePath = "../Asset/container.jpg";
		auto image1 = Image::create(filePath);
		auto texture1 = Texture::create(image1);
		texture1->setWrapping(Wrapping::axis_t::S, Wrapping::wrapping_t::MirroredRepeat);
		texture1->setWrapping(Wrapping::axis_t::T, Wrapping::wrapping_t::MirroredRepeat);
		texture1->setFilter(Filter::operator_t::Mag, Filter::filter_t::Linear);
		texture1->setFilter(Filter::operator_t::Min, Filter::filter_t::Linear);

		filePath = "../Asset/awesomeface.png";
		auto image2 = Image::create(filePath);
		auto texture2 = Texture::create(image2);
		texture2->setWrapping(Wrapping::axis_t::S, Wrapping::wrapping_t::MirroredRepeat);
		texture2->setWrapping(Wrapping::axis_t::T, Wrapping::wrapping_t::MirroredRepeat);
		texture2->setFilter(Filter::operator_t::Mag, Filter::filter_t::Linear);
		texture2->setFilter(Filter::operator_t::Min, Filter::filter_t::Linear);


		ourShader.use(); // 设置shader属性前需要激活程序
		// 绑定纹理
		glActiveTexture(GL_TEXTURE0); // 使用一个纹理时默认激活
		glBindTexture(GL_TEXTURE_2D, *texture1);
		ourShader.setInt("ourTexture1", 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, *texture2);
		ourShader.setInt("ourTexture2", 1);

		// 开启深度测试
		glEnable(GL_DEPTH_TEST);
		glfwSetCursorPosCallback(window, mouse_callback);
		glfwSetScrollCallback(window, scroll_callback);

		std::vector<glm::vec3> positions = {
			{1.0f, 1.0f, -3.0f},
			{-1.7f,  3.0f, -7.5f},
			{1.3f, -2.0f, -2.5f},
			{1.5f,  2.0f, -2.5f},
			{1.5f,  0.2f, -1.5f},
			{-1.3f,  1.0f, -1.5f},
		};

		while (!glfwWindowShouldClose(window)) // 使图像不立即关闭
		{
			glfwPollEvents(); // 检查事件触发：比如键盘输入
			processInput(window);
			float currentFrame = static_cast<float>(glfwGetTime());
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			// 渲染指令
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // 清除颜色缓冲后需要填入的颜色，是一个状态设置函数
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 每次渲染迭代清除颜色缓冲和深度缓冲

			glBindVertexArray(VAO);

			auto count = positions.size();
			for (size_t i = 0; i < count; ++i)
			{
				glm::mat4 model(1.0f);
				model = glm::translate(model, positions[i]);
				auto r = static_cast<float>(glfwGetTime());
				if (i % count >= 3) r = glm::radians(-22.0f * i);
				model = glm::rotate(model, r, glm::vec3(1.0f, 0.4f, 0.5f));
				ourShader.setMatirx4("model", model);
				glDrawArrays(GL_TRIANGLES, 0, 36); // 6 * 2 * 3
			}

			auto view = camera.getViewMatrix();
			ourShader.setMatirx4("view", view);

			glm::mat4 projection;
			projection = glm::perspective(camera.getFov(), windowWidth / windowHight * 1.0f, 0.1f, 100.0f);
			ourShader.setMatirx4("projection", projection);

			glBindVertexArray(0);

			// swap the screen buffers
			glfwSwapBuffers(window);

		}
		// 结束后回收所有分配的资源
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glfwTerminate();
	}
}

int main()
{
	Hub::test();
	return 0;
}