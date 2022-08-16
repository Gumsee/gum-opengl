#include "TextureHDR.h"
#include <GL/glew.h>
#include <iostream>
#include <future>
#include "WrapperFunctions.h"
#include <Essentials/Filesystem/TextureLoader.h>

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

        vfPixelData = imageData.data;
        iChannels = imageData.numComps;
        
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
    int pixelformat = GL_RGBA;
    int pixelinternalformat = GL_RGBA32F;
    switch(iChannels) 
    {
        case 1:  pixelformat = GL_R;    pixelinternalformat = GL_R32F; break;
        case 2:  pixelformat = GL_RG;   pixelinternalformat = GL_RG32F; break;
        case 3:  pixelformat = GL_RGB;  pixelinternalformat = GL_RGB32F; break;
        case 4:  pixelformat = GL_RGBA; pixelinternalformat = GL_RGBA32F; break;
    }
    
    gumTexImage2D(GL_TEXTURE_2D, 0, pixelinternalformat, v2Size, 0, pixelformat, GL_FLOAT, &vfPixelData[0]);
    unbind(0);
}

//
// Setter
//
void TextureHDR::setData(float* data)  { this->vfPixelData = data; }
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