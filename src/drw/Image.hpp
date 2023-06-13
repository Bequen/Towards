#pragma once

struct Image {
public:
	static Image from(unsigned int width, unsigned int height, 
					  int internalFormat, int format, int type);

	Image& set_min_filter(int value);
	Image& set_mag_filter(int value);

	unsigned int get_handle();

private:
	unsigned int texture;
};
