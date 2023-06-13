#pragma once

#include "editor/Editor.hpp"

class EditorWindow {
protected:
	Editor *pEditor;

public:
	EditorWindow(Editor *pEditor) :
	pEditor(pEditor) {

	}

	virtual void draw() = 0;
};
