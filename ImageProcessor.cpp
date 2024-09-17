#include "ImageProcessor.h"
#include <iostream>

void ImageProcessor::applyGrayscale(int*** pixels, int height, int width, FileType fileType) 
{

    if (fileType == FileType::PBM)  // Grayscale transformation is not applicable for PBM files
    {

        std::cout << "PBM files are already monochrome, grayscale transformation is not applicable.\n";
        return;

    }

    if (fileType == FileType::PGM) // Grayscale transformation is not needed for PGM files
    {

        std::cout << "PGM files are already in grayscale.\n";
        return;

    }

    if (fileType == FileType::PPM) // Check and apply grayscale transformation for PPM files
    {

        bool isAlreadyGrayscale = true;

        for (int i = 0; i < height; i++) 
        {

            for (int j = 0; j < width; j++)
            {

                int red = pixels[i][j][0];
                int green = pixels[i][j][1];
                int blue = pixels[i][j][2];

                if (red != green || green != blue)
                {

                    isAlreadyGrayscale = false;
                    break;

                }

            }

            if (!isAlreadyGrayscale) 
            {

                break;

            }

        }
        if (!isAlreadyGrayscale) // Apply grayscale transformation if the image is not already grayscale
        {

            for (int i = 0; i < height; i++) 
            {

                for (int j = 0; j < width; j++) 
                {

                    int red = pixels[i][j][0];
                    int green = pixels[i][j][1];
                    int blue = pixels[i][j][2];
                    int gray = (red + green + blue) / 3;
                    pixels[i][j][0] = gray;
                    pixels[i][j][1] = gray;
                    pixels[i][j][2] = gray;

                }

            }

        }
        else 
        {
            std::cout << "Image is already grayscale.\n";

        }

    }

}

void ImageProcessor::applyMonochrome(int*** pixels, int height, int width, FileType fileType) 
{

    if (fileType == FileType::PBM) // Monochrome transformation is not applicable for PBM files
    {

        std::cout << "PBM files are already monochrome, monochrome transformation is not applicable.\n";
        return;

    }
    if (fileType == FileType::PPM) // Apply monochrome transformation for PPM files
    {

        if (pixels == nullptr) 
        {

            throw std::runtime_error("Pixel data is null. Cannot apply transformation.");

        }

        for (int i = 0; i < height; i++) 
        {

            for (int j = 0; j < width; j++)
            {

                int red = pixels[i][j][0];
                int green = pixels[i][j][1];
                int blue = pixels[i][j][2];

                int gray = static_cast<int>(0.299 * red + 0.587 * green + 0.114 * blue); // Calculate grayscale value using luminance formula
                int monochrome = gray > GRAYSCALE_THRESHOLD ? COLOR_MAX : 0; // Determine monochrome value based on the threshold

                pixels[i][j][0] = monochrome;
                pixels[i][j][1] = monochrome;
                pixels[i][j][2] = monochrome;

            }

        }

    }
    else if (fileType == FileType::PGM) // Apply monochrome transformation for PGM files
    {

        if (pixels == nullptr) 
        {

            throw std::runtime_error("Pixel data is null. Cannot apply transformation.");

        }
        for (int i = 0; i < height; i++) 
        {

            for (int j = 0; j < width; j++) 
            {

                int gray = pixels[i][j][0];
                int monochrome = gray > GRAYSCALE_THRESHOLD ? COLOR_MAX : 0; // Determine monochrome value based on the threshold

                pixels[i][j][0] = monochrome;
                pixels[i][j][1] = monochrome;
                pixels[i][j][2] = monochrome;

            }

        }

    }

}

void ImageProcessor::applyNegative(int*** pixels, int height, int width, FileType fileType) 
{

    for (int i = 0; i < height; i++) // Apply negative transformation to each pixel
    {

        for (int j = 0; j < width; j++) // Invert each color component
        {

            pixels[i][j][0] = 255 - pixels[i][j][0]; // Red component
            pixels[i][j][1] = 255 - pixels[i][j][1]; // Green component
            pixels[i][j][2] = 255 - pixels[i][j][2]; // Blue component

        }

    }

}

void ImageProcessor::rotateLeft(int***& pixels, int& height, int& width) 
{

    int numRows = height;
    int numCols = width;
    int*** rotatedPixels = new int** [numCols];

    for (int i = 0; i < numCols; i++)  // Allocate memory for the rotated image pixels
    {

        rotatedPixels[i] = new int* [numRows];

        for (int j = 0; j < numRows; j++)
        {

            rotatedPixels[i][j] = new int[3];

        }

    }

    for (int i = 0; i < numRows; i++) // Rotate the image 90 degrees to the left
    {

        for (int j = 0; j < numCols; j++) 
        {

            rotatedPixels[numCols - j - 1][i][0] = pixels[i][j][0];
            rotatedPixels[numCols - j - 1][i][1] = pixels[i][j][1];
            rotatedPixels[numCols - j - 1][i][2] = pixels[i][j][2];

        }

    }

    for (int i = 0; i < height; i++) // Deallocate memory of the original image pixels
    {

        for (int j = 0; j < width; j++)
        {

            delete[] pixels[i][j];

        }

        delete[] pixels[i];

    }

    delete[] pixels;

    pixels = rotatedPixels; // Update the pixels pointer to the rotated image
    std::swap(height, width); // Swap the height and width values

}

void ImageProcessor::rotateRight(int***& pixels, int& height, int& width)
{

    int numRows = height;
    int numCols = width;
    int*** rotatedPixels = new int** [numCols];

    for (int i = 0; i < numCols; i++) // Allocate memory for the rotated image pixels
    {

        rotatedPixels[i] = new int* [numRows];

        for (int j = 0; j < numRows; j++)
        {

            rotatedPixels[i][j] = new int[3];
            Utilities::copyPixelData(rotatedPixels[i][j], pixels[numRows - 1 - j][i], 3); // Copy pixel data from original to rotated position

        }

    }

    // Swap the pointers and dimensions
    std::swap(pixels, rotatedPixels);
    std::swap(height, numCols);
    std::swap(width, numRows);

    for (int i = 0; i < numCols; i++) // Deallocate memory of the original image pixels
    {

        for (int j = 0; j < numRows; j++) 
        {

            delete[] rotatedPixels[i][j];

        }

        delete[] rotatedPixels[i];

    }

    delete[] rotatedPixels;

}
