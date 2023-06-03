#include "Object3D.h"
#include "ElementBufferObject.h"
#include "System/Output.h"
#include <Essentials/Tools.h>
#include <System/MemoryManagement.h>
#include <Codecs/Scene3DLoader.h>
#include <string>

Object3D::Object3D()
{
    pVertexArrayObject = new VertexArrayObject();

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

		pIndividualColorsVBO = new VertexBufferObject<vec3>();
		pVertexArrayObject->addAttribute(pIndividualColorsVBO, 10, 3, GL_FLOAT, sizeof(vec3), 0, 1);


        ElementBufferObject* indexBuffer = new ElementBufferObject();
        indexBuffer->setData(pMesh->getIndexBuffer());
        pVertexArrayObject->addElementBuffer(indexBuffer);
        pVertexArrayObject->unbind();


		/*
		Instance *instance = new Instance();
		instance->parentObject = this;
		vInstances.push_back(instance);
		vTransforms.push_back(instance->getMatrix());
		//Get individual ID
		createIndividualColor(instance);*/
		//addInstance();
		
		//getFurthestAwayPoint(vInstances[0]);
	}
	else
	{
		Gum::Output::warn("ObjectCreation: Mesh is nullptr!");
	}
}


void Object3D::createIndividualColor(Object3DInstance *instance)
{
	vec3 individualColor;
	individualColor.r = (instance->getID() & 0x000000FF) >>  0;
	individualColor.g = (instance->getID() & 0x0000FF00) >>  8;
	individualColor.b = (instance->getID() & 0x00FF0000) >> 16;
	vIndividualColors.push_back(individualColor / 255.0f);
	pIndividualColorsVBO->setData(vIndividualColors);
}

//Rendering stuff
void Object3D::render()
{
    renderMesh();
}

void Object3D::renderMesh()
{
	//ShaderProgram::getCurrentlyBoundShader()->loadUniform("isInstanced", numInstances() > 1);
	//ShaderProgram::getCurrentlyBoundShader()->loadUniform("transformationMatrix", getInstance(0)->getMatrix());

    pVertexArrayObject->bind();
    if(renderTessellated)
        glDrawElementsInstanced(GL_PATCHES, pVertexArrayObject->numVertices(), GL_UNSIGNED_INT, 0, vInstances.size());
    else
        glDrawElementsInstanced(GL_TRIANGLES, pVertexArrayObject->numVertices(), GL_UNSIGNED_INT, 0, vInstances.size());

    pVertexArrayObject->unbind();
}

Object3DInstance* Object3D::addInstance(Object3DInstance* instance)
{
	instance->setID(LAST_OBJECT_INSTANCE_ID++);
	vInstances.push_back(instance);
	vTransforms.push_back(instance->getMatrix());
	pTransMatricesVBO->setData(vTransforms, GL_DYNAMIC_DRAW);

	//Get individual ID
	createIndividualColor(instance);

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

void Object3D::getFurthestAwayPoint(Object3DInstance *inst)
{
	if(pMesh != nullptr)
	{
		for(unsigned int i = 0; i < pMesh->numVertices(); i++)
		{
            float dist = vec3::distance(pMesh->getVertex(i).position * inst->getScale(), vec3(0.0f));
            if(dist > this->furthestAwayPoint)
                this->furthestAwayPoint = dist;
		}
	}
}


//
//Setter
//
void Object3D::setShaderProgram(ShaderProgram *shader) { this->pShader = shader; }
void Object3D::setName(std::string name) 			   { this->sName = name; }


//
//Getter
//
Mesh*               Object3D::getMesh()                { return pMesh; }
std::string 		Object3D::getName() 			   { return sName; }
Object3DInstance* 	Object3D::getInstance(int index)   { return vInstances[index]; }
ShaderProgram*		Object3D::getShaderProgram()	   { return pShader; }
unsigned int        Object3D::numInstances() 	       { return vInstances.size(); }
VertexArrayObject*  Object3D::getVertexArrayObject()   { return pVertexArrayObject; }