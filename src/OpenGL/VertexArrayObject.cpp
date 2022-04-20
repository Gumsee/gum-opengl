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


unsigned int VertexArrayObject::addElementBuffer(std::vector<unsigned int> indices, unsigned int usage)
{
    glGenBuffers(1, &iIndexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iIndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]), &indices[0], usage);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    iVertexCount = indices.size();
    return iIndexBuffer;
}



unsigned int VertexArrayObject::numVertices() { return this->iVertexCount; }
unsigned int VertexArrayObject::getID()       { return this->ivaoID; }