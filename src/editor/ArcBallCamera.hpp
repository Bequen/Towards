#pragma once

#include "Camera.hpp"
#include <cglm/vec3.h>

class ArcBallCamera : public Camera {
private:
	bool isRotating = false;
	bool isPanning = false;
	bool isZooming = false;

	vec2 lastMousePosition;
	vec3 center;
	vec3 eye;

	float zoom;

	float rotationSpeed = 1.0f;
	float panningSpeed = 0.1f;
	
public:
	ArcBallCamera(vec3 center, vec3 eye);

	void handle_event(SDL_Event *pEvent) override;
};
