#pragma once
#include <gum-maths.h>
#include <vector>

class Texture
{
protected:
	const int MAX_PIXEL_COLOR = 256 * 256 * 256;
	unsigned int iTextureID;
	unsigned int iType;
	std::string sName;
	bool bLoaded;

	static std::vector<Texture*> vTexturesToLoad;

public:	
	enum Types
	{
		TEXTURE2D,
		TEXTURE3D,
		TEXTURECUBE,
		TEXTUREHDR,
	};

	Texture();
	~Texture();

    virtual void updateImage();
	virtual void bind(int index = 0);
	virtual void unbind(int index = 0);



	//Setter
	void setName(std::string name);
	void setID(int id);
    void markLoaded();

	//Getter
	unsigned int getType();
	unsigned int getID();
	std::string getName();
	bool isLoaded();
	static std::vector<Texture*> getTexturesToLoad();
};