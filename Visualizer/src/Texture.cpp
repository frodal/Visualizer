#include "PreCompiledHeader.h"
#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture()
	: rendererID(0), localBuffer(nullptr), width(0), height(0), BPP(0)
{
	Setup(nullptr, 0, 0, GL_NEAREST);
}

Texture::Texture(const std::string& path)
	: rendererID(0), filepath(path), localBuffer(nullptr), width(0), height(0), BPP(0)
{
	stbi_set_flip_vertically_on_load(1);
	localBuffer = stbi_load(path.c_str(), &width, &height, &BPP, 4);

	Setup(localBuffer, width, height, GL_LINEAR);

	if (localBuffer)
		stbi_image_free(localBuffer);
}

Texture::Texture(unsigned char* buffer, int Width, int Height)
	: rendererID(0), localBuffer(buffer), width(Width), height(Height), BPP(0)
{
	Setup(localBuffer, width, height, GL_NEAREST);
}

Texture::~Texture()
{
	GLCall(glDeleteTextures(1, &rendererID));
}

void Texture::Bind(unsigned int slot) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, rendererID));
}

void Texture::UnBind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

void Texture::UpdateTexture(unsigned char* buffer, int Width, int Height)
{
	localBuffer = buffer;
	width = Width;
	height = Height;

	GLCall(glBindTexture(GL_TEXTURE_2D, rendererID));
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

void Texture::Setup(unsigned char* buffer, int Width, int Height, GLint sampling)
{
	GLCall(glGenTextures(1, &rendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D, rendererID));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, sampling));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, sampling));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}
