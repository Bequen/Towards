#include "gltf.hpp"

#include "cglm/mat4.h"
#include "drw/SceneBuffer.hpp"
#include "drw/Vertex.hpp"
#include <cstring>

#define CGLTF_IMPLEMENTATION
#include <cgltf/cgltf.h>

#include <cglm/cglm.h>
#include <cglm/affine.h>
#include <cglm/quat.h>


void load_primitive(SceneData *pScene, cgltf_primitive *pPrimitive) {
	Vertex *pVertices = pScene->suballoc_vertices(pPrimitive->attributes[0].data->count);
	Index *pIndices = pScene->suballoc_indices(pPrimitive->indices->count);
	printf("Stride: %i\n", pPrimitive->indices->stride);
	for(int i = 0; i < pPrimitive->indices->count; i++) {
		pIndices[i] = *((Index*)((char*)pPrimitive->indices->buffer_view->buffer->data +
										 pPrimitive->indices->buffer_view->offset +
										 pPrimitive->indices->offset +
										 i * pPrimitive->indices->stride));
	}

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
				for(int i = 0; i < attr.data->count; i++) {
					float *pData = (float*)((char*)attr.data->buffer_view->buffer->data + 
											attr.data->buffer_view->offset +
											attr.data->offset +
											i * attr.data->stride);
					pVertices[i].uv[0] = pData[0];
					pVertices[i].uv[1] = pData[1];
				}
				break;
			case cgltf_attribute_type_tangent:
				for(int i = 0; i < attr.data->count; i++) {
					float *pData = (float*)((char*)attr.data->buffer_view->buffer->data + 
											attr.data->buffer_view->offset +
											attr.data->offset +
											i * attr.data->stride);
					pVertices[i].tangent[0] = pData[0];
					pVertices[i].tangent[1] = pData[1];
					pVertices[i].tangent[2] = pData[2];
				}
		}
	}
}

Transform make_node_transform(cgltf_node *pNode) {
	Transform result = Transform();
	if(pNode->has_matrix) {
		memcpy(result.mat, pNode->matrix, sizeof(mat4));
	} else {
		if(pNode->has_translation) {
			vec3 translation = {
				pNode->translation[0],
				pNode->translation[1],
				pNode->translation[2]
			};
			glm_translate(result.mat, translation);
		} 

		if(pNode->has_rotation) {
			vec4 rotation = {
				pNode->rotation[0],
				pNode->rotation[1],
				pNode->rotation[2],
				pNode->rotation[3],
			};
			glm_quat_rotate(result.mat, rotation, result.mat);
		}

		if(pNode->has_scale) {
			vec3 scale = {
				pNode->scale[0],
				pNode->scale[1],
				pNode->scale[2],
			};
			glm_scale(result.mat, scale);
		}
	}

	glm_mat4_copy(result.mat, result.mat);
	return result;
}

int load_mesh(SceneData *pScene, cgltf_data *pData, cgltf_mesh *pMesh) {
	for(int p = 0; p < pMesh->primitives_count; p++) {
		load_primitive(pScene, &pMesh->primitives[p]);

		pScene->push_primitive(pScene->numIndices - pMesh->primitives[p].indices->count,
								pMesh->primitives[p].indices->count,
								pScene->numVertices - pMesh->primitives[p].attributes[0].data->count,
								pMesh->primitives[p].material - pData->materials);
	}
	return pScene->push_mesh(pMesh->primitives_count);
}

int load_node(SceneData *pScene, cgltf_data *pData, cgltf_node *pNode) {
	/* push new node to tree */
	int meshIdx = pNode->mesh ? (int)(pNode->mesh - pData->meshes) : -1;
	char *pName = pNode->name;
	if(!pName) pName = "node";

	int nodeIdx = pScene->graph()->push(SceneNode(pName, pNode->children_count)
											.set_mesh(meshIdx != -1 ? pScene->mesh(meshIdx) : NULL),
										make_node_transform(pNode));

	/* load all the children */
	int degree = 1;
	for(int i = 0; i < pNode->children_count; i++) {
		degree += load_node(pScene, pData, pNode->children[i]);
	}

	/* set the correct degree (number of nodes) */
	pScene->graph()->node(nodeIdx)->set_degree(degree);

	return degree;
}

void load_material(SceneData *pScene, cgltf_material *pMaterial) {
	printf("Loading material %s\n", pMaterial->name);
	MaterialData materialData = MaterialData();

	if(pMaterial->has_pbr_metallic_roughness) {
		auto pbr = pMaterial->pbr_metallic_roughness;

		if(pbr.base_color_texture.texture != nullptr &&
		   pbr.base_color_texture.texture->image != nullptr) {
			ImageData *pBaseColor = new ImageData(strdup((char*)pScene->relative_path_of(pbr.base_color_texture.texture->image->uri).c_str()));
			printf("Texture: %s\n", pBaseColor->pPath);
			pBaseColor->load_file();
			pBaseColor->parse();

			materialData.pBaseColorImage = pBaseColor;
		}

		if(pbr.metallic_roughness_texture.texture &&
		   pbr.metallic_roughness_texture.texture->image) {
			ImageData *pMetallicRoughness = new ImageData(strdup((char*)pScene->relative_path_of(pbr.metallic_roughness_texture.texture->image->uri).c_str()));
			printf("Texture: %s\n", pMetallicRoughness->pPath);
			pMetallicRoughness->load_file();
			pMetallicRoughness->parse();

			materialData.pMetallicRoughnessImage = pMetallicRoughness;
		}
	}

	if(pMaterial->normal_texture.texture &&
	   pMaterial->normal_texture.texture->image) {
		ImageData *pNormal = new ImageData(strdup((char*)pScene->relative_path_of(pMaterial->normal_texture.texture->image->uri).c_str()));
		printf("Texture: %s\n", pNormal->pPath);
		pNormal->load_file();
		pNormal->parse();

		materialData.pNormalImage = pNormal;
	}

	pScene->push_material(materialData);
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

	SceneData *pResult = new SceneData(data->nodes_count + 1, data->meshes_count, 
									   numPrimitives, numVertices, numIndices,
									   data->materials_count, data->textures_count);
	pResult->set_path(path);

	/* Loads meshes to be able to reference them */
	for(int i = 0; i < data->meshes_count; i++) {
		load_mesh(pResult, data, &data->meshes[i]);
	}

	printf("loading %i materials\n", data->materials_count);
	for(int i = 0; i < data->materials_count; i++) {
		load_material(pResult, &data->materials[i]);
	}

	/* load nodes */
	pResult->graph()->push(SceneNode("root", data->scene->nodes_count), Transform());

	int degree = 1;
	for(int i = 0; i < data->scene->nodes_count; i++) {
		degree += load_node(pResult, data, data->scene->nodes[i]);
	}

	pResult->graph()->node(0)->degree = degree;
	pResult->graph()->apply_transform(pResult->graph()->node(0));

	return pResult;
}
