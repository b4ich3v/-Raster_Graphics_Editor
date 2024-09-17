#include "Utilities.h"
#include <cstring>

void Utilities::copyPixelData(int* destination, const int* source, int componentCount) 
{

    for (int i = 0; i < componentCount; i++) // Copy each pixel component from source to destination
    {

        destination[i] = source[i];

    }

}

char Utilities::toLower(char symbol) 
{

    if (symbol >= 'A' && symbol <= 'Z') // Convert uppercase letter to lowercase if within range
    {

        return symbol + ('a' - 'A');

    }

    return symbol;

}

void Utilities::formattedString(char* buffer, int bufferSize, const char* format, const char* string1, const char* string2, const char* string3) 
{

    int written = 0; // Keeps track of the number of characters written to buffer
    const char* args[] = { string1, string2, string3 }; // Array of string arguments
    int argIndex = 0; // Index to track current argument

    while (*format && written < bufferSize - 1) 
    {

        if (*format == '%' && *(format + 1) == 's' && argIndex < 3) // If a '%s' is found in the format string, replace it with the corresponding argument
        {

            const char* str = args[argIndex++];

            while (*str && written < bufferSize - 1) 
            {

                buffer[written++] = *str++;

            }

            format += 2; // Skip over '%s'

        }
        else 
        {

            buffer[written++] = *format++;

        }

    }

    buffer[written] = '\0'; // Null-terminate the resulting string

}

int Utilities::findMaxValue(int*** pixels, int height, int width, FileType fileType)
{

    int maxVal = 0;

    if (fileType == FileType::PPM) 
    {

        for (int i = 0; i < height; i++) // Iterate through each pixel for PPM (RGB) images
        {

            for (int j = 0; j < width; j++)
            {

                for (int k = 0; k < 3; k++) 
                {

                    if (pixels[i][j][k] > maxVal) // Update maxVal if current component is greater
                    {

                        maxVal = pixels[i][j][k];

                    }

                }

            }

        }

    }
    else if (fileType == FileType::PGM) 
    {

        for (int i = 0; i < height; i++) // Iterate through each pixel for PGM (grayscale) images
        {

            for (int j = 0; j < width; j++) 
            {

                if (pixels[i][j][0] > maxVal)  // Update maxVal if current component is greater
                {

                    maxVal = pixels[i][j][0];

                }

            }

        }

    }

    return maxVal; // Return the maximum pixel value found

}

const char* Utilities::fileTypeToString(FileType fileType) 
{

    switch (fileType) // Convert FileType enum to its corresponding string representation
    {

    case FileType::PPM: return "PPM";
    case FileType::PGM: return "PGM";
    case FileType::PBM: return "PBM";
    default: return "UNKNOWN";

    }

}

FileType Utilities::stringToFileType(const char* string) 
{

    // Convert string to corresponding FileType enum
    if (strcmp(string, "PPM") == 0) return FileType::PPM;
    if (strcmp(string, "PGM") == 0) return FileType::PGM;
    if (strcmp(string, "PBM") == 0) return FileType::PBM;
    return FileType::UNKNOWN;

}

std::ostream& Utilities::fileTypeToStream(std::ostream& os, FileType fileType) 
{

    return os << fileTypeToString(fileType);

}

std::istream& Utilities::streamToFileType(std::istream& is, FileType& fileType)
{

    char string[256];
    is >> string;
    fileType = stringToFileType(string);
    return is;

}

int Utilities::stringToInt(const char* string) 
{

    int result = 0;
    int sign = 1;

    if (*string == '-') // Check for negative sign
    {

        sign = -1;
        string++;

    }
    while (*string) // Convert string to integer
    {

        if (*string < '0' || *string > '9') // If non-numeric character is found, return 0
        {

            return 0;

        }

        result = result * 10 + (*string - '0');
        string++;

    }

    return sign * result;

}

void Utilities::removeExtension(char* fileName) 
{

    int length = strlen(fileName);

    for (int i = length - 1; i >= 0; i--) // Iterate backwards through the string to find the last '.' 
    {

        if (fileName[i] == '.') 
        {

            fileName[i] = '\0'; // Terminate the string at the last '.'
            break;

        }

    }

}

std::ostream& operator << (std::ostream& os, FileType fileType)
{

    return Utilities::fileTypeToStream(os, fileType);

}

std::istream& operator >> (std::istream& is, FileType& fileType) 
{

    return Utilities::streamToFileType(is, fileType);

}
