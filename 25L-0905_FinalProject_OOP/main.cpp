#include "FileHandler.h"
#include "GUI.h"

int main()
{
    FileHandler::ensurefilesexist();

    FileHandler::setupwizard();

    GUI gui;
    gui.run();

}