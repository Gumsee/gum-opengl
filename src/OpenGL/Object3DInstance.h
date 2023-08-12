#pragma once
#include "TransformableInstance.h"

#define OBJECT3D_INSTANCE_TYPE_BASE 0

class Object3D;
class Object3DInstance : public TransformableInstance<Object3D, 3U>
{
protected:
	float fFurthestAwayPoint;
	bbox3 bBoundingBox;

    void generateBoundingBox();
	void generateFurthestAwayPoint();
    

public:
    Object3DInstance(Object3D* obj);
    virtual ~Object3DInstance() {};

    void updateMatrix() override;
    virtual void onTransformUpdate() override {};
};