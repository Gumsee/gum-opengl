#include "VertexArrayObject.h"
#include <GL/glew.h>


VertexArrayObject::VertexArrayObject()
{
    ivaoID = 0;
    iIndexBuffer = 0;
    iVertexCount = 0;
    glGenVertexArrays(1, &ivaoID);
}

VertexArrayObject::~VertexArrayObject() 
{
    glDeleteVertexArrays(1, &ivaoID);
	vAttributes.clear();
}

void VertexArrayObject::bind()
{
    glBindVertexArray(ivaoID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iIndexBuffer);
	for (size_t i = 0; i < vAttributes.size(); i++) { glEnableVertexAttribArray(vAttributes[i]); }
}

void VertexArrayObject::unbind()
{
	for (size_t i = 0; i < vAttributes.size(); i++) { glDisableVertexAttribArray(vAttributes[i]); }
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


unsigned int VertexArrayObject::addElementBuffer(ElementBufferObject* elembuffer, unsigned int usage)
{
    iVertexCount = elembuffer->getLength();
    iIndexBuffer = elembuffer->getID();
    return elembuffer->getID();
}

//
// Setter
//
void VertexArrayObject::setVertexCount(const unsigned int& count) { this->iVertexCount = count; }

//
// Getter
//
unsigned int VertexArrayObject::numVertices() const { return this->iVertexCount; }
unsigned int VertexArrayObject::getID() const       { return this->ivaoID; }