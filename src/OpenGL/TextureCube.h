#pragma once
#include "Texture2D.h"
#include <cstdint>

class TextureCube : public Texture
{
protected:
	const int MAX_PIXEL_COLOR = 256 * 256 * 256;
	ivec2 v2Size[6];
    void* vPixelData[6];
	int iChannels[6];
	bool bNeedsFreeing[6];


public:
	TextureCube(std::string name = "unknown", uint16_t datatype = Datatypes::UNSIGNED_CHAR);
	~TextureCube();

	void bind(const int& index = 0);
	void unbind(const int& index = 0);
    
    void repeat(bool mirrored = false);
    void clampToEdge(bool border = false);
    void createMipmaps();
    void setFiltering(FilteringTypes filteringtype);
	
    void updateImage();
    void updateImage(int side);
	void load(std::vector<std::string> texturepaths, bool wait);
	
    void setData(unsigned char* data, const unsigned int& side);
    void setSize(ivec2 size, int side);
    void setSize(ivec2 size);
};