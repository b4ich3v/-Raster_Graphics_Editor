#pragma once

#include <iostream>
#include <fstream>
#include "Utilities.h"

class FileHelper
{
public:

    static void parseFilePPM_Plain(std::ifstream& file, int***& pixels, int& height, int& width);
    static void parseFilePGM_Plain(std::ifstream& file, int***& pixels, int& height, int& width);
    static void parseFilePBM_Plain(std::ifstream& file, int***& pixels, int& height, int& width);
    static void writeFilePlain(const char* fileName, int*** pixels, int height, int width, FileType fileType);
    static void parseFilePPM_Binary(std::ifstream& file, int***& pixels, int& height, int& width);
    static void parseFilePGM_Binary(std::ifstream& file, int***& pixels, int& height, int& width);
    static void parseFilePBM_Binary(std::ifstream& file, int***& pixels, int& height, int& width);
    static void writeFileBinary(const char* fileName, int*** pixels, int height, int width, FileType fileType);
    static void saveToFile(const char* fileName, int*** pixels, int height, int width, FileType fileType, bool binary);

};
