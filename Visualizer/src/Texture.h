#pragma once

class Texture
{
public:
	Texture();
	Texture(const std::string& path);
	Texture(const std::string& path, const std::string& textureType);
	Texture(unsigned char* buffer, int Width, int Height);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void UnBind() const;

	void UpdateTexture(unsigned char* buffer, int Width, int Height);

	inline int GetWidth() const { return width; }
	inline int GetHeigth() const { return height; }

	inline const std::string& GetFilePath() const { return filepath; }
	inline const std::string& GetTextureType() const { return textureType; }
private:
	void Setup(unsigned char* buffer, int Width, int Height, GLint sampling);
	void LoadAndSetup(const std::string& path, GLint sampling);
private:
	unsigned int rendererID;
	std::string filepath;
	std::string textureType;
	unsigned char* localBuffer;
	int width, height, BPP;
};