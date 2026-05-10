#include "AdminMenu.h"
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

int AdminMenu::getnewdoctorid(Storage<Doctor>& doctors)
{
    int maxid = 0;
    for (int i = 0; i < doctors.size(); i++)
    {
        if (doctors.get(i).getid() > maxid)
        {
            maxid = doctors.get(i).getid();
        }
    }
    return maxid + 1;
}

bool AdminMenu::adddoctor(Storage<Doctor>& doctors, const char* name, const char* spec, const char* contact, const char* pass, float fee, char errormsg[200])
{
    if (!Validator::iscontactvalid(contact))
    {
        strcopy(errormsg, "Invalid contact number.", 200);
        return false;
    }

    if (!Validator::ispassvalid(pass))
    {
        strcopy(errormsg, "Password must be at least 6 characters.", 200);
        return false;
    }

    if (!Validator::isfeevalid(fee))
    {
        strcopy(errormsg, "Fee must be a positive number.", 200);
        return false;
    }

    int newid = getnewdoctorid(doctors);
    Doctor newdoctor(newid, name, pass, spec, contact, fee);
    doctors.add(newdoctor);
    FileHandler::appenddoctor(newdoctor);

    strcopy(errormsg, "Doctor added successfully.", 200);
    return true;
}

bool AdminMenu::removedoctor(Storage<Doctor>& doctors, Storage<Appointment>& appointments, int doctorid, char errormsg[200])
{
    bool found = false;
    for (int i = 0; i < doctors.size(); i++)
    {
        if (doctors.get(i).getid() == doctorid)
        {
            found = true;
            break;
        }
    }

    if (!found)
    {
        strcopy(errormsg, "Doctor not found.", 200);
        return false;
    }

    for (int i = 0; i < appointments.size(); i++)
    {
        Appointment& a = appointments.get(i);
        if (a.getdoctorid() == doctorid && strequal(a.getstatus(), "pending"))
        {
            strcopy(errormsg, "Cannot remove doctor with pending appointments.", 200);
            return false;
        }
    }

    doctors.removebyid(doctorid);
    FileHandler::savedoctors(doctors);

    strcopy(errormsg, "Doctor removed successfully.", 200);
    return true;
}

bool AdminMenu::dischargepatient(Storage<Patient>& patients, Storage<Appointment>& appointments, Storage<Bill>& bills, Storage<Prescription>& prescriptions, int patientid, char errormsg[200])
{
    Patient* patient = nullptr;
    for (int i = 0; i < patients.size(); i++)
    {
        if (patients.get(i).getid() == patientid)
        {
            patient = &patients.get(i);
            break;
        }
    }

    if (patient == nullptr)
    {
        strcopy(errormsg, "Patient not found.", 200);
        return false;
    }

    for (int i = 0; i < bills.size(); i++)
    {
        if (bills.get(i).getpatientid() == patientid && strequal(bills.get(i).getstatus(), "unpaid"))
        {
            strcopy(errormsg, "Cannot discharge patient with unpaid bills.", 200);
            return false;
        }
    }

    for (int i = 0; i < appointments.size(); i++)
    {
        if (appointments.get(i).getpatientid() == patientid && strequal(appointments.get(i).getstatus(), "pending"))
        {
            strcopy(errormsg, "Cannot discharge patient with pending appointments.", 200);
            return false;
        }
    }

    FileHandler::appenddischarged(*patient);
    patients.removebyid(patientid);
    FileHandler::savepatients(patients);

    for (int i = appointments.size() - 1; i >= 0; i--)
    {
        if (appointments.get(i).getpatientid() == patientid)
        {
            appointments.removebyid(appointments.get(i).getappointmentid());
        }
    }
    FileHandler::saveappointments(appointments);

    for (int i = bills.size() - 1; i >= 0; i--)
    {
        if (bills.get(i).getpatientid() == patientid)
        {
            bills.removebyid(bills.get(i).getbillid());
        }
    }
    FileHandler::savebills(bills);

    for (int i = prescriptions.size() - 1; i >= 0; i--)
    {
        if (prescriptions.get(i).getpatientid() == patientid)
        {
            prescriptions.removebyid(prescriptions.get(i).getprescriptionid());
        }
    }
    FileHandler::saveprescriptions(prescriptions);

    strcopy(errormsg, "Patient discharged and archived successfully.", 200);
    return true;
}

void AdminMenu::getallunpaidbills(Storage<Bill>& bills, Storage<Bill>& out)
{
    for (int i = 0; i < bills.size(); i++)
    {
        if (strequal(bills.get(i).getstatus(), "unpaid"))
        {
            out.add(bills.get(i));
        }
    }
}

void AdminMenu::getallappointmentssorteddesc(Storage<Appointment>& appointments, Storage<Appointment>& out)
{
    for (int i = 0; i < appointments.size(); i++)
    {
        out.add(appointments.get(i));
    }

    for (int i = 0; i < out.size() - 1; i++)
    {
        for (int j = 0; j < out.size() - i - 1; j++)
        {
            const char* date1 = out.get(j).getdate();
            const char* date2 = out.get(j + 1).getdate();

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
                Appointment temp = out.get(j);
                out.get(j) = out.get(j + 1);
                out.get(j + 1) = temp;
            }
        }
    }
}

bool AdminMenu::isoverdue(const char* billdate)
{
    int day = (billdate[0] - '0') * 10 + (billdate[1] - '0');
    int month = (billdate[3] - '0') * 10 + (billdate[4] - '0');
    int year = (billdate[6] - '0') * 1000 + (billdate[7] - '0') * 100 + (billdate[8] - '0') * 10 + (billdate[9] - '0');

    tm billtime = {};
    billtime.tm_mday = day;
    billtime.tm_mon = month - 1;
    billtime.tm_year = year - 1900;
    time_t billt = mktime(&billtime);

    time_t nowt = time(nullptr);
    double diff = difftime(nowt, billt);

    return diff > 7 * 24 * 3600;
}