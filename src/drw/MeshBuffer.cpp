#include "MeshBuffer.hpp"

#include <glad.h>
#include <cglm/vec3.h>

namespace drw {
void MeshBuffer::init_buffers() {
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);
}

MeshBuffer MeshBuffer::from(MeshData &data) {
}

void MeshBuffer::use() {
	glBindVertexArray(vao);
}

unsigned int MeshBuffer::get_num_indices() {
	return numDrawIndices;
}

}
