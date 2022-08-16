#include "Texture.h"
#include "WrapperFunctions.h"
#include <GL/glew.h>
#include <iostream>

std::vector<Texture*> Texture::vTexturesToLoad;

Texture::Texture()
{
	this->bLoaded = false;
	this->bIsGrayscale = false;
	this->iTextureID = 0;
    gumGenTextures(1, &iTextureID);
	this->sName = "EmptyTexture";
}

Texture::~Texture()
{
	glDeleteTextures(1, &this->iTextureID);
}

void Texture::updateImage() {}
void Texture::bind(const int& index) {}
void Texture::unbind(const int& index) {}

//
// Setter
//
void Texture::setName(const std::string& name)	        { this->sName = name; }
void Texture::setID(const int& id) 				        { this->iTextureID = id; }
void Texture::markLoaded()                              { this->bLoaded = true; }
void Texture::setGrayscale(const bool& isgrayscale)     { this->bIsGrayscale = isgrayscale; }


//
// Getter
//
unsigned int Texture::getID() const				        { return this->iTextureID; }
unsigned int Texture::getType() const 			        { return this->iType; }
bool Texture::isLoaded() const 					        { return this->bLoaded; }
std::string Texture::getName() const			        { return this->sName; }
std::vector<Texture*> Texture::getTexturesToLoad()		{ return vTexturesToLoad; }
bool Texture::isGrayscale() const                       { return this->bIsGrayscale; }