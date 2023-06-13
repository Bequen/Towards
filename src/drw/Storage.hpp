#pragma once

#include <vector> 

#include "Image.hpp"

/* Storage is for passes to output data */
class Storage {
public:
	virtual bool resize(unsigned int width, unsigned int height) = 0;

	virtual std::vector<Image>& get_images() = 0;

	virtual Image *get_depth_image() {
		return nullptr;
	}
};
