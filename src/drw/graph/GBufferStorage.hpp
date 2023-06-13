#pragma once

#include <cstdint>
#include <vector>

#include "drw/Image.hpp"
#include "drw/Storage.hpp"

class GBufferStorage : public Storage {
private:
	std::vector<Image> images;
	Image depthImage;

public:
	bool resize(unsigned int width, unsigned int height) override {
		return true;	
	}

	static GBufferStorage from(uint32_t width, uint32_t height);

	std::vector<Image>& get_images() override;

	Image *get_depth_image() override;
};
