#pragma once

#include "TextureStorage.hpp"
#include "ImageData.hpp"

class ArrayTextureStorage : public TextureStorage {
private:
	Format m_format;
	unsigned m_texture;

	char *m_pBits;

	unsigned m_numTextures;
	unsigned m_width, m_height;

	unsigned get_num_bytes();

	TextureIdx get_available_idx();

public:
	ArrayTextureStorage(Format format,
						unsigned width,
						unsigned height,
						unsigned num);

	TextureIdx upload(ImageData *pInfo) override;

	void unload(TextureIdx texture) override;

	void generate_mipmaps();

	void use(drw::Pipeline *pPipeline, char *pName, unsigned idx) override;
};
