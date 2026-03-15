#include <Graphics/VertexArrayObject.h>
#include <GL/glew.h>


const unsigned int VertexArrayObject::PrimitiveTypes::POINTS         = GL_POINTS;
const unsigned int VertexArrayObject::PrimitiveTypes::TRIANGLES      = GL_TRIANGLES;
const unsigned int VertexArrayObject::PrimitiveTypes::TRIANGLE_STRIP = GL_TRIANGLE_STRIP;
const unsigned int VertexArrayObject::PrimitiveTypes::TRIANGLE_FAN   = GL_TRIANGLE_FAN;
const unsigned int VertexArrayObject::PrimitiveTypes::LINES          = GL_LINES;
const unsigned int VertexArrayObject::PrimitiveTypes::LINE_STRIP     = GL_LINE_STRIP;
const unsigned int VertexArrayObject::PrimitiveTypes::LINE_LOOP      = GL_LINE_LOOP;
const unsigned int VertexArrayObject::PrimitiveTypes::LINE_ADJACENCY = GL_LINES_ADJACENCY;
const unsigned int VertexArrayObject::PrimitiveTypes::QUADS          = GL_QUADS;
const unsigned int VertexArrayObject::PrimitiveTypes::QUAD_STRIP     = GL_QUAD_STRIP;


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



void VertexArrayObject::addAttributeNative(const unsigned int& index, const unsigned int& dimension, const unsigned int& type, const size_t& stride, const size_t& offset, const unsigned int& divisor)
{
    glEnableVertexAttribArray(index);
    if(type == GL_BYTE || type == GL_UNSIGNED_BYTE || type == GL_SHORT || type == GL_UNSIGNED_SHORT || type == GL_INT || type == GL_UNSIGNED_INT)
        glVertexAttribIPointer(index, dimension, type, stride, (void*)offset);
    else
        glVertexAttribPointer(index, dimension, type, GL_FALSE, stride, (void*)offset);
    glVertexAttribDivisor(index, divisor);
    vAttributes.push_back(index);

    #ifdef CHECK_GL_ERRORS
        GLenum error = 0;
        const char *errString;
        while((error = glGetError()) != GL_NO_ERROR) 
        {
            errString = reinterpret_cast<const char*>(gluErrorString(error));
            if(errString != nullptr)
                Gum::Output::error(std::string("addAttributeNative: ") + errString);
        }
    #endif
}


void VertexArrayObject::addAttributeMat4Native(unsigned int index, unsigned int type, const unsigned int& divisor)
{
    size_t vec4Size = sizeof(vec4);
    addAttributeNative(index + 0, 4, type, 4 * vec4Size, 0 * vec4Size, divisor);
    addAttributeNative(index + 1, 4, type, 4 * vec4Size, 1 * vec4Size, divisor);
    addAttributeNative(index + 2, 4, type, 4 * vec4Size, 2 * vec4Size, divisor);
    addAttributeNative(index + 3, 4, type, 4 * vec4Size, 3 * vec4Size, divisor);
}


void VertexArrayObject::addAttributeMat3Native(unsigned int index, unsigned int type, const unsigned int& divisor)
{
    size_t vec3Size = sizeof(vec3);
    addAttributeNative(index + 0, 3, type, 3 * vec3Size, 0 * vec3Size, divisor);
    addAttributeNative(index + 1, 3, type, 3 * vec3Size, 1 * vec3Size, divisor);
    addAttributeNative(index + 2, 3, type, 3 * vec3Size, 2 * vec3Size, divisor);
}