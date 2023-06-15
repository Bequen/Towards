#include "SpaceShipCamera.hpp"

#include <cglm/cglm.h>
#include <cglm/vec3.h>
#include <cglm/cam.h>
#include <cglm/mat4.h>
#include <cglm/quat.h>
#include <cglm/box.h>

#include <imgui.h>

SpaceShipCamera::SpaceShipCamera(vec3 position) {
	glm_vec3_zero(velocity);

	glm_vec3_copy(position, eye);
	glm_mat4_identity(view);

	glm_lookat(position, (vec3){0.0f, 0.0f, 0.0f}, (vec3){0.0f, 0.0f, 1.0f}, view);

	glm_perspective(glm_rad(60.0f), 1.0f, 0.1f, 100.0f, projection);
	
	glm_mat4_identity(translation);
	glm_translate(translation, position);

	glm_mat4_identity(rotation);
}

void SpaceShipCamera::enable() {
	isEnabled = true;
}


void SpaceShipCamera::disable() {
	isEnabled = false;
}

void SpaceShipCamera::on_tick() {
	vec3 front = {rotation[0][2], rotation[1][2], rotation[2][2]};
	glm_vec3_normalize(front);
	if(glm_vec3_dot(acceleration, acceleration) != 0.0f) {
		vec3 accel = {};
		glm_mat4_transpose(rotation);
		glm_mat4_mulv3(rotation, acceleration, 1.0, accel);
		glm_vec3_add(accel, velocity, velocity);
		glm_mat4_transpose(rotation);
	} else {
		/* vec3 decelerationVec = {};
		glm_vec3_sub((vec3){0.0f, 0.0f, 0.0f}, velocity, decelerationVec);
		float len = glm_vec3_dot(decelerationVec, decelerationVec);

		glm_vec3_normalize(decelerationVec);
		glm_vec3_scale(decelerationVec, fmax(deceleration, len), decelerationVec);

		glm_vec3_add(velocity, decelerationVec, velocity); */
		glm_vec3_lerp(velocity, (vec3){0.0f, 0.0f, 0.0f}, 0.1f, velocity);
	}

	glm_vec3_add(torque, torqueAcceleration, torque);


	vec4 rotZ = {}, rotX = {}, rotY = {};
	glm_quat(rotZ, glm_rad(torque[2]), front[0], front[1], front[2]);
	glm_quat(rotX, glm_rad(torque[0]), rotation[0][0], rotation[1][0], rotation[2][0]);
	glm_quat(rotY, glm_rad(torque[1]), rotation[0][1], rotation[1][1], rotation[2][1]);

	vec4 rot = {};
	glm_quat_mul(rotZ, rotX, rot);
	glm_quat_mul(rot, rotY, rot);

	glm_quat_rotate(rotation, rot, rotation);

	/* decrease the torque speed */
	glm_vec3_lerp(torque, (vec3){0.0f, 0.0f, 0.0f}, 0.5f, torque);

	glm_translate(translation, velocity);

	glm_mat4_mul(rotation, translation, view);
}

void SpaceShipCamera::handle_event(SDL_Event* pEvent) {	
	if(pEvent->type == SDL_MOUSEMOTION) {
		vec2 currentMousePosition = {(float)pEvent->motion.x, (float)pEvent->motion.y};
		vec2 motion;
		glm_vec2_sub(lastMousePosition, currentMousePosition, motion);

		glm_vec3_add(torque, (vec3){motion[1], motion[0], 0.0f}, torque);

		glm_vec2_copy(currentMousePosition, lastMousePosition);
	} else if(pEvent->type == SDL_KEYDOWN || pEvent->type == SDL_KEYUP) {
		if(pEvent->key.keysym.sym == SDLK_w) {
			acceleration[2] = pEvent->key.state ? 0.01f : 0.0f;
		} if(pEvent->key.keysym.sym == SDLK_s) {
			acceleration[2] = pEvent->key.state ? -0.01f : 0.0f;
		} if(pEvent->key.keysym.sym == SDLK_a) {
			acceleration[0] = pEvent->key.state ? 0.01f : 0.0f;
		} if(pEvent->key.keysym.sym == SDLK_d) {
			acceleration[0] = pEvent->key.state ? -0.01f : 0.0f;
		} if(pEvent->key.keysym.sym == SDLK_SPACE) {
			acceleration[1] = pEvent->key.state ? -0.01f : 0.0f;
		} if(pEvent->key.keysym.sym == SDLK_LCTRL) {
			acceleration[1] = pEvent->key.state ? 0.01f : 0.0f;
		} if(pEvent->key.keysym.sym == SDLK_e) {
			torqueAcceleration[2] = pEvent->key.state ? 1.0f : 0.0f;
		} if(pEvent->key.keysym.sym == SDLK_q) {
			torqueAcceleration[2] = pEvent->key.state ? -1.0f : 0.0f;
		}
	}
}

void SpaceShipCamera::info() {
	Camera::info();

	ImGui::InputFloat3("Acceleration", acceleration);
	ImGui::InputFloat3("Velocity", velocity);
	ImGui::InputFloat3("Torque", torque);

	ImGui::BeginTable("rotation_mat", 4);
	for(int y = 0; y < 4; y++) {
		ImGui::TableNextRow();
		for(int x = 0; x < 4; x++) {
			ImGui::PushID(y * 4 + x);
			ImGui::TableNextColumn();
			ImGui::InputFloat("val", &rotation[x][y]);
			ImGui::PopID();
		}
	}
	ImGui::EndTable();
}
