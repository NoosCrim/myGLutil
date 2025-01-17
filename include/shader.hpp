#pragma once
#include <cstdio>
#include <unordered_map>
#include <GL/glew.h>
namespace mGLu
{
	class Window;
	class Shader
	{
		friend class Window;
	private:
		unsigned int *instanceCount = nullptr;
		GLuint ID = 0;

	public:
		Shader();
		Shader(const char* const vsCode, const char* const fsCode, const char* const gsCode = nullptr);
		Shader(const Window &window, const char* const vsCode, const char* const fsCode, const char* const gsCode = nullptr);
		Shader(const Shader& other);
		Shader& operator=(const Shader& other);
		~Shader();

		GLuint GetID() const;
		void Use() const;
	};
}