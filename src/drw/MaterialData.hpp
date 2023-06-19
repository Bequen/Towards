#pragma once

#include "ImageData.hpp"

struct MaterialData {
	float baseColor[4];
	ImageData *pBaseColorImage;

	ImageData *pNormalImage;
	ImageData *pMetallicRoughnessImage;
};

