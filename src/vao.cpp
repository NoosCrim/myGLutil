#include <GL/glew.h>
#include <vector>
#include <cstdio>
#include <string>
#include "common.hpp"
#include "vao.hpp"

namespace mGLu
{
    VAOview::VAOview(int):
        refCount(new unsigned int(1)),
        bindingCount(new GLuint(0))
    {
        glCreateVertexArrays(1, &name);
    }
    VAOview::VAOview()
    {

    }
    VAOview::VAOview(const VAOview& other):
        name(other.name),
        refCount(other.refCount),
        bindingCount(other.bindingCount)
    {

    }
    VAOview& VAOview::operator=(const VAOview& other)
    {
        if(name && --*refCount == 0)
        {
            glDeleteVertexArrays(1, &name);
            delete refCount;
            delete bindingCount;
        }
        name = other.name;
        const_cast<unsigned int*&>(refCount) = other.refCount;
        const_cast<GLuint*&>(bindingCount) = other.bindingCount;
        ++*refCount;
        return *this;
    }
    VAOview::~VAOview()
    {
        if(name && --*refCount == 0)
        {
            glDeleteVertexArrays(1, &name);
            delete refCount;
            delete bindingCount;
        }
    }
    void VAOview::BindBufferToAttrib(GLuint bindingPoint, GLuint vbo, GLintptr offset, GLsizei stride)
    {
        glVertexArrayVertexBuffer(name, bindingPoint, vbo, offset, stride);
    }
    void VAOview::BindElementBuffer(unsigned int ebo)
    {
        glVertexArrayElementBuffer(name, ebo);
    }
    GLuint VAOview::GetName()
    {
        return name;
    }
    GLuint VAOview::GetBindingCount()
    {
        if(!name) return 0;
        return *bindingCount;
    }
    

    VAO::VAO():
        VAOview(0)
    {

    }
    GLuint VAO::AddAttrib(GLenum type, unsigned int size, std::string shaderVarName, unsigned int divisor)
    {
        glEnableVertexArrayAttrib(GetName(), nextIndex); 
        glVertexArrayAttribFormat(GetName(), nextIndex, size, type, GL_FALSE, 0);
        glVertexArrayAttribBinding(GetName(), nextIndex, *bindingCount);
        glVertexArrayBindingDivisor(GetName(), *bindingCount, divisor);
        
        static constexpr unsigned int shaderCodeSize = 255;
        char shaderCode[shaderCodeSize];
        int parseLen = std::snprintf( shaderCode, shaderCodeSize, "layout(location = %u) in %s %s;\n", nextIndex, GetGLSLtype(size, type), shaderVarName.data());
        if(parseLen >= shaderCodeSize)
            fputs("VAO error: parsed shaderCode is to big!\n", stderr);
        shaderPrefix += shaderCode;
        ++nextIndex;
        return (*bindingCount)++;
    }
    GLuint VAO::AddFloatMatAttrib(unsigned int cols, unsigned int rows, std::string shaderVarName, unsigned int divisor)
    {
        for (int i = 0; i < cols; i++) {	   // MODEL TRANSFORM MATRIX
            glEnableVertexArrayAttrib(GetName(), nextIndex + i);
            glVertexArrayAttribFormat(GetName(), nextIndex + i, rows, GL_FLOAT, GL_FALSE, i*4*sizeof(float));
            glVertexArrayAttribBinding(GetName(), nextIndex + i, *bindingCount);
        }
        glVertexArrayBindingDivisor(GetName(), *bindingCount, divisor);

        static constexpr unsigned int shaderCodeSize = 255;
        char shaderCode[255] = "layout(location=LLLL) T N;";
        int parseLen = std::snprintf( shaderCode, shaderCodeSize, "layout(location = %u) in mat%ux%u %s;\n", nextIndex, cols, rows, shaderVarName.data());
        if(parseLen >= shaderCodeSize)
            fputs("VAO error: parsed shaderCode is to big!\n", stderr);
        shaderPrefix += shaderCode;
        nextIndex += rows;
        
        return (*bindingCount)++;
    }
    GLuint VAO::AddDoubleMatAttrib(unsigned int cols, unsigned int rows, std::string shaderVarName, unsigned int divisor)
    {
        for (int i = 0; i < rows; i++) {	   // MODEL TRANSFORM MATRIX
            glEnableVertexArrayAttrib(GetName(), nextIndex + i);
            glVertexArrayAttribFormat(GetName(), nextIndex + i, cols, GL_DOUBLE, GL_FALSE, i*4*sizeof(float));
            glVertexArrayAttribBinding(GetName(), nextIndex + i, *bindingCount);
        }
        glVertexArrayBindingDivisor(GetName(), *bindingCount, divisor);

        static constexpr unsigned int shaderCodeSize = 255;
        char shaderCode[255] = "layout(location=LLLL) T N;";
        
        int parseLen = std::snprintf( shaderCode, shaderCodeSize, "layout(location = %u) in dmat%ux%u %s;\n", nextIndex, cols, rows, shaderVarName.data());
        if(parseLen >= shaderCodeSize)
            fputs("VAO error: parsed shaderCode is to big!\n", stderr);
        shaderPrefix += shaderCode;
        nextIndex += rows;
        
        return (*bindingCount)++;
    }
    const std::string& VAO::GetShaderPrefix()
    {
        return shaderPrefix;
    }
}
