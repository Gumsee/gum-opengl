#pragma once
#include "Texture.h"

class Texture2D : public Texture
{
protected:
	const int MAX_PIXEL_COLOR = 256 * 256 * 256;
	ivec2 v2Size;
    std::vector<unsigned char> vPixelData;


public:
	Texture2D();
	Texture2D(std::string name);

    void updateImage();
	void load(std::string TexFilepath, bool wait);
	virtual void bind(int index = 0);
	virtual void unbind(int index = 0);

    float getHeightMapPixel(int x, int y);


    void setSize(ivec2 size);
    void setData(std::vector<unsigned char> data);
    void setPixel(int x, int y, vec4 color);


    ivec2 getSize();
    const unsigned char* getPixelPtr();
    vec4 getPixel(int x, int y);
};