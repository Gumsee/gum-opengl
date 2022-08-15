#pragma once
#include "Texture2D.h"

class TextureCube : public Texture2D
{
protected:
	const int MAX_PIXEL_COLOR = 256 * 256 * 256;
	ivec2 v2Size[6];
    std::vector<unsigned char> vPixelData[6];
	int iChannels[6];


public:
	TextureCube(std::string name);

	void bind(const int& index = 0);
	void unbind(const int& index = 0);
	
    void updateImage();
	void load(std::vector<std::string> texturepaths, bool wait);
	
    void setData(std::vector<unsigned char> data, const unsigned int& side);
};