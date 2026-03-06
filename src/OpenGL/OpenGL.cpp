#include <Graphics/Graphics.h>
#include <GL/glew.h>
#include <Essentials/Tools.h>

namespace Gum {
namespace Graphics
{
    void enableFeature(const unsigned int& feature)
    {
        glEnable(feature);
    }

    void disableFeature(const unsigned int& feature)
    {
        glDisable(feature);
    }

    void renderWireframe(const bool wireframe)
    {
        glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
    }

    void cullBackside(const bool cullback)
    {
        glCullFace(cullback ? GL_BACK : GL_FRONT);
    }


    void printInfo()
    {
        Gum::Output::print(
        std::string("GraphicsContext Info:\n") +
            "  RenderEngine: "            + VARS::RENDERER + "\n" +
            "  OpenGL version supported " + VARS::VERSION + "\n" +
            "  GLSL version supported "   + std::to_string(VARS::SHADING_LANGUAGE_MAJOR_VERSION) + "." + std::to_string(VARS::SHADING_LANGUAGE_MINOR_VERSION) + "\n" +
            "  OpenGL Version "           + std::to_string(VARS::MAJOR_VERSION) + "." + std::to_string(VARS::MINOR_VERSION) + "\n" +
            "  OpenGL Graphics Vendor: "  + VARS::VENDOR
        );
    }

    void init()
    {        
        //Initialize OpenGL Variables and glew
        Gum::Output::info("Initializing OpenGL Variables...");

        #if defined GUM_OS_LINUX
            GLenum err = glewInit();
            if(err != GLEW_OK && err != GLEW_ERROR_NO_GLX_DISPLAY)
            {
                Gum::Output::error("GLEW ERROR: " + std::string(reinterpret_cast<const char*>(glewGetErrorString(err))));
                return;
            }
        #endif

        
        //glEnable(GL_STENCIL_TEST);
        //glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        //glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

        VARS::RENDERER = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
        VARS::VERSION = reinterpret_cast<const char*>(glGetString(GL_VERSION));
        VARS::VENDOR = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
        crate<std::string> shaderver = Tools::splitStr(reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION)), '.');
        if(shaderver.size() > 0)
            VARS::SHADING_LANGUAGE_MAJOR_VERSION = Tools::StringToInt(shaderver[0]);
        if(shaderver.size() > 1)
            VARS::SHADING_LANGUAGE_MINOR_VERSION = Tools::StringToInt(shaderver[1]);

        glGetIntegerv(GL_MAJOR_VERSION, &VARS::MAJOR_VERSION);
        glGetIntegerv(GL_MINOR_VERSION, &VARS::MINOR_VERSION);


        glGetIntegerv(GL_MAX_TEXTURE_SIZE, &VARS::MAX_TEXTURE_SIZE);

        Gum::Output::info("Successfully initialized OpenGL Variables!");
    }


    void loadDefaults()
    {
        glFrontFace(GL_CCW);

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_BLEND);
        #if defined GUM_OS_LINUX
            glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
        #endif
        glEnable(GL_CULL_FACE);

        glCullFace(GL_BACK);
        glDepthFunc(GL_LEQUAL);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glLineWidth(1);
    }

    bool isExtensionSupported(const char *extension)
    {
        const char *start;
        const char *where, *terminator;

        /* Extension names should not have spaces. */
        where = strchr(extension, ' ');
        if (where || *extension == '\0')
            return false;

        /* It takes a bit of care to be fool-proof about parsing the
            OpenGL extensions string. Don't be fooled by sub-strings,
            etc. */
        for (start = VARS::sGLExtentions.c_str();;) 
        {
            where = strstr(start, extension);

            if (!where)
                break;

            terminator = where + strlen(extension);

            if((where == start || *(where - 1) == ' ') && (*terminator == ' ' || *terminator == '\0'))
                return true;

            start = terminator;
        }

        return false;
    }
}}