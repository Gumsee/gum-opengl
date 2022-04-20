#include "Texture.h"
#include <GL/glew.h>
#include <iostream>

std::vector<Texture*> Texture::vTexturesToLoad;

Texture::Texture()
{
	this->bLoaded = false;
	this->iTextureID = 0;
    glGenTextures(1, &iTextureID);
	this->sName = "EmptyTexture";
}

Texture::~Texture()
{
	glDeleteTextures(1, &this->iTextureID);
}

void Texture::updateImage() {}
void Texture::bind(int index) {}
void Texture::unbind(int index) {}

//
// Setter
//
void Texture::setName(std::string name) 		        { this->sName = name; }
void Texture::setID(int id) 					        { this->iTextureID = id; }
void Texture::markLoaded()                              { this->bLoaded = true; }


//
// Getter
//
unsigned int Texture::getID() 					        { return this->iTextureID; }
unsigned int Texture::getType() 				        { return this->iType; }
bool Texture::isLoaded() 						        { return this->bLoaded; }
std::string Texture::getName() 					        { return this->sName; }
std::vector<Texture*> Texture::getTexturesToLoad()		{ return vTexturesToLoad; }