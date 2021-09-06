// GLAD������OpenGL�ĺ���ָ�룬���������������
#include <glad/glad.h>
//GLFW���ṩ���ں�������������Ⱦ
#include <GLFW/glfw3.h>

#include <iostream>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

int main()
{

	glfwInit();
	// version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // ���汾
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // �ΰ汾
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// GLFW �������ڶ���
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

	// Viewport: ����OpenGL��Ⱦ���ڵĳߴ��С���ӿ�
	glViewport(0, 0, 800, 600);
	// ע��������ڴ�С�Ļص��¼�: �ӿ� �� ���ڴ�С �仯
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	while (!glfwWindowShouldClose(window)) // ʹͼ�������ر�
	{
		// ����
		processInput(window);
		// ��Ⱦָ��
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // �����ɫ�������Ҫ�������ɫ����һ��״̬���ú���
		glClear(GL_COLOR_BUFFER_BIT); // �����ɫ���壬��һ��״̬ʹ�ú�����ʹ��Ӧ�����Ϊ�������õ���ɫ
		glfwPollEvents(); // ����¼������������������
		glfwSwapBuffers(window); // ���ƺ���뻺��
		
	}

	glfwTerminate(); // ������������з������Դ

	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height); // ǰ���������������½�λ��
	// OpenGLĻ����(width, height)����2D����ת������OpenGL�е�λ������ת��Ϊ��Ļ����
	// OpenGL���귶Χ��[-1, 1]������Ļ���꽨��ӳ��
}

// �����������
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) // ����ESC�رմ���
	{
		glfwSetWindowShouldClose(window, true);
	}
}