#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <cstring>
#include <string>

#include "common.hpp"
#include "shader.hpp"

mGLu::Shader::Shader(const Shader& other_) :
	ID(other_.ID),
	type(other_.type)
{
	if(instanceCount)
		++*instanceCount;
}
mGLu::Shader::Shader(GLenum type_, const char* const code_):
	type(type_)
{
	GLint compileStatus, logLen;
	ID = glCreateShader(type);
	glShaderSource(ID, 1, &code_, NULL);
	glCompileShader(ID);
	glGetShaderiv(ID, GL_COMPILE_STATUS, &compileStatus);
	glGetShaderiv(ID, GL_INFO_LOG_LENGTH, &logLen);
	if (!compileStatus)
	{
		char log[logLen + 1];
		glGetShaderInfoLog(ID, logLen + 1, 0, log);
		std::fprintf(stderr, "Fragment Shader Compilation Error: %s\n", log);
		return;
	}
}
mGLu::Shader& mGLu::Shader::operator=(const Shader& other_)
{
	if(instanceCount)
		--*instanceCount;

	instanceCount = other_.instanceCount;
	ID = other_.ID;
	type = other_.type;

	if(instanceCount)
		++*instanceCount;
	
	return *this;
}
mGLu::Shader::~Shader()
{
	if (!instanceCount)
		return;
	if(--*instanceCount == 0)
	{
		glDeleteShader(ID);
		delete instanceCount;
	}
	
}

mGLu::Shader mGLu::Shader::FromFile(GLenum type_, const char* const path_)
{
	std::string code;
	if(!ReadTxtFile(path_, code))
		return Shader();
	return Shader(type_, code.c_str());
}

mGLu::ShaderProgram::ShaderProgram(std::initializer_list<Shader> shaders_)
{
	ID = glCreateProgram();
	for(const Shader &shader : shaders_)
	{
		glAttachShader(ID, shader);
	}
	glLinkProgram(ID);

	GLint linkStatus, logLen;
	glGetProgramiv(ID, GL_LINK_STATUS, &linkStatus);
	glGetProgramiv(ID, GL_INFO_LOG_LENGTH, &logLen);
	if (!linkStatus)
	{
		glDeleteProgram(ID);
		ID = 0;
		char log[logLen + 1];
		glGetProgramInfoLog(ID, logLen + 1, 0, log);
		std::fprintf(stderr, "Shader Linking Error: %s\n", log);
	}

	if(ID)
	{
		instanceCount = new GLuint(1);
	}
}

mGLu::ShaderProgram::ShaderProgram(const ShaderProgram& other_) : 
	ID(other_.ID)
{
	if(instanceCount)
		++*instanceCount;
}

mGLu::ShaderProgram& mGLu::ShaderProgram::operator=(const ShaderProgram& other_)
{
	if(instanceCount)
		--*instanceCount;

	instanceCount = other_.instanceCount;
	ID = other_.ID;

	if(instanceCount)
		++*instanceCount;
	
	return *this;
}
mGLu::ShaderProgram::~ShaderProgram()
{
	if (!instanceCount)
		return;
	if(--*instanceCount == 0)
	{
		glDeleteProgram(ID);
		delete instanceCount;
	}
	
}

void mGLu::ShaderProgram::Use() const
{
	glUseProgram(ID);
}
