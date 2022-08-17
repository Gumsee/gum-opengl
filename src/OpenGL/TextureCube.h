#pragma once
#include "Texture2D.h"

class TextureCube : public Texture
{
protected:
	const int MAX_PIXEL_COLOR = 256 * 256 * 256;
	ivec2 v2Size[6];
    unsigned char* vPixelData[6];
	int iChannels[6];
	bool bNeedsFreeing[6];


public:
	TextureCube(std::string name);
	~TextureCube();

	void bind(const int& index = 0);
	void unbind(const int& index = 0);
	
    void updateImage();
	void load(std::vector<std::string> texturepaths, bool wait);
	
    void setData(unsigned char* data, const unsigned int& side);
};