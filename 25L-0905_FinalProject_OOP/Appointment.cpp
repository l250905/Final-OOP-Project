#include "Appointment.h"
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


static bool strcomp(const char* a, const char* b) {
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0')
    {
        if (a[i] != b[i])
        {
            return false;
        }
        i++;
    }

    if (a[i] == b[i])
    {
        return true;
    }
    else
    {
        return false;
    }
}

Appointment::Appointment()
{
    appointmentid = 0;
    patientid = 0;
    doctorid = 0;
    date[0] = '\0';
    timeslot[0] = '\0';
    status[0] = '\0';

}

Appointment::Appointment(int appid, int patid, int docid, const char* date, const char* timeslot, const char* status) 
{
    appointmentid = appid;
    patientid = patid;
    doctorid = docid;
    strcopy(this->date, date, 15);
    strcopy(this->timeslot, timeslot, 10);
    strcopy(this->status, status, 15);
}

int Appointment::getappointmentid()  
{ 
    return appointmentid;
}

int Appointment::getpatientid() 
{
    return patientid;
}

int Appointment::getdoctorid()      
{ 
    return doctorid;
}

int Appointment::getid()
{
    return appointmentid;
}

const char* Appointment::getdate()          
{
    return date;
}

const char* Appointment::gettimeslot()      
{ 
    return timeslot;
}

const char* Appointment::getstatus()        
{ 
    return status;
}


void Appointment::setStatus(const char* s)
{
    strcopy(status, s, 15);
}


bool Appointment::operator==(const Appointment& other) 
{
    if (doctorid != other.doctorid)
    {
        return false;
    }

    if (!strcomp(date, other.date))
    {
        return false;
    }

    if (!strcomp(timeslot, other.timeslot))
    {
        return false;
    }

    if (strcomp(status, "cancelled"))
    {
        return false;
    }

    if (strcomp(other.status, "cancelled"))
    {
        return false;
    }

    return true;
}

ostream& operator<<(ostream& out, const Appointment& a) {
    out << "ApptID: " << a.appointmentid << " | PatID: " << a.patientid << " | DocID: " << a.doctorid << " | Date: " << a.date << " | Slot: " << a.timeslot << " | Status: " << a.status;
    return out;
}