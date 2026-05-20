#include "Essentials/Tools.h"
#include <Graphics/ShaderProgram.h>
#include <System/Output.h>
#include <glad/gl.h>


void ShaderProgram::createNative()
{
	iProgramID = glCreateProgram();
}

void ShaderProgram::destroyNative()
{
	if(this->iProgramID != 0)
		glDeleteProgram(this->iProgramID);
}

void ShaderProgram::linkShaders() 
{
	for(Shader* shader : vShaders)
		glAttachShader(this->iProgramID, shader->getShaderID());

	glLinkProgram(this->iProgramID);

	GLint isLinked = 0;
	glGetProgramiv(this->iProgramID, GL_LINK_STATUS, (int *)&isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(this->iProgramID, GL_INFO_LOG_LENGTH, &maxLength);

		std::string errorLog;
		errorLog.resize(maxLength);
		glGetProgramInfoLog(this->iProgramID, maxLength, &maxLength, &errorLog[0]);

		glDeleteProgram(this->iProgramID);

		//print the error log and quit
		Gum::Output::fatal("ShaderProgram: Linking Error: " + this->sName + ": " + errorLog);
	}

	//Always detach shaders after a successful link.
	for(Shader* shader : vShaders)
	{
		glDetachShader(this->iProgramID, shader->getShaderID());
		//glDeleteShader(vShaders[i]->getShaderID());
	}
}

//Adds an attribute to our shader. SHould be called between compiling and linking.
void ShaderProgram::addAttribute(const std::string& attributeName, const int& number) 
{
	glBindAttribLocation(this->iProgramID, number, attributeName.c_str());
	Attributes[attributeName] = number;
}

void ShaderProgram::use()   { glUseProgram(iProgramID); setCurrentlyBoundShader(this); }
void ShaderProgram::unuse() { glUseProgram(0);          setCurrentlyBoundShader(nullptr); }


int ShaderProgram::getUniformLocation(const std::string& UniformName) { return glGetUniformLocation(iProgramID, UniformName.c_str()); }

#ifdef CHECK_GL_ERRORS
#define UNIFORM_CHECK     if(!Tools::mapHasKey(Locations, uniformName)) Gum::Output::error(sName + ": Uniform " + uniformName + " does not exist!")
#define UNIFORM_ARR_CHECK if(!Tools::mapHasKey(Locations, uniformName + "[0]")) Gum::Output::error(sName + ": Uniform " + uniformName + " does not exist!")
#else
#define UNIFORM_CHECK
#define UNIFORM_ARR_CHECK
#endif

void ShaderProgram::loadUniform(const std::string& uniformName, const bool& var) 	{ UNIFORM_CHECK; glUniform1i(Locations[uniformName], var); }
void ShaderProgram::loadUniform(const std::string& uniformName, const vec2& var) 	{ UNIFORM_CHECK; glUniform2f(Locations[uniformName], var.x, var.y); }
void ShaderProgram::loadUniform(const std::string& uniformName, const ivec2& var) { UNIFORM_CHECK; glUniform2i(Locations[uniformName], var.x, var.y); }
void ShaderProgram::loadUniform(const std::string& uniformName, const vec3& var) 	{ UNIFORM_CHECK; glUniform3f(Locations[uniformName], var.x, var.y, var.z); }
void ShaderProgram::loadUniform(const std::string& uniformName, const ivec3& var) { UNIFORM_CHECK; glUniform3i(Locations[uniformName], var.x, var.y, var.z); }
void ShaderProgram::loadUniform(const std::string& uniformName, const vec4& var) 	{ UNIFORM_CHECK; glUniform4f(Locations[uniformName], var.x, var.y, var.z, var.w); }
void ShaderProgram::loadUniform(const std::string& uniformName, const mat4& var) 	{ UNIFORM_CHECK; glUniformMatrix4fv(Locations[uniformName], 1, GL_FALSE, &var[0][0]); }
void ShaderProgram::loadUniform(const std::string& uniformName, const float& var) { UNIFORM_CHECK; glUniform1f(Locations[uniformName], var); }
void ShaderProgram::loadUniform(const std::string& uniformName, const int& var) 	{ UNIFORM_CHECK; glUniform1i(Locations[uniformName], var); }
void ShaderProgram::loadUniform(const std::string& uniformName, const color& var) { loadUniform(uniformName, var.getGLColor()); }


void ShaderProgram::loadUniform(const std::string& uniformName, const std::vector<vec2>& var)   { UNIFORM_ARR_CHECK; glUniform2fv(Locations[uniformName + "[0]"], (GLsizei)var.size(), &var[0].x); }
void ShaderProgram::loadUniform(const std::string& uniformName, const std::vector<ivec2>& var)  { UNIFORM_ARR_CHECK; glUniform2iv(Locations[uniformName + "[0]"], (GLsizei)var.size(), &var[0].x); }
void ShaderProgram::loadUniform(const std::string& uniformName, const std::vector<vec3>& var)   { UNIFORM_ARR_CHECK; glUniform3fv(Locations[uniformName + "[0]"], (GLsizei)var.size(), &var[0].x); }
void ShaderProgram::loadUniform(const std::string& uniformName, const std::vector<ivec3>& var)  { UNIFORM_ARR_CHECK; glUniform3iv(Locations[uniformName + "[0]"], (GLsizei)var.size(), &var[0].x); }
void ShaderProgram::loadUniform(const std::string& uniformName, const std::vector<vec4>& var)   { UNIFORM_ARR_CHECK; glUniform4fv(Locations[uniformName + "[0]"], (GLsizei)var.size(), &var[0].x); }
void ShaderProgram::loadUniform(const std::string& uniformName, const std::vector<mat4>& var)   { UNIFORM_ARR_CHECK; glUniformMatrix4fv(Locations[uniformName + "[0]"], (GLsizei)var.size(), GL_FALSE, &var[0][0][0]); }
void ShaderProgram::loadUniform(const std::string& uniformName, const std::vector<float>& var)  { UNIFORM_ARR_CHECK; glUniform1fv(Locations[uniformName + "[0]"], (GLsizei)var.size(), var.data()); }
void ShaderProgram::loadUniform(const std::string& uniformName, const std::vector<int>& var)    { UNIFORM_ARR_CHECK; glUniform1iv(Locations[uniformName + "[0]"], (GLsizei)var.size(), var.data()); }