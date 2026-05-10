#include "Prescription.h"

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

Prescription::Prescription()
{
    prescriptionid = 0;
    appointmentid = 0;
    patientid = 0;
    doctorid = 0;
    date[0] = '\0';
    medicines[0] = '\0';
    notes[0] = '\0';
}

Prescription::Prescription(int presid, int apptid, int patid, int docid, const char* date, const char* medicines, const char* notes)
{
    prescriptionid = presid;
    appointmentid = apptid;
    patientid = patid;
    doctorid = docid;
    strcopy(this->date, date, 15);
    strcopy(this->medicines, medicines, 500);
    strcopy(this->notes, notes, 300);
}

int Prescription::getprescriptionid()
{ 
    return prescriptionid; 
}

int Prescription::getappointmentid()  
{ 
    return appointmentid;
}

int Prescription::getpatientid()      
{
    return patientid; 
}

int Prescription::getdoctorid()       
{ 
    return doctorid;
}

int Prescription::getid()
{
    return prescriptionid;
}

const char* Prescription::getdate()           
{ 
    return date; 
}

const char* Prescription::getmedicines()      
{
    return medicines;
}

const char* Prescription::getnotes()         
{
    return notes; 
}