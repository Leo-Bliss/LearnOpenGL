#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glad/glad.h>
#include <glm/glm.hpp>


class Shader
{
public:
	GLuint _programID;

	// 构造器读取并构建 着色器
	Shader(const GLchar* vsPath, const GLchar* fsPath);
	// 使用程序
	void use();

	void checkShaderCompile(const GLuint shader, const GLchar* filePath);
	void checkProgramLink();

	void setFloat(const GLchar* key, const GLfloat val);
	void setVec4(const GLchar* key, const glm::vec4& v);
};