#include "TextureCube.h"
#include <GL/glew.h>
#include <System/Output.h>
#include <System/MemoryManagement.h>
#include <future>
#include "WrapperFunctions.h"
#include <Codecs/TextureLoader.h>

TextureCube::TextureCube(std::string name, uint16_t datatype)
    : Texture(TEXTURECUBE, datatype)
{
	this->sName = name;
	for(int i = 0; i < 0; i++)
	{
		bNeedsFreeing[i] = false;
    	vPixelData[i] = nullptr;
		iChannels[i] = 0;
	}
    clampToEdge();
    setFiltering(LINEAR);
}

TextureCube::~TextureCube()
{
	for(int i = 0; i < 0; i++)
	{
		if(bNeedsFreeing[i] && vPixelData[i] != nullptr)
			free(vPixelData[i]);
	}
}

void TextureCube::bind(const int& index)
{
	glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_CUBE_MAP, iTextureID);
}

void TextureCube::unbind(const int& index)
{
	glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

	
void TextureCube::updateImage()
{
	for(int i = 0; i < 6; i++)
        updateImage(i);
}

void TextureCube::updateImage(int side)
{
    bind(0);
    int pixelformat = GL_RGBA;
    switch(iChannels[side]) 
    {
        case 1:  pixelformat = GL_RED;  break;
        case 2:  pixelformat = GL_RG;   break;
        case 3:  pixelformat = GL_RGB;  break;
        case 4:  pixelformat = GL_RGBA; break;
    }

    if(!gumTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + side, 0, pixelformat, v2Size[side], 0, pixelformat, iDatatype, vPixelData[side]))
        Gum::Output::error("TextureCube::updateImage: glTexImage Failed.");
    if(bIsMipmapped)
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP_POSITIVE_X + side);
    unbind(0);
}

void TextureCube::load(std::vector<std::string> texturepaths, bool wait)
{
	if(texturepaths.size() != 6)
	{
		Gum::Output::error("TextureCube.load: Wrong amount of textures specified.");
		return;
	}

    auto future = std::async(std::launch::async, [texturepaths, wait, this] {
		for(size_t i = 0; i < texturepaths.size(); i++)
		{
			ImageData imageData = TextureLoader::loadImage(texturepaths[i]);
			v2Size[i] = ivec2(imageData.width, imageData.height);
			iChannels[i] = imageData.numComps;
			vPixelData[i] = imageData.data;
			bNeedsFreeing[i] = true;
		}

        vToLoadTextures.push_back(this);
        markLoaded();
    });



    if(wait)
        future.wait();
}


void TextureCube::repeat(bool mirrored)
{
    bind();
    int repeattype = mirrored ? GL_MIRRORED_REPEAT : GL_REPEAT;
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, repeattype);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, repeattype);
    unbind();
}

void TextureCube::clampToEdge(bool border)
{
    bind();
    int clamptype = border ? GL_CLAMP_TO_BORDER : GL_CLAMP_TO_EDGE;
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, clamptype);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, clamptype);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, clamptype);
    unbind();
}

void TextureCube::setFiltering(FilteringTypes filteringtype) 
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



//
// Setter
//
void TextureCube::setData(unsigned char* data, const unsigned int& side)
{
	vPixelData[side] = data;
}

void TextureCube::setSize(ivec2 size, int side)
{
    v2Size[side] = size;
    updateImage(side);
}

void TextureCube::setSize(ivec2 size)
{
    for(int i = 0; i < 6; i++)
    {
        v2Size[i] = size;
        updateImage(i);
    }
}