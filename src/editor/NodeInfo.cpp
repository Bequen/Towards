#include "NodeInfo.hpp"

#include <imgui.h>

NodeInfo::NodeInfo(Editor *pEditor) : EditorWindow(pEditor) {

}

void NodeInfo::draw_transform_info() {
	ImGui::Text("Transform");
	auto selected = pEditor->get_selected_node();

	static ImGuiTableFlags flags =  ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;

	if(ImGui::BeginTable("transform", 4, flags)) {
		for(int y = 0; y < 4; y++) {
			ImGui::TableNextRow();
			ImGui::PushID(y);
			for(int x = 0; x < 4; x++) {
				ImGui::TableNextColumn();

				ImGui::PushID(x);
				ImGui::InputFloat("##transform-float", &selected->transform()->mat[x][y]);
				ImGui::PopID();
			}
			ImGui::PopID();
		}

		ImGui::EndTable();
	}
}

void NodeInfo::draw_mesh_info() {
	ImGui::Text("Mesh Info");
	static ImGuiTableFlags flags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;

	auto selected = pEditor->get_selected_node();
	auto scene = pEditor->get_selected_scene();

	if(selected->mesh() == nullptr) {
		return;
	}

	for(int i = 0; i < selected->mesh()->numPrimitives; i++) {
		ImGui::Text("Primitive %i", i);
		
		auto primitive = scene->mesh_primitive(selected->mesh(), i);
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
		draw_transform_info();
		draw_mesh_info();
	}

	ImGui::End();
}

