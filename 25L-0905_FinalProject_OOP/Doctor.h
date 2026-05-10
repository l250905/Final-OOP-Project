#pragma once
#include "Person.h"
#include <iostream>

class Doctor : public Person 
{
    char specialization[50];
    char contact[15];
    float fee;
public:
    Doctor();
    Doctor(int id, const char* name, const char* pass, const char* specialization, const char* contact, float fee);

    const char* getspecialization();
    const char* getcontact();
    float getfee();

    void setfee(float f);

    bool operator==(const Doctor& other);
    friend std::ostream& operator<<(std::ostream& out, const Doctor& d);

    void displaymenu() override;
    void displayinfo() override;
};