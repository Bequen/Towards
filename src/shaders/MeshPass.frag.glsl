#version 330 core

layout (location = 0) out vec4 outColor;
layout (location = 1) out vec4 outNormal;
layout (location = 2) out vec4 outPosition;

in VS_OUT {
	vec4 pos;
	vec4 norm;
	vec2 uv;
	mat3 tbn;
} fs_in;

const int NUM_MATERIALS = 64;
const int NUM_TRANSFORMS = 128;

struct Material {
	vec4 baseColor;
	int baseColorTexture;

	int normalTexture;
	int metallicRoughnessTexture;
	int fill;
};

uniform int materialIdx;

layout(std140) uniform materialBuffer {
	Material materials[NUM_MATERIALS];
};

uniform sampler2DArray baseColorTextureStorage;
uniform sampler2DArray pbrTextureStorage;
uniform sampler2DArray normalTextureStorage;

void main() {
	vec3 normal = normalize(texture(normalTextureStorage, vec3(fs_in.uv, materials[materialIdx].normalTexture)).rgb * 2.0 - 1.0);
	normal = normalize(fs_in.tbn * normal);

	outColor = vec4(texture(baseColorTextureStorage, vec3(fs_in.uv, materials[materialIdx].baseColorTexture)).rgb, 1.0f);
	outNormal = vec4(normal, 1.0); 
	outPosition = vec4(0.0f, 0.0f, 1.0f, 1.0f);
}
