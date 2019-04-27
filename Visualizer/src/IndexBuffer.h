#pragma once

class IndexBuffer
{
public:
	IndexBuffer(const unsigned int* data, unsigned int _count);
	~IndexBuffer();

	void Bind() const;
	void UnBind() const;

	inline unsigned int GetCount() const { return count; }
private:
	unsigned int rendererID;
	unsigned int count;
};