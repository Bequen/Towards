#pragma once

#include "EditorWindow.hpp"

#include "drw/SceneBuffer.hpp"

class SceneOutline : public EditorWindow {
private:
	SceneBuffer *pScene;
	SceneNode *pSelected = NULL;

	int draw_node(SceneNode *pNode);

public:
	SceneOutline(Editor *pEditor, SceneBuffer *pScene);

	void draw() override;
};
