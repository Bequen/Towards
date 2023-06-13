#pragma once

#define ALLOC(type, count) Alloc(sizeof(type) * count)

struct Alloc {
private:
	void *m_pData;
	unsigned long m_size;

public:
	Alloc(unsigned long size);

	unsigned long size();
};
