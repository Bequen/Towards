#pragma once

#include "Vertex.hpp"
#include "drw/scene/Transform.hpp"

#include <cglm/mat4.h>
#include <stdio.h>

struct Mesh {
	unsigned int primitiveIdx;
	unsigned int numPrimitives;
};

struct Primitive {
	unsigned int offset;
	unsigned int count;
	unsigned int baseVertex;
};

struct SceneNode {
	char *pName;

	int meshIdx;
	unsigned int transformIdx;

	unsigned int numChildren;
	unsigned int degree; // number of nodes in subtree
};

/* Holds mesh data. There should not be any other structure holding actual data. They should all just be pointers. */
/* TODO: Turn pointers into LinearAllocators */
class SceneData {
public:
	/* Information about meshes */
	Vertex *pVertices;
	unsigned long maxVertices;
	unsigned long numVertices;

	Index *pIndices;
	unsigned long maxIndices;
	unsigned long numIndices;

	/* How to interpret the vertex and index data */
	Mesh *pMeshes;
	unsigned long maxMeshes;
	unsigned long numMeshes;

	Primitive *pPrimitives;
	unsigned long maxPrimitives;
	unsigned int numPrimitives;

	/* How and where draw these meshes */
	SceneNode *pNodes;
	unsigned long maxNodes;
	unsigned long numNodes;

	Transform *pTransforms;
	unsigned long maxTransforms;
	unsigned long numTransforms;

public:
	Vertex *get_vertices() {
		return pVertices;
	}
	unsigned long get_num_vertices() {
		return numVertices;
	}

	Index *get_indices() {
		return pIndices;
	}
	unsigned long get_num_indices() {
		return numIndices;
	}

	Mesh *get_meshes() {
		return pMeshes;
	}
	unsigned long get_num_meshes() {
		return numMeshes;
	}

	SceneNode *get_nodes() { return pNodes; }
	unsigned long get_num_nodes() { return numNodes; }

	/*
	 * Allocates scene data with numVertices and numIndices.
	 */
	SceneData(unsigned long numNodes,
			  unsigned long numMeshes,
			  unsigned long numPrimitives,
			  unsigned long numVertices,
			  unsigned long numIndices);

	Vertex *suballoc_vertices(unsigned long count);

	Index *suballoc_indices(unsigned long count);

	Mesh *suballoc_mesh();

	int push_mesh(unsigned int numPrimitives) {
		pMeshes[numMeshes++] = {
			.primitiveIdx = this->numPrimitives - numPrimitives,
			.numPrimitives = numPrimitives
		};

		return numMeshes - 1;
	}

	void push_primitive(unsigned int indexOffset, unsigned int indexCount, unsigned int baseVertex) {
		pPrimitives[numPrimitives++] = {
			.offset = indexOffset,
			.count = indexCount,
			.baseVertex = baseVertex
		};
	}

	unsigned int push_node(SceneNode node) {
		pNodes[numNodes++] = node;
		return numNodes - 1;
	}

	unsigned int push_transform(Transform transform) {
		printf("%i\n", numTransforms);
		fflush(stdout);
		pTransforms[numTransforms++] = transform;
		return numTransforms - 1;
	}
};
