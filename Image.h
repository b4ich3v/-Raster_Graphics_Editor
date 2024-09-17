#pragma once

#include "FileHelper.h"
#include "ImageProcessor.h"
#include "Utilities.h"

class Image
{
private:

    char fileName[MAX_FILENAME_LENGTH];
    FileType fileType;
    int*** pixels;
    int height;
    int width;
    bool binary;

public:

    Image(const char* fileName);
    Image(const char* fileName, int*** pixels, int height, int width, FileType fileType, bool binary);
    ~Image();

    void load();
    bool isBinary() const;
    void setFileType(FileType fileType);
    void save(const char* fileName);
    int getWidth() const;
    Image* clone() const;
    int getHeight() const;
    int* getPixel(int row, int col) const;
    void applyGrayscale();
    void applyMonochrome();
    void applyNegative();
    void rotateLeft();
    void rotateRight();
    FileType getFileType() const;
    const char* getFileName() const;

};
