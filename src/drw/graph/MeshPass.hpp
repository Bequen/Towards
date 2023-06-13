#pragma once

#include "GBufferStorage.hpp"
#include "drw/Framebuffer.hpp"
#include "drw/Pipeline.hpp"
#include "editor/Camera.hpp"
#include "drw/SceneBuffer.hpp"

struct MeshPassCreateInfo {
	GBufferStorage *pStorage;
	Camera *pCamera;
};

struct DrawInfo {

};

/* Outputs to gbuffer */
class MeshPass {
public:
	Framebuffer fb;
	drw::Pipeline pipeline;

	Camera *pCamera;

public:
	static MeshPass make();
	static MeshPass from(MeshPassCreateInfo *pInfo);

	MeshPass& set_camera(Camera *pCamera);
	MeshPass& set_draw_info(DrawInfo *pInfo);

	void begin();

	void draw_scene(SceneBuffer *pScene);

	void end();
};
