#include <Graphics/TextureDepth2D.h>
#include <Graphics/WrapperFunctions.h>
#include <System/Output.h>
#include <GL/glew.h>

void TextureDepth2D::createNative()
{
    bind(0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    unbind(0);
}

void TextureDepth2D::bind(const int& index)
{
	glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_2D, iTextureID);
}

void TextureDepth2D::unbind(const int& index)
{
	glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void TextureDepth2D::updateImage()
{
    bind(0);
    gumPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    bool isFloat = iDatatype == Gum::Graphics::Datatypes::FLOAT;
    int pixelinternalformat = isFloat ? GL_DEPTH_COMPONENT32F : GL_DEPTH_COMPONENT24;
    if(!gumTexImage2D(GL_TEXTURE_2D, iCurrentMipmapLevel, pixelinternalformat, this->v2Size, 0, GL_DEPTH_COMPONENT, iDatatype, 0))
        Gum::Output::error("TextureDepth::updateImage: glTexImage Failed.");
    unbind(0);
}