#pragma once

class Texture
{
public:
	Texture();
	Texture(const std::string& path);
	Texture(unsigned char* buffer, int Width, int Height);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void UnBind() const;

	void UpdateTexture(unsigned char* buffer, int Width, int Height);

	inline int GetWidth() const { return width; }
	inline int GetHeigth() const { return height; }
private:
	void Setup(unsigned char* buffer, int Width, int Height, GLint sampling);
private:
	unsigned int rendererID;
	std::string filepath;
	unsigned char* localBuffer;
	int width, height, BPP;
};