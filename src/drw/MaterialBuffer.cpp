#include "MaterialBuffer.hpp"
#include "drw/ArrayTextureStorage.hpp"
#include "drw/Format.hpp"
#include "drw/TextureStorage.hpp"

#include <glad.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

Material::Material(float baseColor[4]) {
	memcpy(this->baseColor, baseColor, 4 * 4);
}

void Material::set_base_color_texture(TextureIdx texture) {
	this->baseColorTexture = texture;
}

void Material::set_normal_texture(TextureIdx texture) {
	this->normalTexture = texture;
}

void Material::set_metallic_roughness_texture(TextureIdx texture) {
	this->metallicRoughnessTexture = texture;
}

MaterialBuffer::MaterialBuffer(unsigned numTextures, unsigned long numMaterials) :
m_numMaterials(0), m_maxMaterials(numMaterials) {
	m_pBaseColorTextureStorage = new ArrayTextureStorage(BASE_COLOR_FORMAT_RGBA8, 1024, 1024, numTextures);
	m_pNormalTextureStorage = new ArrayTextureStorage(NORMAL_FORMAT_RGBA16F, 1024, 1024, numTextures);
	m_pMetallicRoughnessTextureStorage = new ArrayTextureStorage(METALLIC_ROUGHNESS_FORMAT_RG8, 1024, 1024, numTextures);

	glGenBuffers(1, &m_materialBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, m_materialBuffer);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(MaterialData) * 64, nullptr, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

unsigned MaterialBuffer::push_materials(unsigned numMaterials, MaterialData *pData) {
	Material *pMaterials = (Material*)malloc(sizeof(Material) * numMaterials);
	for(int i = 0; i < numMaterials; i++) {
		pMaterials[i] = Material(pData[i].baseColor);

		if(pData[i].pBaseColorImage) {
			pMaterials[i].set_base_color_texture(m_pBaseColorTextureStorage->upload(pData[i].pBaseColorImage));
		}

		if(pData[i].pNormalImage) {
			pMaterials[i].set_normal_texture(m_pNormalTextureStorage->upload(pData[i].pNormalImage));
		}

		if(pData[i].pMetallicRoughnessImage) {
			pMaterials[i].set_metallic_roughness_texture(m_pMetallicRoughnessTextureStorage->upload(pData[i].pMetallicRoughnessImage));
		}
	}

	glBindBuffer(GL_UNIFORM_BUFFER, m_materialBuffer);

	glBufferSubData(GL_UNIFORM_BUFFER, this->m_numMaterials * sizeof(Material), sizeof(Material) * numMaterials, pMaterials);

	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	free(pMaterials);

	this->m_numMaterials += numMaterials;

	m_pBaseColorTextureStorage->generate_mipmaps();
	m_pNormalTextureStorage->generate_mipmaps();
	m_pMetallicRoughnessTextureStorage->generate_mipmaps();

	return m_numMaterials - numMaterials;
}

void MaterialBuffer::use(drw::Pipeline *pPipeline) {
	unsigned int materialBufferIdx = glGetUniformBlockIndex(pPipeline->handle(), "materialBuffer");
	glUniformBlockBinding(pPipeline->handle(), materialBufferIdx, 0);

	m_pBaseColorTextureStorage->use(pPipeline, "baseColorTextureStorage", 0);
	m_pMetallicRoughnessTextureStorage->use(pPipeline, "pbrTextureStorage", 1);
	m_pNormalTextureStorage->use(pPipeline, "normalTextureStorage", 2);

	glBindBufferRange(GL_UNIFORM_BUFFER, 0, m_materialBuffer, 0, 64 * sizeof(Material));
}
