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
    bool bIsGrayscale;

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
	virtual void bind(const int& index = 0);
	virtual void unbind(const int& index = 0);



	//Setter
	void setName(const std::string& name);
	void setID(const int& id);
    void markLoaded();
    void setGrayscale(const bool& isgrayscale);

	//Getter
	unsigned int getType() const;
	unsigned int getID() const;
	std::string getName() const;
	bool isLoaded() const;
	static std::vector<Texture*> getTexturesToLoad();
    bool isGrayscale() const;
};