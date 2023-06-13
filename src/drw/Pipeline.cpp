#include "Pipeline.hpp"

#include <stdio.h>
#include <glad.h>

namespace drw {
const char* Pipeline::read_file(std::string path, size_t *pOutSize) {
	FILE *pFile = fopen(path.c_str(), "r");
	if(!pFile) {
		return nullptr;
	}
	
	fseek(pFile, 0, SEEK_END);
	size_t size = ftell(pFile);
	fseek(pFile, 0, SEEK_SET);

	char *pBuffer = new char[size];
	fread(pBuffer, size, 1, pFile);

	if(pOutSize)
		*pOutSize = size;

	fclose(pFile);

	return pBuffer;
}

Shader Pipeline::create_shader(PipelineShaderCreateInfo &info) {
	Shader stage = glCreateShader(info.stage);
	glShaderSource(stage, 1, &info.pSource, (int*)&info.size);
	glCompileShader(stage);	

	int success;
	char infoLog[512];
	glGetShaderiv(stage, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(stage, 512, NULL, infoLog);
		printf("Shader compile error: %s\n", infoLog);
	}

	return stage;
}

Pipeline Pipeline::from(PipelineBuildInfo &info) {
	Pipeline result = {};

	PipelineShaderCreateInfo vertexInfo= {
		.stage = PIPELINE_STAGE_VERTEX
	};
	vertexInfo.pSource = result.read_file(info.vertexShaderPath, &vertexInfo.size);

	PipelineShaderCreateInfo fragmentInfo = {
		.stage = PIPELINE_STAGE_FRAGMENT
	};
	fragmentInfo.pSource = result.read_file(info.fragmentShaderPath, &fragmentInfo.size);
	

	auto vertex = result.create_shader(vertexInfo);
	auto fragment = result.create_shader(fragmentInfo);

	result.program = glCreateProgram();
	glAttachShader(result.program, vertex);
	glAttachShader(result.program, fragment);

	glLinkProgram(result.program);
	int success = 0;
	char infoLog[512];
	glGetProgramiv(result.program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(result.program, 512, NULL, infoLog);
		printf("Shader linking failed: %s\n", infoLog);
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);

	delete [] vertexInfo.pSource;
	delete [] fragmentInfo.pSource;

	return result;
}

void Pipeline::use() {
	glUseProgram(program);
}

void Pipeline::push_mat4(std::string name, void *pData) {
	glUniformMatrix4fv(glGetUniformLocation(program, name.c_str()), 1, false, (float*)pData);
}

void Pipeline::push_i32(std::string name, int value) {
	glUniform1i(glGetUniformLocation(program, name.c_str()), value);
}
}
