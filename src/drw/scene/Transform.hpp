#pragma once

#include <cglm/mat4.h>

struct Transform {
	mat4 mat;

	Transform() {
		glm_mat4_identity(mat);
	}

	void apply(Transform *pTransform) {
		glm_mat4_mul(mat, pTransform->mat, mat);
	}
};
