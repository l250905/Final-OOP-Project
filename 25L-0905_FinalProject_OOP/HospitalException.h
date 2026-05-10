#pragma once


class HospitalException
{
protected:
    char message[200];
public:
    HospitalException(const char* msg);

    virtual const char* what();
};