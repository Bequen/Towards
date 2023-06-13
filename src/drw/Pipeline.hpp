#pragma once

#include <string>
#include <cstddef>

namespace drw {
struct PipelineBuildInfo {
	std::string vertexShaderPath;
	std::string fragmentShaderPath;
};

enum PipelineStage {
	PIPELINE_STAGE_VERTEX = 0x8B31,
	PIPELINE_STAGE_FRAGMENT = 0x8B30
};

struct PipelineShaderCreateInfo {
	PipelineStage stage;

	const char *pSource;
	size_t size;
};

typedef int Shader;

class Pipeline {
private:
	unsigned int program;

	const char* read_file(std::string path, size_t *pOutSize);

	Shader create_shader(PipelineShaderCreateInfo& info);

public:
	static Pipeline from(PipelineBuildInfo& info);

	void use();
	

	/* push constants */
	void push_mat4(std::string name, void *pData);
	void push_i32(std::string name, int value);
};
}
