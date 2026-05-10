#pragma once

class Bill 
{
    int   billid;
    int   patientid;
    int   appointmentid;
    float amount;
    char  status[15];   // unpaid, paid, cancelled
    char  date[15];

public:
    Bill();
    Bill(int billid, int patientid, int appointmentid, float amount, const char* status, const char* date);

    int getbillid();
    int getpatientid();
    int getappointmentid();
    int getid();
    float getamount();
    const char* getstatus();
    const char* getdate();

    void setStatus(const char* s);
};