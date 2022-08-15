#include "Shader.h"

Shader::Shader(std::string sourcecode, int shadertype)
{
    this->sSource = sourcecode;
    this->iType = shadertype;
    switch(this->iType)
    {
        case TYPES::VERTEX_SHADER:                  this->sTypeName = "Vertex Shader"; break;
        case TYPES::FRAGMENT_SHADER:                this->sTypeName = "Fragment Shader"; break;
        case TYPES::TESSELLATION_CONTROL_SHADER:    this->sTypeName = "Tessellation Control Shader"; break;
        case TYPES::TESSELLATION_EVALUATION_SHADER: this->sTypeName = "Tessellation Evaluation Shader"; break;
        case TYPES::GEOMETRY_SHADER:                this->sTypeName = "Geometry Shader"; break;
    }
    this->iShaderID = glCreateShader(this->iType);
}

Shader::~Shader()
{
    if(iShaderID != 0)
        glDeleteShader(iShaderID);
}

std::string Shader::compile()
{
    const char* shaderStr = this->sSource.c_str();
    glShaderSource(iShaderID, 1, &shaderStr, nullptr); //Pass sourceCode to OpenGL
    glCompileShader(iShaderID); //compile the shader

    //check for errors
    GLint success = 0;
    glGetShaderiv(iShaderID, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(iShaderID, GL_INFO_LOG_LENGTH, &maxLength);

        //The maxLength includes the NULL character
        std::string errorLog;
        errorLog.resize(maxLength);
        glGetShaderInfoLog(iShaderID, maxLength, &maxLength, &errorLog[0]);

        //Provide the infolog in whatever manor you deem best.
        //Exit with failure.
        glDeleteShader(iShaderID); //Don't leak the shader.

        //Print error log and quit
        return errorLog;
    }
    bIsCompiled = true;
    return "";
}

//Getter
std::string Shader::getSourceCode() { return this->sSource; }
int Shader::getShaderType()         { return this->iType; }
int Shader::getShaderID()           { return this->iShaderID; }