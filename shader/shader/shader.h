#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glad/glad.h>

class Shader
{
public:
	// 程序ID
	GLuint program;
	// 构造器读取并构建 着色器
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	// 使用程序
	void use();

	void setFloat(const GLchar* key, const GLfloat val);
};