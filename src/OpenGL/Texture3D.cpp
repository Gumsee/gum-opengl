#include "Graphics/WrapperFunctions.h"
#include <Graphics/Texture3D.h>
#include <glad/gl.h>

template<typename T>
void tTexture3D<T>::bind(const int& index)
{
	glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_3D, iTextureID);
}

template<typename T>
void tTexture3D<T>::unbind(const int& index)
{
	glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_3D, 0);
}

template<typename T>
void tTexture3D<T>::updateImage()
{
    bind(0);
    int pixelformat = GL_RGBA;
    int pixelinternalformat = GL_RGBA;
    bool isFloat = Data::getDatatype() == Gum::Graphics::Datatypes::FLOAT;
    switch(Data::numChannels(0)) 
    {
        case 1:  pixelformat = GL_RED;  pixelinternalformat = isFloat ? GL_R32F    : GL_RED;  break;
        case 2:  pixelformat = GL_RG;   pixelinternalformat = isFloat ? GL_RG32F   : GL_RG;   break;
        case 3:  pixelformat = GL_RGB;  pixelinternalformat = isFloat ? GL_RGB32F  : GL_RGB;  break;
        case 4:  pixelformat = GL_RGBA; pixelinternalformat = isFloat ? GL_RGBA32F : GL_RGBA; break;
    }
    gumPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    glTexImage3D(GL_TEXTURE_3D, iCurrentMipmapLevel, pixelinternalformat, v3Size.x, v3Size.y, v3Size.z, 0, pixelformat, Data::getDatatype(), Data::getDataPtr(0));
    unbind(0);
}


template<typename T>
void tTexture3D<T>::repeat(bool mirrored)
{
    if(Gum::Graphics::RUN_HEADLESS)
        return;

    bind();
    int repeattype = mirrored ? GL_MIRRORED_REPEAT : GL_REPEAT;
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, repeattype);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, repeattype);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, repeattype);
    unbind();
}

template<typename T>
void tTexture3D<T>::clampToEdge(bool border)
{
    if(Gum::Graphics::RUN_HEADLESS)
        return;

    bind();
    int clamptype = border ? GL_CLAMP_TO_BORDER : GL_CLAMP_TO_EDGE;
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, clamptype);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, clamptype);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, clamptype);
    unbind();
}

template<typename T>
void tTexture3D<T>::setFiltering(FilteringType filteringtype) 
{
    if(Gum::Graphics::RUN_HEADLESS)
        return;
    
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
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, filtering);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, filtering);
    unbind();
}

template class tTexture3D<unsigned char>;
template class tTexture3D<float>;
