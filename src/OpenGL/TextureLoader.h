#pragma once
#include <vector>
#include <string>

class TextureLoader
{
public:
    template<typename T>
    struct ImageData 
    {
        int width = 0;
        int height = 0;
        int numComps = 4;
        T* data;
    };
	static ImageData<unsigned char> loadImage(const std::string& fileName);
	static ImageData<float> loadHDR(const std::string& fileName);
};