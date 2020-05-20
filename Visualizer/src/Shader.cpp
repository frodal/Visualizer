#include "PreCompiledHeader.h"
#include "Shader.h"


Shader::Shader(const std::string& _filepath)
	: filepath(_filepath), rendererID(0)
{
	ShaderProgramSource source = ParseShader(_filepath);
	if (source.FragmentSource.empty() || source.VertexSource.empty())
	{
		std::cout << "Warning: Did not find a valid shader source file '" << _filepath << "'" << std::endl;
		std::cout << "The default shader will be used!" << std::endl;
		source = DefaultShaderSource();
	}
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

void Shader::SetUniform2f(const std::string& name, float v0, float v1)
{
	GLCall(glUniform2f(GetUniformLocation(name), v0, v1));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniform1d(const std::string& name, double value)
{
	GLCall(glUniform1d(GetUniformLocation(name), value));
}

void Shader::SetUniform2d(const std::string& name, double v0, double v1)
{
	GLCall(glUniform2d(GetUniformLocation(name), v0, v1));
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
	GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
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
			ss[static_cast<int>(type)] << line << '\n';
		}
	}
	return { ss[static_cast<int>(ShaderType::VERTEX)].str(), ss[static_cast<int>(ShaderType::FRAGMENT)].str() };
}

ShaderProgramSource Shader::DefaultShaderSource()
{
	const char* vertexSource =
		"#version 330 core\n"
		"\n"
		"layout(location = 0) in vec4 position;\n"
		"\n"
		"\n"
		"uniform mat4 u_MVP;\n"
		"\n"
		"void main()\n"
		"{\n"
		"	gl_Position = u_MVP * position;\n"
		"};\n";
	const char* fragmentSource =
		"#version 330 core\n"
		"\n"
		"layout(location = 0) out vec4 color;\n"
		"\n"
		"\n"
		"\n"
		"void main()\n"
		"{\n"
		"	color = vec4(1.0, 1.0, 1.0, 1.0);\n"
		"};\n";
	return { vertexSource ,fragmentSource };
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
		char* message = static_cast<char*>(_malloca(length * sizeof(char)));
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

	// Check if Link was successful
	int result;
	GLCall(glGetProgramiv(program, GL_LINK_STATUS, &result));
	if (result == GL_FALSE)
	{
		int length;
		GLCall(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length));
		char* message = static_cast<char*>(_malloca(length * sizeof(char)));
		GLCall(glGetProgramInfoLog(program, length, &length, message));
		std::cout << "Failed to link shader!" << std::endl;
		std::cout << message << std::endl;
		GLCall(glDeleteProgram(program));
		GLCall(glDeleteShader(vs));
		GLCall(glDeleteShader(fs));
		_freea(message);
		return 0;
	}
	
	// Checks to see whether the executables contained in program can execute given the current OpenGL state
	GLCall(glValidateProgram(program));

	GLCall(glGetProgramiv(program, GL_VALIDATE_STATUS, &result));
	if (result == GL_FALSE)
	{
		int length;
		GLCall(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length));
		char* message = static_cast<char*>(_malloca(length * sizeof(char)));
		GLCall(glGetProgramInfoLog(program, length, &length, message));
		std::cout << "Failed to validate shader program!" << std::endl;
		std::cout << message << std::endl;
		GLCall(glDeleteProgram(program));
		GLCall(glDeleteShader(vs));
		GLCall(glDeleteShader(fs));
		_freea(message);
		return 0;
	}

	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));

	return program;
}