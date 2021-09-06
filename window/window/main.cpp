// GLAD：管理OpenGL的函数指针，解决驱动兼容问题
#include <glad/glad.h>
//GLFW：提供窗口和上下文用来渲染
#include <GLFW/glfw3.h>

#include <iostream>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

int main()
{

	glfwInit();
	// version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // 主版本
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // 次版本
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// GLFW 创建窗口对象
	GLFWwindow *window = glfwCreateWindow(800, 600, "helloOpenGL", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create a GLFW window" << std::endl;
		glfwTerminate();
		return -1;

	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to init GLAD" << std::endl;
		return -1;
	}

	// Viewport: 告诉OpenGL渲染窗口的尺寸大小：视口
	glViewport(0, 0, 800, 600);
	// 注册调整窗口大小的回调事件: 视口 随 窗口大小 变化
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	while (!glfwWindowShouldClose(window)) // 使图像不立即关闭
	{
		// 输入
		processInput(window);
		// 渲染指令
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // 清除颜色缓冲后需要填入的颜色，是一个状态设置函数
		glClear(GL_COLOR_BUFFER_BIT); // 清除颜色缓冲，是一个状态使用函数，使得应该清除为上面设置的颜色
		glfwPollEvents(); // 检查事件触发：比如键盘输入
		glfwSwapBuffers(window); // 绘制后加入缓冲
		
	}

	glfwTerminate(); // 结束后回收所有分配的资源

	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height); // 前两个参数控制左下角位置
	// OpenGL幕后会对(width, height)进行2D坐标转换，将OpenGL中的位置坐标转换为屏幕坐标
	// OpenGL坐标范围：[-1, 1]，和屏幕坐标建立映射
}

// 处理键盘输入
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) // 输入ESC关闭窗口
	{
		glfwSetWindowShouldClose(window, true);
	}
}