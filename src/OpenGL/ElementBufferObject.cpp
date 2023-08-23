#include <Graphics/ElementBufferObject.h>
#include <GL/glew.h>

void ElementBufferObject::createNative()    { glGenBuffers(1, &ieboID); }
void ElementBufferObject::destroyNative()   { glDeleteBuffers(1, &ieboID); }
void ElementBufferObject::bind()            { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ieboID); }
void ElementBufferObject::unbind()          { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

void ElementBufferObject::setData(const std::vector<unsigned int>& indices)
{
    bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]), &indices[0], GL_STATIC_DRAW);
    unbind();
    
    iLength = indices.size();
}