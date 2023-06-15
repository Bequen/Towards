#pragma once

#include <vector>

#include "Transform.hpp"

struct Mesh;

struct SceneNode {
private:
	Mesh *m_pMesh = NULL;
	Transform *m_pTransform = NULL;

public:
	char *pName = "";

	unsigned int numChildren = 0;
	unsigned int degree = 1; // number of nodes in subtree

public:
	SceneNode() :
	m_pMesh(nullptr) {

	}
	SceneNode(char *pName, unsigned int numChildren);

	SceneNode& set_degree(unsigned int value);
	SceneNode& set_mesh(Mesh *pMesh);
	SceneNode& set_transform(Transform *pTransform);

	Transform *transform();
	Mesh *mesh();
};

class SceneGraph {
private:
	/* TODO: create specific fixed breath allocated tree,
	 * therefore there can be iterator
	 * map or apply */
	std::vector<SceneNode> nodes;
	
	Transform *m_pTransforms;
	unsigned int maxTransforms;
	unsigned int numTransforms;

public:
	SceneGraph(unsigned int numNodes);

	SceneNode* node(int idx) {
		return nodes.data() + idx;
	}

	unsigned int num_nodes() { return nodes.size(); }

	/* Pushes the node and it's transform to the queue and applies parent transform automatically */
	unsigned int push(SceneNode node, Transform transform);

	SceneNode *get_child(SceneNode *pNode, unsigned int idx);

	void apply_transform(SceneNode *pNode);
};
