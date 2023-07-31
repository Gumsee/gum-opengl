#pragma once
#include <functional>
#include <gum-primitives.h>
#include <Essentials/Settings.h>

#include "Renderable.h"
#include "ShaderProgram.h"
#include "VertexArrayObject.h"
#include "Object3DInstance.h"

class Object3D : public Renderable
{
public:
    typedef void (*RenderFunc)(Object3D*);
    typedef std::function<void(Object3DInstance*)> AddInstanceCallback;


private:
    static RenderFunc pRenderStripFunc, pRenderTessellatedStripFunc, pRenderIndexedFunc, pRenderTessellatedIndexedFunc;

protected:
	Object3D();

	ShaderProgram *pShader;
	VertexArrayObject* pVertexArrayObject;
    VertexBufferObject<mat4>* pTransMatricesVBO;
    VertexBufferObject<Vertex>* pVertexVBO;
    VertexBufferObject<vec4>* pIndividualColorsVBO;

	std::vector<Object3DInstance*> vInstances;
	std::vector<mat4> vTransforms;
	std::vector<vec4> vIndividualColors;
    
	//General
	bool hasbackface = true;		//Render only one side?
	bool inverseCulling = false;	//Show insides?
    bool bRenderTessellated = false;

	std::string sName;				//Object Name
    RenderFunc pRenderFunc;
    AddInstanceCallback pAddInstanceCallback;


	//Technical	(OpenGL)
	Mesh *pMesh = nullptr;


	void load();
	Object3DInstance* addInstance(Object3DInstance* instance);
    void selectRenderFunc();

public:
	Object3D(std::string ModelFilePath, std::string name);
	Object3D(Mesh *mesh, std::string name);
	virtual ~Object3D();

    void applyTransformationMatrix(Object3DInstance *inst);

    virtual void prerender() override {};
	virtual void render() override;
	void renderID() override;
	void renderMesh();

	Object3DInstance* addInstance();
	Object3DInstance* operator++();
	void RenderToShadowMap(bool var);

    //Setter
	void setShaderProgram(ShaderProgram *shader);
	void setName(const std::string& name);
    void renderTessellated(bool tessellated);
    void onAddInstance(AddInstanceCallback callback);

    //Getter
	std::string getName();
    Mesh* getMesh();
	Object3DInstance* getInstance(int index = 0);
	ShaderProgram* getShaderProgram();
	unsigned int numInstances();
    VertexArrayObject* getVertexArrayObject();
};
