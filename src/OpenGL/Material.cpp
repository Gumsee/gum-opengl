#include "Material.h"


Material::Material()
{
	sName = "";
	fReflectivity = 0.0f;
	fRefractivity = 0.0f;
	fSpecularity = 0.3f;
	fRoughness = 0.8f;
	iTextureMultiplier = 1;
	bTransparency = false;
	bFlipNormal = false;
	vColor = vec4(0.3,0.3,0.3,1);

	textures.resize(16);
	iNumUsableTextures = 14;

	bHasNormalMap = false;
	bHasSpecularMap = false;
	bHasReflectionMap = false;
	bHasRefractionMap = false;
	bHasRoughnessMap = false;
	bHasDisplacementMap = false;
	bHasAmbientOcclusionMap = false;
	bHasBlendMap = false;
	bHasTexture = false;
}

Material::~Material() {}

//Getter
bool Material::isReflective() 								{ return fReflectivity > 0; }
bool Material::isRefractive() 								{ return fRefractivity > 0; }
bool Material::isTransparent() 								{ return bTransparency; }
bool Material::hasFlippedNormals()							{ return bFlipNormal; }
float *Material::getSpecularity() 							{ return &fSpecularity; }
float *Material::getRoughness() 							{ return &fRoughness; }
float *Material::getReflectivity()							{ return &fReflectivity; }
float *Material::getRefractivity()							{ return &fRefractivity; }
int Material::getTextureMultiplier()						{ return iTextureMultiplier; }
std::string Material::getName() 							{ return this->sName; }
vec4 Material::getColor()								    { return this->vColor; }
vec4 *Material::getColorPtr()							    { return &this->vColor; }
Texture* Material::getTexture(int index) 					{ return this->textures[index]; }
int Material::numTextures() 								{ return iNumUsableTextures; }

//Setter
void Material::setName(std::string name) 					{ this->sName = name; }
void Material::setColor(vec4 color)					        { this->vColor = color; }
void Material::setSpecularity(float specularity)			{ this->fSpecularity = specularity; }
void Material::setRoughness(float roughness)				{ this->fRoughness = roughness; }
void Material::setReflectivity(float reflectivity)			{ this->fReflectivity = reflectivity; }
void Material::setRefractivity(float refractivity)			{ this->fRefractivity = refractivity; }
void Material::setTextureMultiplier(float texMultiplier)	{ this->iTextureMultiplier = texMultiplier; }
void Material::setIsTransparency(bool isTransparent)		{ this->bTransparency = isTransparent; }
void Material::flipNormals(bool shouldFlip)					{ this->bFlipNormal = shouldFlip; }





/*
	Texture unit 16 is always the ShadowMap called "ShadowMap"
	Texture unit 15 is always the EnviormentMap called "Enviorment"
	Texture unit 14 is always the NormalMap called "normalmap"
	Texture unit 13 is always the DisplacementMap called "Displacement"
	Texture unit 12 is always the BlendMap called "blendtex"
	Texture unit 11 is always the Reflection Texture called "reflectionmap"
	Texture unit 10 is always the Refraction Texture called "refractionmap"
	Texture unit 9 is always the SpecularMap Texture called "specularmap"
	Texture unit 8 is always the roughness Texture called "roughnessmap"
	Texture unit 7 is always the ambient occlusion Texture called "ambientOcclusionmap"
	
*/
void Material::setTexture(Texture *Tex, int Index)
{
	textures[Index] = Tex;
	if(Tex != nullptr)
	{
		if(Index == 14) 		{ this->bHasNormalMap = true; }
		else if(Index == 13) 	{ this->bHasDisplacementMap = true; }
		else if(Index == 12) 	{ this->bHasBlendMap = true; }
		else if(Index == 11) 	{ this->bHasReflectionMap = true; }
		else if(Index == 10) 	{ this->bHasRefractionMap = true;	}
		else if(Index == 9) 	{ this->bHasSpecularMap = true; }
		else if(Index == 8)		{ this->bHasRoughnessMap = true; }
		else if(Index == 7)		{ this->bHasAmbientOcclusionMap = true; }
		else if(Index < 7)		{ this->bHasTexture = true;}
	}
	else
	{
		delTexture(Index);
	}
}

void Material::delTexture(int Index)
{
	//Textures.erase(Textures.begin() + Index);
	textures[Index] = nullptr;

	if(Index == 14)			{ this->bHasNormalMap = false;	}
	else if(Index == 13) 	{ this->bHasDisplacementMap = false; }
	else if(Index == 12) 	{ this->bHasBlendMap = false; }
	else if(Index == 11)	{ this->bHasReflectionMap = false;	}
	else if(Index == 10)	{ this->bHasRefractionMap = false; }
	else if(Index == 9)		{ this->bHasSpecularMap = false; }
	else if(Index == 8)		{ this->bHasRoughnessMap = false; }
	else if(Index == 7)		{ this->bHasAmbientOcclusionMap = false; }
	else if(Index < 7)		{ this->bHasTexture = false;}
}

void Material::bindTextures()
{
	for (int i = 0; i < textures.size(); i++)
	{
		if(textures[i] != nullptr)
		{
			textures[i]->bind(i);
		}
	}
}
void Material::unbindTextures()
{
	for (int i = 0; i < textures.size(); i++)
	{
		if(textures[i] != nullptr)
		{
			textures[i]->bind(i);
		}
	}
}

bool Material::hasNormalMap() 			{ return this->bHasNormalMap; }
bool Material::hasSpecularMap() 		{ return this->bHasSpecularMap; }
bool Material::hasReflectionMap() 		{ return this->bHasReflectionMap; }
bool Material::hasRefractionMap() 		{ return this->bHasRefractionMap; }
bool Material::hasRoughnessMap() 		{ return this->bHasRoughnessMap; }
bool Material::hasDisplacementMap() 	{ return this->bHasDisplacementMap; }
bool Material::hasAmbientOcclusionMap() { return this->bHasAmbientOcclusionMap; }
bool Material::hasBlendMap() 			{ return this->bHasBlendMap; }
bool Material::hasTexture() 			{ return this->bHasTexture; }