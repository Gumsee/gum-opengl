#pragma once
#include "Texture2D.h"

class TextureHDR : public Texture2D
{
protected:
	const int MAX_PIXEL_COLOR = 256 * 256 * 256;
    float* afPixelData;

public:
	TextureHDR(std::string name);
	~TextureHDR();

    void updateImage();
	void load(std::string TexFilepath, bool wait);
    void setData(float* data);
    void setPixel(int x, int y, vec4 color);
    void setNumChannels(int c);


    ivec2 getSize();
    const float* getPixelPtr();
    vec4 getPixel(int x, int y);
};