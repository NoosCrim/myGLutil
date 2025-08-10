#pragma once
#include <cstdio>
#include <unordered_map>
#include <GL/glew.h>
namespace mGLu
{
	class Shader
	{
	private:
		unsigned int *instanceCount = nullptr;
		GLuint ID = 0;
		GLenum type;

	public:
		Shader(){}
		Shader(GLenum type, const char* const code);
		Shader(const Shader& other);
		Shader& operator=(const Shader& other);
		~Shader();
		
		static Shader FromFile(GLenum type, const char* const path);

		inline GLuint GetID() const
		{
			return ID;
		}
		inline operator GLuint() const
		{
			return ID;
		}
		inline operator bool() const
		{
			return ID;
		}
	};

	class ShaderProgram
	{
	private:
		unsigned int *instanceCount = nullptr;
		GLuint ID = 0;
	public:
		ShaderProgram(){}
		ShaderProgram(std::initializer_list<Shader> shaders);
		ShaderProgram(const ShaderProgram& other);
		ShaderProgram& operator=(const ShaderProgram& other);
		~ShaderProgram();

		void Use() const;

		inline GLuint GetID() const
		{
			return ID;
		}
		inline operator GLuint() const
		{
			return ID;
		}
		inline operator bool() const
		{
			return ID;
		}
	};
}