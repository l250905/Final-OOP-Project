#include "Patient.h"
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


Patient::Patient() : Person()
{
    age = 0;
    balance = 0;
    gender[0] = '\0';
    contact[0] = '\0';
}

Patient::Patient(int id, const char* name, const char* pass, int age, float balance, const char* gender, const char* contact) : Person(id, name, pass)
{
    this->age = age;
    this->balance = balance;
    strcopy(this->gender, gender, 5);
    strcopy(this->contact, contact, 15);
}

int Patient::getage() 
{
    return age;
}

float Patient::getbalance()
{
    return balance;
}

const char* Patient::getgender()
{
    return gender;
}

const char* Patient::getcontact()
{ 
    return contact;
}


void Patient::setbalance(float b)
{ 
    balance = b;
}

Patient& Patient::operator+=(float amount)
{ 
    balance += amount;
    return *this;
}


Patient& Patient::operator-=(float amount)
{ 
    balance -= amount;
    return *this;
}

bool Patient::operator==(const Patient& other) 
{
    return id == other.id;
}

ostream& operator<<(ostream& out, const Patient& p) {
    out << "ID: " << p.id << " | Name: " << p.name << " | Age: " << p.age << " | Gender: " << p.gender << " | Contact: " << p.contact << " | Balance: PKR " << p.balance;
    return out;
}

void Patient::displaymenu() {};

void Patient::displayinfo() 
{ 
    cout << "ID: " << id << " | Name: " << name << " | Age: " << age << " | Gender: " << gender << " | Contact: " << contact << " | Balance: PKR " << balance;
}