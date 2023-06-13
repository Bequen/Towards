#pragma once

#include "drw/SceneBuffer.hpp"
#include "drw/SceneData.hpp"
#include <SDL.h>

class Editor {
private:
	bool showDemoWindow = true;

	SceneBuffer *m_pSelectedScene = NULL;
	SceneNode *m_pSelected = NULL;

	void init_imgui();

public:
	Editor();

	void begin();
	void end();

	void on_event(SDL_Event* pEvent);

	SceneBuffer *get_selected_scene() { return m_pSelectedScene; }
	SceneNode *get_selected_node() { return m_pSelected; }

	void select_node(SceneBuffer *pScene, SceneNode *pNode) { 
		this->m_pSelectedScene = pScene;
		this->m_pSelected = pNode; 
	}
};
