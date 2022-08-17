#include "ShaderProgram.h"
#include "Essentials/MemoryManagement.h"
#include <iostream>
#include <vector>
#include <Essentials/Output.h>
#include <fstream>

ShaderProgram* ShaderProgram::pCurrentlyBoundShaderProgram = nullptr;

//The : _numAttributes(0) ect. is an initialization list. It is a better way to initialize variables, since it avoids an extra copy. 
ShaderProgram::ShaderProgram()  
{

}

ShaderProgram::~ShaderProgram() 
{
	if(this->iProgramID != 0)
		glDeleteProgram(this->iProgramID);
	
	for(size_t i = 0; i < vShaders.size(); i++)
		Gum::_delete(vShaders[i]);

	Locations.clear();
	vShaders.clear();
}

//Compiles the shaders into a form that your GPU can understand
void ShaderProgram::compileShaders() 
{

	for(size_t i = 0; i < vShaders.size(); i++)
	{
        Gum::Output::debug("ShaderProgram: Compiling " + std::to_string(vShaders[i]->getShaderType()));
        std::string error = vShaders[i]->compile();
        if(error != "")
        {
            Gum::Output::fatal("ShaderProgram: " + this->sName + ": " + error);
        }
	}
}

void ShaderProgram::linkShaders() 
{
	for(size_t i = 0; i < vShaders.size(); i++)
	{
		glAttachShader(this->iProgramID, vShaders[i]->getShaderID());
	}

	//Link our program
	glLinkProgram(this->iProgramID);

	//Note the different functions here: glGetProgram* instead of glGetShader*.
	GLint isLinked = 0;
	glGetProgramiv(this->iProgramID, GL_LINK_STATUS, (int *)&isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(this->iProgramID, GL_INFO_LOG_LENGTH, &maxLength);

		//The maxLength includes the NULL character
		std::string errorLog;
		errorLog.resize(maxLength);
		glGetProgramInfoLog(this->iProgramID, maxLength, &maxLength, &errorLog[0]);



		//We don't need the program anymore.
		glDeleteProgram(this->iProgramID);
		//Don't leak shaders either.
		/*for(int i = 0; i < vShaders.size(); i++)
		{
			glDeleteShader(vShaders[i]->getShaderID());
		}*/

		//print the error log and quit
		Gum::Output::fatal("ShaderProgram: Linking Error: " + this->sName + ": " + errorLog);
	}

	//Always detach shaders after a successful link.
	for(size_t i = 0; i < vShaders.size(); i++)
	{
		glDetachShader(this->iProgramID, vShaders[i]->getShaderID());
		//glDeleteShader(vShaders[i]->getShaderID());
	}
}

//Adds an attribute to our shader. SHould be called between compiling and linking.
void ShaderProgram::addAttribute(const std::string& attributeName, const int& number) {
	glBindAttribLocation(this->iProgramID, number, attributeName.c_str());
	Attributes[attributeName] = number;
}

void ShaderProgram::addUniform(const std::string& Name) { Locations[Name] = GetUniformLocation(Name); }
void ShaderProgram::addUniform(const std::string& Name, const int& size)
{
	for (int i = 0; i < size; i++)
	{
		addUniform(Name + "[" + std::to_string(i) + "]");
	}
}
void ShaderProgram::addTexture(const std::string& Name, const int& index)
{
    Locations[Name] = GetUniformLocation(Name);
    use();
    LoadUniform(Name, index);
    unuse();
}

void ShaderProgram::use()   { glUseProgram(iProgramID); setCurrentlyBoundShader(this); }
void ShaderProgram::unuse() { glUseProgram(0);          setCurrentlyBoundShader(nullptr); }


int ShaderProgram::GetUniformLocation(const std::string& UniformName) { return glGetUniformLocation(iProgramID, UniformName.c_str()); }
void ShaderProgram::addShader(Shader* shader) { this->vShaders.push_back(shader); }
void ShaderProgram::removeShader(int index) { this->vShaders.erase(vShaders.begin() + index); }

void ShaderProgram::build(const std::string& name, std::map<const char*, unsigned int> attributes)
{
	this->sName = name;
    Gum::Output::debug("ShaderProgram: Creating Shader Program for " + sName);
	iProgramID = glCreateProgram();
	compileShaders();

	for (auto attribute : attributes)
	{
		addAttribute(attribute.first, attribute.second);
		Gum::Output::debug("Adding attribute " + std::string(attribute.first) + " (" + std::to_string(attribute.second) + ")");
	}

	if(attributes.size() > 0)
	{
		Gum::Output::debug("");
	}

    Gum::Output::debug("ShaderProgram: Linking " + name);
	linkShaders();


    Gum::Output::debug("ShaderProgram: Adding default Uniforms " + name);
	addUniform("transformationMatrix");
	addUniform("viewMatrix");
	addUniform("projectionMatrix");
}

void ShaderProgram::rebuild()
{
	//glDeleteProgram(iProgramID);
    //build();
}

void ShaderProgram::LoadUniform(const std::string& uniformName, const bool& var) 				{ glUniform1i(Locations[uniformName], var); }
void ShaderProgram::LoadUniform(const std::string& uniformName, const vec2& var) 				{ glUniform2f(Locations[uniformName], var.x, var.y); }
void ShaderProgram::LoadUniform(const std::string& uniformName, const ivec2& var) 				{ glUniform2i(Locations[uniformName], var.x, var.y); }
void ShaderProgram::LoadUniform(const std::string& uniformName, const vec3& var) 				{ glUniform3f(Locations[uniformName], var.x, var.y, var.z); }
void ShaderProgram::LoadUniform(const std::string& uniformName, const ivec3& var) 		    	{ glUniform3i(Locations[uniformName], var.x, var.y, var.z); }
void ShaderProgram::LoadUniform(const std::string& uniformName, const vec4& var) 				{ glUniform4f(Locations[uniformName], var.x, var.y, var.z, var.w); }
void ShaderProgram::LoadUniform(const std::string& uniformName, const mat4& var) 			    { glUniformMatrix4fv(Locations[uniformName], 1, GL_FALSE, &var[0][0]); }
void ShaderProgram::LoadUniform(const std::string& uniformName, const float& var) 				{ glUniform1f(Locations[uniformName], var); }
void ShaderProgram::LoadUniform(const std::string& uniformName, const int& var) 				{ glUniform1i(Locations[uniformName], var); }
void ShaderProgram::LoadUniform(const std::string& uniformName, const std::vector<mat4>& var) 	{ for (size_t i = 0; i < var.size(); i++) { LoadUniform(uniformName + "[" + std::to_string(i) + "]", var[i]); } }


//
// Setter
//
void ShaderProgram::setName(const std::string& name) 				{ this->sName = name; }
void ShaderProgram::setCurrentlyBoundShader(ShaderProgram* program) { pCurrentlyBoundShaderProgram = program; }


//
// Getter
//
std::string ShaderProgram::getName() const           				{ return sName; }
GLuint ShaderProgram::getProgramID() const           				{ return iProgramID; }
ShaderProgram* ShaderProgram::getCurrentlyBoundShader() 			{ return pCurrentlyBoundShaderProgram; }