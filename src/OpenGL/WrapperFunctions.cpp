#include <Graphics/WrapperFunctions.h>
#include <Essentials/Tools.h>
#include <System/Output.h>
#include <System/MemoryManagement.h>
#include <GL/glew.h>

bool gumTexImage2D(const unsigned int& target, const int& level, const int& internalformat, ivec2 size, const int& border, const unsigned int& format, const unsigned int& type, const void* pixels)
{
    glUnmapBuffer(GL_TEXTURE_BUFFER);
    #ifdef CHECK_GL_ERRORS
        std::string callInfoStr = " target: " + Tools::decToHex(target) + ", level: " + std::to_string(level) + ", internalformat: " + Tools::decToHex(internalformat) + 
                                    ", size: " + size.toString() + ", border: " + std::to_string(border) + 
                                    ", format: " + Tools::decToHex(format) + ", type: " + Tools::decToHex(type);
        if(size.x < 0)                                   { Gum::Output::error("glTexImage2D: Texture width less than 0" + callInfoStr); }
        if(size.y < 0)                                   { Gum::Output::error("glTexImage2D: Texture height less than 0" + callInfoStr); }
        //if(size.x > Gum::GLContext::GLVARS::MAX_TEXTURE_SIZE) { Gum::Output::error("glTexImage2D: Texture width more than GL_MAX_TEXTURE_SIZE" + callInfoStr); }
        //if(size.y > Gum::GLContext::GLVARS::MAX_TEXTURE_SIZE) { Gum::Output::error("glTexImage2D: Texture height more than GL_MAX_TEXTURE_SIZE" + callInfoStr); }
        if(level < 0)                                    { Gum::Output::error("glTexImage2D: Texture level is less than 0" + callInfoStr); }
        if(Tools::isInList(type, { GL_UNSIGNED_BYTE_3_3_2, GL_UNSIGNED_BYTE_2_3_3_REV, GL_UNSIGNED_SHORT_5_6_5, GL_UNSIGNED_SHORT_5_6_5_REV, 
                                    GL_UNSIGNED_INT_10F_11F_11F_REV}) && format != GL_RGB)
                                                        { Gum::Output::error("glTexImage2D: Texture format should be GL_RGB" + callInfoStr); }
        if(Tools::isInList(type, { GL_UNSIGNED_SHORT_4_4_4_4, GL_UNSIGNED_SHORT_4_4_4_4_REV, GL_UNSIGNED_SHORT_5_5_5_1, GL_UNSIGNED_SHORT_1_5_5_5_REV, 
                                    GL_UNSIGNED_INT_8_8_8_8, GL_UNSIGNED_INT_8_8_8_8_REV, GL_UNSIGNED_INT_10_10_10_2, GL_UNSIGNED_INT_2_10_10_10_REV, 
                                    GL_UNSIGNED_INT_5_9_9_9_REV}) && format != GL_RGBA && format != GL_BGRA)
                                                        { Gum::Output::error("glTexImage2D: Texture format should be GL_RGBA or GL_BGRA" + callInfoStr); }
        if(format == GL_DEPTH_COMPONENT && !Tools::isInList(internalformat, { GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT16, GL_DEPTH_COMPONENT24, 
                                                                                GL_DEPTH_COMPONENT32F}))
                                                        { Gum::Output::error("glTexImage2D: Texture format is GL_DEPTH_COMPONENT and internalformat is invalid." + callInfoStr); }

                                                    
        while(glGetError() != GL_NO_ERROR) { } //Empty errors
    #endif


    glTexImage2D(target, level, internalformat, size.x, size.y, border, format, type, pixels);
    #ifdef CHECK_GL_ERRORS
        GLenum err;
        while((err = glGetError()) != GL_NO_ERROR)
        {
            switch(err)
            {
                case GL_INVALID_ENUM: Gum::Output::error("glTexImage2D: GL_INVALID_ENUM Texture has invalid type or target" + callInfoStr); return false; break;
                case GL_INVALID_VALUE: Gum::Output::error("glTexImage2D: GL_INVALID_VALUE" + callInfoStr); return false;  break;
                case GL_INVALID_OPERATION: Gum::Output::error("glTexImage2D: GL_INVALID_OPERATION" + callInfoStr); return false; break;
            }
        }
    #endif
    return true;
}

bool gumPixelStorei(const unsigned int& pname, const int& param)
{
    #ifdef CHECK_GL_ERRORS
        while(glGetError() != GL_NO_ERROR) { } //Empty errors
    #endif
    glPixelStorei(pname, param);
    #ifdef CHECK_GL_ERRORS
        GLenum err = glGetError();
        if(err != GL_NO_ERROR)
        {
            Gum::Output::error("glPixelStorei: " + Tools::decToHex(err));
            return false;
        }
    #endif
    return true;
}

bool gumGenTextures(const int& n, unsigned int* textures)
{
    #ifdef CHECK_GL_ERRORS
        while(glGetError() != GL_NO_ERROR) { } //Empty errors
    #endif
    glGenTextures(n, textures);
    #ifdef CHECK_GL_ERRORS
        GLenum err = glGetError();
        if(err != GL_NO_ERROR)
        {
            Gum::Output::error("glGenTextures: " + Tools::decToHex(err));
            return false;
        }
    #endif
    return true;
}

bool gumScissor(bbox2i bbox)
{
    glScissor(bbox.pos.x, bbox.pos.y, bbox.size.x, bbox.size.y);
    return true;
}