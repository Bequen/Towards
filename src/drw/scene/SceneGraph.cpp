#include "SceneGraph.hpp"


SceneNode::SceneNode(char *pName, unsigned int numChildren) :
	pName(pName), numChildren(numChildren), m_pMesh(nullptr), m_pTransform(nullptr), degree(1) {

}

SceneNode& SceneNode::set_degree(unsigned int degree) {
	this->degree = degree;
	return *this;
}


SceneNode& SceneNode::set_mesh(Mesh *pMesh) {
	this->m_pMesh = pMesh;
	return *this;
}

Mesh *SceneNode::mesh() {
	return m_pMesh;
}


SceneNode& SceneNode::set_transform(Transform *pTransform) {
	this->m_pTransform = pTransform;
	return *this;
}

Transform *SceneNode::transform() {
	return m_pTransform;
}



SceneGraph::SceneGraph(unsigned int numNodes) :
nodes(std::vector<SceneNode>()) {
	m_pTransforms = (Transform*)malloc(sizeof(Transform) * numNodes);
	numTransforms = 0;
	maxTransforms = numNodes;
}

unsigned int SceneGraph::push(SceneNode node, Transform transform) {
	// this->transforms.push_back(transform);
	m_pTransforms[numTransforms++] = transform;
	node.set_transform(m_pTransforms + numTransforms - 1);
	this->nodes.push_back(node);
	return numTransforms - 1; 
}

SceneNode *SceneGraph::get_child(SceneNode *pNode, unsigned int idx) {
	return pNode + idx;
}

/* TODO: Move numChildren to transform (Unity does have it this way and it makes sense, every node needs it and it mainly transform uses it ) */
void SceneGraph::apply_transform(SceneNode *pNode) {
	unsigned int offset = 1;
	for(int i = 0; i < pNode->numChildren; i++) {
		SceneNode *pChild = pNode + offset;
		pChild->transform()->apply(pNode->transform());

		apply_transform(pChild);
		offset += pChild->degree;
	}
}
