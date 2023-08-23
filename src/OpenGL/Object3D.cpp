#include <Graphics/Object3D.h>

Object3D::RenderFunc Object3D::pRenderStripFunc = [](Object3D* obj) {
    glDrawArraysInstanced(obj->pVertexArrayObject->getPrimitiveType(), 0, obj->pVertexArrayObject->getRenderCount(), obj->vInstances.size());
};
Object3D::RenderFunc Object3D::pRenderTessellatedStripFunc = [](Object3D* obj) {
    glDrawArraysInstanced(GL_PATCHES, 0, obj->pVertexArrayObject->getRenderCount(), obj->vInstances.size());
};
Object3D::RenderFunc Object3D::pRenderIndexedFunc = [](Object3D* obj) {
    glDrawElementsInstanced(obj->pVertexArrayObject->getPrimitiveType(), obj->pVertexArrayObject->getRenderCount(), GL_UNSIGNED_INT, 0, obj->vInstances.size());
};
Object3D::RenderFunc Object3D::pRenderTessellatedIndexedFunc = [](Object3D* obj) {
    glDrawElementsInstanced(GL_PATCHES, obj->pVertexArrayObject->getRenderCount(), GL_UNSIGNED_INT, 0, obj->vInstances.size());
};