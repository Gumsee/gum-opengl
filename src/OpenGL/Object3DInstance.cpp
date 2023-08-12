#include "Object3DInstance.h"
#include "Object3D.h"

Object3DInstance::Object3DInstance(Object3D* obj)
    : TransformableInstance<Object3D, 3U>(obj, OBJECT3D_INSTANCE_TYPE_BASE)
{
    this->fFurthestAwayPoint = 0.0f;

    generateBoundingBox();
    generateFurthestAwayPoint();
}

void Object3DInstance::updateMatrix()
{
    mTransformation = Gum::Maths::createTransformationMatrix(vPosition, qRotation, vScale);
    if(pParent != nullptr)
        pParent->applyTransformationMatrix(this);
    onTransformUpdate();
}

void Object3DInstance::generateBoundingBox()
{
	if(pParent->getMesh() != nullptr && bBoundingBox.getPos() == vec3(0,0,0) && bBoundingBox.getSize() == vec3(0,0,0))
	{
        for (unsigned int i = 0; i < pParent->getMesh()->numVertices(); i++)
        {
            for(unsigned int j = 0; j < 3; j++)
            {
                float dist = pParent->getMesh()->getVertex(i).position[j] * vScale[j];
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
	if(pParent->getMesh() != nullptr)
	{
		for(unsigned int i = 0; i < pParent->getMesh()->numVertices(); i++)
		{
            float dist = vec3::distance(pParent->getMesh()->getVertex(i).position * vScale, vec3(0.0f));
            if(dist > this->fFurthestAwayPoint)
                this->fFurthestAwayPoint = dist;
		}
	}
}