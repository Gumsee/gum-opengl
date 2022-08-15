#pragma once
#include "Texture.h"

#define GUM_TEXTURE_FILETYPE_BMP 0
#define GUM_TEXTURE_FILETYPE_PNG 1
#define GUM_TEXTURE_FILETYPE_TGA 2
#define GUM_TEXTURE_FILETYPE_JPG 2
#define GUM_TEXTURE_FILETYPE_PNG_HEADER 3

class Texture2D : public Texture
{
protected:
	const unsigned int MAX_PIXEL_COLOR = 256 * 256 * 256;
	ivec2 v2Size;
    unsigned int iChannels;
    std::vector<unsigned char> vPixelData;


public:
	Texture2D();
	Texture2D(std::string name);

    void updateImage();
	void load(std::string TexFilepath, bool wait);
	void loadFromMemory(unsigned char* pixels, size_t size);
	void bind(const int& index = 0);
	void unbind(const int& index = 0);

    float getHeightMapPixel(int x, int y);
    void writeToFile(std::string filename, int filetype);
    void initEmpty();


    //Setter
    void setSize(const ivec2& size);
    void setData(std::vector<unsigned char> data);
    void setPixel(const int& x, const int& y, const vec4& color);
    void setNumChannels(const int& channels);

    //Getter
    ivec2 getSize() const;
    const unsigned char* getPixelPtr();
    vec4 getPixel(int x, int y) const;
    int numChannels() const;
};
