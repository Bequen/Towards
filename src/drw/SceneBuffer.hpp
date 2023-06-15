#pragma once

#include "Vertex.hpp"
#include "Pipeline.hpp"
#include "SceneData.hpp"

#include <assert.h>

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

	SceneGraph *pGraph;

public:
	SceneBuffer(SceneData *pScene);

	void use();

	SceneGraph *graph() {
		return pGraph;
	}

	/* returns the primitiveIdxth primitive of mesh at meshIdx  */
	Primitive *mesh_primitive(Mesh *pMesh, unsigned int primitiveIdx) {
		assert(pMesh && primitiveIdx >= 0 && primitiveIdx < pMesh->numPrimitives);

		return pPrimitives + (pMesh->primitiveIdx + primitiveIdx);
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
