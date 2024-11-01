#include <stdio.h>
#include <iostream>
#include "BitmapImage.h"

BitmapImage::BitmapImage()
{
    m_fileHeader = new BITMAPFILEHEADER;
    m_infoHeader = new BITMAPINFOHEADER;
    m_info = new BITMAPINFO;
    m_pixels = nullptr;
};
BitmapImage::~BitmapImage() 
{
    delete[] m_pixels;
    delete m_fileHeader;
    delete m_infoHeader;
    delete m_info;
};

bool BitmapImage::loadFile(const char* fileName)
{
    FILE* file;
    fopen_s(&file, fileName, "rb");
    if (file == NULL)
        return 0;
    fseek(file, 0, SEEK_END);
    int size = ftell(file);
    uint8_t bufferHeader[54] = {};
    m_pixels = new uint8_t[size - 54];
    fseek(file, 0, SEEK_SET);
    fread(m_fileHeader, sizeof(uint8_t), 14, file);
    fread(m_infoHeader, sizeof(uint8_t), 40, file);

    m_info = (BITMAPINFO*)m_infoHeader;

    fread(m_pixels, sizeof(uint8_t), size - m_fileHeader->bfOffBits, file);
    fclose(file);
    return true;
}

bool BitmapImage::saveFile(const char* fileName)
{
    FILE* file;
    fopen_s(&file, fileName, "wb");
    if (file == NULL)
        return 0;

    fwrite(m_fileHeader, sizeof(uint8_t), 14, file);
    fwrite(m_infoHeader, sizeof(uint8_t), 40, file);
    fwrite(m_pixels, sizeof(uint8_t), m_fileHeader->bfSize, file);
    fclose(file);
    return 1;
}