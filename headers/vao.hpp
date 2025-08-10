#pragma once
#include <GL/glew.h>
#include <vector>
#include <cstdio>
#include <array>
#include <string>
#include "common.hpp"

namespace mGLu
{
    class VAOview
    {
        unsigned int *const refCount = nullptr;
        GLuint name = 0;
    protected:
        GLuint *const bindingCount = nullptr;
        VAOview(int);
    public:
        VAOview();
        VAOview(const VAOview& other);
        VAOview& operator=(const VAOview& other);
        virtual ~VAOview();
        void BindBufferToAttrib(GLuint bindingPoint, GLuint vbo, GLintptr offset, GLsizei stride);
        void BindElementBuffer(unsigned int ebo);
        GLuint GetName();
        GLuint GetBindingCount();
    };
    class VAO : public VAOview
    {
        GLuint nextIndex= 0;
    public:
        VAO();
        GLuint AddAttrib(GLenum type, unsigned int size, std::string shaderVarName, unsigned int divisor = 0);
        GLuint AddFloatMatAttrib(unsigned int cols, unsigned int rows, std::string shaderVarName, unsigned int divisor);
        GLuint AddDoubleMatAttrib(unsigned int cols, unsigned int rows, std::string shaderVarName, unsigned int divisor);
    };
}
