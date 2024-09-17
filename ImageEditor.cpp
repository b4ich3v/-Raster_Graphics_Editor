#include "ImageEditor.h"
#include <iostream>
#include <sstream>
#include <cstring>

ImageEditor::ImageEditor() : currentSessionId(0)
{

    for (int i = 0; i < MAX_SESSIONS; i++) 
    {

        sessions[i] = nullptr;

    }

}

ImageEditor::~ImageEditor() 
{

    for (int i = 0; i < MAX_SESSIONS; i++) 
    {

        delete sessions[i];

    }

}

void ImageEditor::run() 
{

    char command[MAX_FILENAME_LENGTH];

    while (true) 
    {

        std::cout << "> ";
        std::cin.getline(command, MAX_FILENAME_LENGTH);

        if (strcmp(command, "exit") == 0) 
        {

            break;

        }
        try 
        {

            processCommand(command);

        }
        catch (const std::exception& e)
        {

            std::cout << "Error: " << e.what() << std::endl;

        }

    }

}

void ImageEditor::processCommand(const char* command) 
{

    std::istringstream iss(command);
    char cmd[MAX_FILENAME_LENGTH];
    iss >> cmd;

    if (strcmp(cmd, "load") == 0) 
    {

        char fileName[MAX_FILENAME_LENGTH];
        iss >> fileName;

        if (!iss) 
        {

            std::cout << "Error: Filename is required for the load command.\n";
            return;

        }
        try 
        {

            currentSessionId++;
            sessions[currentSessionId - 1] = new Session(currentSessionId);
            sessions[currentSessionId - 1]->addImage(fileName);
            std::cout << "Session with ID: " << currentSessionId << " started.\n";

        }
        catch (const std::exception& e) 
        {

            std::cout << "Error loading image: " << e.what() << std::endl;
            currentSessionId--;

        }

    }
    else if (strcmp(cmd, "help") == 0) 
    {

        displayHelp();

    }
    else if (strcmp(cmd, "add") == 0)
    {

        if (currentSessionId == 0)
        {

            std::cout << "Error: No active session. Load an image first to create a session.\n";
            return;

        }

        char fileName[MAX_FILENAME_LENGTH];
        iss >> fileName;

        if (!iss) 
        {

            std::cout << "Error: Filename is required for the add command.\n";
            return;

        }
        try 
        {

            sessions[currentSessionId - 1]->addImage(fileName);

        }
        catch (const std::exception& e) 
        {

            std::cout << "Error adding image: " << e.what() << std::endl;

        }

    }
    else if (strcmp(cmd, "save") == 0) 
    {

        if (currentSessionId == 0) 
        {

            std::cout << "Error: No active session. Load an image first to create a session.\n";
            return;

        }

        Session* currentSession = sessions[currentSessionId - 1];

        for (int i = 0; i < currentSession->getNumImages(); i++)
        {

            Image* image = currentSession->getImage(i);
            image->save(image->getFileName());
            std::cout << "Image \"" << image->getFileName() << "\" updated\n";

        }

    }
    else if (strcmp(cmd, "saveas") == 0) 
    {

        if (currentSessionId == 0) 
        {

            std::cout << "Error: No active session. Load an image first to create a session.\n";
            return;

        }

        char fileName[MAX_FILENAME_LENGTH];
        iss >> fileName;

        if (!iss) 
        {

            std::cout << "Error: Filename is required for the saveas command.\n";
            return;

        }

        sessions[currentSessionId - 1]->saveAs(fileName);

    }
    else if (strcmp(cmd, "grayscale") == 0)
    {

        if (currentSessionId == 0) 
        {

            std::cout << "Error: No active session. Load an image first to create a session.\n";
            return;

        }

        sessions[currentSessionId - 1]->applyTransformation("grayscale");

    }
    else if (strcmp(cmd, "monochrome") == 0)
    {

        if (currentSessionId == 0) 
        {

            std::cout << "Error: No active session. Load an image first to create a session.\n";
            return;

        }

        sessions[currentSessionId - 1]->applyTransformation("monochrome");

    }
    else if (strcmp(cmd, "negative") == 0) 
    {

        if (currentSessionId == 0) 
        {

            std::cout << "Error: No active session. Load an image first to create a session.\n";
            return;

        }

        sessions[currentSessionId - 1]->applyTransformation("negative");

    }
    else if (strcmp(cmd, "rotate") == 0) 
    {

        if (currentSessionId == 0)
        {

            std::cout << "Error: No active session. Load an image first to create a session.\n";
            return;

        }

        char direction[MAX_FILENAME_LENGTH];
        iss >> direction;

        if (strcmp(direction, "left") == 0 || strcmp(direction, "right") == 0) 
        {

            char transformation[MAX_FILENAME_LENGTH];
            strcpy(transformation, "rotate ");
            strcat(transformation, direction);
            sessions[currentSessionId - 1]->applyTransformation(transformation);

        }
        else 
        {

            std::cout << "Invalid rotate direction\n";

        }

    }
    else if (strcmp(cmd, "undo") == 0) 
    {

        if (currentSessionId == 0)
        {

            std::cout << "Error: No active session. Load an image first to create a session.\n";
            return;

        }

        sessions[currentSessionId - 1]->undo();

    }
    else if (strcmp(cmd, "session") == 0) 
    {

        if (currentSessionId == 0) 
        {

            std::cout << "Error: No active session. Load an image first to create a session.\n";
            return;

        }

        char info[MAX_FILENAME_LENGTH];
        iss >> info;

        if (strcmp(info, "info") == 0) 
        {

            sessions[currentSessionId - 1]->sessionInfo();

        }
        else 
        {

            std::cout << "Error: Unknown session command.\n";

        }

    }
    else if (strcmp(cmd, "switch") == 0)
    {

        int sessionId;
        iss >> sessionId;

        if (!iss) 
        {

            std::cout << "Error: Session ID is required for the switch command.\n";
            return;
        }

        bool found = false;

        for (int i = 0; i < MAX_SESSIONS; i++) 
        {
            if (sessions[i] != nullptr && sessions[i]->getId() == sessionId) 
            {

                currentSessionId = sessionId;
                found = true;
                std::cout << "Switched to session with ID: " << currentSessionId << "\n";
                break;

            }

        }
        if (!found)
        {

            std::cout << "Session with ID " << sessionId << " does not exist.\n";

        }

    }
    else if (strcmp(cmd, "collage") == 0) 
    {

        if (currentSessionId == 0) 
        {

            std::cout << "Error: No active session. Load an image first to create a session.\n";
            return;

        }

        char direction[MAX_FILENAME_LENGTH];
        char image1[MAX_FILENAME_LENGTH];
        char image2[MAX_FILENAME_LENGTH];
        char outImage[MAX_FILENAME_LENGTH];
        iss >> direction >> image1 >> image2 >> outImage;

        if (strcmp(direction, "horizontal") == 0 || strcmp(direction, "vertical") == 0)
        {

            sessions[currentSessionId - 1]->createCollage(direction, image1, image2);

        }
        else 
        {

            std::cout << "Invalid collage direction.\n";

        }

    }
    else 
    {

        std::cout << "Unknown command.\n";

    }

}

void ImageEditor::displayHelp() 
{

    std::cout << "Supported commands:\n";
    std::cout << "  load <filename>            - Load an image and start a new session\n";
    std::cout << "  add <filename>             - Add an image to the current session\n";
    std::cout << "  save                       - Save all images in the current session\n";
    std::cout << "  saveas <filename>          - Save the first image in the session under a new name\n";
    std::cout << "  grayscale                  - Apply grayscale transformation to all images in the session\n";
    std::cout << "  monochrome                 - Apply monochrome transformation to all images in the session\n";
    std::cout << "  negative                   - Apply negative transformation to all images in the session\n";
    std::cout << "  rotate <left|right>        - Rotate all images in the session 90 degrees in the specified direction\n";
    std::cout << "  undo                       - Undo the last transformation applied in the session\n";
    std::cout << "  session info               - Display information about the current session\n";
    std::cout << "  switch <session_id>        - Switch to another session by its ID\n";
    std::cout << "  collage <direction> <image1> <image2> <outimage>  - Create a collage from two images in the specified direction\n";
    std::cout << "  help                       - Display this help message\n";
    std::cout << "  exit                       - Exit the application\n";

}
