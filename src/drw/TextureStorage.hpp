#pragma once

#include "Format.hpp"
#include "ImageData.hpp"
#include "drw/Pipeline.hpp"

typedef unsigned int TextureIdx;

class TextureStorage {
public:
	virtual TextureIdx upload(ImageData *pInfo) = 0;

	virtual void unload(TextureIdx texture) = 0;

	virtual void use(drw::Pipeline *pPipeline, char *pName, unsigned idx) = 0;

	virtual void generate_mipmaps() = 0;
};
