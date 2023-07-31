#pragma once
#include <Primitives/Transformable.h>
#include <Primitives/UniqueID.h>

#define OBJECT3D_INSTANCE_TYPE_BASE 0

class Object3D;
class Object3DInstance : public Transformable, public UniqueID
{
protected:
    Object3D* pObject = nullptr;
    unsigned short iType;
	float fFurthestAwayPoint;
    void* userptr = nullptr;
	bbox3 bBoundingBox;

    void generateBoundingBox();
	void generateFurthestAwayPoint();
    

public:
    Object3DInstance(Object3D* obj);
    virtual ~Object3DInstance() {};

    void updateMatrix() override;
    virtual void onTransformUpdate() override {};

    void setUserPtr(void* ptr);

    Object3D* getParentObject();
    unsigned short getType() const;
    void* getUserPtr();
};