#include "Image.h"
#include <iostream>
#include <cstring>

Image::Image(const char* fileName) 
{

    strncpy(this->fileName, fileName, MAX_FILENAME_LENGTH);
    this->fileName[MAX_FILENAME_LENGTH - 1] = '\0';
    this->pixels = nullptr;
    this->fileType = FileType::UNKNOWN;
    this->binary = false;
    load();

}

Image::Image(const char* fileName, int*** pixels, int height, int width, FileType fileType, bool binary) 
{

    strncpy(this->fileName, fileName, MAX_FILENAME_LENGTH);
    this->fileName[MAX_FILENAME_LENGTH - 1] = '\0';
    this->pixels = pixels;
    this->height = height;
    this->width = width;
    this->fileType = fileType;
    this->binary = binary;

    if (pixels == nullptr) // Check if the pixel data is null
    {

        throw std::runtime_error("Pixel data is null during Image construction.");

    }

    for (int i = 0; i < height; i++) // Check if each row in the pixel data is null
    {

        if (pixels[i] == nullptr) 
        {

            throw std::runtime_error("Row pixel data is null during Image construction.");

        }

        for (int j = 0; j < width; j++) // Check if each column in the pixel data is null
        {

            if (pixels[i][j] == nullptr) 
            {

                throw std::runtime_error("Column pixel data is null during Image construction.");

            }

        }

    }

}

Image::~Image()
{

    if (pixels != nullptr) 
    {

        for (int i = 0; i < height; i++)
        {

            if (pixels[i] != nullptr) 
            {

                for (int j = 0; j < width; j++) 
                {

                    delete[] pixels[i][j];

                }

                delete[] pixels[i];

            }

        }

        delete[] pixels;
        pixels = nullptr;
        std::cout << "Pixel data memory freed for image: " << fileName << std::endl;

    }

}

void Image::load()
{

    std::ifstream file(fileName, std::ios::binary);

    if (!file.is_open()) // Check if the file can be opened
    {

        std::cerr << "Could not open file: " << fileName << std::endl;
        throw std::runtime_error("Could not open file.");

    }

    char line[256];
    file.getline(line, sizeof(line));

    if (line[0] != 'P') // Check if the file format is supported
    {

        std::cout << "Unsupported file format: " << line << std::endl;
        throw std::runtime_error("Unsupported file format.");

    }

    switch (line[1]) // Determine the file type and whether it's binary
    {

    case '1': this->fileType = FileType::PBM; this->binary = false; break;
    case '2': this->fileType = FileType::PGM; this->binary = false; break;
    case '3': this->fileType = FileType::PPM; this->binary = false; break;
    case '4': this->fileType = FileType::PBM; this->binary = true; break;
    case '5': this->fileType = FileType::PGM; this->binary = true; break;
    case '6': this->fileType = FileType::PPM; this->binary = true; break;
    default:
        std::cout << "Unsupported file format: " << line << std::endl;
        throw std::runtime_error("Unsupported file format.");

    }

    // Parse the file based on its type and whether it's binary
    if (this->fileType == FileType::PBM)
    {

        if (this->binary) { FileHelper::parseFilePBM_Binary(file, this->pixels, this->height, this->width); }
        else { FileHelper::parseFilePBM_Plain(file, this->pixels, this->height, this->width); }

    }
    else if (this->fileType == FileType::PGM) 
    {

        if (this->binary) { FileHelper::parseFilePGM_Binary(file, this->pixels, this->height, this->width); }
        else { FileHelper::parseFilePGM_Plain(file, this->pixels, this->height, this->width); }

    }
    else if (this->fileType == FileType::PPM)
    {

        if (this->binary) { FileHelper::parseFilePPM_Binary(file, this->pixels, this->height, this->width); }
        else { FileHelper::parseFilePPM_Plain(file, this->pixels, this->height, this->width); }

    }

    file.close();
    std::cout << "File loaded successfully: " << fileName << std::endl;

}

bool Image::isBinary() const 
{

    return binary;

}

void Image::setFileType(FileType fileType) 
{

    this->fileType = fileType;

}

void Image::save(const char* fileName) 
{
    try 
    {

        FileHelper::saveToFile(fileName, this->pixels, this->height, this->width, this->fileType, this->binary);

    }
    catch (const std::exception& e) 
    {

        std::cout << "Error saving file: " << e.what() << std::endl;

    }

}

int Image::getWidth() const 
{

    return width;

}

Image* Image::clone() const 
{

    int*** newPixels = new int** [height];

    for (int i = 0; i < height; i++) 
    {

        newPixels[i] = new int* [width];

        for (int j = 0; j < width; j++)
        {

            newPixels[i][j] = new int[3];

            for (int k = 0; k < 3; k++) 
            {

                newPixels[i][j][k] = pixels[i][j][k];

            }

        }

    }

    return new Image(fileName, newPixels, height, width, fileType, binary);

}

int Image::getHeight() const 
{

    return height;

}

int* Image::getPixel(int row, int col) const 
{

    return pixels[row][col];

}

void Image::applyGrayscale() 
{

    ImageProcessor::applyGrayscale(this->pixels, this->height, this->width, this->fileType);

}

void Image::applyMonochrome() 
{

    ImageProcessor::applyMonochrome(this->pixels, this->height, this->width, this->fileType);

}

void Image::applyNegative()
{

    ImageProcessor::applyNegative(this->pixels, this->height, this->width, this->fileType);

}

void Image::rotateLeft() 
{

    ImageProcessor::rotateLeft(this->pixels, this->height, this->width);

}

void Image::rotateRight()
{

    ImageProcessor::rotateRight(this->pixels, this->height, this->width);

}

FileType Image::getFileType() const 
{

    return fileType;

}

const char* Image::getFileName() const
{

    return fileName;

}
