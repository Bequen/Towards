#version 330 core

out vec4 outColor;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gColor;

in vec2 outUV;

void main() {
	vec3 col = texture(gColor, outUV).rgb;
	vec3 norm = texture(gNormal, outUV).rgb;

	vec3 f = mix(vec3(0.0f), col, max(0.0, dot(normalize(vec3(-1.0f, 1.0f, -1.0f)), norm)));
	outColor = vec4(f, 1.0f);
	// outColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
}
