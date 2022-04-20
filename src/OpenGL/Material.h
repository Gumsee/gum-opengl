#pragma once
#include <string>
#include <vector>
#include "Texture.h"

class Material
{
private:
	std::string sName;

	float fReflectivity;
	float fRefractivity;
	float fSpecularity;
	float fRoughness;
	int iTextureMultiplier;
	int iNumUsableTextures;
	bool bTransparency;
	bool bFlipNormal;
	vec4 vColor;

	//Texture related stuff
	std::vector<Texture*> textures;	
	bool bHasNormalMap;
	bool bHasSpecularMap;
	bool bHasReflectionMap;
	bool bHasRefractionMap;
	bool bHasRoughnessMap;
	bool bHasDisplacementMap;
	bool bHasAmbientOcclusionMap;
	bool bHasBlendMap;
	bool bHasTexture;
    
public:
	Material();
	~Material();

	bool isReflective();
	bool isRefractive();
	bool isTransparent();
	bool hasFlippedNormals();
	float *getSpecularity();
	float *getRoughness();
	float *getReflectivity();
	float *getRefractivity();
	int getTextureMultiplier();
	std::string getName();
	vec4 getColor();
	vec4* getColorPtr();
	Texture* getTexture(int index);
	int numTextures();

	void setName(std::string name);
	void setColor(vec4 color);
	void setSpecularity(float specularity);
	void setRoughness(float roughness);
	void setReflectivity(float reflectivity);
	void setRefractivity(float refractivity);
	void setTextureMultiplier(float texMultiplier);
	void setIsTransparency(bool isTransparent);
	void flipNormals(bool shouldFlip = true);
	void setTexture(Texture *Tex, int Index);
	void delTexture(int Index);
	void bindTextures();
	void unbindTextures();


	bool hasNormalMap();
	bool hasSpecularMap();
	bool hasReflectionMap();
	bool hasRefractionMap();
	bool hasRoughnessMap();
	bool hasDisplacementMap();
	bool hasAmbientOcclusionMap();
	bool hasBlendMap();
	bool hasTexture();
};