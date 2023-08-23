#include <Graphics/VertexArrayObject.h>
#include <GL/glew.h>


unsigned int VertexArrayObject::PrimitiveTypes::POINTS         = GL_POINTS;
unsigned int VertexArrayObject::PrimitiveTypes::TRIANGLES      = GL_TRIANGLES;
unsigned int VertexArrayObject::PrimitiveTypes::TRIANGLE_STRIP = GL_TRIANGLE_STRIP;
unsigned int VertexArrayObject::PrimitiveTypes::TRIANGLE_FAN   = GL_TRIANGLE_FAN;
unsigned int VertexArrayObject::PrimitiveTypes::LINES          = GL_LINES;
unsigned int VertexArrayObject::PrimitiveTypes::LINE_STRIP     = GL_LINE_STRIP;
unsigned int VertexArrayObject::PrimitiveTypes::LINE_LOOP      = GL_LINE_LOOP;
unsigned int VertexArrayObject::PrimitiveTypes::LINE_ADJACENCY = GL_LINES_ADJACENCY;
unsigned int VertexArrayObject::PrimitiveTypes::QUADS          = GL_QUADS;
unsigned int VertexArrayObject::PrimitiveTypes::QUAD_STRIP     = GL_QUAD_STRIP;


void VertexArrayObject::createNative()
{
    glGenVertexArrays(1, &ivaoID);
}

void VertexArrayObject::destroyNative()
{
    glDeleteVertexArrays(1, &ivaoID);
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