#include "PreCompiledHeader.h"
#include "Shader.h"


Shader::Shader(const std::string& _filepath)
	: filepath(_filepath), rendererID(0)
{
	ShaderProgramSource source = ParseShader(_filepath);
	rendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader()
{
	GLCall(glDeleteProgram(rendererID)); /* Deletes the shader from the GPU*/
}

void Shader::Bind() const
{
	GLCall(glUseProgram(rendererID));
}

void Shader::UnBind() const
{
	GLCall(glUseProgram(0));
}

void Shader::SetUniform1i(const std::string& name, int value)
{
	GLCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniform1f(const std::string& name, float value)
{
	GLCall(glUniform1f(GetUniformLocation(name), value));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

int Shader::GetUniformLocation(const std::string& name)
{
	if (uniformLocationCache.find(name) != uniformLocationCache.end())
		return uniformLocationCache[name];

	GLCall(int location = glGetUniformLocation(rendererID, name.c_str()));
	if (location == -1)
		std::cout << "Warning: uniform '" << name << "' does not exist!" << std::endl;

	uniformLocationCache[name] = location;
	return location;
}

ShaderProgramSource Shader::ParseShader(const std::string& _filepath)
{
	std::fstream stream(_filepath);

	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;

	while (std::getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
				type = ShaderType::VERTEX;
			else if (line.find("fragment") != std::string::npos)
				type = ShaderType::FRAGMENT;
		}
		else if (type != ShaderType::NONE)
		{
			ss[(int)type] << line << '\n';
		}
	}
	return { ss[(int)ShaderType::VERTEX].str(), ss[(int)ShaderType::FRAGMENT].str() };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
	GLCall(unsigned int id = glCreateShader(type));
	const char* src = source.c_str();
	GLCall(glShaderSource(id, 1, &src, nullptr));
	GLCall(glCompileShader(id));

	int result;
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE)
	{
		int length;
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		char* message = (char*)_malloca(length * sizeof(char));
		GLCall(glGetShaderInfoLog(id, length, &length, message));
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
		std::cout << message << std::endl;
		GLCall(glDeleteShader(id));
		_freea(message);
		return 0;
	}

	return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	GLCall(unsigned int program = glCreateProgram());
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	GLCall(glAttachShader(program, vs));
	GLCall(glAttachShader(program, fs));
	GLCall(glLinkProgram(program));
	GLCall(glValidateProgram(program));

	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));

	return program;
}