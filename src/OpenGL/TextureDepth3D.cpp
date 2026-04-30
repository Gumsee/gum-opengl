#include <Graphics/TextureDepth3D.h>
#include <Graphics/WrapperFunctions.h>
#include <System/Output.h>
#include <glad/gl.h>

void TextureDepth3D::createNative()
{
    bind(0);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    //glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    unbind(0);
}

void TextureDepth3D::bind(const int& index)
{
	glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_2D_ARRAY, iTextureID);
}

void TextureDepth3D::unbind(const int& index)
{
	glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
}

void TextureDepth3D::updateImage()
{
    bind(0);
    bool isFloat = iDatatype == Gum::Graphics::Datatypes::FLOAT;
    int pixelinternalformat = isFloat ? GL_DEPTH_COMPONENT32F : GL_DEPTH_COMPONENT24;
    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, pixelinternalformat, v2Size.x, v2Size.y, iNumLayers, 0, GL_DEPTH_COMPONENT, iDatatype, 0);
    unbind(0);
}


void TextureDepth3D::setBordercolor(const color& col)
{
    bind(0);
    glTexParameterfv(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BORDER_COLOR, col.getGLColor().vals);
    unbind(0);
}