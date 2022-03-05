#include "window.h"
#include "image.h"
#include "shader.h"
#include <iostream>

int main()
{
	Hub::Window hWindow(800, 600);
	auto window = hWindow.getGLWindowIns();

	// shader
	Shader ourShader("./shader/shader.vs", "./shader/shader.fs");

	// 三角形三个顶点的标准化设备坐标
	GLfloat vertices[] = {
		// 位置             // 颜色            // 纹理
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // 右下
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // 左下
		-0.5f, 0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, // 左上
		 0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.3f, 1.0f, 1.0f, // 右上
		 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.5f, 1.0f, // 顶部
	};
	GLuint indices[] = {
		0,1,2,
		2,3,0
	};

	GLuint VBO, VAO; // VBO:作为顶点缓冲对象ID; VAO:顶点数组对象ID
	GLuint EBO;
	
	glGenVertexArrays(1, &VAO); // 生成顶点数组对象,包含了绘制某种图形所需要的所有状态
	glGenBuffers(1, &VBO); // 生成顶点缓冲对象
	glGenBuffers(1, &EBO);

	// 绑定VAO
	glBindVertexArray(VAO);

	// 绑定缓冲类型
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// 复制顶点数据到缓冲
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
	// 位置属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// 颜色属性
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat))); // 前面3个为位置，偏移3*..
	glEnableVertexAttribArray(1);

	// 纹理属性
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLfloat*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// 解绑VAO
	glBindVertexArray(0);

	// 生成纹理
	GLuint texture1;
	glGenTextures(1, &texture1); // 第一个表示生成纹理数量，第二个为纹理存储单元，多个则使用GLuint数组存储
	glBindTexture(GL_TEXTURE_2D, texture1);

	// 纹理环绕方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	// 纹理过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// 加载纹理图片
	const char* filePath = "../Asset/container.jpg";
	Image image1(filePath);
	if (image1.loadSuccessfully())
	{
		// 纹理对象附加纹理图片
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image1.getWidth(), image1.getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, image1.getRawIns());
		glGenerateMipmap(GL_TEXTURE_2D); // 自动生成多级渐远纹理，从而不需要我们不断向glTexImage2D传入不同级别
	}
	// 生成纹理和对应多级纹理后： 释放内存和解绑纹理对象
	glBindTexture(GL_TEXTURE_2D, 0);

	
	// 生成纹理
	GLuint texture2;
	glGenTextures(1, &texture2); 
	glBindTexture(GL_TEXTURE_2D, texture2);

	// 纹理环绕方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	// 纹理过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	filePath = "../Asset/awesomeface.png"; // 注意这张图带透明值，后面要使用GL_RGBA存储纹理
	Image image2(filePath);
	if (image2.loadSuccessfully())
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image2.getWidth(), image2.getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image2.getRawIns());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	// 生成纹理和对应多级纹理后： 释放内存和解绑纹理对象
	glBindTexture(GL_TEXTURE_2D, 0);




	while (!glfwWindowShouldClose(window)) // 使图像不立即关闭
	{
		glfwPollEvents(); // 检查事件触发：比如键盘输入

		// 渲染指令
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // 清除颜色缓冲后需要填入的颜色，是一个状态设置函数
		glClear(GL_COLOR_BUFFER_BIT); // 清除颜色缓冲，是一个状态使用函数，使得应该清除为上面设置的颜色

		// 绑定纹理
		glActiveTexture(GL_TEXTURE0); // 使用一个纹理时默认激活
		glBindTexture(GL_TEXTURE_2D, texture1);
		ourShader.setInt("ourTexture1", 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		ourShader.setInt("ourTexture2", 1);
		

		// Draw
		ourShader.use(); // 激活着色程序
		glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 3); // 0: 顶点起始索引，3绘制个数
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // 6个点，索引类型为unsigned int, offset = 0
		glBindVertexArray(0);

		// swap the screen buffers
		glfwSwapBuffers(window);

	}
	// 结束后回收所有分配的资源
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glfwTerminate();

	return 0;
}



