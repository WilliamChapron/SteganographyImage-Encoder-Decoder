#pragma once
#include <Windows.h>
#include <stdint.h>

class BitmapImage
{
public:
	BITMAPFILEHEADER* m_fileHeader;
	BITMAPINFOHEADER* m_infoHeader;
	BITMAPINFO* m_info;
	uint8_t* m_pixels;

	BitmapImage();
	~BitmapImage();

	bool loadFile(const char* fileName);
	bool saveFile(const char* fileName);
	inline int getPixelNumber() { return m_fileHeader->bfSize; }
};

