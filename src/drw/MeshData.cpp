#include "MeshData.hpp"

#include <stdlib.h>
#include <cglm/cglm.h>
#include <cglm/vec3.h>

/* MeshData::MeshData() {

}

MeshData::~MeshData() {
	if(maxVertices != 0) {
		delete [] this->pVertices;
	}
	if(maxIndices != 0) {
		delete [] this->pIndices;
	}
}

void MeshData::set_vertex_data(Vertex *pVertices, unsigned int numVertices) {
	if(maxVertices != 0) {
		delete [] this->pVertices;
	}

	this->numVertices = numVertices;
	this->maxVertices = 0;
	this->pVertices = pVertices;
}

void MeshData::set_index_data(unsigned int *pIndices, unsigned int numIndices) {
	if(maxIndices != 0) {
		delete [] this->pIndices;
	}

	this->numIndices = numIndices;
	this->maxIndices = 0;
	this->pIndices = pIndices;
}

Vertex* MeshData::get_vertex_buffer_chunk(unsigned long numVertices) {
	if(this->numVertices + numVertices < this->maxVertices) {
		this->numVertices += numVertices;
		return this->pVertices + (this->numVertices - numVertices);
	} else {
		return NULL;
	}
}

Index* MeshData::get_index_buffer_chunk(unsigned long numIndices) {
	if(this->numIndices + numIndices < this->maxIndices) {
		this->numIndices += numIndices;
		return this->pIndices + (this->numIndices - numIndices);
	} else {
		return NULL;
	}
}

MeshData MeshData::from_grid(float resolution[2], float size[2]) {
	MeshData result = {};
	result.numIndices = 6 * resolution[0] * resolution[1];
	result.indiceSize = 4;
	result.maxIndices = result.numIndices;

	result.numVertices = (resolution[0]+1) * (resolution[1]+1);
	result.maxVertices = result.numVertices;

	result.pVertices = (Vertex*)malloc(sizeof(Vertex) * result.numVertices);
	result.pIndices = (unsigned int*)malloc(sizeof(unsigned int) * result.numIndices);

	int i = 0;
	for(int y = 0; y <= resolution[1]; y++) {
		for(int x = 0; x <= resolution[0]; x++) {
			result.pVertices[i++] = Vertex {
				.pos = {
					x * (size[0] / resolution[0]),
					y * (size[1] / resolution[1]),
					0.0f
				}, .norm = { 0.0f, 0.0f, 1.0f },
				.uv = {
					(float)(x % 2), (float)(y % 2)
				}
			};
		}
	}

	i = 0;
	for(int y = 0; y < resolution[1]; y++) {
		for(int x = 0; x < resolution[0]; x++) {
			result.pIndices[i++] = x + y * (resolution[0]+1);
			result.pIndices[i++] = (x + 1) + y * (resolution[0]+1);
			result.pIndices[i++] = x + (y + 1) * (resolution[0]+1);

			result.pIndices[i++] = (x+1) + y*(resolution[0]+1);
			result.pIndices[i++] = x + (y+1)*(resolution[0]+1);
			result.pIndices[i++] = (x+1) + (y+1)*(resolution[0]+1);
		}
	}

	return result;
} */
