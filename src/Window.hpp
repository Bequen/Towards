#pragma once

#include <glad.h>

#include <SDL.h>

#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#include <SDL_opengl.h>
#endif

/* Universal interface for windows */
class Window {
private:
	SDL_Window *pWindow;
	SDL_GLContext glContext;

	static void init_sdl();

public:
	static Window open();

	void swap_buffers();

	SDL_GLContext get_context() {
		return glContext;
	}

	SDL_Window *get_window_handle() {
		return pWindow;
	}

	~Window();
};
