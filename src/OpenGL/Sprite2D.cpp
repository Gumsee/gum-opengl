#include "Sprite2D.h"
#include "ElementBufferObject.h"
#include "System/MemoryManagement.h"
#include "System/Output.h"
#include "VertexArrayObject.h"
#include <GumOpenGL/OpenGL/Texture.h>


Sprite2D::Sprite2D()
{
    sName = "Sprite";
    pTexture = nullptr;
    if(pVertexArrayObject == nullptr)
    {
        pVertexArrayObject = new VertexArrayObject();
        
        pVertexArrayObject->bind();
        VertexBufferObject<float> pVertexVBO;
		pVertexVBO.setData(afSpriteVertices);
        pVertexArrayObject->addAttribute(&pVertexVBO, 0, 2, GL_FLOAT, 0, 0);
        pVertexArrayObject->setVertexCount(pVertexVBO.getLength());
        
        pTransMatricesVBO = new VertexBufferObject<mat3>();
		//pTransMatricesVBO->setData(vTransforms, GL_STREAM_DRAW);
		pVertexArrayObject->addAttributeMat3(pTransMatricesVBO, 1, GL_FLOAT, 1);

		pIndividualColorsVBO = new VertexBufferObject<vec4>();
		pVertexArrayObject->addAttribute(pIndividualColorsVBO, 5, 4, GL_FLOAT, sizeof(vec4), 0, 1);

        pIndexBuffer = new ElementBufferObject();
        pIndexBuffer->setData(aiSpriteIndices);
        pVertexArrayObject->addElementBuffer(pIndexBuffer);
        pVertexArrayObject->unbind();
    }

	//Set attributes
	this->pShader = nullptr;
}

Sprite2D::~Sprite2D()
{
    Gum::_delete(pTransMatricesVBO);
    Gum::_delete(pIndividualColorsVBO);
}

Sprite2DInstance* Sprite2D::addInstance(Sprite2DInstance* instance)
{
	vInstances.push_back(instance);
	vTransforms.push_back(instance->getMatrix());
	pTransMatricesVBO->setData(vTransforms, GL_DYNAMIC_DRAW);
    
	vIndividualColors.push_back(instance->getIndividualColor());
	pIndividualColorsVBO->setData(vIndividualColors);

    if(pAddInstanceCallback != nullptr)
        pAddInstanceCallback(instance);

    return instance;
}
Sprite2DInstance* Sprite2D::addInstance()
{
    return addInstance(new Sprite2DInstance(this));
}

Sprite2DInstance* Sprite2D::operator++() { return addInstance(); }


void Sprite2D::applyTransformationMatrix(Sprite2DInstance *inst)
{
	for(size_t i = 0; i < vInstances.size(); i++)
	{
        if(inst == vInstances[i])
        {
            //TODO
			pTransMatricesVBO->setSingleData(inst->getMatrix(), i); //To fix
            vTransforms[i] = inst->getMatrix(); // To get rid of
        }
    }

    pTransMatricesVBO->setData(vTransforms); //Make more efficient
}

//Rendering stuff
void Sprite2D::render()
{
    if(pTexture != nullptr)
        pTexture->bind();
    renderMesh();
}

void Sprite2D::renderID()
{
    renderMesh();
}

void Sprite2D::renderMesh()
{
    pVertexArrayObject->bind();
    glDrawElementsInstanced(pVertexArrayObject->getPrimitiveType(), pVertexArrayObject->getRenderCount(), GL_UNSIGNED_INT, 0, vInstances.size());
    pVertexArrayObject->unbind();
}

//
//Setter
//
void Sprite2D::setShaderProgram(ShaderProgram *shader)     { this->pShader = shader; }
void Sprite2D::setName(const std::string& name) 	       { this->sName = name; }
void Sprite2D::onAddInstance(AddInstanceCallback callback) { this->pAddInstanceCallback = callback; }
void Sprite2D::setTexture(Texture2D* texture)              { this->pTexture = texture; }


//
//Getter
//
std::string 		Sprite2D::getName() 			   { return sName; }
Sprite2DInstance* 	Sprite2D::getInstance(int index)   { return vInstances[index]; }
ShaderProgram*		Sprite2D::getShaderProgram()	   { return pShader; }
unsigned int        Sprite2D::numInstances() 	       { return vInstances.size(); }
VertexArrayObject*  Sprite2D::getVertexArrayObject()   { return pVertexArrayObject; }


void Sprite2D::cleanupSprites()
{
    Gum::_delete(pVertexArrayObject);
    Gum::_delete(pIndexBuffer);
}