#include "Texture3D.h"
#include "TextureLoader.h"
#include <GL/glew.h>
#include <iostream>
#include <future>

Texture3D::Texture3D()
{
}

Texture3D::Texture3D(std::string name)
{
	this->iType = TEXTURE3D;
	this->sName = name;
}

void Texture3D::bind(int index)
{
	glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_3D, iTextureID);
}

void Texture3D::unbind(int index)
{
	glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_3D, 0);
}

void Texture3D::load(std::string TexFilepath, bool wait)
{ 
    auto future = std::async(std::launch::async, [TexFilepath, wait, this] {
        TextureLoader::ImageData<unsigned char> imageData = TextureLoader::loadImage(TexFilepath);
        setSize(ivec3(imageData.width, imageData.height, 0));

        std::vector<unsigned char> pixels;
        for(int i = 0; i < imageData.width * imageData.height * 4; i++)
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

void Texture3D::updateImage()
{
    bind(0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, v3Size.x, v3Size.y, v3Size.z, 0, GL_RGBA, GL_UNSIGNED_BYTE, &vPixelData[0]);
    unbind(0);
}

//
// Setter
//
void Texture3D::setSize(ivec3 size)                       { this->v3Size = size; }
void Texture3D::setData(std::vector<unsigned char> data)  { this->vPixelData = data; }
void Texture3D::setPixel(int x, int y, vec4 color) 
{ 
    int pos = v3Size.x * y + x;
    vPixelData[pos + 0] = color.x;
    vPixelData[pos + 1] = color.y;
    vPixelData[pos + 2] = color.z;
    vPixelData[pos + 3] = color.w; 
}


//
// Getter
//
ivec3 Texture3D::getSize() 						        { return this->v3Size; }
const unsigned char* Texture3D::getPixelPtr() 	        { return &this->vPixelData[0]; }
vec4 Texture3D::getPixel(int x, int y) 	        
{ 
    int pos = v3Size.x * y + x;
    return vec4(vPixelData[pos + 0], vPixelData[pos + 1], vPixelData[pos + 2], vPixelData[pos + 3]);
}