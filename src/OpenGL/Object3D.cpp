#include "Object3D.h"
#include "ElementBufferObject.h"
#include "System/Output.h"
#include <Essentials/Tools.h>
#include <System/MemoryManagement.h>
#include <Codecs/Scene3DLoader.h>
#include <string>

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


Object3D::Object3D()
{
    pVertexArrayObject = new VertexArrayObject();
    pVertexArrayObject->onRenderCountUpdate([this]() {
        selectRenderFunc();
    });

	//Set attributes
	this->pShader = nullptr;
}

/** Loads the Object files content into memory and loads
 *  @param[in] modelFilePath Full path to object file
 *  @param[in] Shader Shader used to render object
 *  @param[in] name Identifier, also used in the objectmanager in most cases
 */
Object3D::Object3D(std::string modelFilePath, std::string name) : Object3D()
{
	//Create and add Properties
	sName = name;
	pMesh = new Mesh();
	Scene3DLoader loader;
    loader.iterateMeshes([this](unsigned int currentMesh, unsigned int numMeshes, Mesh* mesh, Bone* rootbone, std::vector<Bone*> bones) {
		pMesh->addMesh(mesh);
		Gum::_delete(mesh);
    });
    loader.load(modelFilePath);

	load();
}


/** Create an object according to a given mesh
 *  @param[in] mesh Mesh containing all necessary information
 *  @param[in] Shader Shader used to render object
 *  @param[in] name Identifier, also used in the objectmanager in most cases
 */
Object3D::Object3D(Mesh *mesh, std::string name) : Object3D()
{
	//Create and add Properties
	sName = name;
	pMesh = mesh;
	load();
}

Object3D::~Object3D()
{
	Gum::_delete(pVertexArrayObject);
	for(Object3DInstance *inst : vInstances)
		Gum::_delete(inst);
}

/** Gernerates the Objects Vertex Array and Buffers
 *  It also creates the bounding box
 *  and calculates th furthest away point from the objects origin
 */ 
void Object3D::load()
{
	if(pMesh != nullptr)
	{
		//Binding
        pVertexArrayObject->bind();
        pVertexVBO = new VertexBufferObject<Vertex>();
		pVertexVBO->setData(pMesh->getVertexBuffer());
        pVertexArrayObject->addAttribute(pVertexVBO, 0, 3, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, position.x));
        pVertexArrayObject->addAttribute(pVertexVBO, 1, 2, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, textureCoord.x));
        pVertexArrayObject->addAttribute(pVertexVBO, 2, 3, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, normal.x));
        pVertexArrayObject->addAttribute(pVertexVBO, 7, 3, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, tangent.x));
        
        pTransMatricesVBO = new VertexBufferObject<mat4>();
		//pTransMatricesVBO->setData(vTransforms, GL_STREAM_DRAW);
		pVertexArrayObject->addAttributeMat4(pTransMatricesVBO, 3, GL_FLOAT, 1);

		pIndividualColorsVBO = new VertexBufferObject<vec4>();
		pVertexArrayObject->addAttribute(pIndividualColorsVBO, 10, 4, GL_FLOAT, sizeof(vec4), 0, 1);

        pVertexArrayObject->setVertexCount(pVertexVBO->getLength());

        if(pMesh->getIndexBuffer().size() > 0)
        {
            ElementBufferObject* indexBuffer = new ElementBufferObject();
            indexBuffer->setData(pMesh->getIndexBuffer());
            pVertexArrayObject->addElementBuffer(indexBuffer);
            pVertexArrayObject->unbind();
        }

        selectRenderFunc();
	}
	else
	{
		Gum::Output::warn("ObjectCreation: Mesh is nullptr!");
	}
}


//Rendering stuff
void Object3D::render()
{
    renderMesh();
}

void Object3D::renderID()
{
    renderMesh();
}

void Object3D::renderMesh()
{
    pVertexArrayObject->bind();
    pRenderFunc(this);
    pVertexArrayObject->unbind();
}

Object3DInstance* Object3D::addInstance(Object3DInstance* instance)
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
Object3DInstance* Object3D::addInstance()
{
    return addInstance(new Object3DInstance(this));
}

Object3DInstance* Object3D::operator++() { return addInstance(); }


void Object3D::applyTransformationMatrix(Object3DInstance *inst)
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

void Object3D::selectRenderFunc()
{
    unsigned int type = pVertexArrayObject->getPrimitiveType();
    bool isStrip = type == VertexArrayObject::PrimitiveTypes::LINE_STRIP 
                || type == VertexArrayObject::PrimitiveTypes::LINE_LOOP 
                || type == VertexArrayObject::PrimitiveTypes::TRIANGLE_STRIP 
                || type == VertexArrayObject::PrimitiveTypes::QUAD_STRIP;

    if(isStrip)
    {
        if(bRenderTessellated)
            pRenderFunc = pRenderTessellatedStripFunc;
        else
            pRenderFunc = pRenderStripFunc;

        if(pMesh == nullptr)
            return;
    }
    else
    {
        if(bRenderTessellated)
            pRenderFunc = pRenderTessellatedIndexedFunc;
        else
            pRenderFunc = pRenderIndexedFunc;
    }
}

//
//Setter
//
void Object3D::setShaderProgram(ShaderProgram *shader)     { this->pShader = shader; }
void Object3D::setName(const std::string& name) 	       { this->sName = name; }
void Object3D::renderTessellated(bool tessellated)         { this->bRenderTessellated = tessellated; selectRenderFunc(); }
void Object3D::onAddInstance(AddInstanceCallback callback) { this->pAddInstanceCallback = callback; }


//
//Getter
//
Mesh*               Object3D::getMesh()                { return pMesh; }
std::string 		Object3D::getName() 			   { return sName; }
Object3DInstance* 	Object3D::getInstance(int index)   { return vInstances[index]; }
ShaderProgram*		Object3D::getShaderProgram()	   { return pShader; }
unsigned int        Object3D::numInstances() 	       { return vInstances.size(); }
VertexArrayObject*  Object3D::getVertexArrayObject()   { return pVertexArrayObject; }