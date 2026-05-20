#include <Graphics/Sprite2D.h>
#include <glad/gl.h>

void Sprite2D::renderMesh()
{
    pVertexArrayObject->bind();
    glDrawElementsInstanced(pVertexArrayObject->getPrimitiveType(), pVertexArrayObject->getRenderCount(), GL_UNSIGNED_INT, 0, static_cast<GLsizei>(vInstances.size()));
    pVertexArrayObject->unbind();
}