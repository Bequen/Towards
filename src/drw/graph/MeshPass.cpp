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

	for(int i = 0; i < pScene->numNodes; i++) {
		if(pScene->pNodes[i].meshIdx == -1)
			continue;

		pipeline.push_mat4("model", pScene->pTransforms[pScene->pNodes[i].transformIdx].mat);

		Mesh *pMesh = &pScene->pMeshes[pScene->pNodes[i].meshIdx];
		for(int p = 0; p < pMesh->numPrimitives; p++) {
			glDrawElementsBaseVertex(GL_TRIANGLES, pScene->pPrimitives[pMesh->primitiveIdx + p].count, GL_UNSIGNED_SHORT, (void*)(pScene->pPrimitives[pMesh->primitiveIdx + p].offset * 2), pScene->pPrimitives[pMesh->primitiveIdx + p].baseVertex);
		}
	}

	/* auto meshes = pScene->get_meshes();
	for(int i = 0; i < pScene->get_num_meshes(); i++) {
		glDrawElements(GL_TRIANGLES, meshes[i].count, GL_UNSIGNED_SHORT, (void*)meshes[i].offset);
	} */
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
