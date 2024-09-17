#pragma once

#include <iostream>

#pragma warning(disable:4996)
const int MAX_FILENAME_LENGTH = 256;
const int MAX_IMAGES = 100;
const int MAX_TRANSFORMATIONS = 100;
const int MAX_SESSIONS = 100;
const int COLOR_MAX = 255;
const int GRAYSCALE_THRESHOLD = 127;

enum class FileType
{

    PPM,
    PGM,
    PBM,
    UNKNOWN

};

class Utilities
{
public:

    static void copyPixelData(int* destination, const int* source, int componentCount);
    static char toLower(char symbol);
    static void formattedString(char* buffer, int bufferSize, const char* format, const char* string1, const char* string2, const char* string3);
    static int findMaxValue(int*** pixels, int height, int width, FileType fileType);
    static const char* fileTypeToString(FileType fileType);
    static FileType stringToFileType(const char* string);
    static std::ostream& fileTypeToStream(std::ostream& os, FileType fileType);
    static std::istream& streamToFileType(std::istream& is, FileType& fileType);
    static int stringToInt(const char* string);
    static void removeExtension(char* fileName);

};

std::ostream& operator << (std::ostream& os, FileType fileType);
std::istream& operator >> (std::istream& is, FileType& fileType);
