#include <Graphics/Sprite2D.h>
#include <GL/glew.h>

void Sprite2D::renderMesh()
{
    pVertexArrayObject->bind();
    glDrawElementsInstanced(pVertexArrayObject->getPrimitiveType(), pVertexArrayObject->getRenderCount(), GL_UNSIGNED_INT, 0, vInstances.size());
    pVertexArrayObject->unbind();
}