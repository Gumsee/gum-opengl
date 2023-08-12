#include "Sprite2DInstance.h"
#include "Sprite2D.h"
#include "System/Output.h"

Sprite2DInstance::Sprite2DInstance(Sprite2D* sprite)
    : TransformableInstance<Sprite2D, 2U>(sprite, SPRITE2D_INSTANCE_TYPE_BASE)
{
    updateMatrix();
}

void Sprite2DInstance::updateMatrix()
{
    mTransformation = Gum::Maths::createTransformationMatrix(vPosition, qRotation, vScale);
    if(pParent != nullptr)
        pParent->applyTransformationMatrix(this);
    onTransformUpdate();
}