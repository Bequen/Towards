#include "Editor.hpp"

#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>

static bool isImGuiInitialized = false;

void Editor::init_imgui() {
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	isImGuiInitialized = true;
}

Editor::Editor() {
	if(!isImGuiInitialized) {
		init_imgui();
	}
}



void Editor::begin() {
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	if(showDemoWindow)
		ImGui::ShowDemoWindow(&showDemoWindow);
}

void Editor::end() {
	// Rendering
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Editor::on_event(SDL_Event *pEvent) {
	ImGui_ImplSDL2_ProcessEvent(pEvent);
}
