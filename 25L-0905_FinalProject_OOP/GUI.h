#pragma once
#include <SFML/Graphics.hpp>
#include "Patient.h"
#include "Doctor.h"
#include "Admin.h"
#include "Storage.h"
#include "FileHandler.h"
#include "PatientMenu.h"
#include "DoctorMenu.h"
#include "AdminMenu.h"
using namespace sf;

enum class Screen
{
    Login,
    PatientDashboard,
    BookAppointment,
    CancelAppointment,
    ViewAppointments,
    ViewMedicalRecords,
    ViewBills,
    PayBill,
    TopUpBalance,
    DoctorDashboard,
    ViewTodaysAppointments,
    MarkComplete,
    MarkNoShow,
    WritePrescription,
    ViewPatientHistory,
    AdminDashboard,
    AddDoctor,
    RemoveDoctor,
    ViewAllPatients,
    ViewAllDoctors,
    ViewAllAppointments,
    ViewUnpaidBills,
    DischargePatient,
    ViewSecurityLog,
    GenerateDailyReport
};

class GUI
{
    RenderWindow window;
    Font regularfont;
    Font boldfont;

    Screen currentscreen;

    Storage<Patient> patients;
    Storage<Doctor> doctors;
    Storage<Admin> admins;
    Storage<Appointment> appointments;
    Storage<Bill> bills;
    Storage<Prescription> prescriptions;

    Patient* loggedinpatient;
    Doctor* loggedindoctor;
    Admin* loggedinadmin;

    char errormsg[200];
    char successmsg[200];
    bool showerror;
    bool showsuccess;
    float msgtimer;

    int loginfailedattempts;
    bool loginlocked;
    bool loginbuttonprocessed;

    char inputbuf1[256];
    char inputbuf2[256];
    char inputbuf3[256];
    char inputbuf4[256];
    char inputbuf5[256];

    bool input1active;
    bool input2active;
    bool input3active;
    bool input4active;
    bool input5active;

    int selectedrole;
    int selectedid;

    int scrolloffset;

    void loadalldata();
    void drawbackground();
    void drawtext(const char* text, float x, float y, int size, Color color);
    void drawbutton(const char* text, float x, float y, float w, float h, Color color);
    bool buttonclicked(float x, float y, float w, float h, Vector2f mousepos, Event& event);
    void drawinputbox(const char* label, float x, float y, float w, float h, char* buf, bool active, bool hidden);
    void handleinput(Event& event, char* buf, bool& active, bool hidden);
    void drawtableheader(const char** headers, float* widths, int count, float x, float y);
    void drawtablerow(const char** values, float* widths, int count, float x, float y, Color rowcolor);
    void drawpopup();
    void setscreen(Screen s);
    void seterror(const char* msg);
    void setsuccess(const char* msg);

    void drawlogin(Event& event, Vector2f mousepos);
    void drawpatientdashboard(Event& event, Vector2f mousepos);
    void drawbookappointment(Event& event, Vector2f mousepos);
    void drawcancelappointment(Event& event, Vector2f mousepos);
    void drawviewappointments(Event& event, Vector2f mousepos);
    void drawviewmedicalrecords(Event& event, Vector2f mousepos);
    void drawviewbills(Event& event, Vector2f mousepos);
    void drawpaybill(Event& event, Vector2f mousepos);
    void drawtopupbalance(Event& event, Vector2f mousepos);
    void drawdoctordashboard(Event& event, Vector2f mousepos);
    void drawviewtodaysappointments(Event& event, Vector2f mousepos);
    void drawmarkcomplete(Event& event, Vector2f mousepos);
    void drawmarknoshow(Event& event, Vector2f mousepos);
    void drawwriteprescription(Event& event, Vector2f mousepos);
    void drawviewpatienthistory(Event& event, Vector2f mousepos);
    void drawadmindashboard(Event& event, Vector2f mousepos);
    void drawadddoctor(Event& event, Vector2f mousepos);
    void drawremovedoctor(Event& event, Vector2f mousepos);
    void drawviewallpatients(Event& event, Vector2f mousepos);
    void drawviewalldoctors(Event& event, Vector2f mousepos);
    void drawviewallappointments(Event& event, Vector2f mousepos);
    void drawviewunpaidbills(Event& event, Vector2f mousepos);
    void drawdischargepatient(Event& event, Vector2f mousepos);
    void drawviewsecuritylog(Event& event, Vector2f mousepos);
    void drawgeneratedailyreport(Event& event, Vector2f mousepos);

public:
    GUI();
    void run();
};