#include "ArrayTextureStorage.hpp"

#include <math.h>
#include <string.h>
#include <glad.h>
#include <stdio.h>

ArrayTextureStorage::ArrayTextureStorage(Format format,
										 unsigned width,
										 unsigned height,
										 unsigned num) :
m_width(width), m_height(height), m_numTextures(num), m_format(format) {
	
	m_pBits = (char*)malloc(get_num_bytes());
	memset(m_pBits, 0, get_num_bytes());

	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D_ARRAY, m_texture);
	
	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, format.internalFormat, width, height, num, 0, format.format, format.type, NULL);
	
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_WRAP_T,GL_REPEAT);

}

unsigned ArrayTextureStorage::get_num_bytes() {
	return (int)ceil((float)m_numTextures / (sizeof(char) * 8));
}

TextureIdx ArrayTextureStorage::get_available_idx() {
	for(int i = 0; i < get_num_bytes(); i++) {
		for(int b = 0; b < 8; b++) {
			if(!(m_pBits[i] & (1 << b))) {
				m_pBits[i] |= (1 << b);
				return i * 8 + b;
			}
		}
	}

	return -1;
}

TextureIdx ArrayTextureStorage::upload(ImageData *pInfo) {
	TextureIdx result = get_available_idx();
	if(result == -1)
		return result;

	/* Uploads */
	glBindTexture(GL_TEXTURE_2D_ARRAY, m_texture);
	printf("Uploading %i texture %ix%i\n", result, pInfo->width, pInfo->height);
	glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, result, pInfo->width, pInfo->height, 1, m_format.format, m_format.type, pInfo->pData);

	return result;
}

void ArrayTextureStorage::generate_mipmaps() {
	glBindTexture(GL_TEXTURE_2D_ARRAY, m_texture);
	glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
}

void ArrayTextureStorage::unload(TextureIdx texture) {

}

void ArrayTextureStorage::use(drw::Pipeline *pPipeline, char *pName, unsigned idx) {
	glActiveTexture(GL_TEXTURE0 + idx);
	glBindTexture(GL_TEXTURE_2D_ARRAY, m_texture);

	pPipeline->push_i32(pName, idx);	
}
