#pragma once
#include "Doctor.h"
#include "Patient.h"
#include "Appointment.h"
#include "Prescription.h"
#include "Bill.h"
#include "Storage.h"
#include "FileHandler.h"

class DoctorMenu
{
public:
    static void gettodaysappointments(Doctor& doctor, Storage<Appointment>& appointments, Storage<Appointment>& out);
    static bool markcomplete(Doctor& doctor, Storage<Appointment>& appointments, int appointmentid, char errormsg[200]);
    static bool marknoshow(Doctor& doctor, Storage<Appointment>& appointments, Storage<Bill>& bills, int appointmentid, char errormsg[200]);
    static bool writeprescription(Doctor& doctor, Storage<Appointment>& appointments, Storage<Prescription>& prescriptions, int appointmentid, const char* medicines, const char* notes, char errormsg[200]);
    static bool getpatienthistory(Doctor& doctor, Storage<Patient>& patients, Storage<Appointment>& appointments, Storage<Prescription>& prescriptions, int patientid, Storage<Prescription>& out, char errormsg[200]);
    static int getnewprescriptionid(Storage<Prescription>& prescriptions);
    static void gettodaysdate(char* datebuf);
    static void sorttimeslotasc(Storage<Appointment>& appointments);
};