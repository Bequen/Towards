#version 330 core

layout (location = 0) out vec4 outColor;
layout (location = 1) out vec4 outNormal;
layout (location = 2) out vec4 outPosition;

in VS_OUT {
	vec4 pos;
	vec4 norm;
	vec2 uv;
} fs_in;

void main() {
	outColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	outNormal = fs_in.norm;
	outPosition = vec4(0.0f, 0.0f, 1.0f, 1.0f);
}
