#pragma once
#include <Essentials/Settings.h>
#include <gum-maths.h>
#include "Shader.h"
#include <vector>

class ShaderProgram
{
private:
	void GetAllUniformLocations();

	std::vector<Shader*> vShaders;
	std::map<std::string, int> Locations;
	std::map<std::string, int> Attributes;

	GLuint iProgramID;
	std::string sName;
	void compileShaders();
	void linkShaders();

	static ShaderProgram* pCurrentlyBoundShaderProgram;

public:
	ShaderProgram();
	~ShaderProgram();

	void addAttribute(const std::string& attributeName, const int& number);
	void addUniform(const std::string& Name);
	void addUniform(const std::string& Name, const int& size);
	void addTexture(const std::string& Name, const int& index);


	void use();
	void unuse();

	void LoadUniform(const std::string& uniformName, const bool& var);
	void LoadUniform(const std::string& uniformName, const vec2& var);
	void LoadUniform(const std::string& uniformName, const ivec2& var);
	void LoadUniform(const std::string& uniformName, const vec3& var);
	void LoadUniform(const std::string& uniformName, const ivec3& var);
	void LoadUniform(const std::string& uniformName, const vec4& var);
	void LoadUniform(const std::string& uniformName, const mat4& var);
	void LoadUniform(const std::string& uniformName, const std::vector<mat4>& var);
	void LoadUniform(const std::string& uniformName, const float& var);
	void LoadUniform(const std::string& uniformName, const int& var);

	int GetUniformLocation(const std::string& UniformName);

	void build(const std::string& name, std::map<const char*, unsigned int> attributes = {
		{"vertexPosition", 0}, {"TextureCoords", 1}, {"Normals", 2}, {"TransMatrix", 3}, 
		{"tangentNormals", 7}, {"jointIndices", 8}, {"weights", 9}, {"individualColor", 10}
	});
	void rebuild();

	void addShader(Shader* shader);
	void removeShader(int index);


	//Setter
	void setName(const std::string& name);
	static void setCurrentlyBoundShader(ShaderProgram* program);

	//Getter
	std::string getName() const;
	GLuint getProgramID() const;
	static ShaderProgram* getCurrentlyBoundShader();
	Shader* getShader(int index);
};

