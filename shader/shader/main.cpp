

// GLAD������OpenGL�ĺ���ָ�룬���������������
#include <glad/glad.h>

//GLFW���ṩ���ں�������������Ⱦ
#include <GLFW/glfw3.h>

#include <iostream>

// ʹ����ɫ����Shader
#include "shader.h"

/*
�������飺 Vertex Array Object, VAO
���㻺�����Vertex Buffer Object, VBO
����������� Element Buffer Object, EBO or Index Buffer Onject, IBO
*/
// Shader: ��ɫ��

// �������� -> ������ -> ��դ�� -> ��ɫ

/*
OpenGLͼԪ�����˶����������ɫ����ָ����Ⱦ���ͣ������ɵ���״����
GL_POINT: ��
GL_TRIANGLES: ������
GL_lINE_STRIP: ��
*/
// ���� -> ͼԪװ�䣬���ͼԪ��״�ĵ��� -> ������ɫ���������¶��㹹����ͼԪ����������״
// -> ��դ����ӳ�����Ļ�϶�Ӧ���أ����� -> Ƭ����ɫ��������һ������������ɫ
// -> ������ȣ����͸����ֵalpha����������

// OpenGLֻ�����׼���豸���꣬����Ҫ�������3D����(x,y,z) -> x,y,z����[-1.0, 1.0]

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
const GLuint WIDTH = 800;
const GLuint HEIGHT = 600;


int main()
{

	glfwInit();
	// version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // ���汾
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // �ΰ汾
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// GLFW �������ڶ���
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Triangle", nullptr, nullptr);
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
	int width = 800;
	int height = 600;
	glViewport(0, 0, width, height);
	// ע��������ڴ�С�Ļص��¼�: �ӿ� �� ���ڴ�С �仯
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	// ��������ص�
	glfwSetKeyCallback(window, key_callback);


	// shader
	Shader ourShader("./shader.vs", "./shader.frag");
	GLfloat offset = 15.f;
	ourShader.setFloat("xOffset", offset);

	// ��������������ı�׼���豸����
	GLfloat vertices[] = {
		// λ��             // ��ɫ
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // ����
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // ����
		 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f  // ����
	};

	GLuint VBO, VAO; // VBO:��Ϊ���㻺�����ID; VAO:�����������ID

	glGenVertexArrays(1, &VAO); // ���ɶ����������,�����˻���ĳ��ͼ������Ҫ������״̬
	glGenBuffers(1, &VBO); // ���ɶ��㻺�����

	// ��VAO
	glBindVertexArray(VAO);
	// �󶨻�������
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// ���ƶ������ݵ�����
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// �Կ�����ָ������
	// ���������� ������ ���ã� GL_STATIC_DRAW
	// ���ݻ�ı�ܶ� ���ã� GL_DYNAMIC_DRAW 
	// ����ÿ�λ���ʱ���ı� ���ã� GL_STREAM_DRAW

	//// ����OpenGL����ν�����������
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	//// 0�� ��vertexShader��ʹ�õ�layout(location = 0)��pos��������λ��ֵ��location��
	//// 3�� ָ����������ֵ��С�� it is a vec3,so value is 3
	//// GL_FLOAT: ָ����������
	//// GL_FALSE: �Ƿ�����ݱ�׼���� �����GL_TRUE�ᱻӳ�䵽[-1, 1]
	//// 3 * sizeof(GLfloat)�� ����������ÿ������vec3,vec3��������ΪGLfloat
	//// (void*)0�� ��Ҫǿ������ת������ʶλ�������ڻ����е���ʼλ��ƫ������offset��,λ�����ݾ��ڿ�ͷ��so is 0

	//// ������������������Ϊ������������Ĭ���ǽ��õ�
	//glEnableVertexAttribArray(0);

	// λ������
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// ��ɫ����
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat))); // ǰ��3��Ϊλ�ã�ƫ��3*..
	glEnableVertexAttribArray(1);

	// unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// ���VAO
	glBindVertexArray(0);




	while (!glfwWindowShouldClose(window)) // ʹͼ�������ر�
	{
		glfwPollEvents(); // ����¼������������������

		// ��Ⱦָ��
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // �����ɫ�������Ҫ�������ɫ����һ��״̬���ú���
		glClear(GL_COLOR_BUFFER_BIT); // �����ɫ���壬��һ��״̬ʹ�ú�����ʹ��Ӧ�����Ϊ�������õ���ɫ

		// Draw
		ourShader.use(); // ������ɫ����
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		// swap the screen buffers
		glfwSwapBuffers(window);

	}
	// ������������з������Դ
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate();

	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height); // ǰ���������������½�λ��
	// OpenGLĻ����(width, height)����2D����ת������OpenGL�е�λ������ת��Ϊ��Ļ����
	// OpenGL���귶Χ��[-1, 1]������Ļ���꽨��ӳ��
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) // ����ESC�رմ���
	{
		glfwSetWindowShouldClose(window, true);
	}
}



