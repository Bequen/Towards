#pragma once

#include "editor/Editor.hpp"
#include "editor/EditorWindow.hpp"

class NodeInfo : public EditorWindow {
private:
	void draw_transform_info();
	void draw_mesh_info();

public:
	NodeInfo(Editor *pEditor);

	void draw() override;
};
