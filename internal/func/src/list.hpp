#pragma once

#include <stdlib.h>
#include <err.h>
#include <errno.h>
#include <functional>
#include <iostream>

template<typename T>
struct List {
private:
	unsigned long size;
	T* pData;

public:
	List() {
		pData = NULL;
	}

	List(unsigned long size) {
		pData = (T*)malloc(size * sizeof(T));
	}

	List(unsigned long size, T *pData) :
	size(size), pData(pData) {
		
	}
	
	List(std::initializer_list<T> lst) {
		resize(lst.size());

		std::copy(lst.begin(), lst.end(), pData);	
	}

	~List() {
		free(pData);
	}

	void resize(unsigned long size) {
		pData = (T*)realloc(pData, size);
		if(pData == nullptr) {
			err(EINVAL, "Failed to reallocate memory. Probably out of memory");
		}
		this->size = size;
	}

	unsigned int get_size() {
		return size;
	}

	template<typename R>
	R apply(std::function<R(R, T)> func) {
		R begin = NULL;
		for(size_t i = 0; i < size; i++) {
			begin = func(begin, pData[i]);
		}
		return begin;
	}

	template<typename R>
	List<R> map(std::function<R(T)> func) {
		List<R> result = List<R>(size);

		for(size_t i = 0; i < size; i++) {
			result[i] = func(pData[i]);
		}

		return result;
	}

	T& operator[](int index) {
		if(index < size) err(EFAULT, "Index is reaching outside of list");
		return pData[index];
	}
};
