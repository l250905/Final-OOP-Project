#pragma once

class Prescription {
private:
    int  prescriptionid;
    int  appointmentid;
    int  patientid;
    int  doctorid;
    char date[15];
    char medicines[500];
    char notes[300];

public:
    Prescription();
    Prescription(int presid, int apptid, int patid, int docid, const char* date, const char* medicines, const char* notes);

    int getprescriptionid();
    int getappointmentid();
    int getpatientid();
    int getdoctorid();
    int getid();
    const char* getdate();
    const char* getmedicines();
    const char* getnotes();
};