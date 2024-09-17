#include "FileHelper.h"
#include <sstream>

void FileHelper::parseFilePPM_Plain(std::ifstream& file, int***& pixels, int& height, int& width) 
{

    char line[256];
    file.getline(line, 256);

    while (strlen(line) == 0 || line[0] == '#') // Skip comments and empty lines
    {

        file.getline(line, 256);

    }
    std::istringstream iss(line);

    if (!(iss >> width >> height)) // Read image dimensions
    {

        throw std::runtime_error("Invalid image dimensions in file.");

    }

    int maxValue;
    file >> maxValue;

    if (file.fail() || maxValue <= 0) // Validate max value
    {

        throw std::runtime_error("Invalid max value in file.");

    }

    file.ignore();

    pixels = new int** [height];

    for (int i = 0; i < height; i++) // Allocate memory for the pixel data
    {

        pixels[i] = new int* [width];

        for (int j = 0; j < width; j++) 
        {

            pixels[i][j] = new int[3];

        }

    }

    int r, g, b;

    for (int i = 0; i < height; i++) // Read pixel data
    {

        for (int j = 0; j < width; j++) 
        {

            if (!(file >> r >> g >> b)) 
            {

                throw std::runtime_error("Invalid pixel data in file.");

            }

            pixels[i][j][0] = r;
            pixels[i][j][1] = g;
            pixels[i][j][2] = b;

        }

    }

}

void FileHelper::parseFilePGM_Plain(std::ifstream& file, int***& pixels, int& height, int& width)
{

    char line[256];
    file.getline(line, 256);

    while (strlen(line) == 0 || line[0] == '#') // Skip comments and empty lines
    {

        file.getline(line, 256);

    }

    std::istringstream iss(line);

    if (!(iss >> width >> height)) // Read image dimensions
    {

        throw std::runtime_error("Invalid image dimensions in file.");

    }

    int maxValue;
    file >> maxValue;

    if (file.fail() || maxValue <= 0)  // Validate max value
    {

        throw std::runtime_error("Invalid max value in file.");

    }

    file.ignore();

    pixels = new int** [height];

    for (int i = 0; i < height; i++) // Allocate memory for the pixel data
    {

        pixels[i] = new int* [width];

        for (int j = 0; j < width; j++) 
        {

            pixels[i][j] = new int[3];

        }

    }

    int gray;

    for (int i = 0; i < height; i++) // Read pixel data
    {

        for (int j = 0; j < width; j++)
        {

            if (!(file >> gray)) 
            {

                throw std::runtime_error("Invalid pixel data in file.");

            }

            // Set all three color components to the grayscale value
            pixels[i][j][0] = gray;
            pixels[i][j][1] = gray;
            pixels[i][j][2] = gray;

        }

    }

}

void FileHelper::parseFilePBM_Plain(std::ifstream& file, int***& pixels, int& height, int& width) 
{

    char line[256];
    file.getline(line, 256);

    while (strlen(line) == 0 || line[0] == '#') // Skip comments and empty lines
    {

        file.getline(line, 256);

    }

    std::istringstream iss(line);

    if (!(iss >> width >> height))  // Read image dimensions
    {

        throw std::runtime_error("Invalid image dimensions in file.");

    }

    pixels = new int** [height];

    for (int i = 0; i < height; i++) // Allocate memory for the pixel data
    {

        pixels[i] = new int* [width];

        for (int j = 0; j < width; j++) 
        {

            pixels[i][j] = new int[3];

        }

    }

    int bit;

    for (int i = 0; i < height; i++) // Read pixel data
    {

        for (int j = 0; j < width; j++)
        {

            if (!(file >> bit))
            {

                throw std::runtime_error("Invalid pixel data in file.");

            }

            int color = bit * 255; // Convert bit to 255 or 0

            pixels[i][j][0] = color;
            pixels[i][j][1] = color;
            pixels[i][j][2] = color;

        }

    }

}

void FileHelper::writeFilePlain(const char* fileName, int*** pixels, int height, int width, FileType fileType) 
{

    std::ofstream file(fileName);

    if (!file.is_open()) // Check if the file can be opened for writing
    {

        throw std::runtime_error("Could not open file for writing.");

    }

    if (pixels == nullptr) // Check if the pixel data is null
    {

        throw std::runtime_error("Pixel data is null. Cannot save file.");

    }

    int maxValue = Utilities::findMaxValue(pixels, height, width, fileType);

    if (fileType == FileType::PPM) // Write the file header and pixel data based on the file type
    {

        file << "P3\n";
        file << width << " " << height << "\n";
        file << maxValue << "\n";

        for (int i = 0; i < height; i++) 
        {

            if (pixels[i] == nullptr)
            {

                throw std::runtime_error("Row pixel data is null. Cannot save file.");

            }
            for (int j = 0; j < width; j++) 
            {

                if (pixels[i][j] == nullptr) 
                {

                    throw std::runtime_error("Column pixel data is null. Cannot save file.");

                }

                file << pixels[i][j][0] << " " << pixels[i][j][1] << " " << pixels[i][j][2];

                if (j < width - 1)
                {

                    file << " ";

                }

            }

            file << "\n";

        }

    }
    else if (fileType == FileType::PGM)
    {

        file << "P2\n";
        file << width << " " << height << "\n";
        file << maxValue << "\n";

        for (int i = 0; i < height; i++) 
        {

            if (pixels[i] == nullptr) 
            {

                throw std::runtime_error("Row pixel data is null. Cannot save file.");

            }

            for (int j = 0; j < width; j++) 
            {

                if (pixels[i][j] == nullptr) 
                {

                    throw std::runtime_error("Column pixel data is null. Cannot save file.");

                }

                file << pixels[i][j][0];

                if (j < width - 1) 
                {

                    file << " ";

                }

            }

            file << "\n";

        }

    }
    else if (fileType == FileType::PBM)
    {

        file << "P1\n";
        file << width << " " << height << "\n";

        for (int i = 0; i < height; i++) 
        {

            if (pixels[i] == nullptr) 
            {

                throw std::runtime_error("Row pixel data is null. Cannot save file.");

            }

            for (int j = 0; j < width; j++) 
            {

                if (pixels[i][j] == nullptr) 
                {

                    throw std::runtime_error("Column pixel data is null. Cannot save file.");

                }

                file << (pixels[i][j][0] > GRAYSCALE_THRESHOLD ? 1 : 0);

                if (j < width - 1) 
                {

                    file << " ";

                }

            }

            file << "\n";

        }

    }
    if (file.fail()) // Check if the file writing failed
    {

        throw std::runtime_error("Failed to write to file.");

    }

    file.close();

}

void FileHelper::parseFilePPM_Binary(std::ifstream& file, int***& pixels, int& height, int& width) 
{

    char line[256];
    file.getline(line, 256);

    while (strlen(line) == 0 || line[0] == '#') // Skip comments and empty lines
    {

        file.getline(line, 256);
        std::cout << "Debug: Skipping comment/empty line: " << line << std::endl;

    }

    std::istringstream iss(line);

    if (!(iss >> width >> height)) // Read image dimensions
    {

        throw std::runtime_error("Invalid image dimensions in file.");

    }

    int maxValue;
    file >> maxValue;

    if (file.fail() || maxValue <= 0)  // Validate max value
    {

        throw std::runtime_error("Invalid max value in file.");

    }

    file.ignore();

    pixels = new int** [height];

    for (int i = 0; i < height; i++) // Allocate memory for the pixel data
    {

        pixels[i] = new int* [width];

        for (int j = 0; j < width; j++)
        {

            pixels[i][j] = new int[3];

        }

    }

    for (int i = 0; i < height; i++) // Read pixel data
    {

        for (int j = 0; j < width; j++) 
        {

            unsigned char rgb[3];

            file.read((char*)(rgb), 3);

            if (file.fail()) 
            {

                throw std::runtime_error("Invalid pixel data in file.");

            }

            // Assign pixel values
            pixels[i][j][0] = rgb[0];
            pixels[i][j][1] = rgb[1];
            pixels[i][j][2] = rgb[2];

        }

    }

}

void FileHelper::parseFilePGM_Binary(std::ifstream& file, int***& pixels, int& height, int& width)
{

    char line[256];
    file.getline(line, 256);

    while (strlen(line) == 0 || line[0] == '#') // Skip comments and empty lines
    {

        file.getline(line, 256);

    }

    std::istringstream iss(line);

    if (!(iss >> width >> height)) // Read image dimensions
    {

        throw std::runtime_error("Invalid image dimensions in file.");

    }

    int maxValue;
    file >> maxValue;

    if (file.fail() || maxValue <= 0) // Validate max value
    {

        throw std::runtime_error("Invalid max value in file.");

    }

    file.ignore();

    pixels = new int** [height];

    for (int i = 0; i < height; i++) // Allocate memory for the pixel data
    {

        pixels[i] = new int* [width];

        for (int j = 0; j < width; j++) 
        {

            pixels[i][j] = new int[1];

        }

    }

    for (int i = 0; i < height; i++) // Read pixel data
    {

        file.read((char*)(pixels[i]), width);

        if (file.fail()) 
        {

            throw std::runtime_error("Invalid pixel data in file.");

        }

    }

}

void FileHelper::parseFilePBM_Binary(std::ifstream& file, int***& pixels, int& height, int& width) 
{

    char line[256];
    file.getline(line, 256);

    while (strlen(line) == 0 || line[0] == '#') // Skip comments and empty lines
    {

        file.getline(line, 256);

    }

    std::istringstream iss(line);

    if (!(iss >> width >> height)) // Read image dimensions
    {

        throw std::runtime_error("Invalid image dimensions in file.");

    }

    pixels = new int** [height];

    for (int i = 0; i < height; i++) // Allocate memory for the pixel data
    {

        pixels[i] = new int* [width];

        for (int j = 0; j < width; j++)
        {

            pixels[i][j] = new int[1];

        }

    }

    for (int i = 0; i < height; i++) // Read pixel data
    {

        file.read((char*)(pixels[i]), width / 8);

        if (file.fail()) 
        {

            throw std::runtime_error("Invalid pixel data in file.");

        }

    }

}

void FileHelper::writeFileBinary(const char* fileName, int*** pixels, int height, int width, FileType fileType) 
{

    std::ofstream file(fileName, std::ios::binary | std::ios::out);

    if (!file.is_open()) // Check if the file can be opened for writing
    {

        throw std::runtime_error("Could not open file for writing.");

    }

    if (pixels == nullptr) // Check if the pixel data is null
    {

        throw std::runtime_error("Pixel data is null. Cannot save file.");

    }

    int maxValue = Utilities::findMaxValue(pixels, height, width, fileType);

    if (fileType == FileType::PPM) // Write the file header and pixel data based on the file type
    {

        file << "P6\n";
        file << width << " " << height << "\n";
        file << maxValue << "\n";

        for (int i = 0; i < height; i++) 
        {

            for (int j = 0; j < width; j++) 
            {

                int r = pixels[i][j][0];
                int g = pixels[i][j][1];
                int b = pixels[i][j][2];

                if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255) // Validate pixel values
                {

                    throw std::runtime_error("Pixel value out of range.");

                }

                unsigned char rgb[3] = 
                {

                    static_cast<unsigned char>(r),
                    static_cast<unsigned char>(g),
                    static_cast<unsigned char>(b)

                };

                file.put(rgb[0]);
                file.put(rgb[1]);
                file.put(rgb[2]);

                if (file.fail()) 
                {

                    throw std::runtime_error("Failed to write pixel data.");

                }

                file.flush();

            }

        }

    }
    else if (fileType == FileType::PGM)
    {

        file << "P5\n";
        file << width << " " << height << "\n";
        file << maxValue << "\n";

        for (int i = 0; i < height; i++) 
        {

            for (int j = 0; j < width; j++) 
            {

                file.put(static_cast<unsigned char>(pixels[i][j][0]));

                if (file.fail()) 
                {

                    throw std::runtime_error("Failed to write pixel data.");

                }

                file.flush();

            }

        }

    }
    else if (fileType == FileType::PBM)
    {

        file << "P4\n";
        file << width << " " << height << "\n";

        for (int i = 0; i < height; i++) 
        {

            for (int j = 0; j < width; j++) 
            {

                file.put(static_cast<unsigned char>(pixels[i][j][0] > GRAYSCALE_THRESHOLD ? 1 : 0));

                if (file.fail()) 
                {

                    throw std::runtime_error("Failed to write pixel data.");

                }

                file.flush();

            }

        }

    }

    if (file.fail()) // Check if the file writing failed
    {

        throw std::runtime_error("Failed to write to file.");

    }

    file.close();

}

void FileHelper::saveToFile(const char* fileName, int*** pixels, int height, int width, FileType fileType, bool binary) 
{

    if (binary) 
    {

        writeFileBinary(fileName, pixels, height, width, fileType);

    }
    else 
    {

        writeFilePlain(fileName, pixels, height, width, fileType);

    }

}
