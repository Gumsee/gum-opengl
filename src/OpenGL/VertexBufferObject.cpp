#include <Graphics/VertexBufferObject.h>
#include <GL/glew.h>

namespace Gum {
namespace Graphics {
namespace VertexBufferObject
{

unsigned int createNative()
{
    unsigned int id;
    glGenBuffers(1, &id);
    return id;
}

void destroyNative(const unsigned int& id) 
{
    glDeleteBuffers(1, &id);
}

void bind(const unsigned int& id)
{
    glBindBuffer(GL_ARRAY_BUFFER, id); 
}

void unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void setDataNative(size_t datasize, const void* data, size_t length, const unsigned int& usage = GL_STATIC_DRAW)
{
    glBufferData(GL_ARRAY_BUFFER, length * datasize, data, usage);
}

void* getDataPointerNative(size_t datasize, const unsigned int& offset, const unsigned int& amount)
{
    glMapBufferRange(GL_ARRAY_BUFFER, offset * datasize, amount * datasize, GL_MAP_WRITE_BIT);
    return glMapBufferRange(GL_ARRAY_BUFFER, offset * datasize, amount * datasize, GL_MAP_WRITE_BIT);
}

void setSingleDataNative(size_t datasize, const void* data, const unsigned int& offset)
{
    glBufferSubData(GL_ARRAY_BUFFER, offset * datasize, datasize, &data);
}

}}};