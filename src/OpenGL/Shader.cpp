#include <Graphics/Shader.h>
#include <Graphics/Graphics.h>
#include <GL/glew.h>
#include <regex>

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
  if(sSource.find_first_of(GUM_SHADER_VERSION_STR) != std::string::npos)
  {
    std::string ver = "#version "+std::to_string(Gum::Graphics::VARS::SHADING_LANGUAGE_MAJOR_VERSION)+std::to_string(Gum::Graphics::VARS::SHADING_LANGUAGE_MINOR_VERSION)+" core \n";
    sSource = std::regex_replace(sSource, std::regex(GUM_SHADER_VERSION_STR), ver);
  }
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

    std::cout << sSource << std::endl;
    //Print error log and quit
    return errorLog;
  }
  bIsCompiled = true;
  return "";
}