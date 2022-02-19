#pragma once

class IndexBuffer {
private:
	unsigned int rendererID;
	unsigned int count;
public:
	IndexBuffer(const unsigned int* data, unsigned int p_count);
	~IndexBuffer();

	void bind() const;
	void unbind() const;

	inline unsigned int getCount() const { return count; }
};