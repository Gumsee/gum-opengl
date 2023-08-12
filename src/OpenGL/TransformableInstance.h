#pragma once
#include <Primitives/Transformable.h>
#include <Primitives/UniqueID.h>

template<typename T, unsigned int N>
class TransformableInstance : public Transformable<N>, public UniqueID
{
protected:
    T* pParent = nullptr;
    void* pUserptr = nullptr;
    unsigned short iType;

public:
    TransformableInstance(T* parent, const unsigned short& type)
    {
        this->pParent = parent;
        this->iType = type;
        updateMatrix();
    }
    virtual ~TransformableInstance() {};

    virtual void updateMatrix() override {};
    virtual void onTransformUpdate() override {};

    //Setter
    void setUserPtr(void* ptr)      { this->pUserptr = ptr; }

    //Getter
    T* getParentObject()            { return pParent; }
    unsigned short getType() const  { return iType; }
    void* getUserPtr()              { return pUserptr; }
};