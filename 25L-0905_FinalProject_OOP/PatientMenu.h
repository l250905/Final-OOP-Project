#pragma once

#include "Patient.h"
#include "Doctor.h"
#include "Appointment.h"
#include "Bill.h"
#include "Prescription.h"
#include "Storage.h"
#include "FileHandler.h"
#include "Validator.h"

class PatientMenu
{
public:
    static bool bookappointment(Patient& patient, Storage<Patient>& patients, Storage<Doctor>& doctors, Storage<Appointment>& appointments, Storage<Bill>& bills, int doctorid, const char* date, const char* timeslot, char errormsg[200]);
    static bool cancelappointment(Patient& patient, Storage<Patient>& patients, Storage<Appointment>& appointments, Storage<Bill>& bills, int appointmentid, char errormsg[200]);
    static bool paybill(Patient& patient, Storage<Patient>& patients, Storage<Bill>& bills, int billid, char errormsg[200]);
    static bool topupbalance(Patient& patient, Storage<Patient>& patients, float amount, char errormsg[200]);
    static void getpatientappointments(Patient& patient, Storage<Appointment>& appointments, Storage<Appointment>& out);
    static void getpatientbills(Patient& patient, Storage<Bill>& bills, Storage<Bill>& out);
    static void getpatientprescriptions(Patient& patient, Storage<Prescription>& prescriptions, Storage<Prescription>& out);
    static bool isslotavailable(int doctorid, const char* date, const char* timeslot, Storage<Appointment>& appointments);
    static int getnewappointmentid(Storage<Appointment>& appointments);
    static int getnewbillid(Storage<Bill>& bills);
    static void sortappointmentsasc(Storage<Appointment>& appointments);
    static void sortprescriptionsdesc(Storage<Prescription>& prescriptions);
};