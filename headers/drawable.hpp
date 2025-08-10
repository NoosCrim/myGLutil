#pragma once
#include "shader.hpp"
#include "buffer.hpp"
#include "vao.hpp"
#include <vector>
namespace mGLu
{
	class Drawable
	{
		struct BufferBinding
		{
			int bufferIndex = -1; 	// index of buffer in buffers vector
			GLintptr offset; 			// offset of first attrib we bind to in buffer
			GLsizei stride; 			// distance between attribs we bind to
		};
		std::vector<BufferBinding> bindingData; // we store binding data for binding point i in bindingData[i]
		GLsizei InferVertexCount(); // infers amout of vertices to render based on attrib strides and buffer sizes of defined, non empty buffers
		GLsizei InferIndexCount(GLenum indexType);
		void UpdateBindingsSize();
		void BindToVAO();
	public:
		VAOview vao;
		std::vector<Buffer> buffers;
		Buffer indexBuffer;
		ShaderProgram shaderProgram;
		Drawable();

		Drawable(const Drawable& other);
		~Drawable();
		bool SetBinding(GLuint bindingPoint, unsigned int bufferIndex, GLintptr offset, GLsizei stride);
		void Draw(GLsizei vertexCount = 0, GLsizei firstVertex = 0, GLenum draw_mode = GL_TRIANGLES); // if vertexCount is not passed it will be infered from attrib strides and sizes of buffers (which has some overhead)
		void DrawIndexed(GLsizei indexCount = 0, GLenum draw_mode = GL_TRIANGLES, GLenum indexType = GL_UNSIGNED_INT); // if indexCount is not provided it is infered
		void DrawInstanced(GLsizei instanceCount, GLsizei vertexCount = 0, GLsizei firstVertex = 0, GLenum draw_mode = GL_TRIANGLES); // if vertexCount is not passed it will be infered from attrib strides and sizes of buffers (which has some overhead)
		void DrawIndexedInstanced(GLsizei instanceCount, GLsizei indexCount = 0, GLenum draw_mode = GL_TRIANGLES, GLenum indexType = GL_UNSIGNED_INT); // if indexCount is not provided it is infered
	};
}