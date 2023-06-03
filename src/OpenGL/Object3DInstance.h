#pragma once
#include <Primitives/Transformable.h>

#define OBJECT3D_INSTANCE_TYPE_BASE 0

class Object3D;
class Object3DInstance : public Transformable
{
protected:
    Object3D* pObject = nullptr;
    unsigned int iID = 0; //Global ID
    unsigned short iType;
    void* userptr = nullptr;
	bbox3 bBoundingBox;

    void generateBoundingBox();

public:
    Object3DInstance(Object3D* obj);
    virtual ~Object3DInstance() {};

    void updateMatrix();
    virtual void onTransformUpdate() {};

    void setUserPtr(void* ptr);
    void setID(unsigned int id);

    unsigned int getID() const;
    unsigned short getType() const;
    void* getUserPtr();
};