#pragma once
#include <iostream>

class Appointment 
{
    int  appointmentid;
    int  patientid;
    int  doctorid;
    char date[15];      
    char timeslot[10];  
    char status[15]; 

public:
    Appointment();
    Appointment(int appid, int patid, int docid, const char* date, const char* timeslot, const char* status);

    int getappointmentid();
    int getpatientid();
    int getdoctorid();
    int getid();
    const char* getdate();
    const char* gettimeslot();
    const char* getstatus();

    void setStatus(const char* s);

    bool operator==(const Appointment& other);

    friend std::ostream& operator<<(std::ostream& out, const Appointment& a);
};