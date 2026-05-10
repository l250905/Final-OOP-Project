#pragma once
#include "Admin.h"
#include "Patient.h"
#include "Doctor.h"
#include "Appointment.h"
#include "Bill.h"
#include "Prescription.h"
#include "Storage.h"
#include "FileHandler.h"
#include "Validator.h"

class AdminMenu
{
public:
    static bool adddoctor(Storage<Doctor>& doctors, const char* name, const char* spec, const char* contact, const char* pass, float fee, char errormsg[200]);
    static bool removedoctor(Storage<Doctor>& doctors, Storage<Appointment>& appointments, int doctorid, char errormsg[200]);
    static bool dischargepatient(Storage<Patient>& patients, Storage<Appointment>& appointments, Storage<Bill>& bills, Storage<Prescription>& prescriptions, int patientid, char errormsg[200]);
    static void getallunpaidbills(Storage<Bill>& bills, Storage<Bill>& out);
    static void getallappointmentssorteddesc(Storage<Appointment>& appointments, Storage<Appointment>& out);
    static int getnewdoctorid(Storage<Doctor>& doctors);
    static bool isoverdue(const char* billdate);
};