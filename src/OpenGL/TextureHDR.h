#pragma once
#include "Texture2D.h"

class TextureHDR : public Texture2D
{
public:
    TextureHDR(std::string name = "unknown")
        : Texture2D(name, Datatypes::FLOAT)
    {
        this->iType = TEXTUREHDR;
        clampToEdge();
        setFiltering(LINEAR);
    }
};