#pragma once
#include "Renderable.h"
#include "ShaderProgram.h"
#include "Sprite2DInstance.h"
#include "Texture2D.h"
#include "VertexArrayObject.h"
#include <functional>

class Sprite2D : public Renderable
{
private:
    typedef void (*RenderFunc)(Sprite2D*);
    typedef std::function<void(Sprite2DInstance*)> AddInstanceCallback;
    inline static const std::vector<float> afSpriteVertices {
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
    };

    inline static const std::vector<unsigned int> aiSpriteIndices {
        0, 1, 2, 2, 3, 0
    };

protected:
	ShaderProgram *pShader;
	inline static VertexArrayObject* pVertexArrayObject = nullptr;
    inline static ElementBufferObject *pIndexBuffer = nullptr;
    VertexBufferObject<mat3>* pTransMatricesVBO;
    VertexBufferObject<vec4>* pIndividualColorsVBO;

    Texture2D* pTexture;
	std::string sName;
	std::vector<Sprite2DInstance*> vInstances;
	std::vector<mat3> vTransforms;
	std::vector<vec4> vIndividualColors;
    AddInstanceCallback pAddInstanceCallback;
	Sprite2DInstance* addInstance(Sprite2DInstance* instance);

public:
    Sprite2D();
    ~Sprite2D();
    
    virtual void prerender() override {};
	virtual void render() override;
    void renderMesh();
	void renderID() override;

    void applyTransformationMatrix(Sprite2DInstance *inst);
	Sprite2DInstance* addInstance();
	Sprite2DInstance* operator++();
    

    //Setter
	void setShaderProgram(ShaderProgram *shader);
	void setName(const std::string& name);
    void onAddInstance(AddInstanceCallback callback);
    void setTexture(Texture2D* texture);


    //Getter
	std::string getName();
	Sprite2DInstance* getInstance(int index = 0);
	ShaderProgram* getShaderProgram();
	unsigned int numInstances();
    VertexArrayObject* getVertexArrayObject();

    static void cleanupSprites();
};