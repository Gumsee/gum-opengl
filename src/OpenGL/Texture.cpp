#include <Graphics/Texture.h>
#include <Graphics/WrapperFunctions.h>
#include <GL/glew.h>

const uint16_t Texture::Pixelformat::RGBA        = GL_RGBA;
const uint16_t Texture::Pixelformat::RGB         = GL_RGB;
const uint16_t Texture::Pixelformat::RG          = GL_RG;
const uint16_t Texture::Pixelformat::R           = GL_R;
const uint16_t Texture::Pixelformat::BGRA        = GL_BGRA;
const uint16_t Texture::Pixelformat::BGR         = GL_BGR;
const uint16_t Texture::Pixelformat::RGBA16F     = GL_RGBA16F;
const uint16_t Texture::Pixelformat::RGBA32F     = GL_RGBA32F;

void Texture::createNative()
{
    gumGenTextures(1, &iTextureID);
}

void Texture::destroyNative()
{
	glDeleteTextures(1, &this->iTextureID);
}