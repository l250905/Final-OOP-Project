#pragma once
#include "Patient.h"
#include "Doctor.h"
#include "Admin.h"
#include "Appointment.h"
#include "Bill.h"
#include "Prescription.h"
#include "Storage.h"

class FileHandler
{
public:
    static bool loadpatients(Storage<Patient>& out);
    static bool loaddoctors(Storage<Doctor>& out);
    static bool loadadmins(Storage<Admin>& out);
    static bool loadappointments(Storage<Appointment>& out);
    static bool loadbills(Storage<Bill>& out);
    static bool loadprescriptions(Storage<Prescription>& out);

   
    static void savepatients(Storage<Patient>& patients);
    static void savedoctors(Storage<Doctor>& doctors);
    static void saveappointments(Storage<Appointment>& appts);
    static void savebills(Storage<Bill>& bills);
    static void saveprescriptions(Storage<Prescription>& prescriptions);

    static void appendpatient(Patient& p);
    static void appenddoctor(Doctor& d);
    static void appendappointment(Appointment& a);
    static void appendbill(Bill& b);
    static void appendprescription(Prescription& p);

    static void appendsecuritylog(const char* timestamp, const char* role,
        const char* enteredid, const char* result);
    static void loadsecuritylog(char* buffer, int buffersize);

    static void appenddischarged(Patient& p);

   
    static void ensurefilesexist();   
    static void setupwizard();     

private:
    static void inttostr(int val, char* buf);
    static void floattostr(float val, char* buf);
    static int strtoint(const char* s);
    static float strtofloat(const char* s);
    static void readfield(const char*& cur, char* field, int maxlen);


    static void wizardclearline();
    static void wizardreadstr(const char* prompt, char* buf, int maxlen);
    static int  wizardreadint(const char* prompt);
    static float wizardreadfloat(const char* prompt);
    static bool wizardiscontactvalid(const char* c);
    static bool wizardispassvalid(const char* p);
    static bool wizardisagevalid(int a);
    static bool wizardisgendervalid(const char* g);
    static int  wizardstrlen(const char* s);
    static bool wizardisdigitsonly(const char* s);
    static void wizardwriteadmins();
    static void wizardwritedoctors();
    static void wizardwritepatients();
};