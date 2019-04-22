#pragma once
#include "PreCompiledHeader.h"

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader
{
public:
	Shader(const std::string& _filepath);
	~Shader();

	void Bind() const;
	void UnBind() const;

	// Set uniform
	void SetUniform1i(const std::string& name, int value);
	void SetUniform1f(const std::string& name, float value);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
private:
	std::string filepath;
	unsigned int rendererID;
	std::unordered_map<std::string, int> uniformLocationCache;

	ShaderProgramSource ParseShader(const std::string& _filepath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	int GetUniformLocation(const std::string& name);
};