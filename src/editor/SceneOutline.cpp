#include "SceneOutline.hpp"

#include <imgui.h>

SceneOutline::SceneOutline(Editor *pEditor, SceneBuffer *pBuffer) :
EditorWindow(pEditor), pScene(pBuffer) {
	
}


int SceneOutline::draw_node(SceneNode *pNode) {
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;

	if(pNode == pEditor->get_selected_node()) {
		flags |= ImGuiTreeNodeFlags_Selected;
	}

	if(pNode->numChildren == 0) {
		flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
	}

	bool isOpen = ImGui::TreeNodeEx(pNode->pName, flags, pNode->pName);

	if(ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
		pEditor->select_node(pScene, pNode);
	}

	if(pNode->numChildren > 0 && isOpen) {
		int numChildren = 1;

		for(int i = 0; i < pNode->numChildren; i++) {
			numChildren += draw_node(pNode + numChildren);
		}
		ImGui::TreePop();
	}

	return pNode->degree;
}

void SceneOutline::draw() {
	ImGui::Begin("Scene Outline");
	draw_node(pScene->graph()->node(0));
	ImGui::End();
}
