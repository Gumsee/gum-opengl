#include "ElementBufferObject.h"

ElementBufferObject::ElementBufferObject()
{
    glGenBuffers(1, &ieboID);
}

ElementBufferObject::~ElementBufferObject() 
{
    glDeleteBuffers(1, &ieboID);
}

void ElementBufferObject::bind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ieboID); 
}

void ElementBufferObject::unbind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void ElementBufferObject::setData(const std::vector<unsigned int>& indices, const unsigned int& usage)
{
    bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]), &indices[0], usage);
    unbind();
    
    iLength = indices.size();
}

unsigned int ElementBufferObject::getID() const     { return this->ieboID; }
unsigned int ElementBufferObject::getLength() const { return this->iLength; }