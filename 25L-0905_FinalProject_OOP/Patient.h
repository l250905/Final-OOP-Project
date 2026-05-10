#pragma once
#include "Person.h"
#include <iostream>


class Patient : public Person {
    int   age;
    char  gender[5];
    char  contact[15];
    float balance;

public:
    Patient();
    Patient(int id, const char* name, const char* pass, int age, float balance, const char* gender, const char* contact);

    int getage();
    const char* getgender();
    const char* getcontact();
    float getbalance();

    void setbalance(float b);

    
    Patient& operator+=(float amount);           
    Patient& operator-=(float amount);          
    bool operator==(const Patient& other);

    friend std::ostream& operator<<(std::ostream& out, const Patient& p);

    void displaymenu() override;
    void displayinfo()  override;

};