#include <Graphics/ShaderProgram.h>
#include <System/Output.h>
#include <GL/glew.h>


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
void ShaderProgram::addAttribute(const std::string& attributeName, const int& number) 
{
	glBindAttribLocation(this->iProgramID, number, attributeName.c_str());
	Attributes[attributeName] = number;
}

void ShaderProgram::use()   { glUseProgram(iProgramID); setCurrentlyBoundShader(this); }
void ShaderProgram::unuse() { glUseProgram(0);          setCurrentlyBoundShader(nullptr); }


int ShaderProgram::getUniformLocation(const std::string& UniformName) { return glGetUniformLocation(iProgramID, UniformName.c_str()); }

void ShaderProgram::loadUniform(const std::string& uniformName, const bool& var) 				{ glUniform1i(Locations[uniformName], var); }
void ShaderProgram::loadUniform(const std::string& uniformName, const vec2& var) 				{ glUniform2f(Locations[uniformName], var.x, var.y); }
void ShaderProgram::loadUniform(const std::string& uniformName, const ivec2& var) 				{ glUniform2i(Locations[uniformName], var.x, var.y); }
void ShaderProgram::loadUniform(const std::string& uniformName, const vec3& var) 				{ glUniform3f(Locations[uniformName], var.x, var.y, var.z); }
void ShaderProgram::loadUniform(const std::string& uniformName, const ivec3& var) 		    	{ glUniform3i(Locations[uniformName], var.x, var.y, var.z); }
void ShaderProgram::loadUniform(const std::string& uniformName, const vec4& var) 				{ glUniform4f(Locations[uniformName], var.x, var.y, var.z, var.w); }
void ShaderProgram::loadUniform(const std::string& uniformName, const mat4& var) 			    { glUniformMatrix4fv(Locations[uniformName], 1, GL_FALSE, &var[0][0]); }
void ShaderProgram::loadUniform(const std::string& uniformName, const float& var) 				{ glUniform1f(Locations[uniformName], var); }
void ShaderProgram::loadUniform(const std::string& uniformName, const int& var) 				{ glUniform1i(Locations[uniformName], var); }
void ShaderProgram::loadUniform(const std::string& uniformName, const std::vector<mat4>& var) 	{ for (size_t i = 0; i < var.size(); i++) { loadUniform(uniformName + "[" + std::to_string(i) + "]", var[i]); } }