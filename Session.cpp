#include "Session.h"
#include <iostream>
#include <cstring>

Session::Session() : id(0), numImages(0), numTransformations(0) 
{

    for (int i = 0; i < MAX_IMAGES; i++) 
    {

        stateIndex[i] = -1;

    }

}

Session::Session(int id) : id(id), numImages(0), numTransformations(0) 
{

    for (int i = 0; i < MAX_IMAGES; i++) 
    {

        stateIndex[i] = -1;

    }

}

Session::~Session() 
{

    for (int i = 0; i < numImages; i++) 
    {

        if (images[i]) 
        {

            delete images[i];
            images[i] = nullptr;

        }

    }

}

void Session::addImage(const char* fileName) 
{

    if (numImages >= MAX_IMAGES) // Check if the maximum number of images is reached
    {

        std::cout << "Maximum number of images reached.\n";
        return;

    }

    for (int i = 0; i < numImages; i++)  // Check if the image is already added
    {

        if (strcmp(images[i]->getFileName(), fileName) == 0)
        {

            std::cout << "Image \"" << fileName << "\" is already added.\n";
            return;

        }

    }

    images[numImages] = new Image(fileName); // Add new image to the session
    numImages++;
    std::cout << "Image \"" << fileName << "\" added\n";

}

void Session::applyTransformation(const char* transformation)
{

    if (numTransformations < MAX_TRANSFORMATIONS)  // Check if the maximum number of transformations is reached
    {

        // Store the transformation in the transformations array
        strncpy(transformations[numTransformations], transformation, MAX_FILENAME_LENGTH);
        transformations[numTransformations][MAX_FILENAME_LENGTH - 1] = '\0';
        numTransformations++;

        for (int i = 0; i < numImages; i++) // Apply the transformation to each image in the session
        {

            applyTransformationToImage(images[i], transformation);

        }

        std::cout << "Transformation \"" << transformation << "\" applied\n";

    }

}

void Session::save() 
{

    for (int i = 0; i < numImages; i++) 
    {

        images[i]->save(images[i]->getFileName());
        std::cout << "Image \"" << images[i]->getFileName() << "\" updated\n";

    }

}

int Session::getId() const
{

    return id;

}

Image* Session::getImage(int index) const 
{

    if (index < 0 || index >= numImages) 
    {

        return nullptr;

    }

    return images[index];

}

int Session::getNumImages() const 
{

    return numImages;

}

void Session::saveAs(const char* fileName)
{

    if (numImages > 0)  // Check if there are any images to save
    {

        const char* newExtension = nullptr;

        for (int i = strlen(fileName) - 1; i >= 0; i--) // Find the file extension in the provided fileName
        {

            if (fileName[i] == '.') 
            {

                newExtension = &fileName[i + 1];
                break;

            }

        }
        if (!newExtension) // If no extension found, print an error message
        {

            std::cout << "Invalid file name. No extension found.\n";
            return;

        }

        FileType originalFileType = images[0]->getFileType();
        bool match = false;

        // Check if the new extension matches the original file type
        if ((strcmp(newExtension, "ppm") == 0 && originalFileType == FileType::PPM) ||
            (strcmp(newExtension, "pgm") == 0 && originalFileType == FileType::PGM) ||
            (strcmp(newExtension, "pbm") == 0 && originalFileType == FileType::PBM)) 
        {

            match = true;

        }

        if (match) // Save the image if the extension matches the file type
        {

            images[0]->save(fileName);
            std::cout << "Image saved as \"" << fileName << "\"\n";

        }
        else 
        {

            std::cout << "File extension does not match the image format.\n";

        }

    }
    else
    {

        std::cout << "No images to save.\n";

    }

}

void Session::undo() 
{

    if (numTransformations > 0) // Check if there are any transformations to undo
    {

        for (int i = 0; i < numImages; i++) // Restore the previous state for each image in the session
        {

            restoreState(i);

        }

        numTransformations--; // Decrement the number of transformations
        std::cout << "Last transformation undone.\n";

    }
    else 
    {

        std::cout << "No transformations to undo.\n";

    }

}

void Session::sessionInfo() const 
{

    std::cout << "Session ID: " << id << "\n";

    for (int i = 0; i < numImages; i++) 
    {

        std::cout << "Image: " << images[i]->getFileName() << " (" << Utilities::fileTypeToString(images[i]->getFileType()) << ")\n";

    }
    for (int i = 0; i < numTransformations; i++) 
    {

        std::cout << "Pending transformation: " << transformations[i] << "\n";

    }

}

void Session::createCollage(const char* direction, const char* image1, const char* image2) 
{

    Image* img1 = findImageByName(image1);
    Image* img2 = findImageByName(image2);

    if (img1 == nullptr || img2 == nullptr) // Check if both images are found in the current session
    {

        std::cout << "Images not found in current session.\n";
        return;

    }

    if (img1->getFileType() != img2->getFileType()) // Check if both images have the same file type
    {

        std::cout << "Cannot make a collage from different types!\n";
        return;

    }

    if (img1->isBinary() != img2->isBinary()) // Check if both images have the same binary mode
    {

        std::cout << "Cannot make a collage from different binary modes!\n";
        return;

    }

    int width1 = img1->getWidth();
    int height1 = img1->getHeight();
    int width2 = img2->getWidth();
    int height2 = img2->getHeight();

    // Check if the dimensions are compatible for the specified direction
    if ((strcmp(direction, "horizontal") == 0 && height1 != height2) ||
        (strcmp(direction, "vertical") == 0 && width1 != width2)) 
    {

        std::cout << "Cannot make a collage from different sizes!\n";
        return;

    }

    int newWidth = (strcmp(direction, "horizontal") == 0) ? width1 + width2 : width1;
    int newHeight = (strcmp(direction, "horizontal") == 0) ? height1 : height1 + height2;
    int*** newPixels = new int** [newHeight];

    for (int i = 0; i < newHeight; i++) // Allocate memory for the new image pixels
    {

        newPixels[i] = new int* [newWidth];

        for (int j = 0; j < newWidth; j++) 
        {

            newPixels[i][j] = new int[3] {0, 0, 0};

        }

    }

    for (int i = 0; i < height1; i++) // Copy pixels from the first image
    {

        for (int j = 0; j < width1; j++) 
        {

            Utilities::copyPixelData(newPixels[i][j], img1->getPixel(i, j), 3);

        }

    }
    for (int i = 0; i < height2; i++) // Copy pixels from the second image
    {

        for (int j = 0; j < width2; j++) 
        {

            if (strcmp(direction, "horizontal") == 0) 
            {

                Utilities::copyPixelData(newPixels[i][j + width1], img2->getPixel(i, j), 3);

            }
            else 
            {

                Utilities::copyPixelData(newPixels[i + height1][j], img2->getPixel(i, j), 3);

            }

        }

    }

    char outFileName[MAX_FILENAME_LENGTH];
    const char* extension;

    switch (img1->getFileType()) // Determine the file extension based on the file type
    {

    case FileType::PPM: extension = ".ppm"; break;
    case FileType::PGM: extension = ".pgm"; break;
    case FileType::PBM: extension = ".pbm"; break;
    default: std::cout << "Unsupported file type.\n";  return;

    }
    char baseName1[MAX_FILENAME_LENGTH];
    char baseName2[MAX_FILENAME_LENGTH];

    // Get the base names of the input images
    strncpy(baseName1, image1, MAX_FILENAME_LENGTH);
    baseName1[MAX_FILENAME_LENGTH - 1] = '\0';
    Utilities::removeExtension(baseName1);

    strncpy(baseName2, image2, MAX_FILENAME_LENGTH);
    baseName2[MAX_FILENAME_LENGTH - 1] = '\0';
    Utilities::removeExtension(baseName2);

    char baseName[MAX_FILENAME_LENGTH];
    Utilities::formattedString(baseName, MAX_FILENAME_LENGTH, "%s_collage_of_%s_and_%s", direction, baseName1, baseName2);

    // Append the extension to the base name
    int baseLength = strlen(baseName);
    int extLength = strlen(extension);

    for (int i = 0; i < extLength && baseLength + i < MAX_FILENAME_LENGTH - 1; i++) 
    {

        baseName[baseLength + i] = extension[i];

    }

    baseName[baseLength + extLength] = '\0';

    // Create and save the new collage image
    Image* newImage = new Image(baseName, newPixels, newHeight, newWidth, img1->getFileType(), img1->isBinary());

    addImage(newImage);
    newImage->save(baseName);
    std::cout << "New collage \"" << baseName << "\" created and saved.\n";

}

void Session::saveState(Image* image, int index) 
{

    if (stateIndex[index] < MAX_TRANSFORMATIONS - 1) // Check if the maximum number of transformations for the image is not exceeded
    {

        stateIndex[index]++; // Increment the state index for the image
        imageStates[index][stateIndex[index]] = image->clone(); // Save a clone of the current image state

    }

}

void Session::restoreState(int index) 
{

    if (stateIndex[index] >= 0) // Check if there is a previous state to restore
    {

        delete images[index]; // Delete the current image
        images[index] = imageStates[index][stateIndex[index]]; // Restore the previous state
        stateIndex[index]--; // Decrement the state index

    }

}

int Session::findImageIndexByName(const char* name) const 
{

    for (int i = 0; i < numImages; i++) 
    {

        if (strcmp(images[i]->getFileName(), name) == 0) 
        {

            return i;

        }

    }

    return -1;

}

Image* Session::findImageByName(const char* name) const 
{

    for (int i = 0; i < numImages; i++) 
    {

        if (strcmp(images[i]->getFileName(), name) == 0) 
        {

            return images[i];

        }

    }

    return nullptr;

}

void Session::addImage(Image* image) 
{

    if (numImages < MAX_IMAGES) 
    {

        images[numImages] = image;
        numImages++;

    }

}

void Session::applyTransformationToImage(Image* image, const char* transformation)
{

    if (image == nullptr) // Check if the image is null
    {

        std::cout << "Error: Image is null. Cannot apply transformation: " << transformation << std::endl;
        return;

    }

    int index = findImageIndexByName(image->getFileName());  // Find the index of the image in the session

    if (index != -1) // Save the current state of the image if it's found
    {

        saveState(image, index);

    }

    try // Apply the specified transformation
    {

        if (strcmp(transformation, "grayscale") == 0) 
        {

            image->applyGrayscale();

        }
        else if (strcmp(transformation, "monochrome") == 0) 
        {

            image->applyMonochrome();

        }
        else if (strcmp(transformation, "negative") == 0) 
        {

            image->applyNegative();

        }
        else if (strcmp(transformation, "rotate left") == 0) 
        {

            image->rotateLeft();

        }
        else if (strcmp(transformation, "rotate right") == 0) 
        {

            image->rotateRight();

        }
        else 
        {

            std::cout << "Unknown transformation: " << transformation << std::endl;

        }

    }
    catch (const std::exception& e)
    {

        std::cout << "Error applying transformation: " << e.what() << std::endl;

    }

}
