#include <Graphics/VertexArrayObject.h>
#include <Graphics/WrapperFunctions.h>
#include <Desktop/GraphicsContext.h>
#include <glad/gl.h>


const unsigned int VertexArrayObject::PrimitiveTypes::POINTS         = GL_POINTS;
const unsigned int VertexArrayObject::PrimitiveTypes::TRIANGLES      = GL_TRIANGLES;
const unsigned int VertexArrayObject::PrimitiveTypes::TRIANGLE_STRIP = GL_TRIANGLE_STRIP;
const unsigned int VertexArrayObject::PrimitiveTypes::TRIANGLE_FAN   = GL_TRIANGLE_FAN;
const unsigned int VertexArrayObject::PrimitiveTypes::LINES          = GL_LINES;
const unsigned int VertexArrayObject::PrimitiveTypes::LINE_STRIP     = GL_LINE_STRIP;
const unsigned int VertexArrayObject::PrimitiveTypes::LINE_LOOP      = GL_LINE_LOOP;
const unsigned int VertexArrayObject::PrimitiveTypes::LINE_ADJACENCY = GL_LINES_ADJACENCY;

//Deprecated
//const unsigned int VertexArrayObject::PrimitiveTypes::QUADS          = GL_QUADS;
//const unsigned int VertexArrayObject::PrimitiveTypes::QUAD_STRIP     = GL_QUAD_STRIP;


void VertexArrayObject::createNative()
{
    glGenVertexArrays(1, &this->iID[GraphicsContext::CurrentlyBoundContext]);
}

void VertexArrayObject::destroyNative()
{
    glDeleteVertexArrays(1, &this->iID[GraphicsContext::CurrentlyBoundContext]);
}

void VertexArrayObject::bind()
{
    glBindVertexArray(this->iID[GraphicsContext::CurrentlyBoundContext]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iIndexBuffer);
	for (size_t i = 0; i < vAttributes.size(); i++) { glEnableVertexAttribArray(vAttributes[i].index); }
}

void VertexArrayObject::unbind()
{
	for (size_t i = 0; i < vAttributes.size(); i++) { glDisableVertexAttribArray(vAttributes[i].index); }
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void VertexArrayObject::render(const unsigned int& instances)
{
    glDrawArraysInstanced(getPrimitiveType(), 0, getRenderCount(), instances);
}

void VertexArrayObject::renderIndexed(const unsigned int& instances) 
{ 
    glDrawElementsInstanced(getPrimitiveType(), getRenderCount(), GL_UNSIGNED_INT, 0, instances); 
}

void VertexArrayObject::renderTesselated(const unsigned int& instances)
{
    glDrawArraysInstanced(GL_PATCHES, 0, getRenderCount(), instances);
}

void VertexArrayObject::renderTesselatedIndexed(const unsigned int& instances)
{
    glDrawElementsInstanced(GL_PATCHES, getRenderCount(), GL_UNSIGNED_INT, 0, instances);
}



void VertexArrayObject::addAttributeNative(AttributeProperties& properties)
{
    glEnableVertexAttribArray(properties.index);
    switch(properties.type)
    {
      case GL_BYTE:
      case GL_UNSIGNED_BYTE:
      case GL_SHORT:
      case GL_UNSIGNED_SHORT:
      case GL_INT:
      case GL_UNSIGNED_INT:
        glVertexAttribIPointer(properties.index, properties.dimension, properties.type, static_cast<GLsizei>(properties.stride), (void*)properties.offset);
      default:
        glVertexAttribPointer(properties.index, properties.dimension, properties.type, GL_FALSE, static_cast<GLsizei>(properties.stride), (void*)properties.offset);
    }
        
    glVertexAttribDivisor(properties.index, properties.divisor);

    #ifdef CHECK_GL_ERRORS
        GLenum error = 0;
        while((error = glGetError()) != GL_NO_ERROR) 
          Gum::Output::error(std::string("addAttributeNative: ") + graphicsErrorCodeToString(error));
    #endif
}
