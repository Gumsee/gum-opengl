#pragma once
#include <vector>
#include <GL/glew.h>

class ElementBufferObject
{
private:
    unsigned int ieboID = 0;
    unsigned int iLength = 0;

public:
    ElementBufferObject();
    ~ElementBufferObject();

    void bind();
    void unbind();


    void setData(const std::vector<unsigned int>& indices, const unsigned int& usage = GL_STATIC_DRAW);

    unsigned int getID() const;
    unsigned int getLength() const;
};