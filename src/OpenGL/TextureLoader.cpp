#include "TextureLoader.h"
#include <Essentials/Output.h>
#include "Texture.h"
#include <Essentials/Tools.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <iostream>

/*#define TINYEXR_USE_MINIZ 0
#define TINYEXR_IMPLEMENTATION
#include <tinyexr.h>*/

TextureLoader::ImageData<unsigned char> TextureLoader::loadImage(const std::string& fileName)
{
    ImageData<unsigned char> imagedata;
	std::string extension = Tools::toUpperCase(fileName.substr(fileName.length() - 3, 3));

    if(extension == "BMP" || 
       extension == "PNG" || 
       extension == "TGA" || 
       extension == "JPG" || 
       extension == "JPEG" || 
       extension == "GIF" || 
       extension == "PSD" ||
       extension == "PIC"
       )
    {        
        unsigned char* data = stbi_load(fileName.c_str(), &imagedata.width, &imagedata.height, &imagedata.numComps, 0);
        size_t dataSize = imagedata.width * imagedata.height * imagedata.numComps;
        imagedata.data = (unsigned char*)malloc(dataSize);
        memcpy(imagedata.data, data, dataSize);

        
        if(!data)
            Gum::Output::error("TextureLoader: Failed to load image.");
        stbi_image_free(data);

    }
    else
    {
        Gum::Output::error("TextureLoader: Unknown file extension ." + extension);
    }

	Gum::Output::info("TextureLoader: Texture loaded! (" + fileName + ")");
    return imagedata;
}

TextureLoader::ImageData<float> TextureLoader::loadHDR(const std::string& fileName)
{
    ImageData<float> imagedata;
	std::string extension = Tools::toUpperCase(fileName.substr(fileName.length() - 3, 3));

    if(extension == "HDR")
    {
        //stbi_set_flip_vertically_on_load(true);
        stbi_ldr_to_hdr_gamma(1.0f);
        stbi_hdr_to_ldr_scale(1.0f);
        float* data = stbi_loadf(fileName.c_str(), &imagedata.width, &imagedata.height, &imagedata.numComps, 0);

        size_t dataSize = imagedata.width * imagedata.height * imagedata.numComps * sizeof(float);
        imagedata.data = (float*)malloc(dataSize);
        memcpy(imagedata.data, data, dataSize);
        
        if(!data)
            Gum::Output::error("TextureLoader: Failed to load HDR image.");
        stbi_image_free(data);
    }
    /*else if(extension == "EXR")
    {
        float* out; 
        const char* err = nullptr;
        if(LoadEXR(&out, &imagedata.width, &imagedata.height, fileName.c_str(), &err) != TINYEXR_SUCCESS) 
        {
            if (err) 
            {
                fprintf(stderr, "ERR : %s\n", err);
                FreeEXRErrorMessage(err); // release memory of error message.
            }
        } 
        else 
        {
            
            free(out); // release memory of image data
        }
    }*/
    else
    {
        Gum::Output::error("TextureLoader: Unknown file extension ." + extension);
    }

	Gum::Output::info("TextureLoader: Texture loaded! (" + fileName + ")");
    return imagedata;
}