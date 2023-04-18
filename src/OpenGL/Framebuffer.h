#pragma once
#include <gum-maths.h>
#include "Texture2D.h"
#include "TextureCube.h"
#include <vector>
#include <GL/glew.h>

class Framebuffer
{
private:
    unsigned int framebufferID;
    unsigned int iDepthBufferID;
    ivec2 v2Size;
    ivec2 v2Offset;

    std::vector<Texture*> vTextureAttachments;
    std::vector<unsigned int> vDrawBuffers;
    Texture2D* pDepthTexture;
    mat4 m4ScreenMatrix;
    float fAspectRatio, fAspectRatioWidthToHeight;


    void checkStatus();
    void updateMatrix();

public:
    Framebuffer(const ivec2& size, bool iswindow);
    ~Framebuffer();

    inline static Framebuffer* CurrentlyBoundFramebuffer = nullptr;

    void bind();
    void unbind(const ivec2& viewportsize = ivec2(0,0));

    Texture2D* addTextureAttachment(unsigned int index = 0, std::string name = "framebufferTexture", int type = GL_RGBA, int internalType = GL_RGBA, int datatype = GL_UNSIGNED_BYTE);
    TextureCube* addCubeTextureAttachment(unsigned int index = 0, std::string name = "framebufferCubeTexture", int type = GL_RGBA, int internalType = GL_RGBA, int datatype = GL_UNSIGNED_BYTE);
    Texture2D* addDepthTextureAttachment(std::string name = "framebufferDepthTexture");
    Texture2D* addDepthStencilTextureAttachment(std::string name = "framebufferDepthStencilTexture");
    void addDepthAttachment();
    
    void drawAttachmentTexture(const int& textureattachmentindex, const int& colorattachmentindex, const int& target);
    void blitDepthToOtherFramebuffer(Framebuffer* fbo);


    //Setter
    void setDepthAttachment(unsigned int attachment);
    void setDepthTextureAttachment(Texture2D* depthMap);
    void setOffset(ivec2 offset);
    void setSize(ivec2 size);

    //Getter
    Texture* getTextureAttachment(int index = 0);
    Texture2D* getDepthTextureAttachment();
    int getDepthAttachmentID();
    int numTextureAttachments();
    ivec2 getSize();
    ivec2 getOffset();
    unsigned int getID();
    mat4 getScreenMatrix();
    float getAspectRatio();
    float getAspectRatioWidthToHeight();
};