#include <stdexcept>
#include <GL/glew.h>
#include "drawable.hpp"
#include "shader.hpp"
#include "buffer.hpp"
#include "vao.hpp"
#include <vector>
typedef unsigned long long ull;
namespace mGLu
{
	GLsizei Drawable::InferVertexCount() // infers amout of vertices to render based on attrib strides and buffer sizes of defined, non empty buffers
	{
		unsigned int vertexCount;
		for(unsigned int bindingI = 0; bindingI < bindingData.size(); bindingI++)
		{
			int bufferIndex = bindingData[bindingI].bufferIndex;
			if(bufferIndex == -1 || buffers[bufferIndex].GetSize() == 0)
				continue;
			GLsizeiptr bufferSize = buffers[bufferIndex].GetSize();
			unsigned int stridesInBuffer = bufferSize / bindingData[bindingI].stride;
			if(stridesInBuffer < vertexCount) vertexCount = stridesInBuffer;
		}
		return vertexCount;
	}
	GLsizei Drawable::InferIndexCount(GLenum indexType)
	{
		unsigned int indexTypeSize = sizeof(GLuint);
		switch (indexType)
		{
		case GL_UNSIGNED_INT:
			indexTypeSize =  sizeof(GLuint);
			break;
		case GL_UNSIGNED_SHORT:
			indexTypeSize =  sizeof(GLushort);
			break;
		case GL_UNSIGNED_BYTE:
			indexTypeSize =  sizeof(GLubyte);
			break;
		}
		return indexBuffer.GetSize() / indexTypeSize;
	}
	void Drawable::UpdateBindingsSize()
	{
		GLuint t = vao.GetBindingCount();
		if(t != bindingData.size())
			bindingData.resize(t);
	}
	void Drawable::BindToVAO()
	{
		for(GLuint bindingI = 0; bindingI < bindingData.size(); bindingI++)
		{
			vao.BindBufferToAttrib(bindingI, buffers[bindingData[bindingI].bufferIndex].GetName(), bindingData[bindingI].offset, bindingData[bindingI].stride);
		}
	}
	Drawable::Drawable()
	{
		
	}
	Drawable::Drawable(const Drawable& other): 
		bindingData(other.bindingData),
		vao(other.vao),
		buffers(other.buffers),
		indexBuffer(other.indexBuffer)
	{

	}
	Drawable::~Drawable()
	{
		
	}
	bool Drawable::SetBinding(GLuint bindingPoint, unsigned int bufferIndex, GLintptr offset, GLsizei stride)
	{
		UpdateBindingsSize();
		if(bindingPoint >= bindingData.size())
		{
			std::fputs("Drawable: Error: Tried to bind to set bindingData for non-existant binding point!\n", stderr);
			return false;
		}
		bindingData[bindingPoint] = {(int)bufferIndex, offset, stride};
		return true;
	}
	void Drawable::Draw(GLsizei vertexCount, GLsizei firstVertex, GLenum draw_mode) // if vertexCount is not passed it will be infered from attrib strides and sizes of buffers (which has some overhead)
	{
		BindToVAO();
		
		shaderProgram.Use();

		glBindVertexArray(vao.GetName());
		glDrawArrays(draw_mode, firstVertex, vertexCount?vertexCount : InferVertexCount());
	}
	void Drawable::DrawIndexed(GLsizei indexCount, GLenum draw_mode, GLenum indexType) // if indexCount is not provided it is infered
	{
		BindToVAO();
		
		vao.BindElementBuffer(indexBuffer.GetName());
		shaderProgram.Use();

		glBindVertexArray(vao.GetName());
		glDrawElements(draw_mode, indexCount ? indexCount : InferIndexCount(indexType), indexType, nullptr);
	}
	void Drawable::DrawInstanced(GLsizei instanceCount, GLsizei vertexCount, GLsizei firstVertex, GLenum draw_mode) // if vertexCount is not passed it will be infered from attrib strides and sizes of buffers (which has some overhead)
	{
		BindToVAO();
		
		shaderProgram.Use();

		glBindVertexArray(vao.GetName());
		glDrawArraysInstanced(draw_mode, firstVertex, vertexCount?vertexCount : InferVertexCount(), instanceCount);
	}
	void Drawable::DrawIndexedInstanced(GLsizei instanceCount, GLsizei indexCount, GLenum draw_mode, GLenum indexType) // if indexCount is not provided it is infered
	{
		BindToVAO();
		
		vao.BindElementBuffer(indexBuffer.GetName());
		shaderProgram.Use();

		glBindVertexArray(vao.GetName());
		glDrawElementsInstanced(draw_mode, indexCount ? indexCount : InferIndexCount(indexType), indexType, nullptr, instanceCount);
	}
}