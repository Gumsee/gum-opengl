#include "Texture2D.h"
#include "TextureLoader.h"
#include <GL/glew.h>
#include <iostream>
#include <future>
#include "WrapperFunctions.h"
#include <Essentials/Output.h>
#include <vector>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

Texture2D::Texture2D()
{
	this->iType = TEXTURE2D;
	this->sName = "unknown";
	this->iChannels = 4;
    bind(0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    unbind(0);
}

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

void Texture2D::bind(int index)
{
	glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_2D, iTextureID);
}

void Texture2D::unbind(int index)
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

        std::vector<unsigned char> pixels;
        for(unsigned int i = 0; i < imageData.width * imageData.height * iChannels; i++)
        {
            pixels.push_back(imageData.data[i]);
        }
        setData(pixels);
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

void Texture2D::loadFromMemory(unsigned char* pixels, size_t size, ivec2 dimensions, int numchannels)
{
    char* img;
    int w, h, n;
    extern char _binary_font_png_start, _binary_font_png_end;
    img = (char*)stbi_load_from_memory((unsigned char*)&_binary_font_png_start, 
        &_binary_font_png_end - &_binary_font_png_start, &w, &h, &n, 4);
    setNumChannels(numchannels);
    setSize(dimensions);
    for(size_t i = 0; i < size; i++)
        vPixelData.push_back(pixels[i]);
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
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    if(!gumTexImage2D(GL_TEXTURE_2D, 0, pixelformat, v2Size, 0, pixelformat, GL_UNSIGNED_BYTE, &vPixelData[0]))
        Gum::Output::error("Texture2D::updateImage: glTexImage Failed.");
    unbind(0);
}

void Texture2D::writeToFile(std::string filename, int filetype)
{
    if(filetype == GUM_TEXTURE_FILETYPE_PNG)
        stbi_write_png(filename.c_str(), getSize().x, getSize().y, iChannels, &vPixelData[0], getSize().x * iChannels);
    if(filetype == GUM_TEXTURE_FILETYPE_JPG)
        stbi_write_jpg(filename.c_str(), getSize().x, getSize().y, iChannels, &vPixelData[0], 100);
}
    
void Texture2D::initEmpty()
{
    vPixelData.resize(v2Size.x * v2Size.y * iChannels);
    std::fill(vPixelData.begin(), vPixelData.end(), 255);
}

//
// Setter
//
void Texture2D::setSize(ivec2 size)                       { this->v2Size = size; }
void Texture2D::setData(std::vector<unsigned char> data)  { this->vPixelData = data; }
void Texture2D::setPixel(int x, int y, vec4 color) 
{
    int pos = v2Size.x * y * iChannels + x * iChannels;
    for(unsigned int i = 0; i < iChannels; i++)
        vPixelData[pos + i] = color[i] * 255;
}
void Texture2D::setNumChannels(int channels)
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
ivec2 Texture2D::getSize() 						        { return this->v2Size; }
const unsigned char* Texture2D::getPixelPtr() 	        { return &this->vPixelData[0]; }
vec4 Texture2D::getPixel(int x, int y) 	        
{ 
    int pos = v2Size.x * y * iChannels + x * iChannels;
    vec4 retcol;
    for(unsigned int i = 0; i < iChannels; i++)
        retcol[i] = (float)vPixelData[pos + i] / 255.0f;
    return retcol;
}
int Texture2D::numChannels() { return this->iChannels; }
