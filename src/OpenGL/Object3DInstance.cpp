#include "Object3DInstance.h"
#include "Object3D.h"
#include "Renderable.h"
#include "System/Output.h"

Object3DInstance::Object3DInstance(Object3D* obj)
{
    this->pObject = obj;
    this->iType = OBJECT3D_INSTANCE_TYPE_BASE;
    this->fFurthestAwayPoint = 0.0f;

    updateMatrix();
    generateBoundingBox();
    generateFurthestAwayPoint();
}

void Object3DInstance::updateMatrix()
{
    m4Transformation = Gum::Maths::createTransformationMatrix(v3Position, qRotation, v3Scale);
    if(pObject != nullptr)
        pObject->applyTransformationMatrix(this);
    onTransformUpdate();
}

void Object3DInstance::generateBoundingBox()
{
	if(pObject->getMesh() != nullptr && bBoundingBox.getPos() == vec3(0,0,0) && bBoundingBox.getSize() == vec3(0,0,0))
	{
        for (unsigned int i = 0; i < pObject->getMesh()->numVertices(); i++)
        {
            for(unsigned int j = 0; j < 3; j++)
            {
                float dist = pObject->getMesh()->getVertex(i).position[j] * v3Scale[j];
                if(dist < bBoundingBox.pos[j])
                    bBoundingBox.pos[j] = dist;
                else if(dist > bBoundingBox.size[j])
                    bBoundingBox.size[j] = dist;
            }
        }
	}
}

void Object3DInstance::generateFurthestAwayPoint()
{
	if(pObject->getMesh() != nullptr)
	{
		for(unsigned int i = 0; i < pObject->getMesh()->numVertices(); i++)
		{
            float dist = vec3::distance(pObject->getMesh()->getVertex(i).position * v3Scale, vec3(0.0f));
            if(dist > this->fFurthestAwayPoint)
                this->fFurthestAwayPoint = dist;
		}
	}
}


//
// Setter
//
void Object3DInstance::setUserPtr(void* ptr)      { userptr = ptr; }


//
// Getter
//
Object3D* Object3DInstance::getParentObject()     { return this->pObject; }
unsigned short Object3DInstance::getType() const  { return this->iType; }
void* Object3DInstance::getUserPtr()              { return this->userptr; }