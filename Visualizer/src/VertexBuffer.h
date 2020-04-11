#pragma once

class VertexBuffer
{
public:
	VertexBuffer(unsigned int size);
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void Bind() const;
	void UnBind() const;

	void SetData(const void* data, unsigned int size) const;
private:
	unsigned int rendererID;
};