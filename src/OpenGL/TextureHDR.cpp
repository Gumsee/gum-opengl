#include "TextureHDR.h"
#include "TextureLoader.h"
#include <GL/glew.h>
#include <iostream>
#include <future>
#include "WrapperFunctions.h"

TextureHDR::TextureHDR(std::string name)
{
	this->iType = TEXTUREHDR;
	this->sName = name;
    bind(0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    unbind(0);
}

void TextureHDR::load(std::string TexFilepath, bool wait)
{ 
    auto future = std::async(std::launch::async, [TexFilepath, wait, this] {
        TextureLoader::ImageData<float> imageData = TextureLoader::loadHDR(TexFilepath);
        setSize(ivec2(imageData.width, imageData.height));

        std::vector<float> pixels;
        for(int i = 0; i < imageData.width * imageData.height * imageData.numComps; i++)
        {
            pixels.push_back(imageData.data[i]);
        }

        setData(pixels);
        setNumChannels(imageData.numComps);
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

void TextureHDR::updateImage()
{
    bind(0);
    if(iNumChannels == 4)
        gumTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, v2Size, 0, GL_RGBA, GL_FLOAT, &vfPixelData[0]);
    else if(iNumChannels == 3)
        gumTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, v2Size, 0, GL_RGB, GL_FLOAT, &vfPixelData[0]);
    unbind(0);
}

//
// Setter
//
void TextureHDR::setData(std::vector<float> data)  { this->vfPixelData = data; }
void TextureHDR::setNumChannels(int c)             { this->iNumChannels = c; }
void TextureHDR::setPixel(int x, int y, vec4 color) 
{ 
    int pos = v2Size.x * y + x;
    vfPixelData[pos + 0] = color.x;
    vfPixelData[pos + 1] = color.y;
    vfPixelData[pos + 2] = color.z;
    vfPixelData[pos + 3] = color.w; 
}


//
// Getter
//
ivec2 TextureHDR::getSize() 						        { return this->v2Size; }
const float* TextureHDR::getPixelPtr() 	        { return &this->vfPixelData[0]; }
vec4 TextureHDR::getPixel(int x, int y) 	        
{ 
    int pos = v2Size.x * y + x;
    return vec4(vfPixelData[pos + 0], vfPixelData[pos + 1], vfPixelData[pos + 2], vfPixelData[pos + 3]);
}