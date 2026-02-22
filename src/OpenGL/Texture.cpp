#include <Graphics/Texture.h>
#include <Graphics/WrapperFunctions.h>
#include <GL/glew.h>

void Texture::createNative()
{
    gumGenTextures(1, &iTextureID);
}

void Texture::destroyNative()
{
	glDeleteTextures(1, &this->iTextureID);
}