#include <System/Output.h>
#include <Graphics/Texture2DBasic.h>
#include <Graphics/Texture2D.h>
#include <Graphics/WrapperFunctions.h>
#include <GL/glew.h>

void Texture2DBasic::createNative()
{
    gumGenTextures(1, &iTextureID);
}

void Texture2DBasic::destroyNative()
{
	glDeleteTextures(1, &this->iTextureID);
}

void Texture2DBasic::bind(const int& index)
{
	glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_2D, iTextureID);
}

void Texture2DBasic::unbind(const int& index)
{
    Texture2D::unbindGlobal(index);
}

void Texture2DBasic::updateImage(const ivec2& size, const int& numchannels, const void* data, uint16_t datatype)
{
    v2Size = size;
    bind(0);
    int pixelformat = GL_RGBA;
    int pixelinternalformat = GL_RGBA;
    bool isFloat = datatype == Gum::Graphics::Datatypes::FLOAT;
    switch(numchannels) 
    {
        case 1:  pixelformat = GL_RED;  pixelinternalformat = isFloat ? GL_R32F    : GL_RED;  break;
        case 2:  pixelformat = GL_RG;   pixelinternalformat = isFloat ? GL_RG32F   : GL_RG;   break;
        case 3:  pixelformat = GL_RGB;  pixelinternalformat = isFloat ? GL_RGB32F  : GL_RGB;  break;
        case 4:  pixelformat = GL_RGBA; pixelinternalformat = isFloat ? GL_RGBA32F : GL_RGBA; break;
    }
    gumPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    if(!gumTexImage2D(GL_TEXTURE_2D, 0, pixelinternalformat, v2Size, 0, pixelformat, datatype, data))
        Gum::Output::error("Texture2DBasic::updateImage: glTexImage Failed.");

    unbind(0);
}


void Texture2DBasic::repeat(bool mirrored)
{
    bind();
    int repeattype = mirrored ? GL_MIRRORED_REPEAT : GL_REPEAT;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, repeattype);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, repeattype);
    unbind();
}

void Texture2DBasic::clampToEdge(bool border)
{
    bind();
    int clamptype = border ? GL_CLAMP_TO_BORDER : GL_CLAMP_TO_EDGE;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, clamptype);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, clamptype);
    unbind();
}

void Texture2DBasic::setFiltering(Texture::FilteringType filteringtype) 
{
    bind();
    int filtering = 0;
    switch(filteringtype)
    {
        case Texture::LINEAR:                 filtering = GL_LINEAR; break;
        case Texture::NEAREST_NEIGHBOR:       filtering = GL_NEAREST; break;
        case Texture::LINEAR_MIPMAP_LINEAR:   filtering = GL_LINEAR_MIPMAP_LINEAR; break;
        case Texture::LINEAR_MIPMAP_NEAREST:  filtering = GL_LINEAR_MIPMAP_NEAREST; break;
        case Texture::NEAREST_MIPMAP_LINEAR:  filtering = GL_NEAREST_MIPMAP_LINEAR; break;
        case Texture::NEAREST_MIPMAP_NEAREST: filtering = GL_NEAREST_MIPMAP_NEAREST; break;
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtering);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtering);
    unbind();
}