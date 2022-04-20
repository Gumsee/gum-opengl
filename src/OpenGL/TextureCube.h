#pragma once
#include "Texture2D.h"

class TextureCube : public Texture2D
{
protected:
	const int MAX_PIXEL_COLOR = 256 * 256 * 256;
	ivec2 v2Size;
    std::vector<unsigned char> vPixelData;


public:
	TextureCube(std::string name);

	void bind(int index = 0);
	void unbind(int index = 0);
};