#pragma once
#include <gum-primitives.h>
#include <Essentials/Settings.h>

#include "ShaderProgram.h"
#include "VertexArrayObject.h"
#include "Object3DInstance.h"

class Object3D
{
private:
    inline static unsigned int LAST_OBJECT_INSTANCE_ID = 0;
	void createIndividualColor(Object3DInstance *instance);

protected:
	Object3D();

	ShaderProgram *pShader;
	VertexArrayObject* pVertexArrayObject;
    VertexBufferObject<mat4>* pTransMatricesVBO;
    VertexBufferObject<Vertex>* pVertexVBO;
    VertexBufferObject<vec3>* pIndividualColorsVBO;

	std::vector<Object3DInstance*> vInstances;
	std::vector<mat4> vTransforms;
	std::vector<vec3> vIndividualColors;
    
	//General
	bool hasbackface = true;		//Render only one side?
	bool inverseCulling = false;	//Show insides?
    bool renderTessellated = false;
	int ObjectType = 0; 			//TYPE
	float furthestAwayPoint = 0.0f;
	std::string sName;				//Object Name


	//Technical	(OpenGL)
	Mesh *pMesh = nullptr;


	void getFurthestAwayPoint(Object3DInstance *inst);
	void load();
	Object3DInstance* addInstance(Object3DInstance* instance);

public:
	Object3D(std::string ModelFilePath, std::string name);
	Object3D(Mesh *mesh, std::string name);
	virtual ~Object3D();

    void applyTransformationMatrix(Object3DInstance *inst);

	virtual void render();
	void renderMesh();

	Object3DInstance* addInstance();
	Object3DInstance* operator++();
	void RenderToShadowMap(bool var);

    //Setter
	void setShaderProgram(ShaderProgram *shader);
	void setName(std::string name);

    //Getter
	std::string getName();
    Mesh* getMesh();
	Object3DInstance* getInstance(int index = 0);
	ShaderProgram* getShaderProgram();
	unsigned int numInstances();
    VertexArrayObject* getVertexArrayObject();
};
