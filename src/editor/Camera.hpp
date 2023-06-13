#pragma once

#include <cglm/mat4.h>
#include <SDL.h>

class Camera {
protected:
	mat4 view;
	mat4 projection;

public:
	virtual void handle_event(SDL_Event *pEvent) = 0;

	virtual void info() {

	}

	virtual void on_tick() {

	}

	mat4& get_view() {
		return view;
	}

	mat4& get_projection() {
		return projection;
	}

	void get_transform(mat4 output) {
		glm_mat4_mul(projection, view, output);
	}
};
