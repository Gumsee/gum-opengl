#include "VertexArrayObject.h"
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

VertexArrayObject::VertexArrayObject(const unsigned int& primitivetype)
{
    this->ivaoID = 0;
    this->iIndexBuffer = 0;
    this->iVertexCount = 0;
    this->iPrimitiveType = primitivetype;

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
    iRenderCount = elembuffer->getLength();
    iIndexBuffer = elembuffer->getID();
    return elembuffer->getID();
}

void VertexArrayObject::updateRenderCount()
{
    if(iIndexBuffer == 0)
    {
        int divider = (iPrimitiveType == PrimitiveTypes::LINES) ? 2 
                    : (iPrimitiveType == PrimitiveTypes::TRIANGLES) ? 3 
                    : (iPrimitiveType == PrimitiveTypes::QUADS) ? 4
                    : 1;
        iRenderCount = iVertexCount / divider;
    }
    if(pOnRenderCountFunc != nullptr)
        pOnRenderCountFunc();
}

//
// Setter
//
void VertexArrayObject::setVertexCount(const unsigned int& count)       { this->iVertexCount = count; updateRenderCount(); }
void VertexArrayObject::setPrimitiveType(const unsigned int& type)      { this->iPrimitiveType = type; updateRenderCount(); }
void VertexArrayObject::onRenderCountUpdate(std::function<void()> func) { this->pOnRenderCountFunc = func; }

//
// Getter
//
unsigned int VertexArrayObject::numVertices() const      { return this->iVertexCount; }
unsigned int VertexArrayObject::getID() const            { return this->ivaoID; }
unsigned int VertexArrayObject::getRenderCount() const   { return this->iRenderCount; }
unsigned int VertexArrayObject::getPrimitiveType() const { return this->iPrimitiveType; }