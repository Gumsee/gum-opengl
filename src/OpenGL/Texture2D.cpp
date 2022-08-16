#include "Texture2D.h"
#include <GL/glew.h>
#include <iostream>
#include <future>
#include "WrapperFunctions.h"
#include <Essentials/Output.h>
#include <Essentials/Filesystem/TextureLoader.h>
#include <Essentials/Tools.h>
#include <vector>

Texture2D::Texture2D() : Texture2D("unknown") {}
Texture2D::Texture2D(std::string name)
{
	this->iType = TEXTURE2D;
	this->sName = name;
	this->iChannels = 4;
    bind(0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    unbind(0);
}

Texture2D::~Texture2D()
{
    free(vPixelData);
}

float Texture2D::getHeightMapPixel(int x, int y)
{
	if(x < 0 || x > v2Size.x || 
       y < 0 || y > v2Size.y)
	{
		return 0.0f;
	}
	else
	{
		float returnHeight = getPixel(x, y).x * getPixel(x, y).y * getPixel(x, y).z;
		returnHeight += MAX_PIXEL_COLOR / 2.0f;
		returnHeight /= MAX_PIXEL_COLOR / 2.0f;
		return returnHeight;
	}
}

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

void Texture2D::load(std::string TexFilepath, bool wait)
{ 
    auto future = std::async(std::launch::async, [TexFilepath, wait, this] {
        TextureLoader::ImageData<unsigned char> imageData = TextureLoader::loadImage(TexFilepath);
        setSize(ivec2(imageData.width, imageData.height));
        iChannels = imageData.numComps;
        vPixelData = imageData.data;

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

void Texture2D::loadFromMemory(unsigned char* pixels, size_t size)
{
    TextureLoader::ImageData<unsigned char> imageData = TextureLoader::loadImage(pixels, size);
    setNumChannels(imageData.numComps);
    setSize(ivec2(imageData.width, imageData.height));
    vPixelData = imageData.data;

    updateImage();
}

void Texture2D::updateImage()
{
    bind(0);
    int pixelformat = GL_RGBA;
    switch(iChannels) {
        case 1:  pixelformat = GL_RED;  break;
        case 2:  pixelformat = GL_RG;   break;
        case 3:  pixelformat = GL_RGB;  break;
        case 4:  pixelformat = GL_RGBA; break;
    }
    gumPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    if(!gumTexImage2D(GL_TEXTURE_2D, 0, pixelformat, v2Size, 0, pixelformat, GL_UNSIGNED_BYTE, &vPixelData[0]))
        Gum::Output::error("Texture2D::updateImage: glTexImage Failed.");
    unbind(0);
}
    
void Texture2D::initEmpty()
{
    //vPixelData.resize(v2Size.x * v2Size.y * iChannels);
    //std::fill(vPixelData.begin(), vPixelData.end(), 255);
}

//
// Setter
//
void Texture2D::setSize(const ivec2& size)                              { this->v2Size = size; }
void Texture2D::setData(unsigned char* data)                { this->vPixelData = data; }
void Texture2D::setPixel(const int& x, const int& y, const vec4& color) 
{
    int pos = v2Size.x * y * iChannels + x * iChannels;
    for(unsigned int i = 0; i < iChannels; i++)
        vPixelData[pos + i] = color[i] * 255;
}
void Texture2D::setNumChannels(const int& channels)
{
    if(channels > 4)
    {
        Gum::Output::error("Invalid number of channels");
        return;
    }
    this->iChannels = channels;
}


//
// Getter
//
ivec2 Texture2D::getSize() const				        { return this->v2Size; }
const unsigned char* Texture2D::getPixelPtr() 	        { return &this->vPixelData[0]; }
vec4 Texture2D::getPixel(int x, int y) const
{ 
    int pos = v2Size.x * y * iChannels + x * iChannels;
    vec4 retcol;
    for(unsigned int i = 0; i < iChannels; i++)
        retcol[i] = (float)vPixelData[pos + i] / 255.0f;
    return retcol;
}
int Texture2D::numChannels() const                      { return this->iChannels; }