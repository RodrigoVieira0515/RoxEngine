#include <glad/gl.h>
#include "DefaultGLBuffer.h"
#include "RoxEngine/Renderer/Buffer.h"

namespace RoxEngine::Platforms::Renderer {
    DefaultGLVertexBuffer::DefaultGLVertexBuffer(void* vertices, size_t size) : mSize(size) {
        glCreateBuffers(1, &mID);
        glBindBuffer(GL_ARRAY_BUFFER, mID);
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_DYNAMIC_DRAW);
    }
    DefaultGLVertexBuffer::~DefaultGLVertexBuffer() {
        glDeleteBuffers(1, &mID);
    }
    void DefaultGLVertexBuffer::SetData(const void* data, size_t size) {
        glBindBuffer(GL_ARRAY_BUFFER, mID);
        if(size > mSize || size < mSize) {
            glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
            mSize = size;
        }
        else {
            glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
        }
    }
    const BufferLayout& DefaultGLVertexBuffer::GetLayout() const {
        return mLayout;
    }
    void DefaultGLVertexBuffer::SetLayout(const BufferLayout& layout) {
        mLayout = layout;
    }

    DefaultGLIndexBuffer::DefaultGLIndexBuffer(uint32_t* data,size_t count) {
        glCreateBuffers(1, &mID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), data, GL_DYNAMIC_DRAW);
    }
    DefaultGLIndexBuffer::~DefaultGLIndexBuffer() {
        glDeleteBuffers(1, &mID);
    }
    void DefaultGLIndexBuffer::SetData(const uint32_t *data, size_t count) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mID);
        if(count > mCount || count < mCount) {
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, count*sizeof(uint32_t), data, GL_DYNAMIC_DRAW);
            mCount = count;
        }
        else {
            glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, count*sizeof(uint32_t), data);
        }
    }
    size_t DefaultGLIndexBuffer::GetCount() const {
        return mCount;
    }
}