#pragma once
#include <GL/glew.h>
#include <cstdio>
namespace mGLu
{
    class Buffer
    {
        unsigned int *const refCount = nullptr;
    protected:
        GLsizeiptr *const size = 0;
        const GLuint name = 0;
        Buffer(GLsizeiptr bufferSize);
    public:
        Buffer();
        Buffer(const Buffer& other);
        Buffer& operator=(const Buffer& other);
        virtual ~Buffer();
        inline bool IsDefined()
        {
            return name;
        }
        inline GLuint GetName()
        {
            return name;
        }
        inline GLsizeiptr GetSize()
        {
            return *size;
        }
        bool SetData(GLintptr offset, GLsizeiptr dataSize, const void* data);

        template<typename T>
        bool SetData(GLintptr elemOffset, GLsizeiptr dataElemCount, const T* data)
        {
            return SetData(sizeof(T[elemOffset]), dataElemCount * sizeof(T), (const void*)data);
        }
    };
    class FixedBuffer : public Buffer
    {
    public:
        FixedBuffer();
        FixedBuffer(const Buffer& other);
        FixedBuffer& operator=(const Buffer& other);
        FixedBuffer(GLsizeiptr bufferSize, const void* data, GLbitfield flags = 0);
        
        template<typename T>
        FixedBuffer(unsigned int elemCount, const T* data, GLbitfield flags = 0) : 
            FixedBuffer(sizeof(T[elemCount]), (const void*) data, flags)
        {

        }
    };
    class FlexBuffer : public Buffer
    {
        GLenum usage;
    public:
        FlexBuffer();
        FlexBuffer(GLsizeiptr bufferSize, const void* data, GLenum bufferUsage = GL_DYNAMIC_DRAW);
        
        template<typename T>
        FlexBuffer(unsigned int elemCount, const T* data, GLbitfield flags = 0) : 
            FlexBuffer(sizeof(T[elemCount]), (const void*) data, flags)
        {

        }
        FlexBuffer(const FlexBuffer& other);
        FlexBuffer& operator=(const FlexBuffer& other);
        void ReallocBuffer(GLsizeiptr bufferSize, const void* data, GLenum bufferUsage = 0); // if bufferUsage is not specified, earlier one is used
        
        template<typename T>
        void ReallocBuffer(unsigned int elemCount, const T* data, GLenum bufferUsage = 0) // if bufferUsage is not specified, earlier one is used
        {
            if(bufferUsage)
                usage = bufferUsage;
            glNamedBufferData(name, *size = sizeof(T[elemCount]), (const void*)data, usage);
        }
    };
}