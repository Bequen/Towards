#include "path.hpp"
#include <linux/limits.h>
#include <unistd.h>
#include <err.h>
#include <string.h>

static char *pExePath;

void io::path::setup_exe_path(char *pArg) {
	char *pBuffer = (char*)malloc(PATH_MAX);
	int bytes = readlink("/proc/self/exe", pBuffer, PATH_MAX);
	if(bytes >= 0)
		pBuffer[bytes] = '\0';

	unsigned int lastSlash = 0;
	for(int i = bytes; i >= 0; i--) {
		if(pBuffer[i] == '/') {
			lastSlash = i + 1;
			break;
		}
	}

	pBuffer[lastSlash] = '\0';

	pExePath = pBuffer;
}

std::string io::path::exec_dir() {
	return std::string(pExePath);
}

std::string io::path::shader(std::string name) {
	return exec_dir().append("src/shaders/").append(name);
}
