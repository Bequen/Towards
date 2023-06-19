#version 330 core

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNorm;
layout(location = 2) in vec3 vTangent;
layout(location = 3) in vec2 vCoords;

out VS_OUT {
	vec4 pos;
	vec4 norm;
	vec2 uv;

	mat3 tbn;
} vs_out;

uniform mat4 model;
uniform mat4 cameraTransform;

void main() {
	vs_out.pos = model * vec4(vPos, 1.0f);
	gl_Position = cameraTransform * vs_out.pos;

	mat3 normalMatrix = transpose(inverse(mat3(model)));
	vs_out.norm = vec4(normalMatrix * vNorm, 1.0);

	vec3 bitangent = cross(vNorm, vTangent);
	mat3 TBN = mat3(vTangent, bitangent, vNorm);
	vs_out.tbn = TBN;

	vs_out.uv = vCoords;
}
