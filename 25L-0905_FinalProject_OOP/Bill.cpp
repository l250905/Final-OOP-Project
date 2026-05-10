#include "Bill.h"

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

Bill::Bill()
{
    billid = 0;
    patientid = 0;
    appointmentid = 0;
    amount = 0;
    status[0] = '\0';
    date[0] = '\0';
}

Bill::Bill(int billid, int patientid, int appointmentid, float amount, const char* status, const char* date)
{
    this->billid = billid;
   this->patientid = patientid;
    this->appointmentid = appointmentid;
    this->amount = amount;
    strcopy(this->status, status, 15);
    strcopy(this->date, date, 15);
}

int Bill::getbillid() 
{ 
    return billid;
}

int Bill::getpatientid() 
{ 
    return patientid;
}

int Bill::getappointmentid()
{
    return appointmentid;
}

int Bill::getid()
{
    return billid;
}

float Bill::getamount()    
{ 
    return amount;
}

const char* Bill::getstatus() 
{ 
    return status;
}

const char* Bill::getdate()   
{ 
    return date;
}


void Bill::setStatus(const char* s) 
{ 
    strcopy(status, s, 15);
}