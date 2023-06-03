#pragma once
#include "Texture.h"

class TextureDepth : public Texture
{
protected:
	ivec2 v2Size;

public:
	TextureDepth(std::string name = "unknown", uint16_t datatype = Datatypes::UNSIGNED_CHAR);
	~TextureDepth();

    void updateImage();
	void bind(const int& index = 0);
	void unbind(const int& index = 0);


    //Setter
    void setSize(const ivec2& size);

    //Getter
    ivec2 getSize() const;
};
