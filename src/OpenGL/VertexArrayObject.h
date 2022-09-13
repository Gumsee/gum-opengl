#pragma once
#include <algorithm>
#include <vector>
#include <string>
#include <GL/glew.h>
#include <gum-maths.h>
#include <System/IO/Output.h>
#include "VertexBufferObject.h"

class VertexArrayObject
{
private:
    unsigned int ivaoID;
    unsigned int iIndexBuffer;
    unsigned int iVertexCount;
    std::vector<unsigned int> vBufferIDs;
	std::vector<unsigned int> vAttributes;

public:
    VertexArrayObject();
    ~VertexArrayObject();

    void bind();
    void unbind();

    unsigned int addElementBuffer(std::vector<unsigned int> indices, unsigned int usage = GL_STATIC_DRAW);

    template<typename T>
    unsigned int addAttribute(VertexBufferObject<T>* vbo, const unsigned int& index, const unsigned int& dimension, 
        const unsigned int& type, const size_t& stride, const size_t& offset, const unsigned int& divisor = 0)
    {
        bind();
        vbo->bind();
        if(std::find(vAttributes.begin(), vAttributes.end(), index) != vAttributes.end())
        {
            Gum::Output::warn("VertexArrayObject: Attribute " + std::to_string(index) + " has already been added! (Not doing anything..)");
        }
        else
        {
            glEnableVertexAttribArray(index);
            if(type == GL_BYTE || type ==  GL_UNSIGNED_BYTE || type == GL_SHORT || type == GL_UNSIGNED_SHORT || type == GL_INT || type == GL_UNSIGNED_INT)
                glVertexAttribIPointer(index, dimension, type, stride, (void*)offset);
            else
                glVertexAttribPointer(index, dimension, type, GL_FALSE, stride, (void*)offset);
            glVertexAttribDivisor(index, divisor);
            vAttributes.push_back(index);
        }
        vbo->unbind();
        unbind();
        return index;
    }


    template<typename T>
    unsigned int addAttributeMat4(VertexBufferObject<T>* vbo, unsigned int index, unsigned int type, const unsigned int& divisor)
    {
        GLsizei vec4Size = sizeof(vec4);
        addAttribute(vbo, 3, 4, type, 4 * vec4Size, 0 * vec4Size, divisor);
        addAttribute(vbo, 4, 4, type, 4 * vec4Size, 1 * vec4Size, divisor);
        addAttribute(vbo, 5, 4, type, 4 * vec4Size, 2 * vec4Size, divisor);
        addAttribute(vbo, 6, 4, type, 4 * vec4Size, 3 * vec4Size, divisor);
        return index;
    }

    unsigned int numVertices();
    unsigned int getID();
};