#include "Texture.h"
#include "WrapperFunctions.h"
#include <GL/glew.h>
#include <iostream>



uint16_t Texture::Datatypes::UNSIGNED_CHAR = GL_UNSIGNED_BYTE;
uint16_t Texture::Datatypes::FLOAT = GL_FLOAT;
uint16_t Texture::Datatypes::INTEGER = GL_INT;

Texture::Texture(Types type, uint16_t datatype)
{
    this->iType = type;
    this->iDatatype = datatype;
	this->bLoaded = false;
	this->bNeedsFreeing = false;
	this->bIsGrayscale = false;
    this->bIsMipmapped = false;
	this->iTextureID = 0;
    gumGenTextures(1, &iTextureID);
	this->sName = "EmptyTexture";
}

Texture::~Texture()
{
	glDeleteTextures(1, &this->iTextureID);
}

void Texture::loadTextures()
{
    std::lock_guard<decltype(loadMutex)> lock(loadMutex);
    for(Texture* tex : vToLoadTextures)
        tex->updateImage();
    vToLoadTextures.clear();
}

void Texture::createMipmaps()
{
    this->bIsMipmapped = true;
    updateImage();
}



//
// Setter
//
void Texture::setName(const std::string& name)	    { this->sName = name; }
void Texture::setID(const int& id) 				    { this->iTextureID = id; }
void Texture::markLoaded()                          { this->bLoaded = true; }
void Texture::setGrayscale(const bool& isgrayscale) { this->bIsGrayscale = isgrayscale; }


//
// Getter
//
unsigned int Texture::getID() const				    { return this->iTextureID; }
Texture::Types Texture::getType() const 			{ return this->iType; }
uint16_t Texture::getDatatype() const               { return this->iDatatype; }
bool Texture::isLoaded() const 					    { return this->bLoaded; }
std::string Texture::getName() const			    { return this->sName; }
bool Texture::isGrayscale() const                   { return this->bIsGrayscale; }