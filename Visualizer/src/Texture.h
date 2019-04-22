#pragma once

class Texture
{
public:
	Texture(const std::string& path);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void UnBind() const;

	inline int GetWidth() const { return width; }
	inline int GetHeigth() const { return height; }
private:
	unsigned int rendererID;
	std::string filepath;
	unsigned char* localBuffer;
	int width, height, BPP;
};