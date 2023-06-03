#pragma once
#include "Texture.h"

class Texture3D : public Texture
{
protected:
	const int MAX_PIXEL_COLOR = 256 * 256 * 256;
	ivec3 v3Size;
    std::vector<unsigned char> vPixelData;


public:	
	Texture3D(std::string name = "unknown", uint16_t datatype = Datatypes::UNSIGNED_CHAR);

    void updateImage();
	void load(std::string TexFilepath, bool wait);
	void bind(const int& index = 0);
	void unbind(const int& index = 0);


    void setSize(ivec3 size);
    void setData(std::vector<unsigned char> data);
    void setPixel(int x, int y, vec4 color);


    ivec3 getSize();
    const unsigned char* getPixelPtr();
    vec4 getPixel(int x, int y);
};