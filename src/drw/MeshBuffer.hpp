#pragma once

#include <cglm/vec2.h>

#include "MeshData.hpp"

namespace drw {
class MeshBuffer {
private:
	unsigned int vao, vbo, ebo;
	unsigned int numIndices = 0;
	unsigned int numVertices = 0;

	unsigned int numDrawIndices = 0;
	
	void init_buffers();

public:
	static MeshBuffer from(MeshData& data);
	
	unsigned int get_num_indices();

	void use();
};
}
