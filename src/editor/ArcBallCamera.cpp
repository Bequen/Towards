#include "ArcBallCamera.hpp"

#include <cglm/cglm.h>
#include <cglm/vec3.h>
#include <cglm/cam.h>

ArcBallCamera::ArcBallCamera(vec3 center, vec3 eye) {
	glm_lookat(eye, center, (vec3){0.0f, 0.0f, 1.0f}, view);
	glm_perspective(glm_rad(60.0f), 1.0f, 0.01f, 100.0f, projection);
}

void ArcBallCamera::handle_event(SDL_Event *pEvent) {
	if(pEvent->type == SDL_MOUSEBUTTONDOWN) {
		if(pEvent->button.button == SDL_BUTTON_MIDDLE) {
			isRotating = true;
		}
	} else if(pEvent->type == SDL_MOUSEMOTION) {
		if(isRotating) {
			glm_rotate(view, pEvent->motion.x, (vec3){0.0f, 0.0f, 1.0f});
		}
	}
}
