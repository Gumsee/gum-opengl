#include <Graphics/Framebuffer.h>
#include <Graphics/WrapperFunctions.h>
#include <GL/glew.h>
#include <System/Output.h>
#include <Essentials/Tools.h>
#include <Graphics/TextureDepth.h>
#include <Graphics/TextureHDR.h>


void Framebuffer::createNative()
{
    glGenFramebuffers(1, &this->iID);
    //bind();
    //glClearColor(clearcolor.x, clearcolor.y, clearcolor.z, clearcolor.w);
    //glDrawBuffer(GL_NONE);
    //glDrawBuffer(GL_COLOR_ATTACHMENT0);
}

void Framebuffer::destroyNative()
{
    if(this->iID != 0)
	    glDeleteFramebuffers(1, &this->iID);
}

void Framebuffer::bind()
{
    //glBindTexture(GL_TEXTURE_2D, 0);//To make sure the texture isn't bound
    CurrentlyBoundFramebuffer = this;
    glBindFramebuffer(GL_FRAMEBUFFER, this->iID);
    resetViewport();
}

void Framebuffer::unbind(const ivec2& viewportsize)
{
    CurrentlyBoundFramebuffer = WindowFramebuffer; // Dont check for nullptr
    if(WindowFramebuffer != nullptr)
    {
        WindowFramebuffer->bind();
    }
    else
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        if(viewportsize != ivec2(0,0))
            glViewport(0, 0, viewportsize.x, viewportsize.y);
    }
}


Texture2D* Framebuffer::addTextureAttachment(unsigned int index, std::string name, int datatype)
{
    if(std::find(vDrawBuffers.begin(), vDrawBuffers.end(), index) != vDrawBuffers.end())
    {
        Gum::Output::error("Framebuffer: Texture has already been attached to index " + std::to_string(index));
        return nullptr;
    }

    Texture2D* texture = new Texture2D(name, datatype);
    texture->setSize(v2Size);
    texture->clampToEdge();
    texture->setFiltering(Texture::NEAREST_NEIGHBOR);
    this->mTextureAttachments[index] = texture;

    Framebuffer* currFramebuffer = CurrentlyBoundFramebuffer;
    bind();
    texture->bind();
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, texture->getID(), 0);
    vDrawBuffers.push_back(GL_COLOR_ATTACHMENT0 + index);
	glDrawBuffers(vDrawBuffers.size(), &vDrawBuffers[0]);
    texture->unbind();
    
    #ifdef CHECK_GL_ERRORS
    checkStatus();
    #endif
    currFramebuffer->bind();
    return texture;
}

TextureCube* Framebuffer::addCubeTextureAttachment(unsigned int index, std::string name, uint16_t format, uint16_t internalFormat, int datatype)
{
    if(std::find(vDrawBuffers.begin(), vDrawBuffers.end(), index) != vDrawBuffers.end())
    {
        Gum::Output::error("Framebuffer: Texture has already been attached to index " + std::to_string(index));
        return nullptr;
    }

    TextureCube* texture = new TextureCube(name);
    texture->clampToEdge();
    texture->setFiltering(Texture::LINEAR);
    mTextureAttachments[index] = texture;
    //glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, texture->getID(), 0);

    Framebuffer* currFramebuffer = CurrentlyBoundFramebuffer;
    bind();
    texture->bind();
    for (unsigned int i = 0; i < 6; ++i)
	{
		if(!gumTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, this->v2Size, 0, format, datatype, (void*)nullptr))
            Gum::Output::error("Framebuffer::addCubeTextureAttachment: glTexImage Failed.");
        //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, texture->getID(), 0);
        drawAttachmentTexture(0, index, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i);
	}
    texture->unbind();

    //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_TEXTURE_CUBE_MAP_POSITIVE_X, texture->getID(), 0);
    vDrawBuffers.push_back(GL_COLOR_ATTACHMENT0 + index);
	glDrawBuffers(vDrawBuffers.size(), &vDrawBuffers[0]);
        
    #ifdef CHECK_GL_ERRORS
    checkStatus();
    #endif
    currFramebuffer->bind();

    return texture;
}

TextureDepth* Framebuffer::addDepthTextureAttachment(std::string name)
{
    if(this->pDepthTexture != nullptr)
    {
        Gum::Output::error("Framebuffer: Depthtexture has already been attached");
        return pDepthTexture;
    }
    else if(this->iDepthBufferID != 0)
    {
        Gum::Output::error("Framebuffer: Depthattachment has already been attached");
        return nullptr;
    }

    Framebuffer* currFramebuffer = CurrentlyBoundFramebuffer;
    bind();
    pDepthTexture = new TextureDepth(name, Texture::Datatypes::FLOAT);
    pDepthTexture->bind();
    pDepthTexture->setSize(v2Size);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, this->pDepthTexture->getID(), 0);
    pDepthTexture->unbind();
    
    #ifdef CHECK_GL_ERRORS
    checkStatus();
    #endif
    currFramebuffer->bind();
    return this->pDepthTexture;
}


TextureDepth* Framebuffer::addDepthStencilTextureAttachment(std::string name)
{
    if(this->pDepthTexture != nullptr)
    {
        Gum::Output::error("Framebuffer: Depthtexture has already been attached");
        return pDepthTexture;
    }
    else if(this->iDepthBufferID != 0)
    {
        Gum::Output::error("Framebuffer: Depthattachment has already been attached");
        return nullptr;
    }
    
    Framebuffer* currFramebuffer = CurrentlyBoundFramebuffer;
    bind();
    //Gum::Output::print("Adding stencildepthbuffer to " + this->getTextureAttachment()->getName());
    this->pDepthTexture = new TextureDepth(name);
    //glDrawBuffer(GL_BACK);
    this->pDepthTexture->bind();
    if(!gumTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, this->v2Size, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL))
        Gum::Output::error("Framebuffer::addDepthStencilTextureAttachment: glTexImage Failed.");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, this->pDepthTexture->getID(), 0);
    this->pDepthTexture->unbind();
    
    #ifdef CHECK_GL_ERRORS
    checkStatus();
    #endif
    currFramebuffer->bind();
    return this->pDepthTexture;
}

void Framebuffer::addDepthAttachment()
{
    if(this->pDepthTexture != nullptr)
        Gum::Output::error("Framebuffer: Depthtexture has already been attached");
    else if(this->iDepthBufferID != 0)
        Gum::Output::error("Framebuffer: Depthattachment has already been attached");

    Framebuffer* currFramebuffer = CurrentlyBoundFramebuffer;
    bind();
    glGenRenderbuffers(1, &this->iDepthBufferID);
    glBindRenderbuffer(GL_RENDERBUFFER, this->iDepthBufferID);
    //glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, this->v2Size.x, this->v2Size.y);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, this->v2Size.x, this->v2Size.y);
    //glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, this->iDepthBufferID);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, this->iDepthBufferID);

    #ifdef CHECK_GL_ERRORS
    checkStatus();
    #endif
    currFramebuffer->bind();
}

void Framebuffer::drawAttachmentTexture(const int& textureattachmentindex, const int& colorattachmentindex, const int& target)
{
    Framebuffer* currFramebuffer = CurrentlyBoundFramebuffer;
    bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + colorattachmentindex, target, getTextureAttachment(textureattachmentindex)->getID(), 0);
    #ifdef CHECK_GL_ERRORS
    checkStatus();
    #endif
    currFramebuffer->bind();
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

void Framebuffer::resetViewport()
{
    glViewport(this->v2Offset.x, this->v2Offset.y, this->v2Size.x, this->v2Size.y);
    glScissor(0.0f, 0.0f, getSize().x, getSize().y);
}

void Framebuffer::resizeTextures()
{
    Framebuffer* currFramebuffer = CurrentlyBoundFramebuffer;
    bind();
    for(auto entry : mTextureAttachments)
    {
        Texture* texture = entry.second;
        texture->bind();
        switch(texture->getType())
        {
            case Texture::TEXTURE2D:   ((Texture2D*)texture)->setSize(v2Size); break;
            case Texture::TEXTURECUBE: ((TextureCube*)texture)->setSize(v2Size); break;
            case Texture::TEXTUREHDR:  ((TextureHDR*)texture)->setSize(v2Size); break;
            default: break;
        }
        texture->unbind();
    }

    if(this->iDepthBufferID != 0)
    {
        glBindRenderbuffer(GL_RENDERBUFFER, this->iDepthBufferID);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, this->v2Size.x, this->v2Size.y);
    }
    else if(this->pDepthTexture != nullptr)
    {
        pDepthTexture->setSize(v2Size);
    }
    currFramebuffer->bind();
}


//
// Setter
//
void Framebuffer::setDepthAttachment(unsigned int attachment)
{
    this->iDepthBufferID = attachment;
    Framebuffer* currFramebuffer = CurrentlyBoundFramebuffer;
    bind();
    glBindRenderbuffer(GL_RENDERBUFFER, this->iDepthBufferID);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, this->v2Size.x, this->v2Size.y);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, this->iDepthBufferID);
    currFramebuffer->bind();
}

void Framebuffer::setDepthTextureAttachment(TextureDepth* depthMap)
{
    this->pDepthTexture = depthMap;
    Framebuffer* currFramebuffer = CurrentlyBoundFramebuffer;
    bind();
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, this->pDepthTexture->getID(), 0);
    currFramebuffer->bind();
}

vec4 Framebuffer::getPixel(ivec2 pos)
{
    vec4 pixelcolor;
    Framebuffer* currFramebuffer = CurrentlyBoundFramebuffer;
    bind();
	unsigned char data[4];
    glReadPixels(pos.x, v2Size.y - pos.y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);
    
    pixelcolor.r = (float)data[0];
    pixelcolor.g = (float)data[1];
    pixelcolor.b = (float)data[2];
    pixelcolor.a = (float)data[3];

    currFramebuffer->bind();

    return pixelcolor / 255.0f;
}