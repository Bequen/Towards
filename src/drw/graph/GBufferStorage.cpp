#include "GBufferStorage.hpp"

#include <glad.h>
#include <string>
#include <algorithm>

struct ImageCreateInfo {
	std::string name;
	int internalFormat;
	int format;
	int type;
};

GBufferStorage GBufferStorage::from(uint32_t width, uint32_t height) {
	GBufferStorage result = {};

	result.images.resize(3);
	
	std::vector<ImageCreateInfo> config {
		{ "position", GL_RGBA16F, GL_RGBA, GL_FLOAT },
		{ "normal", GL_RGBA16F, GL_RGBA, GL_FLOAT },
		{ "albedo", GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE }
	};

	std::transform(config.begin(), config.end(), result.images.begin(),
		[width, height](ImageCreateInfo info) -> Image {
			return Image::from(width, height, info.internalFormat, info.format, info.type)
				.set_mag_filter(GL_NEAREST)
				.set_min_filter(GL_NEAREST);
		});

	result.depthImage = Image::from(width, height, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT)
		.set_mag_filter(GL_NEAREST)
		.set_min_filter(GL_NEAREST);

	return result;
}

std::vector<Image>& GBufferStorage::get_images() {
	return images;
}

Image *GBufferStorage::get_depth_image() {
	return &depthImage;
}
