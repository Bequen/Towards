#pragma once

#include "Vertex.hpp"
#include "Pipeline.hpp"
#include "SceneData.hpp"

class SceneShader {
public:
	drw::Pipeline *pPipeline;
	unsigned int numMaterials;
};

class SceneMaterial {
	
};

class MaterialBuffer {
public:

};

/* Holds multiple meshes in one buffer for optimization */
class SceneBuffer {
public:
	unsigned int vbo, ebo, vao;

	Mesh *pMeshes;
	unsigned int numMeshes;

	Primitive *pPrimitives;
	unsigned int numPrimitives;

	Transform *pTransforms;
	unsigned int numTransforms;

	SceneNode *pNodes;
	unsigned int numNodes;

public:
	SceneBuffer(SceneData *pScene);

	void use();

	/* returns the primitiveIdxth primitive of mesh at meshIdx  */
	Primitive *mesh_primitive(unsigned int meshIdx, unsigned int primitiveIdx) {
		if(pMeshes[meshIdx].numPrimitives > primitiveIdx)
			return pPrimitives + (pMeshes[meshIdx].primitiveIdx + primitiveIdx);
		return NULL;
	}

	Mesh *mesh(unsigned int idx) {
		return &pMeshes[idx];
	}

	unsigned int get_num_meshes() {
		return numMeshes;
	}
	Mesh *get_meshes() {
		return pMeshes;
	}
};
