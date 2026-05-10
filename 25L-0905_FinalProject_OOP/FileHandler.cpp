#define _CRT_SECURE_NO_WARNINGS
#include "FileHandler.h"
#include <iostream>
#include <fstream>
using namespace std;

void FileHandler::inttostr(int val, char* buf)
{
    if (val == 0)
    {
        buf[0] = '0';
        buf[1] = '\0';
        return;
    }
    char tmp[20];
    int i = 0;
    bool neg = (val < 0);
    if (neg)
    {
        val = -val;
    }
    while (val > 0)
    {
        tmp[i++] = '0' + (val % 10);
        val /= 10;
    }
    if (neg)
    {
        tmp[i++] = '-';
    }
    int index = 0;
    for (int j = i - 1; j >= 0; j--)
    {
        buf[index++] = tmp[j];
    }
    buf[index] = '\0';
}

void FileHandler::floattostr(float val, char* buf)
{
    int intpart = (int)val;
    int fracpart = (int)((val - intpart) * 100 + 0.5f);
    char tmp[20];
    inttostr(intpart, tmp);
    int i = 0;
    while (tmp[i])
    {
        buf[i] = tmp[i];
        i++;
    }
    buf[i++] = '.';
    buf[i++] = '0' + (fracpart / 10);
    buf[i++] = '0' + (fracpart % 10);
    buf[i] = '\0';
}

int FileHandler::strtoint(const char* s)
{
    int result = 0;
    bool neg = false;
    if (*s == '-')
    {
        neg = true;
        s++;
    }
    while (*s >= '0' && *s <= '9')
    {
        result = result * 10 + (*s - '0');
        s++;
    }
    return neg ? -result : result;
}

float FileHandler::strtofloat(const char* s)
{
    float result = 0.0f;
    bool neg = false;
    if (*s == '-')
    {
        neg = true;
        s++;
    }
    while (*s >= '0' && *s <= '9')
    {
        result = result * 10 + (*s - '0');
        s++;
    }
    if (*s == '.')
    {
        s++;
        float frac = 0.1f;
        while (*s >= '0' && *s <= '9')
        {
            result += (*s - '0') * frac;
            frac *= 0.1f;
            s++;
        }
    }
    return neg ? -result : result;
}

static void stripcarriagereturn(char* line)
{
    int i = 0;
    while (line[i] != '\0')
    {
        i++;
    }
    if (i > 0 && line[i - 1] == '\r')
    {
        line[i - 1] = '\0';
    }
}

void FileHandler::readfield(const char*& cur, char* field, int maxlen)
{
    int i = 0;
    while (*cur != '\0' && *cur != ',' && *cur != '\n' && *cur != '\r')
    {
        if (i < maxlen - 1)
        {
            field[i++] = *cur;
        }
        cur++;
    }
    field[i] = '\0';
    if (*cur == ',')
    {
        cur++;
    }
}

bool FileHandler::loadpatients(Storage<Patient>& out)
{
    ifstream file("patients.txt");
    if (!file.is_open())
    {
        return false;
    }
    char line[256];
    while (file.getline(line, 256))
    {
        stripcarriagereturn(line);
        const char* lp = line;
        char patientid[20], patientname[60], patientage[10], patientgender[5];
        char patientcontact[20], patientpass[60], patientbalance[20];
        readfield(lp, patientid, 20);
        readfield(lp, patientname, 60);
        readfield(lp, patientage, 10);
        readfield(lp, patientgender, 5);
        readfield(lp, patientcontact, 20);
        readfield(lp, patientpass, 60);
        readfield(lp, patientbalance, 20);
        if (patientid[0] == '\0')
        {
            continue;
        }
        Patient p(strtoint(patientid), patientname, patientpass, strtoint(patientage), strtofloat(patientbalance), patientgender, patientcontact);
        out.add(p);
    }
    file.close();
    return true;
}

bool FileHandler::loaddoctors(Storage<Doctor>& out)
{
    ifstream file("doctors.txt");
    if (!file.is_open())
    {
        return false;
    }
    char line[256];
    while (file.getline(line, 256))
    {
        stripcarriagereturn(line);
        const char* lp = line;
        char doctorid[20], doctorname[60], doctorspec[60];
        char doctorcontact[20], doctorpass[60], doctorfee[20];
        readfield(lp, doctorid, 20);
        readfield(lp, doctorname, 60);
        readfield(lp, doctorspec, 60);
        readfield(lp, doctorcontact, 20);
        readfield(lp, doctorpass, 60);
        readfield(lp, doctorfee, 20);
        if (doctorid[0] == '\0')
        {
            continue;
        }
        Doctor d(strtoint(doctorid), doctorname, doctorpass, doctorspec, doctorcontact, strtofloat(doctorfee));
        out.add(d);
    }
    file.close();
    return true;
}

bool FileHandler::loadadmins(Storage<Admin>& out)
{
    ifstream file("admin.txt");
    if (!file.is_open())
    {
        return false;
    }
    char line[256];
    while (file.getline(line, 256))
    {
        stripcarriagereturn(line);
        const char* lp = line;
        char adminid[20], adminname[60], adminpass[60];
        readfield(lp, adminid, 20);
        readfield(lp, adminname, 60);
        readfield(lp, adminpass, 60);
        if (adminid[0] == '\0')
        {
            continue;
        }
        Admin a(strtoint(adminid), adminname, adminpass);
        out.add(a);
    }
    file.close();
    return true;
}

bool FileHandler::loadappointments(Storage<Appointment>& out)
{
    ifstream file("appointments.txt");
    if (!file.is_open())
    {
        return false;
    }
    char line[256];
    while (file.getline(line, 256))
    {
        stripcarriagereturn(line);
        const char* lp = line;
        char apptid[20], apptpatientid[20], apptdoctorid[20];
        char apptdate[15], appttimeslot[10], apptstatus[15];
        readfield(lp, apptid, 20);
        readfield(lp, apptpatientid, 20);
        readfield(lp, apptdoctorid, 20);
        readfield(lp, apptdate, 15);
        readfield(lp, appttimeslot, 10);
        readfield(lp, apptstatus, 15);
        if (apptid[0] == '\0')
        {
            continue;
        }
        Appointment a(strtoint(apptid), strtoint(apptpatientid), strtoint(apptdoctorid), apptdate, appttimeslot, apptstatus);
        out.add(a);
    }
    file.close();
    return true;
}

bool FileHandler::loadbills(Storage<Bill>& out)
{
    ifstream file("bills.txt");
    if (!file.is_open())
    {
        return false;
    }
    char line[256];
    while (file.getline(line, 256))
    {
        stripcarriagereturn(line);
        const char* lp = line;
        char billid[20], billpatientid[20], billapptid[20];
        char billamount[20], billstatus[15], billdate[15];
        readfield(lp, billid, 20);
        readfield(lp, billpatientid, 20);
        readfield(lp, billapptid, 20);
        readfield(lp, billamount, 20);
        readfield(lp, billstatus, 15);
        readfield(lp, billdate, 15);
        if (billid[0] == '\0')
        {
            continue;
        }
        Bill b(strtoint(billid), strtoint(billpatientid), strtoint(billapptid), strtofloat(billamount), billstatus, billdate);
        out.add(b);
    }
    file.close();
    return true;
}

bool FileHandler::loadprescriptions(Storage<Prescription>& out)
{
    ifstream file("prescriptions.txt");
    if (!file.is_open())
    {
        return false;
    }
    char line[1024];
    while (file.getline(line, 1024))
    {
        stripcarriagereturn(line);
        const char* lp = line;
        char presid[20], presapptid[20], prespatientid[20], presdoctorid[20];
        char presdate[15], presmedicines[500], presnotes[300];
        readfield(lp, presid, 20);
        readfield(lp, presapptid, 20);
        readfield(lp, prespatientid, 20);
        readfield(lp, presdoctorid, 20);
        readfield(lp, presdate, 15);
        readfield(lp, presmedicines, 500);
        readfield(lp, presnotes, 300);
        if (presid[0] == '\0')
        {
            continue;
        }
        Prescription p(strtoint(presid), strtoint(presapptid), strtoint(prespatientid), strtoint(presdoctorid), presdate, presmedicines, presnotes);
        out.add(p);
    }
    file.close();
    return true;
}

void FileHandler::savepatients(Storage<Patient>& patients)
{
    ofstream file("patients.txt");
    if (!file.is_open())
    {
        return;
    }
    for (int i = 0; i < patients.size(); i++)
    {
        Patient& p = patients.get(i);
        char patientid[20], patientage[10], patientbalance[20];
        inttostr(p.getid(), patientid);
        inttostr(p.getage(), patientage);
        floattostr(p.getbalance(), patientbalance);
        char gender[2] = { p.getgender()[0], '\0' };
        file << patientid << "," << p.getname() << "," << patientage << "," << gender << ","
            << p.getcontact() << "," << p.getpass() << "," << patientbalance << "\n";
    }
    file.close();
}

void FileHandler::savedoctors(Storage<Doctor>& doctors)
{
    ofstream file("doctors.txt");
    if (!file.is_open())
    {
        return;
    }
    for (int i = 0; i < doctors.size(); i++)
    {
        Doctor& d = doctors.get(i);
        char doctorid[20], doctorfee[20];
        inttostr(d.getid(), doctorid);
        floattostr(d.getfee(), doctorfee);
        file << doctorid << "," << d.getname() << "," << d.getspecialization() << ","
            << d.getcontact() << "," << d.getpass() << "," << doctorfee << "\n";
    }
    file.close();
}

void FileHandler::saveappointments(Storage<Appointment>& appts)
{
    ofstream file("appointments.txt");
    if (!file.is_open())
    {
        return;
    }
    for (int i = 0; i < appts.size(); i++)
    {
        Appointment& a = appts.get(i);
        char apptid[20], patientid[20], doctorid[20];
        inttostr(a.getappointmentid(), apptid);
        inttostr(a.getpatientid(), patientid);
        inttostr(a.getdoctorid(), doctorid);
        file << apptid << "," << patientid << "," << doctorid << ","
            << a.getdate() << "," << a.gettimeslot() << "," << a.getstatus() << "\n";
    }
    file.close();
}

void FileHandler::savebills(Storage<Bill>& bills)
{
    ofstream file("bills.txt");
    if (!file.is_open())
    {
        return;
    }
    for (int i = 0; i < bills.size(); i++)
    {
        Bill& b = bills.get(i);
        char billid[20], patientid[20], apptid[20], amount[20];
        inttostr(b.getbillid(), billid);
        inttostr(b.getpatientid(), patientid);
        inttostr(b.getappointmentid(), apptid);
        floattostr(b.getamount(), amount);
        file << billid << "," << patientid << "," << apptid << "," << amount << ","
            << b.getstatus() << "," << b.getdate() << "\n";
    }
    file.close();
}

void FileHandler::saveprescriptions(Storage<Prescription>& prescriptions)
{
    ofstream file("prescriptions.txt");
    if (!file.is_open())
    {
        return;
    }
    for (int i = 0; i < prescriptions.size(); i++)
    {
        Prescription& p = prescriptions.get(i);
        char presid[20], apptid[20], patientid[20], doctorid[20];
        inttostr(p.getprescriptionid(), presid);
        inttostr(p.getappointmentid(), apptid);
        inttostr(p.getpatientid(), patientid);
        inttostr(p.getdoctorid(), doctorid);
        file << presid << "," << apptid << "," << patientid << "," << doctorid << ","
            << p.getdate() << "," << p.getmedicines() << "," << p.getnotes() << "\n";
    }
    file.close();
}

void FileHandler::appendpatient(Patient& p)
{
    ofstream file("patients.txt", ios::app);
    if (!file.is_open())
    {
        return;
    }
    char patientid[20], patientage[10], patientbalance[20];
    inttostr(p.getid(), patientid);
    inttostr(p.getage(), patientage);
    floattostr(p.getbalance(), patientbalance);
    char gender[2] = { p.getgender()[0], '\0' };
    file << patientid << "," << p.getname() << "," << patientage << "," << gender << ","
        << p.getcontact() << "," << p.getpass() << "," << patientbalance << "\n";
    file.close();
}

void FileHandler::appenddoctor(Doctor& d)
{
    ofstream file("doctors.txt", ios::app);
    if (!file.is_open())
    {
        return;
    }
    char doctorid[20], doctorfee[20];
    inttostr(d.getid(), doctorid);
    floattostr(d.getfee(), doctorfee);
    file << doctorid << "," << d.getname() << "," << d.getspecialization() << ","
        << d.getcontact() << "," << d.getpass() << "," << doctorfee << "\n";
    file.close();
}

void FileHandler::appendappointment(Appointment& a)
{
    ofstream file("appointments.txt", ios::app);
    if (!file.is_open())
    {
        return;
    }
    char apptid[20], patientid[20], doctorid[20];
    inttostr(a.getappointmentid(), apptid);
    inttostr(a.getpatientid(), patientid);
    inttostr(a.getdoctorid(), doctorid);
    file << apptid << "," << patientid << "," << doctorid << ","
        << a.getdate() << "," << a.gettimeslot() << "," << a.getstatus() << "\n";
    file.close();
}

void FileHandler::appendbill(Bill& b)
{
    ofstream file("bills.txt", ios::app);
    if (!file.is_open())
    {
        return;
    }
    char billid[20], patientid[20], apptid[20], amount[20];
    inttostr(b.getbillid(), billid);
    inttostr(b.getpatientid(), patientid);
    inttostr(b.getappointmentid(), apptid);
    floattostr(b.getamount(), amount);
    file << billid << "," << patientid << "," << apptid << "," << amount << ","
        << b.getstatus() << "," << b.getdate() << "\n";
    file.close();
}

void FileHandler::appendprescription(Prescription& p)
{
    ofstream file("prescriptions.txt", ios::app);
    if (!file.is_open())
    {
        return;
    }
    char presid[20], apptid[20], patientid[20], doctorid[20];
    inttostr(p.getprescriptionid(), presid);
    inttostr(p.getappointmentid(), apptid);
    inttostr(p.getpatientid(), patientid);
    inttostr(p.getdoctorid(), doctorid);
    file << presid << "," << apptid << "," << patientid << "," << doctorid << ","
        << p.getdate() << "," << p.getmedicines() << "," << p.getnotes() << "\n";
    file.close();
}

void FileHandler::appendsecuritylog(const char* timestamp, const char* role,
    const char* enteredid, const char* result)
{
    ofstream file("security_log.txt", ios::app);
    if (!file.is_open())
    {
        return;
    }
    file << timestamp << "," << role << "," << enteredid << "," << result << "\n";
    file.close();
}

void FileHandler::loadsecuritylog(char* buffer, int buffersize)
{
    buffer[0] = '\0';
    ifstream file("security_log.txt");
    if (!file.is_open())
    {
        return;
    }
    int i = 0;
    char c;
    while (file.get(c) && i < buffersize - 1)
    {
        buffer[i++] = c;
    }
    buffer[i] = '\0';
    file.close();
}

void FileHandler::appenddischarged(Patient& p)
{
    ofstream file("discharged.txt", ios::app);
    if (!file.is_open())
    {
        return;
    }
    char patientid[20], patientage[10], patientbalance[20];
    inttostr(p.getid(), patientid);
    inttostr(p.getage(), patientage);
    floattostr(p.getbalance(), patientbalance);
    char gender[2] = { p.getgender()[0], '\0' };
    file << patientid << "," << p.getname() << "," << patientage << "," << gender << ","
        << p.getcontact() << "," << p.getpass() << "," << patientbalance << "\n";
    file.close();
}


int FileHandler::wizardstrlen(const char* s)
{
    int i = 0;
    while (s[i]) i++;
    return i;
}

bool FileHandler::wizardisdigitsonly(const char* s)
{
    for (int i = 0; s[i]; i++)
        if (s[i] < '0' || s[i] > '9') return false;
    return true;
}

void FileHandler::wizardclearline()
{
    char c;
    while (cin.get(c) && c != '\n') {}
}

void FileHandler::wizardreadstr(const char* prompt, char* buf, int maxlen)
{
    cout << prompt;
    int i = 0;
    char c;
    while (cin.get(c) && c != '\n')
    {
        if (i < maxlen - 1)
            buf[i++] = c;
    }
    buf[i] = '\0';
}

int FileHandler::wizardreadint(const char* prompt)
{
    cout << prompt;
    int v = 0;
    cin >> v;
    wizardclearline();
    return v;
}

float FileHandler::wizardreadfloat(const char* prompt)
{
    cout << prompt;
    float v = 0.0f;
    cin >> v;
    wizardclearline();
    return v;
}

bool FileHandler::wizardiscontactvalid(const char* c)
{
    return wizardstrlen(c) == 11 && wizardisdigitsonly(c);
}

bool FileHandler::wizardispassvalid(const char* p)
{
    return wizardstrlen(p) >= 6;
}

bool FileHandler::wizardisagevalid(int a)
{
    return a > 0 && a < 150;
}

bool FileHandler::wizardisgendervalid(const char* g)
{
    return (g[0] == 'M' || g[0] == 'm' || g[0] == 'F' || g[0] == 'f') && g[1] == '\0';
}


void FileHandler::wizardwriteadmins()
{
    int n = wizardreadint("\nHow many admin accounts to create? ");
    if (n <= 0)
    {
        cout << "  No admins entered — keeping existing admin.txt.\n";
        return;
    }

    ofstream f("admin.txt"); 
    for (int i = 0; i < n; i++)
    {
        cout << "\n--- Admin " << (i + 1) << " ---\n";

        char name[60], pass[60];
        bool ok = false;

        wizardreadstr("  Name: ", name, 60);

        do {
            wizardreadstr("  Password (min 6 chars): ", pass, 60);
            if (!wizardispassvalid(pass))
                cout << "  ! Password must be at least 6 characters. Try again.\n";
            else
                ok = true;
        } while (!ok);

        f << (i + 1) << "," << name << "," << pass << "\n";
        cout << "  Done. Login ID = " << (i + 1) << "\n";
    }
    f.close();
}

void FileHandler::wizardwritedoctors()
{
    int n = wizardreadint("\nHow many doctor accounts to create? ");
    if (n <= 0)
    {
        cout << "  No doctors entered — keeping existing doctors.txt.\n";
        return;
    }

    ofstream f("doctors.txt");
    for (int i = 0; i < n; i++)
    {
        cout << "\n--- Doctor " << (i + 1) << " ---\n";

        char name[60], spec[60], contact[20], pass[60];
        float fee = 0;
        bool ok;

        wizardreadstr("  Full name (e.g. Dr. Ali Khan): ", name, 60);
        wizardreadstr("  Specialization (e.g. Cardiology): ", spec, 60);

        ok = false;
        do {
            wizardreadstr("  Contact (11 digits, e.g. 03001234567): ", contact, 20);
            if (!wizardiscontactvalid(contact))
                cout << "  ! Must be exactly 11 digits. Try again.\n";
            else
                ok = true;
        } while (!ok);

        ok = false;
        do {
            wizardreadstr("  Password (min 6 chars): ", pass, 60);
            if (!wizardispassvalid(pass))
                cout << "  ! Password must be at least 6 characters. Try again.\n";
            else
                ok = true;
        } while (!ok);

        ok = false;
        do {
            fee = wizardreadfloat("  Consultation fee in PKR (e.g. 1500): ");
            if (fee <= 0)
                cout << "  ! Fee must be greater than 0. Try again.\n";
            else
                ok = true;
        } while (!ok);

        char feebuf[20];
        floattostr(fee, feebuf);
        f << (i + 1) << "," << name << "," << spec << "," << contact << "," << pass << "," << feebuf << "\n";
        cout << "  Done. Login ID = " << (i + 1) << "\n";
    }
    f.close();
}

void FileHandler::wizardwritepatients()
{
    int n = wizardreadint("\nHow many patient accounts to create? ");
    if (n <= 0)
    {
        cout << "  No patients entered — keeping existing patients.txt.\n";
        return;
    }

    ofstream f("patients.txt"); 
    for (int i = 0; i < n; i++)
    {
        cout << "\n--- Patient " << (i + 1) << " ---\n";

        char name[60], gender[5], contact[20], pass[60];
        int age = 0;
        float balance = 0;
        bool ok;

        wizardreadstr("  Full name: ", name, 60);

        ok = false;
        do {
            age = wizardreadint("  Age: ");
            if (!wizardisagevalid(age))
                cout << "  ! Age must be between 1 and 149. Try again.\n";
            else
                ok = true;
        } while (!ok);

        ok = false;
        do {
            wizardreadstr("  Gender (M or F): ", gender, 5);
            if (!wizardisgendervalid(gender))
                cout << "  ! Enter M or F only.\n";
            else
                ok = true;
        } while (!ok);
        if (gender[0] == 'm') gender[0] = 'M';
        if (gender[0] == 'f') gender[0] = 'F';

        ok = false;
        do {
            wizardreadstr("  Contact (11 digits): ", contact, 20);
            if (!wizardiscontactvalid(contact))
                cout << "  ! Must be exactly 11 digits. Try again.\n";
            else
                ok = true;
        } while (!ok);

        ok = false;
        do {
            wizardreadstr("  Password (min 6 chars): ", pass, 60);
            if (!wizardispassvalid(pass))
                cout << "  ! Password must be at least 6 characters. Try again.\n";
            else
                ok = true;
        } while (!ok);

        balance = wizardreadfloat("  Starting balance in PKR (can be 0): ");
        if (balance < 0) balance = 0;

        char balancebuf[20];
        floattostr(balance, balancebuf);


        f << (i + 1) << "," << name << "," << age << "," << gender << ","
            << contact << "," << pass << "," << balancebuf << "\n";
        cout << "  Done. Login ID = " << (i + 1) << "\n";
    }
    f.close();
}



void FileHandler::ensurefilesexist()
{
    { ofstream f("admin.txt", ios::app); }
    { ofstream f("doctors.txt", ios::app); }
    { ofstream f("patients.txt", ios::app); }
    { ofstream f("appointments.txt", ios::app); }
    { ofstream f("bills.txt", ios::app); }
    { ofstream f("prescriptions.txt", ios::app); }
    { ofstream f("discharged.txt", ios::app); }
    { ofstream f("security_log.txt", ios::app); }
}

void FileHandler::setupwizard()
{

    bool hasdata = false;
    {
        ifstream f("admin.txt");
        if (f.is_open())
        {
            char ch;
            while (f.get(ch))
            {
                if (ch != '\n' && ch != '\r' && ch != ' ')
                {
                    hasdata = true;
                    break;
                }
            }
        }
    }

    if (hasdata)
    {
        return;
    }

  
    cout << "\n";
    cout << "============================================\n";
    cout << "   Medicare's Setup\n";
    cout << "   You must create at least 1 admin.\n";
    cout << "============================================\n";


    do {
        wizardwriteadmins();

        hasdata = false;
        ifstream f("admin.txt");
        if (f.is_open())
        {
            char ch;
            while (f.get(ch))
            {
                if (ch != '\n' && ch != '\r' && ch != ' ')
                {
                    hasdata = true;
                    break;
                }
            }
        }

        if (!hasdata)
            cout << "\n  ! You must create at least 1 admin account.\n";

    } while (!hasdata);

    wizardwritedoctors();
    wizardwritepatients();


    { ofstream f("appointments.txt", ios::app); }
    { ofstream f("bills.txt", ios::app); }
    { ofstream f("prescriptions.txt", ios::app); }
    { ofstream f("discharged.txt", ios::app); }
    { ofstream f("security_log.txt", ios::app); }

    cout << "\n============================================\n";
    cout << "  Setup complete!!! Starting Medicare...\n";
    cout << "============================================\n\n";
}