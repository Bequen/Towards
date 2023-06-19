// Dear ImGui: standalone example application for SDL2 + OpenGL
// (SDL is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs


#include "SDL_mouse.h"
#include "Window.hpp"
#include "drw/graph/GBufferStorage.hpp"
#include "drw/graph/MeshPass.hpp"
#include "drw/graph/ShadingPass.hpp"
#include "editor/Editor.hpp"
#include "editor/SceneOutline.hpp"
#include "editor/NodeInfo.hpp"
#include "io/gltf.hpp"

#define _XOPEN_SOURCE 600
#include <glad.h>
#include <time.h>
#include <stdlib.h>
#include "editor/ArcBallCamera.hpp"
#include "editor/SpaceShipCamera.hpp"
#include "drw/MeshBuffer.hpp"
#include "drw/Pipeline.hpp"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include "io/path.hpp"
#include <stdio.h>
#include <SDL.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#include <SDL_opengl.h>
#endif

#include <cglm/cglm.h>
#include <cglm/cam.h>
#include <cglm/vec3.h>
#include <cglm/mat4.h>
#include <unistd.h>
#include <err.h>

#include <functional>

int main(int argc, char** argv)
{
	if(argc == 1) {
		printf("Please input path to glTF file to render.\n");
		return 1;
	}

	io::path::setup_exe_path(argv[0]);

    // Setup SDL
	Window window = Window::open();
	Editor editor = Editor();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForOpenGL(window.get_window_handle(), window.get_context());
    ImGui_ImplOpenGL3_Init("#version 330");
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	SpaceShipCamera camera = SpaceShipCamera((vec3){.0f, .0f, .0f});

	bool done = false;
	bool isEditorFocused = true;
	
	/* load mesh */
	auto scene = io::gltf::load_scene(argv[1]);
	if(scene == NULL) {
		err(1, "Failed to load gltf file");
	}

	/* setup rendering pipeline */
	GBufferStorage gbuffer = GBufferStorage::from(1000, 1000);

	MeshPassCreateInfo meshPassInfo = {
		.pStorage = &gbuffer,
		.pCamera = &camera
	};
	MeshPass meshpass = MeshPass::from(&meshPassInfo);

	SceneBuffer sceneBuffer = SceneBuffer(scene);
	SceneOutline sceneOutline = SceneOutline(&editor, &sceneBuffer);
	NodeInfo nodeInfo = NodeInfo(&editor);

	ShadingPassCreateInfo shadingPassInfo = {
		.pInput = &gbuffer
	};
	ShadingPass shadingpass = ShadingPass::from(&shadingPassInfo);

	struct timespec start, end;
    while (!done)
    {
		clock_gettime(CLOCK_MONOTONIC, &start);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		glViewport(0, 0, 1000, 1000);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
			editor.on_event(&event);
            if (event.type == SDL_QUIT)
                done = true;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE &&
				event.window.windowID == SDL_GetWindowID(window.get_window_handle()))
                done = true;
			if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
				isEditorFocused = !isEditorFocused;
			}

			if(!isEditorFocused)
				camera.handle_event(&event);
        }

		editor.begin();

		camera.on_tick();

		meshpass.begin();
		meshpass.draw_scene(&sceneBuffer);
		meshpass.end();

		shadingpass.record();

		clock_gettime(CLOCK_MONOTONIC_RAW, &end);
		uint64_t delta_us = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_nsec - start.tv_nsec) / 1000;

		sceneOutline.draw();
		nodeInfo.draw();

		editor.end();

		window.swap_buffers();
    }

	// Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    return 0;
}
