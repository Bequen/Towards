#pragma once

#include <string>

namespace io::path {


void setup_exe_path(char *pArg);

std::string exec_dir();

std::string shader(std::string name);

std::string asset(std::string name);
}
