#include "SceneData.hpp"

#include <stdlib.h>

SceneData::SceneData(unsigned long numNodes,
					 unsigned long numMeshes,
					 unsigned long numPrimitives,
					 unsigned long numVertices,
					 unsigned long numIndices) {
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

	pNodes = (SceneNode*)malloc(sizeof(SceneNode) * numNodes);
	pTransforms = (Transform*)malloc(sizeof(Transform) * numNodes);
	this->maxNodes = this->maxTransforms = numNodes;
	this->numNodes = this->numTransforms = 0;
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
