#pragma once
#include <gum-maths.h>
#include <list>
#include <mutex>
#include <vector>

class Texture
{
public:	
	enum Types
	{
		TEXTURE2D,
		TEXTURE3D,
		TEXTURECUBE,
		TEXTUREHDR,
        TEXTUREDEPTH,
	};

    enum FilteringTypes
    {
        LINEAR,
        NEAREST_NEIGHBOR,
        LINEAR_MIPMAP_LINEAR,
        LINEAR_MIPMAP_NEAREST,
        NEAREST_MIPMAP_LINEAR,
        NEAREST_MIPMAP_NEAREST,
    };

	struct Datatypes
	{
		static uint16_t UNSIGNED_CHAR;
		static uint16_t FLOAT;
		static uint16_t INTEGER;
	};

protected:
	inline static const int MAX_PIXEL_COLOR = 256 * 256 * 256;
    inline static std::list<Texture*> vToLoadTextures;
    inline static std::mutex loadMutex;
	unsigned int iTextureID;
	Types iType;
    uint16_t iDatatype;
	std::string sName;
	bool bLoaded;
	bool bNeedsFreeing;
    bool bIsGrayscale;
    bool bIsMipmapped;

public:
	Texture(Types type, uint16_t datatype);
	virtual ~Texture();

    virtual void updateImage() { std::cout << "wrong" << std::endl; }
	virtual void bind(const int& index = 0) {}
	virtual void unbind(const int& index = 0) {}

    static void loadTextures();
    
    void createMipmaps();
    virtual void repeat(bool mirrored = false) {}
    virtual void clampToEdge(bool border = false) {}
    virtual void setFiltering(FilteringTypes filteringtype) {}

	//Setter
	void setName(const std::string& name);
	void setID(const int& id);
    void markLoaded();
    void setGrayscale(const bool& isgrayscale);

	//Getter
	Types getType() const;
	uint16_t getDatatype() const;
	unsigned int getID() const;
	std::string getName() const;
	bool isLoaded() const;
    bool isGrayscale() const;
};