#include "Framebuffer.hpp"

#include <glad.h>
#include <stdio.h>

Framebuffer Framebuffer::to(Storage *pStorage) {
	Framebuffer result = {};
	glGenFramebuffers(1, &result.framebuffer);
	result.set_output(pStorage);

	return result;
}

Framebuffer& Framebuffer::set_output(Storage* pStorage) {
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	
	/* get images */
	auto images = pStorage->get_images();
	auto depthImage = pStorage->get_depth_image();
	uint32_t *pAttachments = new uint32_t[images.size()];

	/* add images to framebuffer */
	for(int i = 0; i < images.size(); i++) {
		glBindTexture(GL_TEXTURE_2D, images[i].get_handle());
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, 
							   GL_TEXTURE_2D, images[i].get_handle(), 0);
		pAttachments[i] = GL_COLOR_ATTACHMENT0 + i;
	}

	/* TODO: Handle depth image */
	if(depthImage) {
		glBindTexture(GL_TEXTURE_2D, depthImage->get_handle());
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthImage->get_handle(), 0);
	}

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		printf("Invalid framebuffer created\n");
	}

	/* draw all as color attachments, do not use depth yet */
	glDrawBuffers(images.size(), pAttachments);
	
	return *this;
}

void Framebuffer::use() {
	glBindFramebuffer(GL_FRAMEBUFFER, this->framebuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
