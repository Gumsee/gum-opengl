#include "TextureCube.h"
#include <GL/glew.h>
#include <System/Output.h>
#include <System/MemoryManagement.h>
#include <future>
#include "WrapperFunctions.h"
#include <Codecs/TextureLoader.h>

TextureCube::TextureCube(std::string name)
{
	this->iType = TEXTURECUBE;
	this->sName = name;
	for(int i = 0; i < 0; i++)
	{
		bNeedsFreeing[i] = false;
    	vPixelData[i] = nullptr;
		iChannels[i] = 0;
	}
    bind(0);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    unbind();
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
    bind(0);
    //glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	for(int i = 0; i < 6; i++)
	{
		int pixelformat = GL_RGBA;
		switch(iChannels[i]) 
		{
			case 1:  pixelformat = GL_RED;  break;
			case 2:  pixelformat = GL_RG;   break;
			case 3:  pixelformat = GL_RGB;  break;
			case 4:  pixelformat = GL_RGBA; break;
		}

		if(!gumTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, pixelformat, v2Size[i], 0, pixelformat, GL_UNSIGNED_BYTE, &(vPixelData[i])[0]))
			Gum::Output::error("TextureCube::updateImage: glTexImage Failed.");
	}
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
			ImageData<unsigned char> imageData = TextureLoader::loadImage(texturepaths[i]);
			v2Size[i] = ivec2(imageData.width, imageData.height);
			iChannels[i] = imageData.numComps;
			vPixelData[i] = imageData.data;
			bNeedsFreeing[i] = true;
		}

		updateImage();
		if(!wait)
			vTexturesToLoad.push_back(this);	
        markLoaded();
    });



    if(wait)
    {
        future.wait();
        updateImage();
    }
}
	
void TextureCube::setData(unsigned char* data, const unsigned int& side)
{
	vPixelData[side] = data;
}