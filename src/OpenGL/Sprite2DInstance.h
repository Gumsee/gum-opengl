#pragma once
#include "TransformableInstance.h"

#define SPRITE2D_INSTANCE_TYPE_BASE 0

class Sprite2D;
class Sprite2DInstance : public TransformableInstance<Sprite2D, 2U>
{
protected:

public:
    Sprite2DInstance(Sprite2D* sprite);
    virtual ~Sprite2DInstance() {};

    void updateMatrix() override;
    virtual void onTransformUpdate() override {};

    //Setter
    void setSize(ivec2 pixels)
    {
        TransformableInstance<Sprite2D, 2U>::setScale(pixels);
    }

    //Getter
    ivec2 getSize()
    {
        return TransformableInstance<Sprite2D, 2U>::getScale();
    }
};