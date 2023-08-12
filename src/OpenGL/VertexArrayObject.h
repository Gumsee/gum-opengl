#pragma once
#include <algorithm>
#include <functional>
#include <vector>
#include <string>
#include <GL/glew.h>
#include <gum-maths.h>
#include <System/Output.h>
#include "ElementBufferObject.h"
#include "VertexBufferObject.h"

class VertexArrayObject
{
public:
    struct PrimitiveTypes
    {
        static unsigned int POINTS;
        static unsigned int TRIANGLES;
        static unsigned int TRIANGLE_STRIP;
        static unsigned int TRIANGLE_FAN;
        static unsigned int LINES;
        static unsigned int LINE_STRIP;
        static unsigned int LINE_LOOP;
        static unsigned int LINE_ADJACENCY;
        static unsigned int QUADS;
        static unsigned int QUAD_STRIP;
    };

private:
    unsigned int ivaoID;
    unsigned int iIndexBuffer;
    unsigned int iVertexCount;
    unsigned int iRenderCount;
    unsigned int iPrimitiveType;
    std::vector<unsigned int> vBufferIDs;
	std::vector<unsigned int> vAttributes;
    std::function<void()> pOnRenderCountFunc;

    void updateRenderCount();

public:
    VertexArrayObject(const unsigned int& primitivetype = PrimitiveTypes::TRIANGLES);
    ~VertexArrayObject();

    void bind();
    void unbind();

    unsigned int addElementBuffer(ElementBufferObject* elembuffer, unsigned int usage = GL_STATIC_DRAW);

    template<typename T>
    unsigned int addAttribute(VertexBufferObject<T>* vbo, const unsigned int& index, const unsigned int& dimension, 
        const unsigned int& type, const size_t& stride = sizeof(T), const size_t& offset = 0, const unsigned int& divisor = 0)
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
        addAttribute(vbo, index + 0, 4, type, 4 * vec4Size, 0 * vec4Size, divisor);
        addAttribute(vbo, index + 1, 4, type, 4 * vec4Size, 1 * vec4Size, divisor);
        addAttribute(vbo, index + 2, 4, type, 4 * vec4Size, 2 * vec4Size, divisor);
        addAttribute(vbo, index + 3, 4, type, 4 * vec4Size, 3 * vec4Size, divisor);
        return index;
    }


    template<typename T>
    unsigned int addAttributeMat3(VertexBufferObject<T>* vbo, unsigned int index, unsigned int type, const unsigned int& divisor)
    {
        GLsizei vec3Size = sizeof(vec3);
        addAttribute(vbo, index + 0, 3, type, 3 * vec3Size, 0 * vec3Size, divisor);
        addAttribute(vbo, index + 1, 3, type, 3 * vec3Size, 1 * vec3Size, divisor);
        addAttribute(vbo, index + 2, 3, type, 3 * vec3Size, 2 * vec3Size, divisor);
        return index;
    }

    //Setter
	void setPrimitiveType(const unsigned int& type);
    void setVertexCount(const unsigned int& count);
    void onRenderCountUpdate(std::function<void()> func);

    //Getter
    unsigned int numVertices() const;
    unsigned int getRenderCount() const;
    unsigned int getID() const;
    unsigned int getPrimitiveType() const;
};