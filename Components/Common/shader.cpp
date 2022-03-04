#include "shader.h"

Shader::Shader(const GLchar* vsPath, const GLchar* fsPath)
{
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	// 保证ifstream对象可以抛出异常；
	vShaderFile.exceptions(std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::badbit);
	try {
		// 打开文件
		vShaderFile.open(vsPath);
		fShaderFile.open(fsPath);
		// 读取文件到缓冲内容到流中
		std::stringstream vShaderStream, fShaderStream;
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// 关闭文件
		vShaderFile.close();
		fShaderFile.close();
		// 转换至GLchar数组
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCEDDFULLY_READ" << std::endl;
	}
	const GLchar* vShaderCode = vertexCode.c_str();
	const GLchar* fShaderCode = fragmentCode.c_str();

	GLuint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER); 
	glShaderSource(vertexShader, 1, &vShaderCode, NULL); // 着色源码附加到着色对象， 第二个参数指传递的源码字符串数量
	glCompileShader(vertexShader);
	checkShaderCompile(vertexShader, vsPath);
	
	GLuint fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
	glCompileShader(fragmentShader);
	checkShaderCompile(fragmentShader, fsPath);
	
	// 创建一个着色器程序: 用于链接shader
	_programID = glCreateProgram();

	// 将着色器对象附加到着色程序上
	glAttachShader(_programID, vertexShader);
	glAttachShader(_programID, fragmentShader);
	glLinkProgram(_programID);
	checkProgramLink();
	
	// 链接完成删除着色对象
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::use()
{
	glUseProgram(_programID);
}

void Shader::checkShaderCompile(const GLuint shader, const GLchar* filePath)
{
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);// iv: info view
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "Error::SHADER COMPILE, FileInfo:" << filePath<<", " << infoLog << std::endl;
	}
}

void Shader::checkProgramLink()
{
	GLint success;
	GLchar infoLog[512];
	glGetProgramiv(_programID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(_programID, 512, NULL, infoLog);
		std::cout << "ERROR::PROGRAM::LINK::FAILED\n" << infoLog << std::endl;
	}
}

void Shader::setFloat(const GLchar* key, const GLfloat val)
{
	GLint keyLocation = glGetUniformLocation(_programID, key);
	// std::cout <<key<<" getlocal res = "<< keyLocation << std::endl;
	glUniform1f(keyLocation, val);
}

void Shader::setVec4(const GLchar* key, const glm::vec4& v)
{
	GLint keyLocation = glGetUniformLocation(_programID, key);
	glUniform4f(keyLocation, v.x, v.y, v.z, v.w);
}

void Shader::setInt(const GLchar* key, const GLint val)
{
	GLint keyLocation = glGetUniformLocation(_programID, key);
	glUniform1i(keyLocation, val);
}


