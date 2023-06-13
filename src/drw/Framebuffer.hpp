#pragma once

#include "Storage.hpp"

/* Outputs to some storage */
class Framebuffer {
public:
	unsigned int framebuffer;

public:
	static Framebuffer to(Storage* pStorage);	
	
	static Framebuffer screen() {
		return {
			.framebuffer = 0
		};
	}

	Framebuffer& set_output(Storage* pStorage);

	void use();
};
