#include "TextureDepth.h"
#include <GL/glew.h>
#include <iostream>
#include <future>
#include "WrapperFunctions.h"
#include <System/Output.h>
#include <System/MemoryManagement.h>
#include <Codecs/TextureLoader.h>
#include <Essentials/Tools.h>
#include <vector>

TextureDepth::TextureDepth(std::string name, uint16_t datatype)
    : Texture(TEXTUREDEPTH, datatype)
{
	this->sName = name;
    
    bind(0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    unbind(0);
}

TextureDepth::~TextureDepth()
{
}


void TextureDepth::bind(const int& index)
{
	glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_2D, iTextureID);
}

void TextureDepth::unbind(const int& index)
{
	glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void TextureDepth::updateImage()
{
    bind(0);
    gumPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    if(!gumTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, this->v2Size, 0, GL_DEPTH_COMPONENT, iDatatype, 0))
        Gum::Output::error("TextureDepth::updateImage: glTexImage Failed.");
    unbind(0);
}

//
// Setter
//
void TextureDepth::setSize(const ivec2& size) { this->v2Size = size; updateImage(); }


//
// Getter
//
ivec2 TextureDepth::getSize() const			  { return this->v2Size; }