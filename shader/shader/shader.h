#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glad/glad.h>

class Shader
{
public:
	// ����ID
	GLuint program;
	// ��������ȡ������ ��ɫ��
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	// ʹ�ó���
	void use();

	void setFloat(const GLchar* key, const GLfloat val);
};