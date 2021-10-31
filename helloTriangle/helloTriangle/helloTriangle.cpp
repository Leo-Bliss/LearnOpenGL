// GLAD：管理OpenGL的函数指针，解决驱动兼容问题
#include <glad/glad.h>

//GLFW：提供窗口和上下文用来渲染
#include <GLFW/glfw3.h>

#include <iostream>
/*
顶点数组： Vertex Array Object, VAO
顶点缓冲对象：Vertex Buffer Object, VBO
索引缓冲对象： Element Buffer Object, EBO or Index Buffer Onject, IBO
*/
// Shader: 着色器

// 顶点数据 -> 三角形 -> 光栅化 -> 着色

/*
OpenGL图元（有了顶点坐标和颜色，可指定渲染类型，即构成的形状）：
GL_POINT: 点
GL_TRIANGLES: 三角形
GL_lINE_STRIP: 线
*/
// 顶点 -> 图元装配，输出图元形状的点阵 -> 几何着色器，产生新顶点构造新图元生成其他形状
// -> 光栅化，映射成屏幕上对应像素，裁切 -> 片段着色器，计算一个像素最终颜色
// -> 计算深度，检查透明度值alpha并对物体混合

// OpenGL只处理标准化设备坐标，所以要将输入的3D坐标(x,y,z) -> x,y,z处理到[-1.0, 1.0]



void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
const GLuint WIDTH = 800;
const GLuint HEIGHT = 600;

// Shader source
const char *vertexShaderSource = "#version 330 core\n"
		"layout(location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";

const char *fragmentShaderSource = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}\n\0";
//vec4, alpha: 1.0f 完全不透明


int main()
{

	glfwInit();
	// version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // 主版本
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // 次版本
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// GLFW 创建窗口对象
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
	// Viewport: 告诉OpenGL渲染窗口的尺寸大小：视口
	int width = 800;
	int height = 600;
	glViewport(0, 0, width, height);
	// 注册调整窗口大小的回调事件: 视口 随 窗口大小 变化
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	// 键盘输入回调
	glfwSetKeyCallback(window, key_callback);


	// Vertex Shader: 顶点着色器
	GLuint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER); // 创建顶点着色器
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // 着色源码附加到着色对象， 第二个参数指传递的源码字符串数量
	glCompileShader(vertexShader); // 编译着色源码
	// 检测是否编译成功
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);// glGetShader iv,iv可看成information view
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "Error::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// fragment Shader: 片段着色器
	GLuint fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// check compile result 
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGEMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// 创建一个着色器程序: 用于链接shader
	GLuint shaderProgram;
	shaderProgram = glCreateProgram();
	// 将着色器对象附加到着色程序上
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	// 链接程序
	glLinkProgram(shaderProgram);
	// 和检测编译一样检测链接是否成功
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::PROGRAM::LINK::FAILED\n" << infoLog << std::endl;
	}

	// 链接完成删除着色对象
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// 三角形三个顶点的标准化设备坐标
	GLfloat vertices1[] = {
		0.5f, 0.5f, 0.0f, // top right
		0.5f, -0.5f, 0.0f, // bottom right
		-0.5f, 0.5f, 0.0f, // top left

	};

	GLfloat vertices2[] = {
	0.5f, 0.5f, 0.0f, // top right
	0.5f, -0.5f, 0.0f, // bottom right
	-0.5f, 0.5f, 0.0f, // top left

	};


	GLuint VBO[2], VAO[2]; // VBO:作为顶点缓冲对象ID; VAO:顶点数组对象ID
	
	glGenVertexArrays(2, VAO); // 生成顶点数组对象,包含了绘制某种图形所需要的所有状态
	glGenBuffers(2, VBO); // 生成顶点缓冲对象
	

	// first
	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// second
	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // 线框模式绘制
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //设置回默认模式
	while (!glfwWindowShouldClose(window)) // 使图像不立即关闭
	{
		glfwPollEvents(); // 检查事件触发：比如键盘输入
	
		// 渲染指令
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // 清除颜色缓冲后需要填入的颜色，是一个状态设置函数
		glClear(GL_COLOR_BUFFER_BIT); // 清除颜色缓冲，是一个状态使用函数，使得应该清除为上面设置的颜色
		
		// Draw
		glUseProgram(shaderProgram); // 激活着色程序

		glBindVertexArray(VAO[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3); // 0: 顶点起始索引，3绘制个数
		//glBindVertexArray(0);

		glBindVertexArray(VAO[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3); // 0: 顶点起始索引，3绘制个数
		glBindVertexArray(0);

		// swap the screen buffers
		glfwSwapBuffers(window); 

	}
	// 结束后回收所有分配的资源
	glDeleteVertexArrays(2, VAO); // 第一个参数是指数量
	glDeleteBuffers(2, VBO);
	glfwTerminate(); 

	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height); // 前两个参数控制左下角位置
	// OpenGL幕后会对(width, height)进行2D坐标转换，将OpenGL中的位置坐标转换为屏幕坐标
	// OpenGL坐标范围：[-1, 1]，和屏幕坐标建立映射
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) // 输入ESC关闭窗口
	{
		glfwSetWindowShouldClose(window, true);
	}
}



