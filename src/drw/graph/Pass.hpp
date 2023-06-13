#pragma once

#include "drw/Storage.hpp"

class Pass {
public:
	void record();

	void set_output(Storage *pStorage);
};
