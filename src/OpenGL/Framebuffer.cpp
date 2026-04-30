#include <Graphics/Framebuffer.h>
#include <Graphics/WrapperFunctions.h>
#include <System/Output.h>
#include <Essentials/Tools.h>
#include <Graphics/TextureHDR.h>
#include <string>
#include <glad/gl.h>

const unsigned short Framebuffer::ClearFlags::COLOR   = GL_COLOR_BUFFER_BIT;
const unsigned short Framebuffer::ClearFlags::DEPTH   = GL_DEPTH_BUFFER_BIT;
const unsigned short Framebuffer::ClearFlags::STENCIL = GL_STENCIL_BUFFER_BIT;


const unsigned short Framebuffer::TextureTargets::TEXTURE2D          = GL_TEXTURE_2D;
const unsigned short Framebuffer::TextureTargets::CUBEMAP_POSITIVE_X = GL_TEXTURE_CUBE_MAP_POSITIVE_X;
const unsigned short Framebuffer::TextureTargets::CUBEMAP_NEGATIVE_X = GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
const unsigned short Framebuffer::TextureTargets::CUBEMAP_POSITIVE_Y = GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
const unsigned short Framebuffer::TextureTargets::CUBEMAP_NEGATIVE_Y = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
const unsigned short Framebuffer::TextureTargets::CUBEMAP_POSITIVE_Z = GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
const unsigned short Framebuffer::TextureTargets::CUBEMAP_NEGATIVE_Z = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;

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
    if(this->iID != 0u)
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
        glBindFramebuffer(GL_FRAMEBUFFER, DefaultFramebufferID);
        if(viewportsize != ivec2(0,0))
            glViewport(0, 0, viewportsize.x, viewportsize.y);
    }
}

void Framebuffer::clear(const unsigned short& flags)
{
    vec4 glcolor = cClearColor.getGLColor();
    glClearColor(glcolor.r, glcolor.g, glcolor.b, glcolor.a);
    glClear(flags);
}

template tTexture2D<unsigned char>* Framebuffer::addTextureAttachment(uint8_t, std::string, uint16_t);
template tTexture2D<float>* Framebuffer::addTextureAttachment(uint8_t, std::string, uint16_t);
template<typename T> tTexture2D<T>* Framebuffer::addTextureAttachment(uint8_t index, std::string name, uint16_t numChannels)
{
    if(std::find(vDrawBuffers.begin(), vDrawBuffers.end(), index) != vDrawBuffers.end())
    {
        Gum::Output::error("Framebuffer: Texture has already been attached to index " + std::to_string(index));
        return nullptr;
    }

    tTexture2D<T>* texture = new tTexture2D<T>(name);
    texture->setNumChannels(numChannels, 0);
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
    checkStatus("addTextureAttachment");
    #endif
    currFramebuffer->bind();
    return texture;
}

template tTextureCube<unsigned char>* Framebuffer::addCubeTextureAttachment(uint8_t, std::string, uint16_t, uint16_t);
template tTextureCube<float>* Framebuffer::addCubeTextureAttachment(uint8_t, std::string, uint16_t, uint16_t);
template<typename T> tTextureCube<T>* Framebuffer::addCubeTextureAttachment(uint8_t index, std::string name, uint16_t format, uint16_t internalFormat)
{
    if(std::find(vDrawBuffers.begin(), vDrawBuffers.end(), index) != vDrawBuffers.end())
    {
        Gum::Output::error("Framebuffer: Texture has already been attached to index " + std::to_string(index));
        return nullptr;
    }

    tTextureCube<T>* texture = new tTextureCube<T>(name);
    texture->clampToEdge();
    texture->setFiltering(Texture::LINEAR);
    mTextureAttachments[index] = texture;
    //glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, texture->getID(), 0);

    Framebuffer* currFramebuffer = CurrentlyBoundFramebuffer;
    bind();
    texture->bind();
    for (unsigned short i = 0; i < 6; ++i)
    {
      if(!gumTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, this->v2Size, 0, format, texture->getDatatype(), 0))
              Gum::Output::error("Framebuffer::addCubeTextureAttachment: glTexImage Failed.");
          //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, texture->getID(), 0);
          attachTexture(index, texture, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i);
    }
    texture->unbind();

    //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_TEXTURE_CUBE_MAP_POSITIVE_X, texture->getID(), 0);
    vDrawBuffers.push_back(GL_COLOR_ATTACHMENT0 + index);
	glDrawBuffers(vDrawBuffers.size(), &vDrawBuffers[0]);
        
    #ifdef CHECK_GL_ERRORS
    checkStatus("addCubeTextureAttachment");
    #endif
    currFramebuffer->bind();

    return texture;
}

template tTextureCube<unsigned char>* Framebuffer::addCubeTextureAttachment(uint8_t, tTextureCube<unsigned char>*);
template tTextureCube<float>* Framebuffer::addCubeTextureAttachment(uint8_t, tTextureCube<float>*);
template<typename T> tTextureCube<T>* Framebuffer::addCubeTextureAttachment(uint8_t index, tTextureCube<T>* texture)
{
   if(std::find(vDrawBuffers.begin(), vDrawBuffers.end(), index) != vDrawBuffers.end())
    {
        Gum::Output::error("Framebuffer: Texture has already been attached to index " + std::to_string(index));
        return nullptr;
    }
    
    mTextureAttachments[index] = texture;

    Framebuffer* currFramebuffer = CurrentlyBoundFramebuffer;
    bind();
    texture->bind();
    for (unsigned short i = 0; i < 6; ++i)
        attachTexture(index, texture, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i);
    texture->unbind();

    //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_TEXTURE_CUBE_MAP_POSITIVE_X, texture->getID(), 0);
    vDrawBuffers.push_back(GL_COLOR_ATTACHMENT0 + index);
	glDrawBuffers(vDrawBuffers.size(), &vDrawBuffers[0]);
        
    #ifdef CHECK_GL_ERRORS
    checkStatus("addCubeTextureAttachment");
    #endif
    currFramebuffer->bind();

    return texture;
}

Texture* Framebuffer::addDepthTextureAttachment(std::string name)
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
    pDepthTexture = new TextureDepth2D(name, Gum::Graphics::Datatypes::FLOAT);
    pDepthTexture->bind();
    ((TextureDepth2D*)pDepthTexture)->setSize(v2Size);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, this->pDepthTexture->getID(), 0);
    pDepthTexture->unbind();
    
    #ifdef CHECK_GL_ERRORS
    checkStatus("addDepthTextureAttachment");
    #endif
    currFramebuffer->bind();
    return this->pDepthTexture;
}

Texture* Framebuffer::addDepthTextureArrayAttachment(const unsigned int& numlayers, std::string name)
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
    pDepthTexture = new TextureDepth3D(numlayers, name, Gum::Graphics::Datatypes::FLOAT);
    pDepthTexture->bind();
    ((TextureDepth3D*)pDepthTexture)->setSize(v2Size);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, this->pDepthTexture->getID(), 0);
    pDepthTexture->unbind();
    
    #ifdef CHECK_GL_ERRORS
    checkStatus("addDepthTextureArrayAttachment");
    #endif
    currFramebuffer->bind();
    return this->pDepthTexture;
}


Texture* Framebuffer::addDepthStencilTextureAttachment(std::string name)
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
    this->pDepthTexture = new TextureDepth2D(name);
    //glDrawBuffer(GL_BACK);
    this->pDepthTexture->bind();
    if(!gumTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, this->v2Size, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL))
        Gum::Output::error("Framebuffer::addDepthStencilTextureAttachment: glTexImage Failed.");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, this->pDepthTexture->getID(), 0);
    this->pDepthTexture->unbind();
    
    #ifdef CHECK_GL_ERRORS
    checkStatus("addDepthStencilTextureAttachment");
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
    checkStatus("addDepthAttachment");
    #endif
    currFramebuffer->bind();
}

void Framebuffer::attachTexture(const int& index, Texture* texture, const unsigned short& target, const unsigned int& mipmaplevel)
{
    Framebuffer* currFramebuffer = CurrentlyBoundFramebuffer;
    bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, target, texture->getID(), mipmaplevel);
    #ifdef CHECK_GL_ERRORS
    checkStatus("attachTexture");
    #endif
    currFramebuffer->bind();
}

void Framebuffer::blitDepthToOtherFramebuffer(Framebuffer* fbo)
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, getID());
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo->getID()); // write to default framebuffer 
    glBlitFramebuffer(0, 0, getSize().x, getSize().y, 0, 0, fbo->getSize().x, fbo->getSize().y, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
}


void Framebuffer::checkStatus(std::string funcname)
{
    GLenum status;
    switch((status = glCheckFramebufferStatus(GL_FRAMEBUFFER)))
    {
        case GL_FRAMEBUFFER_COMPLETE: break; //Everything is fine
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:          Gum::Output::error("Framebuffer ("+funcname+"): Incomplete Attachment"); break;
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:  Gum::Output::error("Framebuffer ("+funcname+"): Missing Attachment"); break;
        case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:         Gum::Output::error("Framebuffer ("+funcname+"): Incomplete Drawbuffer"); break;
        case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:         Gum::Output::error("Framebuffer ("+funcname+"): Incomplete Readbuffer"); break;
        case GL_FRAMEBUFFER_UNSUPPORTED:                    Gum::Output::error("Framebuffer ("+funcname+"): Framebuffer Unsupported"); break;
        default:                                            Gum::Output::error("Framebuffer ("+funcname+"): Status error: " + Tools::decToHex(status));
    }
}

void Framebuffer::resetViewport()
{
    glViewport(this->v2Offset.x, this->v2Offset.y, this->v2Size.x, this->v2Size.y);
    setRenderArea(bbox2i(ivec2(0.0f, 0.0f), getSize()));
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
        if(pDepthTexture->getType() == Texture::TEXTUREDEPTH2D)
            ((TextureDepth2D*)pDepthTexture)->setSize(v2Size);
        else
            ((TextureDepth3D*)pDepthTexture)->setSize(v2Size);
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

void Framebuffer::setDepthTextureAttachment(Texture* depthMap)
{
    this->pDepthTexture = depthMap;
    Framebuffer* currFramebuffer = CurrentlyBoundFramebuffer;
    bind();
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, this->pDepthTexture->getID(), 0);
    currFramebuffer->bind();
}

color Framebuffer::getPixel(ivec2 pos)
{
    color pixelcolor;
    Framebuffer* currFramebuffer = CurrentlyBoundFramebuffer;
    bind();
	unsigned char data[4];
    glReadPixels(pos.x, v2Size.y - pos.y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);
    
    pixelcolor.r = (float)data[0];
    pixelcolor.g = (float)data[1];
    pixelcolor.b = (float)data[2];
    pixelcolor.a = (float)data[3];

    currFramebuffer->bind();

    return pixelcolor;
}

bool Framebuffer::readPixelData(void* image, ivec2 position, ivec2 size, unsigned int pixelformat, unsigned int datatype)
{
    Framebuffer* currFramebuffer = CurrentlyBoundFramebuffer;
    bind();
    glReadPixels(position.x, position.y, size.x, size.y, pixelformat, datatype, image);

    
    if(currFramebuffer != nullptr)
      currFramebuffer->bind();
  
    return true;
}