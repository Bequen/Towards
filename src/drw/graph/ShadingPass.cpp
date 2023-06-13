#include "ShadingPass.hpp"
#include "io/path.hpp"
#include <glad.h>

ShadingPass ShadingPass::from(ShadingPassCreateInfo *pInfo) {
	ShadingPass result = {};

	result.pGbuffer = pInfo->pInput;
	result.fb = Framebuffer::screen();

	drw::PipelineBuildInfo pipelineInfo {
		.vertexShaderPath = io::path::shader("Quad.vert.glsl"),
		.fragmentShaderPath = io::path::shader("ShadingPass.frag.glsl")
	};

	result.pipeline = drw::Pipeline::from(pipelineInfo);

	result.pipeline.push_i32("gPosition", 0);
	result.pipeline.push_i32("gNormal", 1);
	result.pipeline.push_i32("gColor", 2);

	return result;
}

void ShadingPass::record() {

	fb.use();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	pipeline.use();
	pipeline.push_i32("gPosition", 2);
	pipeline.push_i32("gNormal", 1);
	pipeline.push_i32("gColor", 0);
	auto images = pGbuffer->get_images();
	for(int i = 0; i < images.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, images[i].get_handle());
	}

	glDrawArrays(GL_TRIANGLES, 0, 3);
}
