#pragma once

#include "Session.h"

class ImageEditor
{
private:

    Session* sessions[MAX_SESSIONS];
    int currentSessionId;

public:

    ImageEditor();
    ~ImageEditor();
    void run();
    void processCommand(const char* command);
    void displayHelp();

};
