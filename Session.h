#pragma once

#include "Image.h"

class Session
{
private:

    int id;
    int numImages;
    int numTransformations;
    Image* images[MAX_IMAGES];
    char transformations[MAX_TRANSFORMATIONS][MAX_FILENAME_LENGTH];
    Image* imageStates[MAX_IMAGES][MAX_TRANSFORMATIONS];
    int stateIndex[MAX_IMAGES];

    void saveState(Image* image, int index);
    void restoreState(int index);
    int findImageIndexByName(const char* name) const;
    Image* findImageByName(const char* name) const;
    void addImage(Image* image);
    void applyTransformationToImage(Image* image, const char* transformation);

public:

    Session();
    Session(int id);
    ~Session();

    void addImage(const char* fileName);
    void applyTransformation(const char* transformation);
    void save();
    int getId() const;
    Image* getImage(int index) const;
    int getNumImages() const;
    void saveAs(const char* fileName);
    void undo();
    void sessionInfo() const;
    void createCollage(const char* direction, const char* image1, const char* image2);

};
