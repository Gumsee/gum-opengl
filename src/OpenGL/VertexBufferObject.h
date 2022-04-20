#pragma once
#include <vector>
#include <GL/glew.h>

template<typename T>
class VertexBufferObject
{
private:
    unsigned int ivboID;
    unsigned int iLength;

public:
    VertexBufferObject()
    {
        ivboID = 0;
        glGenBuffers(1, &ivboID);
    }

    ~VertexBufferObject() 
    {
        glDeleteBuffers(1, &ivboID);
    }

    void bind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, ivboID); 
    }

    void unbind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void setData(const std::vector<T>& data, const unsigned int& usage = GL_STATIC_DRAW)
    {
        bind();
        glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(T), data.data(), usage);
        iLength = data.size();
        unbind();
    }

    T* getDataPointer(const unsigned int& offset, const unsigned int& amount)
    {
        bind();
        T* data = (T*)glMapBufferRange(GL_ARRAY_BUFFER, offset * sizeof(T), amount * sizeof(T), GL_MAP_WRITE_BIT);
        unbind();
        return data;
    }

    void setSingleData(const T& data, const unsigned int& offset)
    {
        bind();
        glBufferSubData(GL_ARRAY_BUFFER, offset * sizeof(T), sizeof(T), &data);
        unbind();
    }

    unsigned int getID() const     { return this->ivboID; }
    unsigned int getLength() const { return this->iLength; }
};