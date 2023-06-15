#include "MeshPass.hpp"
#include "cglm/mat4.h"
#include "drw/SceneBuffer.hpp"
#include "io/path.hpp"

#include <glad.h>

MeshPass MeshPass::from(MeshPassCreateInfo *pInfo) {
	MeshPass result = {};
	result.fb = Framebuffer::to(pInfo->pStorage);
	result.pCamera = pInfo->pCamera;

	drw::PipelineBuildInfo pipelineInfo {
		.vertexShaderPath = io::path::shader("MeshPass.vert.glsl"),
		.fragmentShaderPath = io::path::shader("MeshPass.frag.glsl")
	};
	result.pipeline = drw::Pipeline::from(pipelineInfo);

	glEnable(GL_DEPTH_TEST);

	return result;
}

void MeshPass::draw_scene(SceneBuffer *pScene) {
	
	pScene->use();

	for(int i = 0; i < pScene->pGraph->num_nodes(); i++) {
		auto node = pScene->graph()->node(i);
		if(node->mesh() != NULL) {
			pipeline.push_mat4("model", node->transform()->mat);

			for(int p = 0; p < node->mesh()->numPrimitives; p++) {
				glDrawElementsBaseVertex(GL_TRIANGLES, pScene->pPrimitives[node->mesh()->primitiveIdx + p].count, 
										 GL_UNSIGNED_SHORT, (void*)(pScene->pPrimitives[node->mesh()->primitiveIdx + p].offset * 2),
										 pScene->pPrimitives[node->mesh()->primitiveIdx + p].baseVertex);
			}
		}
	}
}

void MeshPass::begin() {
	fb.use();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

	mat4 cameraTransform;
	pCamera->get_transform(cameraTransform);

	mat4 model;
	glm_mat4_identity(model);

	pipeline.use();
	pipeline.push_mat4("cameraTransform", cameraTransform);
	pipeline.push_mat4("model", model);
}

void MeshPass::end() {

}
