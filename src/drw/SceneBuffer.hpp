#pragma once

#include "Vertex.hpp"
#include "Pipeline.hpp"
#include "SceneData.hpp"
#include "MaterialBuffer.hpp"

#include <assert.h>

class SceneShader {
public:
	drw::Pipeline *pPipeline;
	unsigned int numMaterials;
};

class SceneMaterial {
	
};

/* Holds multiple meshes in one buffer for optimization */
class SceneBuffer {
public:
	unsigned int vbo, ebo, vao;

	MaterialBuffer *m_pMaterialBuffer;

	Mesh *pMeshes;
	unsigned int numMeshes;

	Primitive *pPrimitives;
	unsigned int numPrimitives;

	SceneGraph *pGraph;

public:
	SceneBuffer(SceneData *pScene);

	void use(drw::Pipeline *pPipeline);

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
