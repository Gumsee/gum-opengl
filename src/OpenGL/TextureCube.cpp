#include <Graphics/TextureCube.h>
#include <Graphics/WrapperFunctions.h>
#include <System/Output.h>
#include <GL/glew.h>

template<typename T>
void tTextureCube<T>::bind(const int& index)
{
	glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_CUBE_MAP, iTextureID);
}

template<typename T>
void tTextureCube<T>::unbind(const int& index)
{
    tTextureCube::unbindGlobal(index);
}

template<typename T>
void tTextureCube<T>::unbindGlobal(const int& index)
{
	glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

template<typename T>
void tTextureCube<T>::updateImage(int side)
{
    bind(0);
    
    int pixelformat = GL_RGBA;
    int pixelinternalformat = GL_RGBA;
    bool isFloat = Data::getDatatype() == Gum::Graphics::Datatypes::FLOAT;
    switch(Data::numChannels(side)) 
    {
        case 1:  pixelformat = GL_RED;  pixelinternalformat = isFloat ? GL_R32F    : GL_RED;  break;
        case 2:  pixelformat = GL_RG;   pixelinternalformat = isFloat ? GL_RG32F   : GL_RG;   break;
        case 3:  pixelformat = GL_RGB;  pixelinternalformat = isFloat ? GL_RGB32F  : GL_RGB;  break;
        case 4:  pixelformat = GL_RGBA; pixelinternalformat = isFloat ? GL_RGBA32F : GL_RGBA; break;
    }

    if(!gumTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + side, iCurrentMipmapLevel, pixelinternalformat, v2Size[side], 0, pixelformat, Data::getDatatype(), Data::getDataPtr(side)))
        Gum::Output::error("tTextureCube::updateImage: glTexImage Failed.");
    if(side == 0 && bIsMipmapped)
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    unbind(0);
}


template<typename T>
void tTextureCube<T>::repeat(bool mirrored)
{
    bind();
    int repeattype = mirrored ? GL_MIRRORED_REPEAT : GL_REPEAT;
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, repeattype);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, repeattype);
    unbind();
}

template<typename T>
void tTextureCube<T>::clampToEdge(bool border)
{
    bind();
    int clamptype = border ? GL_CLAMP_TO_BORDER : GL_CLAMP_TO_EDGE;
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, clamptype);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, clamptype);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, clamptype);
    unbind();
}

template<typename T>
void tTextureCube<T>::setFiltering(FilteringType filteringtype) 
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
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, filtering);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, filtering);
    unbind();
}