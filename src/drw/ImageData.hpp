#pragma once

struct ImageData {
	int numChannels;
	unsigned stride;
	int width, height;

	unsigned char *pData;
	bool isParsed;

	char *pPath;

	ImageData();

	ImageData(char *pPath);

	void load_file();
	void parse();
};


