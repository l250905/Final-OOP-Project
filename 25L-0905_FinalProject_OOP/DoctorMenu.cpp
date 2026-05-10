#define _CRT_SECURE_NO_WARNINGS
#include "DoctorMenu.h"
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

void DoctorMenu::gettodaysdate(char* datebuf)
{
    time_t t = time(nullptr);
    tm* now = localtime(&t);
    int day = now->tm_mday;
    int month = now->tm_mon + 1;
    int year = now->tm_year + 1900;

    datebuf[0] = '0' + day / 10;
    datebuf[1] = '0' + day % 10;
    datebuf[2] = '-';
    datebuf[3] = '0' + month / 10;
    datebuf[4] = '0' + month % 10;
    datebuf[5] = '-';
    datebuf[6] = '0' + year / 1000;
    datebuf[7] = '0' + (year % 1000) / 100;
    datebuf[8] = '0' + (year % 100) / 10;
    datebuf[9] = '0' + year % 10;
    datebuf[10] = '\0';
}

int DoctorMenu::getnewprescriptionid(Storage<Prescription>& prescriptions)
{
    int maxid = 0;
    for (int i = 0; i < prescriptions.size(); i++)
    {
        if (prescriptions.get(i).getprescriptionid() > maxid)
        {
            maxid = prescriptions.get(i).getprescriptionid();
        }
    }
    return maxid + 1;
}

void DoctorMenu::gettodaysappointments(Doctor& doctor, Storage<Appointment>& appointments, Storage<Appointment>& out)
{
    char today[11];
    gettodaysdate(today);

    for (int i = 0; i < appointments.size(); i++)
    {
        Appointment& a = appointments.get(i);
        if (a.getdoctorid() == doctor.getid() && strequal(a.getdate(), today))
        {
            out.add(a);
        }
    }
    sorttimeslotasc(out);
}

void DoctorMenu::sorttimeslotasc(Storage<Appointment>& appointments)
{
    for (int i = 0; i < appointments.size() - 1; i++)
    {
        for (int j = 0; j < appointments.size() - i - 1; j++)
        {
            const char* slot1 = appointments.get(j).gettimeslot();
            const char* slot2 = appointments.get(j + 1).gettimeslot();

            int hour1 = (slot1[0] - '0') * 10 + (slot1[1] - '0');
            int hour2 = (slot2[0] - '0') * 10 + (slot2[1] - '0');

            if (hour1 > hour2)
            {
                Appointment temp = appointments.get(j);
                appointments.get(j) = appointments.get(j + 1);
                appointments.get(j + 1) = temp;
            }
        }
    }
}

bool DoctorMenu::markcomplete(Doctor& doctor, Storage<Appointment>& appointments, int appointmentid, char errormsg[200])
{
    char today[11];
    gettodaysdate(today);

    for (int i = 0; i < appointments.size(); i++)
    {
        Appointment& a = appointments.get(i);
        if (a.getappointmentid() == appointmentid && a.getdoctorid() == doctor.getid() && strequal(a.getstatus(), "pending") && strequal(a.getdate(), today))
        {
            a.setStatus("completed");
            FileHandler::saveappointments(appointments);
            strcopy(errormsg, "Appointment marked as completed.", 200);
            return true;
        }
    }

    strcopy(errormsg, "Invalid appointment ID.", 200);
    return false;
}

bool DoctorMenu::marknoshow(Doctor& doctor, Storage<Appointment>& appointments, Storage<Bill>& bills, int appointmentid, char errormsg[200])
{
    char today[11];
    gettodaysdate(today);

    for (int i = 0; i < appointments.size(); i++)
    {
        Appointment& a = appointments.get(i);
        if (a.getappointmentid() == appointmentid && a.getdoctorid() == doctor.getid() && strequal(a.getstatus(), "pending") && strequal(a.getdate(), today))
        {
            a.setStatus("noshow");

            for (int j = 0; j < bills.size(); j++)
            {
                if (bills.get(j).getappointmentid() == appointmentid && strequal(bills.get(j).getstatus(), "unpaid"))
                {
                    bills.get(j).setStatus("cancelled");
                    break;
                }
            }

            FileHandler::saveappointments(appointments);
            FileHandler::savebills(bills);
            strcopy(errormsg, "Appointment marked as no-show.", 200);
            return true;
        }
    }

    strcopy(errormsg, "Invalid appointment ID.", 200);
    return false;
}

bool DoctorMenu::writeprescription(Doctor& doctor, Storage<Appointment>& appointments, Storage<Prescription>& prescriptions, int appointmentid, const char* medicines, const char* notes, char errormsg[200])
{
    bool apptfound = false;
    for (int i = 0; i < appointments.size(); i++)
    {
        Appointment& a = appointments.get(i);
        if (a.getappointmentid() == appointmentid && a.getdoctorid() == doctor.getid() && strequal(a.getstatus(), "completed"))
        {
            apptfound = true;
            break;
        }
    }

    if (!apptfound)
    {
        strcopy(errormsg, "Invalid appointment ID.", 200);
        return false;
    }

    for (int i = 0; i < prescriptions.size(); i++)
    {
        if (prescriptions.get(i).getappointmentid() == appointmentid)
        {
            strcopy(errormsg, "Prescription already written for this appointment.", 200);
            return false;
        }
    }

    char today[11];
    gettodaysdate(today);

    int newpresid = getnewprescriptionid(prescriptions);

    int patientid = 0;
    for (int i = 0; i < appointments.size(); i++)
    {
        if (appointments.get(i).getappointmentid() == appointmentid)
        {
            patientid = appointments.get(i).getpatientid();
            break;
        }
    }

    Prescription newpres(newpresid, appointmentid, patientid, doctor.getid(), today, medicines, notes);
    prescriptions.add(newpres);
    FileHandler::appendprescription(newpres);

    strcopy(errormsg, "Prescription saved.", 200);
    return true;
}

bool DoctorMenu::getpatienthistory(Doctor& doctor, Storage<Patient>& patients, Storage<Appointment>& appointments, Storage<Prescription>& prescriptions, int patientid, Storage<Prescription>& out, char errormsg[200])
{
    bool patientfound = false;
    for (int i = 0; i < patients.size(); i++)
    {
        if (patients.get(i).getid() == patientid)
        {
            patientfound = true;
            break;
        }
    }

    if (!patientfound)
    {
        strcopy(errormsg, "Patient not found.", 200);
        return false;
    }

    bool hascompletedappt = false;
    for (int i = 0; i < appointments.size(); i++)
    {
        Appointment& a = appointments.get(i);
        if (a.getpatientid() == patientid && a.getdoctorid() == doctor.getid() && strequal(a.getstatus(), "completed"))
        {
            hascompletedappt = true;
            break;
        }
    }

    if (!hascompletedappt)
    {
        strcopy(errormsg, "Access denied. You can only view records of your own patients.", 200);
        return false;
    }

    for (int i = 0; i < prescriptions.size(); i++)
    {
        Prescription& p = prescriptions.get(i);
        if (p.getpatientid() == patientid && p.getdoctorid() == doctor.getid())
        {
            out.add(p);
        }
    }

    strcopy(errormsg, "", 200);
    return true;
}