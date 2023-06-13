#include "NodeInfo.hpp"

#include <imgui.h>

NodeInfo::NodeInfo(Editor *pEditor) : EditorWindow(pEditor) {

}

void NodeInfo::draw_mesh_info() {
	ImGui::Text("Mesh Info");
	static ImGuiTableFlags flags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;

	auto selected = pEditor->get_selected_node();
	auto scene = pEditor->get_selected_scene();

	if(selected->meshIdx == -1) {
		return;
	}

	for(int i = 0; i < scene->mesh(selected->meshIdx)->numPrimitives; i++) {
		ImGui::Text("Primitive %i", i);
		auto primitive = scene->mesh_primitive(selected->meshIdx, i);

		if(ImGui::BeginTable("selected_node_info", 2, flags)) {
			ImGui::TableNextRow();

			ImGui::TableNextColumn();
			ImGui::Text("Index count");
			ImGui::TableNextColumn();
			ImGui::Text("%i", primitive->count);

			ImGui::TableNextRow();

			ImGui::TableNextColumn();
			ImGui::Text("Index offset");
			ImGui::TableNextColumn();
			ImGui::Text("%i", primitive->offset);


			ImGui::TableNextRow();

			ImGui::TableNextColumn();
			ImGui::Text("Base vertex");
			ImGui::TableNextColumn();
			ImGui::Text("%i", primitive->baseVertex);


			ImGui::EndTable();
		}
	}
}

void NodeInfo::draw() {
	ImGui::Begin("Node Info");

	if(pEditor->get_selected_node() != NULL) {
		/* mesh */
		draw_mesh_info();
	}

	ImGui::End();
}

