#include "gltf.hpp"

#include "cglm/mat4.h"
#include "drw/SceneBuffer.hpp"
#include "drw/Vertex.hpp"

#define CGLTF_IMPLEMENTATION
#include <cgltf/cgltf.h>

#include <cglm/cglm.h>
#include <cglm/affine.h>
#include <cglm/quat.h>


void load_primitive(SceneData *pScene, cgltf_primitive *pPrimitive) {
	Vertex *pVertices = pScene->suballoc_vertices(pPrimitive->attributes[0].data->count);
	Index *pIndices = pScene->suballoc_indices(pPrimitive->indices->count);

	printf("stride: %i %i\n", pPrimitive->indices->offset, pPrimitive->indices->count);

	memcpy(pIndices, (char*)pPrimitive->indices->buffer_view->buffer->data + pPrimitive->indices->buffer_view->offset + pPrimitive->indices->offset, pPrimitive->indices->count * 2);

	for(int a = 0; a < pPrimitive->attributes_count; a++) {
		auto attr = pPrimitive->attributes[a];

		switch(attr.type) {
			case cgltf_attribute_type_position:
				for(int i = 0; i < attr.data->count; i++) {
					float *pData = (float*)((char*)attr.data->buffer_view->buffer->data + 
											attr.data->buffer_view->offset +
											attr.data->offset +
											i * attr.data->stride);
					pVertices[i].pos[0] = pData[0];
					pVertices[i].pos[1] = pData[1];
					pVertices[i].pos[2] = pData[2];
				}
				break;
			case cgltf_attribute_type_normal:
				for(int i = 0; i < attr.data->count; i++) {
					float *pData = (float*)((char*)attr.data->buffer_view->buffer->data + 
											attr.data->buffer_view->offset +
											attr.data->offset +
											i * attr.data->stride);
					pVertices[i].norm[0] = pData[0];
					pVertices[i].norm[1] = pData[1];
					pVertices[i].norm[2] = pData[2];
				}
				break;
			case cgltf_attribute_type_texcoord:
				break;
		}
	}
}

Transform make_node_transform(cgltf_node *pNode) {
	mat4 mat;
	if(pNode->has_matrix) {
		memcpy(mat, pNode->matrix, sizeof(mat4));
	} else {
		glm_mat4_identity(mat);
		if(pNode->has_translation) {
			glm_translate(mat, (vec3){pNode->translation[0], pNode->translation[1], pNode->translation[2]});
		} 

		if(pNode->has_rotation) {
			glm_quat_rotate(mat, (vec4){
								pNode->rotation[0],
								pNode->rotation[1],
								pNode->rotation[2],
								pNode->rotation[3]
							}, mat);
		}
		if(pNode->has_scale) {
			glm_scale(mat, (vec3){pNode->scale[0], pNode->scale[1], pNode->scale[2]});
		}
	}

	Transform result = Transform();
	glm_mat4_copy(mat, result.mat);
	return result;
}

int load_mesh(SceneData *pScene, cgltf_mesh *pMesh) {
	for(int p = 0; p < pMesh->primitives_count; p++) {
		load_primitive(pScene, &pMesh->primitives[p]);

		printf("pushing %i primitive: %i %i\n", pScene->numPrimitives,
			   pScene->numIndices - pMesh->primitives[p].indices->count, 
			   pMesh->primitives[p].indices->count);

		pScene->push_primitive(pScene->numIndices - pMesh->primitives[p].indices->count,
								pMesh->primitives[p].indices->count,
								pScene->numVertices - pMesh->primitives[p].attributes[0].data->count);
	}
	return pScene->push_mesh(pMesh->primitives_count);
}

int load_node(SceneData *pScene, cgltf_data *pData, Transform *pParent, cgltf_node *pNode) {
	auto transform = make_node_transform(pNode);
	transform.apply(pParent);
	auto transformIdx = pScene->push_transform(transform);

	/* get mesh index */
	int meshIdx = pNode->mesh ? (int)(pNode->mesh - pData->meshes) : -1;
	printf("Loading mesh %i\n", meshIdx);

	unsigned int nodeIdx = pScene->push_node({
						.pName = pNode->name,
						.meshIdx = meshIdx,	
						.transformIdx = transformIdx,
						.numChildren = (unsigned int)pNode->children_count
					  });

	int degree = 1;
	for(int i = 0; i < pNode->children_count; i++) {
		degree += load_node(pScene, pData, &pScene->pTransforms[transformIdx], pNode->children[i]);
	}

	pScene->pNodes[nodeIdx].degree = degree;

	return degree;
}

/* loads mesh to specified memory
 * Can load to GPU and CPU */
SceneData* io::gltf::load_scene(std::string path) {
	cgltf_options options = {};
	cgltf_data* data = nullptr;
	cgltf_result status = cgltf_parse_file(&options, path.c_str(), &data);

	if (status != cgltf_result_success) {
		cgltf_free(data);
		return NULL;
	}

	cgltf_load_buffers(&options, data, path.c_str());

	/* count the size for allocation */
	int numVertices = 0;
	int numIndices = 0;
	int numPrimitives = 0;

	for(int i = 0; i < data->meshes_count; i++) {
		numPrimitives += data->meshes[i].primitives_count;
		for(int p = 0; p < data->meshes[i].primitives_count; p++) {
			numIndices += data->meshes[i].primitives[p].indices->count;
			for(int a = 0; a < data->meshes[i].primitives[p].attributes_count; a++) {
				auto attr = data->meshes[i].primitives[p].attributes[a];
				
				numVertices += attr.data->count;
			}
		}
	}

	SceneData *pResult = new SceneData(data->nodes_count + 1, data->meshes_count, numPrimitives, numVertices, numIndices);

	for(int i = 0; i < data->meshes_count; i++) {
		load_mesh(pResult, &data->meshes[i]);
	}

	printf("numPrimitives: %i\n", numPrimitives);
	/* load nodes */
	printf("Loading %i nodes\n", data->scene->nodes_count);
	auto rootIdx = pResult->push_node({
							.pName = "root",
						   .meshIdx = -1,
						   .transformIdx = pResult->push_transform(Transform()),
						   .numChildren = (unsigned int)data->scene->nodes_count
					   });
	int degree = 1;
	for(int i = 0; i < data->scene->nodes_count; i++) {
		degree += load_node(pResult, data, &pResult->pTransforms[0], data->scene->nodes[i]);
	}

	pResult->pNodes[rootIdx].degree = degree;

	return pResult;
}
