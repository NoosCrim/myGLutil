#include "buffer.hpp"
#include <GL/glew.h>
#include <cstdio>
namespace mGLu
{
    Buffer::Buffer(GLsizeiptr bufferSize):
        refCount(new unsigned int(1)),
        size(new GLsizeiptr(bufferSize))
    {
        glCreateBuffers(1, (GLuint*)&name);
    }
    Buffer::Buffer()
    {
        
    }
    Buffer::Buffer(const Buffer& other):
        refCount(other.refCount),
        size(other.size),
        name(other.name)
    {
        ++(*refCount);
    }
    Buffer& Buffer::operator=(const Buffer& other)
    {
        if(name && --(*refCount) == 0)
        {
            delete refCount;
            delete size;
            glDeleteBuffers(1, &name);
        }

        const_cast<GLsizeiptr*&>(size) = other.size;
        const_cast<unsigned int*&>(refCount) = other.refCount;
        const_cast<GLuint&>(name) = other.name;
        ++(*refCount);
        return *this;
    }
    Buffer::~Buffer()
    {
        if(name && --(*refCount) == 0)
        {
            delete refCount;
            delete size;
            glDeleteBuffers(1, &name);
        }
    }
    bool Buffer::SetData(GLintptr offset, GLsizeiptr dataSize, const void* data)
    {
        if(GetSize() < offset + dataSize)
        {
            std::fputs("Buffer: Error: tried setting data where offset + dataSize > size of buffer\n", stderr);
            return false;
        }
        glNamedBufferSubData(name, offset, GetSize(), data);
        return true;
    }
    
    FixedBuffer::FixedBuffer(): Buffer(){}

    FixedBuffer::FixedBuffer(const Buffer& other):
        Buffer(other)
    {

    }

    FixedBuffer& FixedBuffer::operator=(const Buffer& other)
    {
        Buffer::operator=(other);
        return *this;
    }

    FixedBuffer::FixedBuffer(GLsizeiptr bufferSize, const void* data, GLbitfield flags) : 
        Buffer(bufferSize)
    {
        glNamedBufferStorage(name, GetSize(), data, flags);
    }
    FlexBuffer::FlexBuffer():
        Buffer()
    {
        
    }
    FlexBuffer::FlexBuffer(GLsizeiptr bufferSize, const void* data, GLenum bufferUsage):
        Buffer(bufferSize),
        usage(bufferUsage)
    {
        glNamedBufferData(name, bufferSize, data, usage);
    }

    FlexBuffer::FlexBuffer(const FlexBuffer& other):
        Buffer(other)
    {
        
    }
    FlexBuffer& FlexBuffer::operator=(const FlexBuffer& other)
    {
        Buffer::operator=(other);
        usage = other.usage; 
        return *this;
    }
    void FlexBuffer::ReallocBuffer(GLsizeiptr bufferSize, const void* data, GLenum bufferUsage) // if bufferUsage is not specified, earlier one is used
    {
        if(bufferUsage)
            usage = bufferUsage;
        glNamedBufferData(name, *size = bufferSize, data, usage);
    }
}