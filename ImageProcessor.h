#pragma once

#include "Utilities.h"

class ImageProcessor
{
public:

    static void applyGrayscale(int*** pixels, int height, int width, FileType fileType);
    static void applyMonochrome(int*** pixels, int height, int width, FileType fileType);
    static void applyNegative(int*** pixels, int height, int width, FileType fileType);
    static void rotateLeft(int***& pixels, int& height, int& width);
    static void rotateRight(int***& pixels, int& height, int& width);

};
