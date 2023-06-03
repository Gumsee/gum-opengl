#pragma once
#include <gum-maths.h>
#include "TextureDepth.h"
#include "TextureCube.h"
#include <vector>
#include <GL/glew.h>

class Framebuffer
{
private:
    unsigned int iID;
    unsigned int iDepthBufferID;
    ivec2 v2Size;
    ivec2 v2Offset;

    std::vector<Texture*> vTextureAttachments;
    std::vector<unsigned int> vDrawBuffers;
    TextureDepth* pDepthTexture;
    mat4 m4ScreenMatrix;
    float fAspectRatio, fAspectRatioWidthToHeight;


    void checkStatus();
    void updateMatrix();
    void resizeTextures();

public:
    Framebuffer(const ivec2& size, bool iswindow = false);
    ~Framebuffer();

    inline static Framebuffer* CurrentlyBoundFramebuffer = nullptr;
    inline static Framebuffer* WindowFramebuffer = nullptr;

    void bind();
    static void unbind(const ivec2& viewportsize = ivec2(0,0));
    void resetViewport();

    Texture2D* addTextureAttachment(unsigned int index = 0, std::string name = "framebufferTexture", int type = GL_RGBA, int internalType = GL_RGBA, int datatype = GL_UNSIGNED_BYTE);
    TextureCube* addCubeTextureAttachment(unsigned int index = 0, std::string name = "framebufferCubeTexture", int type = GL_RGBA, int internalType = GL_RGBA, int datatype = GL_UNSIGNED_BYTE);
    TextureDepth* addDepthTextureAttachment(std::string name = "framebufferDepthTexture");
    TextureDepth* addDepthStencilTextureAttachment(std::string name = "framebufferDepthStencilTexture");
    void addDepthAttachment();
    
    void drawAttachmentTexture(const int& textureattachmentindex, const int& colorattachmentindex, const int& target);
    void blitDepthToOtherFramebuffer(Framebuffer* fbo);


    //Setter
    void setDepthAttachment(unsigned int attachment);
    void setDepthTextureAttachment(TextureDepth* depthMap);
    void setOffset(ivec2 offset);
    void setSize(ivec2 size);

    //Getter
    Texture* getTextureAttachment(int index = 0);
    TextureDepth* getDepthTextureAttachment();
    int getDepthAttachmentID();
    int numTextureAttachments();
    ivec2 getSize();
    ivec2 getOffset();
    unsigned int getID();
    mat4 getScreenMatrix();
    float getAspectRatio();
    float getAspectRatioWidthToHeight();
};