#pragma once

#include "drw/Pipeline.hpp"
#include "drw/Framebuffer.hpp"
#include "GBufferStorage.hpp"

struct ShadingPassCreateInfo {
	GBufferStorage *pInput;
};

class ShadingPass {
public:
	Framebuffer fb;
	drw::Pipeline pipeline;

	GBufferStorage *pGbuffer;

public:
	static ShadingPass from(ShadingPassCreateInfo *pInfo);

	void record();
};
