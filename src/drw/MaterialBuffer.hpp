#pragma once

#include "drw/Pipeline.hpp"
#include "ImageData.hpp"
#include "TextureStorage.hpp"
#include "MaterialData.hpp"

struct Material {
	float baseColor[4];
	TextureIdx baseColorTexture;

	TextureIdx normalTexture;
	TextureIdx metallicRoughnessTexture;
	int fill;

	Material(float baseColor[4]);

	void set_base_color_texture(TextureIdx texture);

	void set_normal_texture(TextureIdx texture);

	void set_metallic_roughness_texture(TextureIdx texture);
};



class MaterialBuffer {
private:
	TextureStorage *m_pBaseColorTextureStorage;
	TextureStorage *m_pMetallicRoughnessTextureStorage;
	TextureStorage *m_pNormalTextureStorage;

	unsigned m_materialBuffer;
	unsigned m_numMaterials;
	unsigned m_maxMaterials;

public:
	MaterialBuffer(unsigned numTextures, 
				   unsigned long numMaterials);

	unsigned push_materials(unsigned numMaterials, MaterialData *pData);

	void use(drw::Pipeline *pPipeline);
};
