#include "SceneData.hpp"

#include <stdlib.h>

SceneData::SceneData(unsigned long numNodes,
					 unsigned long numMeshes,
					 unsigned long numPrimitives,
					 unsigned long numVertices,
					 unsigned long numIndices, 
					 unsigned long numMaterials,
					 unsigned long numTextures) {
	pVertices = (Vertex*)malloc(sizeof(Vertex) * numVertices);
	this->maxVertices = numVertices;
	this->numVertices = 0;

	pIndices = (Index*)malloc(sizeof(Index) * numIndices);
	this->maxIndices = numIndices;
	this->numIndices = 0;

	pMeshes = (Mesh*)malloc(sizeof(Mesh) * numMeshes);
	this->maxMeshes = numMeshes;
	this->numMeshes = 0;

	pPrimitives = (Primitive*)malloc(sizeof(Primitive) * numPrimitives);
	this->maxPrimitives = numPrimitives;
	this->numPrimitives = 0;

	pGraph = new SceneGraph(numNodes);

	this->pMaterials = (MaterialData*)malloc(sizeof(MaterialData) * numMaterials);
	this->maxMaterials = numMaterials;
	this->numMaterials = 0;

	this->pTextures = (ImageData*)malloc(sizeof(ImageData) * numTextures);
	this->maxTextures = numTextures;
	this->numTextures = 0;

	/* pNodes = (SceneNode*)malloc(sizeof(SceneNode) * numNodes);
	pTransforms = (Transform*)malloc(sizeof(Transform) * numNodes);
	this->maxNodes = this->maxTransforms = numNodes;
	this->numNodes = this->numTransforms = 0; */
}

void SceneData::set_path(std::string path) {
	unsigned split = path.find_last_of("/\\");
	this->m_dir = path.substr(0, split + 1);
	this->m_filename = path.substr(split);
	printf("set path: %s\n", m_dir.c_str());
}

std::string SceneData::relative_path_of(std::string file) {
	return m_dir + file;
}

Vertex* SceneData::suballoc_vertices(unsigned long count) {
	Vertex *pResult = pVertices + numVertices;
	numVertices += count;
	return pResult;
}

Index* SceneData::suballoc_indices(unsigned long count) {
	Index *pResult = pIndices + numIndices;
	numIndices += count;
	return pResult;
}

Mesh *SceneData::suballoc_mesh() {
	return pMeshes + (numMeshes++);
}
