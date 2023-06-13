#include "Image.hpp"

#include <glad.h>

Image Image::from(unsigned int width, unsigned int height,
				  int internalFormat, int format, int type) {
	Image result = {};

	glGenTextures(1, &result.texture);
	glBindTexture(GL_TEXTURE_2D, result.texture);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height,
				 0, format, type, nullptr);

	return result;
}

Image& Image::set_min_filter(int value) {
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, value);
	return *this;
}

Image& Image::set_mag_filter(int value) {
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, value);
	return *this;
}

unsigned int Image::get_handle() {
	return texture;
}
