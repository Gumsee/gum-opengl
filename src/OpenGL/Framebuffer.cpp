#include <GL/glew.h>
#include <System/Output.h>
#include <System/MemoryManagement.h>

#include <Essentials/Tools.h>
#include "Framebuffer.h"
#include "WrapperFunctions.h"
#include <algorithm>

Framebuffer* Framebuffer::pCurrentlyBoundFramebuffer = nullptr;


Framebuffer::Framebuffer(const ivec2& size, const vec4& clearcolor)
{
    this->v2Size = size;
    this->v2Offset = ivec2(0,0);
    this->framebufferID = 0;
    this->iDepthBufferID = 0;
    this->pDepthTexture = nullptr;

    glGenFramebuffers(1, &this->framebufferID);
    
    bind();
    glClearColor(clearcolor.x, clearcolor.y, clearcolor.z, clearcolor.w);
    glDrawBuffer(GL_NONE);
    //glDrawBuffer(GL_COLOR_ATTACHMENT0);
    unbind();
}

Framebuffer::~Framebuffer() 
{
	glDeleteFramebuffers(1, &this->framebufferID);
    for(size_t i = 0; i < vTextureAttachments.size(); i++)
        Gum::_delete(vTextureAttachments[i]);

    Gum::_delete(pDepthTexture);
}

void Framebuffer::bind()
{
    //glBindTexture(GL_TEXTURE_2D, 0);//To make sure the texture isn't bound
    pCurrentlyBoundFramebuffer = this;
    glBindFramebuffer(GL_FRAMEBUFFER, this->framebufferID);
    glViewport(this->v2Offset.x, this->v2Offset.y, this->v2Size.x, this->v2Size.y);
}

void Framebuffer::unbind(const ivec2& viewportsize)
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    if(viewportsize != ivec2(0,0))
        glViewport(0, 0, viewportsize.x, viewportsize.y);
}


Texture2D* Framebuffer::addTextureAttachment(unsigned int index, std::string name, int type, int internalType, int datatype)
{
    if(std::find(vDrawBuffers.begin(), vDrawBuffers.end(), index) != vDrawBuffers.end())
    {
        Gum::Output::warn("Framebuffer: Texture has already been attached to index " + std::to_string(index));
    }

    bind();
    Texture2D* texture = new Texture2D(name);
    texture->bind();
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    if(!gumTexImage2D(GL_TEXTURE_2D, 0, internalType, this->v2Size, 0, type, datatype, nullptr)) 
        Gum::Output::error("Framebuffer::addTextureAttachment: glTexImage Failed.");
    
    
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, texture->getID(), 0);
    vDrawBuffers.push_back(GL_COLOR_ATTACHMENT0 + index);
	glDrawBuffers(vDrawBuffers.size(), &vDrawBuffers[0]);
    
    unbind();
    texture->unbind();
    this->vTextureAttachments.push_back(texture);
        
    #ifdef CHECK_GL_ERRORS
    checkStatus();
    #endif
    return texture;
}

TextureCube* Framebuffer::addCubeTextureAttachment(unsigned int index , std::string name, int type, int internalType, int datatype)
{
    if(std::find(vDrawBuffers.begin(), vDrawBuffers.end(), index) != vDrawBuffers.end())
    {
        Gum::Output::warn("Framebuffer: Texture has already been attached to index " + std::to_string(index));
    }

    bind();
    TextureCube* texture = new TextureCube(name);
    this->vTextureAttachments.push_back(texture);
    //glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, texture->getID(), 0);

    texture->bind();
    for (unsigned int i = 0; i < 6; ++i)
	{
		if(!gumTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalType, this->v2Size, 0, type, datatype, (void*)nullptr))
            Gum::Output::error("Framebuffer::addCubeTextureAttachment: glTexImage Failed.");
        //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, texture->getID(), 0);
        drawAttachmentTexture(0, index, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    texture->unbind();

    //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_TEXTURE_CUBE_MAP_POSITIVE_X, texture->getID(), 0);
    vDrawBuffers.push_back(GL_COLOR_ATTACHMENT0 + index);
	glDrawBuffers(vDrawBuffers.size(), &vDrawBuffers[0]);

    unbind();
        
    #ifdef CHECK_GL_ERRORS
    checkStatus();
    #endif

    return texture;
}

Texture2D* Framebuffer::addDepthTextureAttachment(std::string name)
{
    if(this->pDepthTexture == nullptr)
    {
        bind();
        this->pDepthTexture = new Texture2D(name);
        this->pDepthTexture->bind();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        if(!gumTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, this->v2Size, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0))
            Gum::Output::error("Framebuffer::addDepthTextureAttachment: glTexImage Failed.");
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, this->pDepthTexture->getID(), 0);
        this->pDepthTexture->unbind();
        unbind();
        
        #ifdef CHECK_GL_ERRORS
        checkStatus();
        #endif
    }
    return this->pDepthTexture;
}


Texture2D* Framebuffer::addDepthStencilTextureAttachment(std::string name)
{
    if(this->pDepthTexture == nullptr)
    {
        bind();
        Gum::Output::print("Adding stencildepthbuffer to " + this->getTextureAttachment()->getName());
        this->pDepthTexture = new Texture2D(name);
        //glDrawBuffer(GL_BACK);
        this->pDepthTexture->bind();
        if(!gumTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, this->v2Size, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL))
            Gum::Output::error("Framebuffer::addDepthStencilTextureAttachment: glTexImage Failed.");
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, this->pDepthTexture->getID(), 0);
        this->pDepthTexture->unbind();
        unbind();
        
        #ifdef CHECK_GL_ERRORS
        checkStatus();
        #endif
    }
    return this->pDepthTexture;
}

void Framebuffer::addDepthAttachment()
{
    if(this->iDepthBufferID == 0)
    {
        bind();
        glGenRenderbuffers(1, &this->iDepthBufferID);
        glBindRenderbuffer(GL_RENDERBUFFER, this->iDepthBufferID);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, this->v2Size.x, this->v2Size.y); //GL_DEPTH_COMPONENT24
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, this->iDepthBufferID);
        unbind();

        #ifdef CHECK_GL_ERRORS
        checkStatus();
        #endif
    }
}

void Framebuffer::drawAttachmentTexture(const int& textureattachmentindex, const int& colorattachmentindex, const int& target)
{
    bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + colorattachmentindex, target, getTextureAttachment(textureattachmentindex)->getID(), 0);
    #ifdef CHECK_GL_ERRORS
    checkStatus();
    #endif
    
    //dont unbind automatically
}

void Framebuffer::blitDepthToOtherFramebuffer(Framebuffer* fbo)
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, getID());
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo->getID()); // write to default framebuffer 
    glBlitFramebuffer(0, 0, getSize().x, getSize().y, 0, 0, fbo->getSize().x, fbo->getSize().y, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
}


void Framebuffer::checkStatus()
{
    GLenum status;
    switch((status = glCheckFramebufferStatus(GL_FRAMEBUFFER)))
    {
        case GL_FRAMEBUFFER_COMPLETE: break; //Everything is fine
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:          Gum::Output::error("Framebuffer: Incomplete Attachment"); break;
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:  Gum::Output::error("Framebuffer: Missing Attachment"); break;
        case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:         Gum::Output::error("Framebuffer: Incomplete Drawbuffer"); break;
        case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:         Gum::Output::error("Framebuffer: Incomplete Readbuffer"); break;
        case GL_FRAMEBUFFER_UNSUPPORTED:                    Gum::Output::error("Framebuffer: Framebuffer Unsupported"); break;
        default:                                            Gum::Output::error("Framebuffer: Status error: " + Tools::decToHex(status));
    }
}


//
// Setter
//
void Framebuffer::setDepthAttachment(unsigned int attachment)
{
    this->iDepthBufferID = attachment;
    bind();
    glBindRenderbuffer(GL_RENDERBUFFER, this->iDepthBufferID);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, this->v2Size.x, this->v2Size.y);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, this->iDepthBufferID);
    unbind();
}

void Framebuffer::setDepthTextureAttachment(Texture2D* depthMap)
{
    this->pDepthTexture = depthMap;
    bind();
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, this->pDepthTexture->getID(), 0);
    unbind();
}

void Framebuffer::setOffset(ivec2 offset)               { this->v2Offset = offset; }
void Framebuffer::setSize(ivec2 size)                   { this->v2Size = size; }


//
// Getter
//
Texture* Framebuffer::getTextureAttachment(int index)   { return this->vTextureAttachments[index]; }
Texture2D* Framebuffer::getDepthTextureAttachment()     { return this->pDepthTexture; }
int Framebuffer::getDepthAttachmentID()                 { return this->iDepthBufferID; }
int Framebuffer::numTextureAttachments()                { return this->vTextureAttachments.size(); }
ivec2 Framebuffer::getSize()                            { return this->v2Size; }
ivec2 Framebuffer::getOffset()                          { return this->v2Offset; }
unsigned int Framebuffer::getID()                       { return this->framebufferID; }