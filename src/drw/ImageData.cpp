#include "ImageData.hpp"

#define STB_IMAGE_IMPLEMENTATION 
#include "stb_image.h"

ImageData::ImageData() :
numChannels(0), stride(0), width(0), height(0), pData(nullptr), isParsed(false) {

}

/* ImageData::ImageData(unsigned numChannels, unsigned stride, 
					 unsigned width, unsigned height,
					 char *pPath, void *pData) :
numChannels(numChannels), stride(stride), width(width), height(height), pPath(pPath), pData(pData) {

} */

ImageData::ImageData(char *pPath) : ImageData() {
	this->pPath = pPath;
}

void ImageData::load_file() {
	/* if(!pPath)
		return;

	FILE *f = fopen(pPath, "rb");
	if(!f)
		return;

	fseek(f, 0, SEEK_END);
	size = ftell(f);
	printf("Image of size: %u\n", size);
	fseek(f, 0, SEEK_SET);

	pData = (char*)malloc(size + 1);
	((char*)pData)[size] = 0;
	
	fread(pData, 1, size, f);

	fclose(f); */
}

void ImageData::parse() {
	if(!isParsed && pData == nullptr) {
		/* void *pData = stbi_load_from_memory((const stbi_uc*)pData, (int)size,
											(int*)&width, (int*)&height, 
											(int*)&numChannels, 0); */
		
		printf("opening: %s\n", pPath);
		this->pData = stbi_load(pPath, &width, &height, &numChannels, 4);
		isParsed = true;

		//free(this->pData);

		//this->pData = pData;
	}
	if(!this->pData) {
		printf("Failed loading texture %s\n%s\n", pPath, stbi_failure_reason());
	} else {
		printf("Loaded texture: %ix%i\n", width, height);
	}
}
