#pragma once
#include "Person.h"

class Admin : public Person 
{
public:
    Admin();
    Admin(int id, const char* name, const char* pass);

    void displaymenu() override;
    void displayinfo() override;
};