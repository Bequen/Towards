#pragma once

#include "Camera.hpp"
#include <cglm/vec3.h>

class SpaceShipCamera : public Camera {
private:
	vec2 lastMousePosition;
	
	vec3 eye;

	vec3 velocity;
	vec3 torque = {};

	mat4 rotation;
	mat4 translation;


	vec3 acceleration = {0.0f, 0.0f, 0.0f};
	vec3 torqueAcceleration = {};
	float deceleration = 0.1f;

	float rotationX, rotationY;

	float rotationSpeed = 1.0f;

	bool isAccelerating = false;

public:
	SpaceShipCamera(vec3 position);

	void on_tick() override;

	void handle_event(SDL_Event *pEvent) override;

	void info() override;
};
