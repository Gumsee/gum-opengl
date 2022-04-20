#pragma once
#include <GL/glew.h>
#include <string>

class Shader
{
private:
    std::string sSource;
    std::string sTypeName;
    int iType;
    int iShaderID;
    bool bIsCompiled = false;

public:
    enum TYPES {
        VERTEX_SHADER                  = GL_VERTEX_SHADER,
        FRAGMENT_SHADER                = GL_FRAGMENT_SHADER,
        TESSELLATION_CONTROL_SHADER    = GL_TESS_CONTROL_SHADER,
        TESSELLATION_EVALUATION_SHADER = GL_TESS_EVALUATION_SHADER,
        GEOMETRY_SHADER                = GL_GEOMETRY_SHADER,
    };

    inline static const std::string SHADER_VERSION_STR = "#version 330 core \n";

    Shader(std::string sourcecode, int shadertype);

    std::string compile();

    //Getter
    std::string getSourceCode();
    int getShaderType();
    int getShaderID();
};