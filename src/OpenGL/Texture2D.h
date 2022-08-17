#pragma once
#include "Texture.h"

class Texture2D : public Texture
{
protected:
	const unsigned int MAX_PIXEL_COLOR = 256 * 256 * 256;
	ivec2 v2Size;
    unsigned int iChannels;
    unsigned char* vPixelData;


public:
	Texture2D();
	Texture2D(std::string name);
	virtual ~Texture2D();

    void updateImage();
	void load(std::string TexFilepath, bool wait);
	void loadFromMemory(unsigned char* pixels, size_t size);
	void bind(const int& index = 0);
	void unbind(const int& index = 0);

    float getHeightMapPixel(int x, int y);
    void initEmpty();


    //Setter
    void setSize(const ivec2& size);
    void setData(unsigned char* data);
    void setPixel(const int& x, const int& y, const vec4& color);
    void setNumChannels(const int& channels);

    //Getter
    ivec2 getSize() const;
    const unsigned char* getPixelPtr();
    vec4 getPixel(int x, int y) const;
    int numChannels() const;
};
