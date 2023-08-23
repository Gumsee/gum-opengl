#include <Graphics/Texture2D.h>
#include <Graphics/WrapperFunctions.h>
#include <System/Output.h>
#include <GL/glew.h>

void Texture2D::bind(const int& index)
{
	glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_2D, iTextureID);
}

void Texture2D::unbind(const int& index)
{
	glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::updateImage()
{
    bind(0);
    int pixelformat = GL_RGBA;
    int pixelinternalformat = GL_RGBA;
    bool isFloat = iDatatype == Datatypes::FLOAT;
    switch(iChannels) {
        case 1:  pixelformat = GL_RED;  pixelinternalformat = isFloat ? GL_R32F    : GL_RED;  break;
        case 2:  pixelformat = GL_RG;   pixelinternalformat = isFloat ? GL_RG32F   : GL_RG;   break;
        case 3:  pixelformat = GL_RGB;  pixelinternalformat = isFloat ? GL_RGB32F  : GL_RGB;  break;
        case 4:  pixelformat = GL_RGBA; pixelinternalformat = isFloat ? GL_RGBA32F : GL_RGBA; break;
    }
    gumPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    if(!gumTexImage2D(GL_TEXTURE_2D, 0, pixelinternalformat, v2Size, 0, pixelformat, iDatatype, vPixelData))
        Gum::Output::error("Texture2D::updateImage: glTexImage Failed.");
    if(bIsMipmapped)
        glGenerateMipmap(GL_TEXTURE_2D);
    unbind(0);
}


void Texture2D::repeat(bool mirrored)
{
    bind();
    int repeattype = mirrored ? GL_MIRRORED_REPEAT : GL_REPEAT;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, repeattype);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, repeattype);
    unbind();
}

void Texture2D::clampToEdge(bool border)
{
    bind();
    int clamptype = border ? GL_CLAMP_TO_BORDER : GL_CLAMP_TO_EDGE;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, clamptype);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, clamptype);
    unbind();
}

void Texture2D::setFiltering(FilteringTypes filteringtype) 
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