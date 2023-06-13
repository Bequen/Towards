#pragma once

#include "Vertex.hpp"

class SceneData;

class MeshData {
private:
	SceneData *pScene;
	unsigned long indexOffset;

public:
	MeshData(SceneData *pScene, unsigned long indexOffset);
	~MeshData();
};
