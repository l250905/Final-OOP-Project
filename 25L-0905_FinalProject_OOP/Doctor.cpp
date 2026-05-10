#include "Doctor.h"
#include <iostream>
using namespace std;

static void strcopy(char* dest, const char* src, int len)
{
    int index = 0;
    while (src[index] != '\0' && index < len - 1)
    {
        dest[index] = src[index];
        index++;
    }
    dest[index] = '\0';
}

Doctor::Doctor() : Person()
{
    specialization[0] = '\0';
    contact[0] = '\0';
    fee = 0;
}

Doctor::Doctor(int id, const char* name, const char* pass, const char* specialization, const char* contact, float fee) : Person(id, name, pass)
{
    strcopy(this->specialization, specialization, 50);
    strcopy(this->contact, contact, 15);
    fee = 0;
}

const char* Doctor::getspecialization()
{ 
    return specialization;
}

const char* Doctor::getcontact()
{ 
    return contact;
}

float Doctor::getfee() 
{ 
    return fee;
}


void Doctor::setfee(float f)
{
    fee = f;
}

bool Doctor::operator==(const Doctor& other)
{
    return id == other.id;
}

ostream& operator<<(ostream& out, const Doctor& d) 
{
    out << "ID: " << d.id << " | Name: " << d.name << " | Spec: " << d.specialization << " | Contact: " << d.contact << " | Fee: PKR " << d.fee;
    return out;
}

void Doctor::displaymenu() {};

void Doctor::displayinfo() 
{
    cout << "ID: " << id << " | Name: " << name << " | Spec: " << specialization << " | Contact: " << contact << " | Fee: PKR " << fee;
}