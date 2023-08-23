#include <Graphics/Shader.h>
#include <GL/glew.h>

const unsigned int Shader::TYPES::VERTEX_SHADER                  = GL_VERTEX_SHADER;
const unsigned int Shader::TYPES::FRAGMENT_SHADER                = GL_FRAGMENT_SHADER;
const unsigned int Shader::TYPES::TESSELLATION_CONTROL_SHADER    = GL_TESS_CONTROL_SHADER;
const unsigned int Shader::TYPES::TESSELLATION_EVALUATION_SHADER = GL_TESS_EVALUATION_SHADER;
const unsigned int Shader::TYPES::GEOMETRY_SHADER                = GL_GEOMETRY_SHADER;

void Shader::createNative()
{
    this->iShaderID = glCreateShader(this->iType);
}

void Shader::destroyNative()
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