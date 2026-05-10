#include "PatientMenu.h"
#include <iostream>
#include <ctime>
using namespace std;

static bool strequal(const char* a, const char* b)
{
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0')
    {
        if (a[i] != b[i])
        {
            return false;
        }
        i++;
    }
    return a[i] == b[i];
}

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

bool PatientMenu::isslotavailable(int doctorid, const char* date, const char* timeslot, Storage<Appointment>& appointments)
{
    for (int i = 0; i < appointments.size(); i++)
    {
        Appointment& a = appointments.get(i);
        if (a.getdoctorid() == doctorid && strequal(a.getdate(), date) && strequal(a.gettimeslot(), timeslot) && !strequal(a.getstatus(), "cancelled"))
        {
            return false;
        }
    }
    return true;
}

int PatientMenu::getnewappointmentid(Storage<Appointment>& appointments)
{
    int maxid = 0;
    for (int i = 0; i < appointments.size(); i++)
    {
        if (appointments.get(i).getappointmentid() > maxid)
        {
            maxid = appointments.get(i).getappointmentid();
        }
    }
    return maxid + 1;
}

int PatientMenu::getnewbillid(Storage<Bill>& bills)
{
    int maxid = 0;
    for (int i = 0; i < bills.size(); i++)
    {
        if (bills.get(i).getbillid() > maxid)
        {
            maxid = bills.get(i).getbillid();
        }
    }
    return maxid + 1;
}

bool PatientMenu::bookappointment(Patient& patient, Storage<Patient>& patients, Storage<Doctor>& doctors, Storage<Appointment>& appointments, Storage<Bill>& bills, int doctorid, const char* date, const char* timeslot, char errormsg[200])
{
    Doctor* doctor = nullptr;
    for (int i = 0; i < doctors.size(); i++)
    {
        if (doctors.get(i).getid() == doctorid)
        {
            doctor = &doctors.get(i);
            break;
        }
    }

    if (doctor == nullptr)
    {
        strcopy(errormsg, "Doctor not found.", 200);
        return false;
    }

    if (!isslotavailable(doctorid, date, timeslot, appointments))
    {
        strcopy(errormsg, "This slot is not available.", 200);
        return false;
    }

    if (patient.getbalance() < doctor->getfee())
    {
        strcopy(errormsg, "Insufficient funds.", 200);
        return false;
    }

    patient -= doctor->getfee();

    int newapptid = getnewappointmentid(appointments);
    int newbillid = getnewbillid(bills);

    Appointment newappt(newapptid, patient.getid(), doctorid, date, timeslot, "pending");
    appointments.add(newappt);
    FileHandler::appendappointment(newappt);

    Bill newbill(newbillid, patient.getid(), newapptid, doctor->getfee(), "unpaid", date);
    bills.add(newbill);
    FileHandler::appendbill(newbill);

    FileHandler::savepatients(patients);

    strcopy(errormsg, "Appointment booked successfully.", 200);
    return true;
}

bool PatientMenu::cancelappointment(Patient& patient, Storage<Patient>& patients, Storage<Appointment>& appointments, Storage<Bill>& bills, int appointmentid, char errormsg[200])
{
    Appointment* appt = nullptr;
    for (int i = 0; i < appointments.size(); i++)
    {
        if (appointments.get(i).getappointmentid() == appointmentid && appointments.get(i).getpatientid() == patient.getid() && strequal(appointments.get(i).getstatus(), "pending"))
        {
            appt = &appointments.get(i);
            break;
        }
    }

    if (appt == nullptr)
    {
        strcopy(errormsg, "Invalid appointment ID.", 200);
        return false;
    }

    appt->setStatus("cancelled");

    for (int i = 0; i < bills.size(); i++)
    {
        if (bills.get(i).getappointmentid() == appointmentid && strequal(bills.get(i).getstatus(), "unpaid"))
        {
            patient += bills.get(i).getamount();
            bills.get(i).setStatus("cancelled");
            break;
        }
    }

    FileHandler::saveappointments(appointments);
    FileHandler::savebills(bills);
    FileHandler::savepatients(patients);

    strcopy(errormsg, "Appointment cancelled successfully.", 200);
    return true;
}

bool PatientMenu::paybill(Patient& patient, Storage<Patient>& patients, Storage<Bill>& bills, int billid, char errormsg[200])
{
    Bill* bill = nullptr;
    for (int i = 0; i < bills.size(); i++)
    {
        if (bills.get(i).getbillid() == billid && bills.get(i).getpatientid() == patient.getid() && strequal(bills.get(i).getstatus(), "unpaid"))
        {
            bill = &bills.get(i);
            break;
        }
    }

    if (bill == nullptr)
    {
        strcopy(errormsg, "Invalid bill ID.", 200);
        return false;
    }

    if (patient.getbalance() < bill->getamount())
    {
        strcopy(errormsg, "Insufficient funds.", 200);
        return false;
    }

    patient -= bill->getamount();
    bill->setStatus("paid");

    FileHandler::savebills(bills);
    FileHandler::savepatients(patients);

    strcopy(errormsg, "Bill paid successfully.", 200);
    return true;
}

bool PatientMenu::topupbalance(Patient& patient, Storage<Patient>& patients, float amount, char errormsg[200])
{
    if (amount <= 0)
    {
        strcopy(errormsg, "Amount must be greater than 0.", 200);
        return false;
    }

    patient += amount;
    FileHandler::savepatients(patients);

    strcopy(errormsg, "Balance updated successfully.", 200);
    return true;
}

void PatientMenu::getpatientappointments(Patient& patient, Storage<Appointment>& appointments, Storage<Appointment>& out)
{
    for (int i = 0; i < appointments.size(); i++)
    {
        if (appointments.get(i).getpatientid() == patient.getid())
        {
            out.add(appointments.get(i));
        }
    }
    sortappointmentsasc(out);
}

void PatientMenu::getpatientbills(Patient& patient, Storage<Bill>& bills, Storage<Bill>& out)
{
    for (int i = 0; i < bills.size(); i++)
    {
        if (bills.get(i).getpatientid() == patient.getid())
        {
            out.add(bills.get(i));
        }
    }
}

void PatientMenu::getpatientprescriptions(Patient& patient, Storage<Prescription>& prescriptions, Storage<Prescription>& out)
{
    for (int i = 0; i < prescriptions.size(); i++)
    {
        if (prescriptions.get(i).getpatientid() == patient.getid())
        {
            out.add(prescriptions.get(i));
        }
    }
    sortprescriptionsdesc(out);
}

void PatientMenu::sortappointmentsasc(Storage<Appointment>& appointments)
{
    for (int i = 0; i < appointments.size() - 1; i++)
    {
        for (int j = 0; j < appointments.size() - i - 1; j++)
        {
            const char* date1 = appointments.get(j).getdate();
            const char* date2 = appointments.get(j + 1).getdate();

            int day1 = (date1[0] - '0') * 10 + (date1[1] - '0');
            int month1 = (date1[3] - '0') * 10 + (date1[4] - '0');
            int year1 = (date1[6] - '0') * 1000 + (date1[7] - '0') * 100 + (date1[8] - '0') * 10 + (date1[9] - '0');

            int day2 = (date2[0] - '0') * 10 + (date2[1] - '0');
            int month2 = (date2[3] - '0') * 10 + (date2[4] - '0');
            int year2 = (date2[6] - '0') * 1000 + (date2[7] - '0') * 100 + (date2[8] - '0') * 10 + (date2[9] - '0');

            bool needswap = false;
            if (year1 > year2) needswap = true;
            else if (year1 == year2 && month1 > month2) needswap = true;
            else if (year1 == year2 && month1 == month2 && day1 > day2) needswap = true;

            if (needswap)
            {
                Appointment temp = appointments.get(j);
                appointments.get(j) = appointments.get(j + 1);
                appointments.get(j + 1) = temp;
            }
        }
    }
}

void PatientMenu::sortprescriptionsdesc(Storage<Prescription>& prescriptions)
{
    for (int i = 0; i < prescriptions.size() - 1; i++)
    {
        for (int j = 0; j < prescriptions.size() - i - 1; j++)
        {
            const char* date1 = prescriptions.get(j).getdate();
            const char* date2 = prescriptions.get(j + 1).getdate();

            int day1 = (date1[0] - '0') * 10 + (date1[1] - '0');
            int month1 = (date1[3] - '0') * 10 + (date1[4] - '0');
            int year1 = (date1[6] - '0') * 1000 + (date1[7] - '0') * 100 + (date1[8] - '0') * 10 + (date1[9] - '0');

            int day2 = (date2[0] - '0') * 10 + (date2[1] - '0');
            int month2 = (date2[3] - '0') * 10 + (date2[4] - '0');
            int year2 = (date2[6] - '0') * 1000 + (date2[7] - '0') * 100 + (date2[8] - '0') * 10 + (date2[9] - '0');

            bool needswap = false;
            if (year1 < year2) needswap = true;
            else if (year1 == year2 && month1 < month2) needswap = true;
            else if (year1 == year2 && month1 == month2 && day1 < day2) needswap = true;

            if (needswap)
            {
                Prescription temp = prescriptions.get(j);
                prescriptions.get(j) = prescriptions.get(j + 1);
                prescriptions.get(j + 1) = temp;
            }
        }
    }
}