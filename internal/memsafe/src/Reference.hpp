#pragma once

template<typename T>
struct Ref {
private:
	void *ptr;
	unsigned int *counter;

public:
	Ref(T *ptr);
};
