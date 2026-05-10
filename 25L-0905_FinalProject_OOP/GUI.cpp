#define _CRT_SECURE_NO_WARNINGS
#include "GUI.h"
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

static int strtoint(const char* s)
{
    int result = 0;
    while (*s >= '0' && *s <= '9')
    {
        result = result * 10 + (*s - '0');
        s++;
    }
    return result;
}

static float strtofloat(const char* s)
{
    float result = 0.0f;
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
    return result;
}

static void inttostr(int val, char* buf)
{
    if (val == 0)
    {
        buf[0] = '0';
        buf[1] = '\0';
        return;
    }
    char tmp[20];
    int i = 0;
    while (val > 0)
    {
        tmp[i++] = '0' + (val % 10);
        val /= 10;
    }
    int index = 0;
    for (int j = i - 1; j >= 0; j--)
    {
        buf[index++] = tmp[j];
    }
    buf[index] = '\0';
}

static void floattostr(float val, char* buf)
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

GUI::GUI() : window(VideoMode(1280, 720), "Medicare - Hospital Management System")
{
    window.setFramerateLimit(60);
    regularfont.loadFromFile("Roboto-Regular.ttf");
    boldfont.loadFromFile("Roboto-Bold.ttf");

    currentscreen = Screen::Login;
    loggedinpatient = nullptr;
    loggedindoctor = nullptr;
    loggedinadmin = nullptr;

    errormsg[0] = '\0';
    successmsg[0] = '\0';
    showerror = false;
    showsuccess = false;
    msgtimer = 0;

    loginfailedattempts = 0;
    loginlocked = false;
    loginbuttonprocessed = false;

    inputbuf1[0] = '\0';
    inputbuf2[0] = '\0';
    inputbuf3[0] = '\0';
    inputbuf4[0] = '\0';
    inputbuf5[0] = '\0';

    input1active = false;
    input2active = false;
    input3active = false;
    input4active = false;
    input5active = false;

    selectedrole = 0;
    selectedid = 0;
    scrolloffset = 0;

    loadalldata();
}

void GUI::loadalldata()
{
    FileHandler::loadpatients(patients);
    FileHandler::loaddoctors(doctors);
    FileHandler::loadadmins(admins);
    FileHandler::loadappointments(appointments);
    FileHandler::loadbills(bills);
    FileHandler::loadprescriptions(prescriptions);
}

void GUI::setscreen(Screen s)
{
    currentscreen = s;
    loginbuttonprocessed = false;
    inputbuf1[0] = '\0';
    inputbuf2[0] = '\0';
    inputbuf3[0] = '\0';
    inputbuf4[0] = '\0';
    inputbuf5[0] = '\0';
    input1active = false;
    input2active = false;
    input3active = false;
    input4active = false;
    input5active = false;
    scrolloffset = 0;
    showerror = false;
    showsuccess = false;
}

void GUI::seterror(const char* msg)
{
    strcopy(errormsg, msg, 200);
    showerror = true;
    showsuccess = false;
    msgtimer = 3.0f;
}

void GUI::setsuccess(const char* msg)
{
    strcopy(successmsg, msg, 200);
    showsuccess = true;
    showerror = false;
    msgtimer = 3.0f;
}

void GUI::drawbackground()
{
    window.clear(Color(245, 247, 250));
}

void GUI::drawtext(const char* text, float x, float y, int size, Color color)
{
    Text t;
    t.setFont(regularfont);
    t.setString(text);
    t.setCharacterSize(size);
    t.setFillColor(color);
    t.setPosition((int)x, (int)y);
    window.draw(t);
}

void GUI::drawbutton(const char* text, float x, float y, float w, float h, Color color)
{
    RectangleShape box(Vector2f(w, h));
    box.setPosition((int)x, (int)y);
    box.setFillColor(color);
    window.draw(box);

    Text t;
    t.setFont(boldfont);
    t.setString(text);
    t.setCharacterSize(15);
    t.setFillColor(Color::White);
    t.setPosition(0, 0);
    FloatRect bounds = t.getLocalBounds();
    int tx = (int)(x + (w - bounds.width) / 2.0f - bounds.left);
    int ty = (int)(y + (h - bounds.height) / 2.0f - bounds.top);
    t.setPosition(tx, ty);
    window.draw(t);
}

bool GUI::buttonclicked(float x, float y, float w, float h, Vector2f mousepos, Event& event)
{
    if (event.type == Event::MouseButtonReleased && event.mouseButton.button == Mouse::Left)
    {
        if (mousepos.x >= x && mousepos.x <= x + w && mousepos.y >= y && mousepos.y <= y + h)
        {
            return true;
        }
    }
    return false;
}

void GUI::drawinputbox(const char* label, float x, float y, float w, float h, char* buf, bool active, bool hidden)
{
    drawtext(label, x, y - 22, 13, Color(80, 80, 80));

    RectangleShape box(Vector2f(w, h));
    box.setPosition((int)x, (int)y);
    box.setFillColor(Color::White);
    box.setOutlineThickness(1.5f);
    if (active)
    {
        box.setOutlineColor(Color(26, 115, 232));
    }
    else
    {
        box.setOutlineColor(Color(200, 200, 200));
    }
    window.draw(box);

    char displaybuf[256];
    if (hidden)
    {
        int len = 0;
        while (buf[len])
        {
            len++;
        }
        for (int i = 0; i < len; i++)
        {
            displaybuf[i] = '*';
        }
        displaybuf[len] = '\0';
    }
    else
    {
        strcopy(displaybuf, buf, 256);
    }

    Text t;
    t.setFont(regularfont);
    t.setString(displaybuf);
    t.setCharacterSize(14);
    t.setFillColor(Color(30, 30, 30));
    t.setPosition((int)(x + 10), (int)(y + (h - 16) / 2));
    window.draw(t);
}

void GUI::handleinput(Event& event, char* buf, bool& active, bool hidden)
{
    if (event.type == Event::TextEntered && active)
    {
        if (event.text.unicode == 8)
        {
            int len = 0;
            while (buf[len])
            {
                len++;
            }
            if (len > 0)
            {
                buf[len - 1] = '\0';
            }
        }
        else if (event.text.unicode >= 32 && event.text.unicode < 127)
        {
            int len = 0;
            while (buf[len])
            {
                len++;
            }
            if (len < 255)
            {
                buf[len] = (char)event.text.unicode;
                buf[len + 1] = '\0';
            }
        }
    }
}

void GUI::drawtableheader(const char** headers, float* widths, int count, float x, float y)
{
    float totalwidth = 0;
    for (int i = 0; i < count; i++)
    {
        totalwidth += widths[i];
    }
    RectangleShape headerbox(Vector2f(totalwidth, 35));
    headerbox.setPosition((int)x, (int)y);
    headerbox.setFillColor(Color(26, 115, 232));
    window.draw(headerbox);

    float cx = x;
    for (int i = 0; i < count; i++)
    {
        Text t;
        t.setFont(boldfont);
        t.setString(headers[i]);
        t.setCharacterSize(13);
        t.setFillColor(Color::White);
        t.setPosition((int)(cx + 8), (int)(y + 9));
        window.draw(t);
        cx += widths[i];
    }
}

void GUI::drawtablerow(const char** values, float* widths, int count, float x, float y, Color rowcolor)
{
    float totalwidth = 0;
    for (int i = 0; i < count; i++)
    {
        totalwidth += widths[i];
    }
    RectangleShape rowbox(Vector2f(totalwidth, 32));
    rowbox.setPosition((int)x, (int)y);
    rowbox.setFillColor(rowcolor);
    rowbox.setOutlineThickness(0.5f);
    rowbox.setOutlineColor(Color(220, 220, 220));
    window.draw(rowbox);

    float cx = x;
    for (int i = 0; i < count; i++)
    {
        Text t;
        t.setFont(regularfont);
        t.setString(values[i]);
        t.setCharacterSize(13);
        t.setFillColor(Color(40, 40, 40));
        t.setPosition((int)(cx + 8), (int)(y + 8));
        window.draw(t);
        cx += widths[i];
    }
}

void GUI::drawpopup()
{
    if (showerror)
    {
        RectangleShape box(Vector2f(600, 50));
        box.setPosition(340, 660);
        box.setFillColor(Color(220, 53, 69));
        window.draw(box);
        drawtext(errormsg, 350, 673, 14, Color::White);
    }
    if (showsuccess)
    {
        RectangleShape box(Vector2f(600, 50));
        box.setPosition(340, 660);
        box.setFillColor(Color(40, 167, 69));
        window.draw(box);
        drawtext(successmsg, 350, 673, 14, Color::White);
    }
}

void GUI::drawlogin(Event& event, Vector2f mousepos)
{
    float centerx = 640;

    Text title;
    title.setFont(boldfont);
    title.setString("Medicare");
    title.setCharacterSize(40);
    title.setFillColor(Color(26, 115, 232));
    FloatRect titlebounds = title.getLocalBounds();
    title.setPosition((int)(centerx - titlebounds.width / 2.0f - titlebounds.left), 60);
    window.draw(title);

    Text subtitle;
    subtitle.setFont(regularfont);
    subtitle.setString("Hospital Management System");
    subtitle.setCharacterSize(18);
    subtitle.setFillColor(Color(100, 100, 100));
    FloatRect subtitlebounds = subtitle.getLocalBounds();
    subtitle.setPosition((int)(centerx - subtitlebounds.width / 2.0f - subtitlebounds.left), 115);
    window.draw(subtitle);

    Text rolelabel;
    rolelabel.setFont(regularfont);
    rolelabel.setString("Select Role");
    rolelabel.setCharacterSize(14);
    rolelabel.setFillColor(Color(80, 80, 80));
    FloatRect rolebounds = rolelabel.getLocalBounds();
    rolelabel.setPosition((int)(centerx - rolebounds.width / 2.0f - rolebounds.left), 180);
    window.draw(rolelabel);

    Color patientbtncolor;
    Color doctorbtncolor;
    Color adminbtncolor;

    if (selectedrole == 1)
    {
        patientbtncolor = Color(5, 50, 140);
    }
    else
    {
        patientbtncolor = Color(26, 115, 232);
    }

    if (selectedrole == 2)
    {
        doctorbtncolor = Color(5, 50, 140);
    }
    else
    {
        doctorbtncolor = Color(26, 115, 232);
    }

    if (selectedrole == 3)
    {
        adminbtncolor = Color(5, 50, 140);
    }
    else
    {
        adminbtncolor = Color(26, 115, 232);
    }

    drawbutton("Patient", 340, 205, 150, 40, patientbtncolor);
    drawbutton("Doctor", 510, 205, 150, 40, doctorbtncolor);
    drawbutton("Admin", 680, 205, 150, 40, adminbtncolor);

    if (buttonclicked(340, 205, 150, 40, mousepos, event))
    {
        selectedrole = 1;
    }
    if (buttonclicked(510, 205, 150, 40, mousepos, event))
    {
        selectedrole = 2;
    }
    if (buttonclicked(680, 205, 150, 40, mousepos, event))
    {
        selectedrole = 3;
    }

    if (event.type == Event::MouseButtonReleased)
    {
        FloatRect box1(340, 270, 500, 45);
        FloatRect box2(340, 345, 500, 45);
        input1active = box1.contains(mousepos);
        input2active = box2.contains(mousepos);
    }

    handleinput(event, inputbuf1, input1active, false);
    handleinput(event, inputbuf2, input2active, true);

    drawinputbox("ID", 340, 270, 500, 45, inputbuf1, input1active, false);
    drawinputbox("Password", 340, 345, 500, 45, inputbuf2, input2active, true);

    if (loginlocked)
    {
        Text lockedtext;
        lockedtext.setFont(regularfont);
        lockedtext.setString("Account locked. Contact admin.");
        lockedtext.setCharacterSize(14);
        lockedtext.setFillColor(Color(220, 53, 69));
        FloatRect lb = lockedtext.getLocalBounds();
        lockedtext.setPosition((int)(centerx - lb.width / 2.0f - lb.left), 415);
        window.draw(lockedtext);
    }
    else
    {
        drawbutton("Login", 490, 415, 200, 45, Color(26, 115, 232));

        if (buttonclicked(490, 415, 200, 45, mousepos, event))
        {
            if (loginbuttonprocessed)
            {
                return;
            }
            loginbuttonprocessed = true;

            if (selectedrole == 0)
            {
                seterror("Please select a role.");
                loginbuttonprocessed = false;
                return;
            }

            int enteredid = strtoint(inputbuf1);
            bool loginsuccess = false;

            if (selectedrole == 1)
            {
                for (int i = 0; i < patients.size(); i++)
                {
                    if (patients.get(i).getid() == enteredid && strequal(patients.get(i).getpass(), inputbuf2))
                    {
                        loggedinpatient = &patients.get(i);
                        loginsuccess = true;
                        break;
                    }
                }
            }
            else if (selectedrole == 2)
            {
                for (int i = 0; i < doctors.size(); i++)
                {
                    if (doctors.get(i).getid() == enteredid && strequal(doctors.get(i).getpass(), inputbuf2))
                    {
                        loggedindoctor = &doctors.get(i);
                        loginsuccess = true;
                        break;
                    }
                }
            }
            else if (selectedrole == 3)
            {
                for (int i = 0; i < admins.size(); i++)
                {
                    if (admins.get(i).getid() == enteredid && strequal(admins.get(i).getpass(), inputbuf2))
                    {
                        loggedinadmin = &admins.get(i);
                        loginsuccess = true;
                        break;
                    }
                }
            }

            if (loginsuccess)
            {
                loginfailedattempts = 0;
                loginbuttonprocessed = false;
                if (selectedrole == 1)
                {
                    setscreen(Screen::PatientDashboard);
                }
                else if (selectedrole == 2)
                {
                    setscreen(Screen::DoctorDashboard);
                }
                else if (selectedrole == 3)
                {
                    setscreen(Screen::AdminDashboard);
                }
            }
            else
            {
                loginfailedattempts++;
                loginbuttonprocessed = false;

                char roles[3][10] = { "Patient", "Doctor", "Admin" };
                char enteredidstr[20];
                inttostr(enteredid, enteredidstr);

                time_t t = time(nullptr);
                char* timestr = ctime(&t);
                char timestamp[30];
                int ti = 0;
                while (timestr[ti] && timestr[ti] != '\n' && ti < 29)
                {
                    timestamp[ti] = timestr[ti];
                    ti++;
                }
                timestamp[ti] = '\0';

                FileHandler::appendsecuritylog(timestamp, roles[selectedrole - 1], enteredidstr, "FAILED");

                if (loginfailedattempts >= 3)
                {
                    loginlocked = true;
                }
                else
                {
                    seterror("Invalid ID or password.");
                }
            }
        }
    }
}

void GUI::drawpatientdashboard(Event& event, Vector2f mousepos)
{
    char balancebuf[30];
    floattostr(loggedinpatient->getbalance(), balancebuf);

    char welcomemsg[100];
    strcopy(welcomemsg, "Welcome, ", 100);
    int wlen = 0;
    while (welcomemsg[wlen])
    {
        wlen++;
    }
    const char* pname = loggedinpatient->getname();
    int ni = 0;
    while (pname[ni] && wlen < 99)
    {
        welcomemsg[wlen++] = pname[ni++];
    }
    welcomemsg[wlen] = '\0';

    drawtext(welcomemsg, 40, 30, 22, Color(26, 115, 232));

    char baltext[60];
    strcopy(baltext, "Balance: PKR ", 60);
    int bl = 0;
    while (baltext[bl])
    {
        bl++;
    }
    int bi = 0;
    while (balancebuf[bi] && bl < 59)
    {
        baltext[bl++] = balancebuf[bi++];
    }
    baltext[bl] = '\0';
    drawtext(baltext, 40, 65, 15, Color(80, 80, 80));

    float btnx = 40;
    float btny = 120;
    float btnw = 220;
    float btnh = 50;
    float gap = 15;

    drawbutton("Book Appointment", btnx, btny, btnw, btnh, Color(26, 115, 232));
    drawbutton("Cancel Appointment", btnx, btny + (btnh + gap), btnw, btnh, Color(26, 115, 232));
    drawbutton("View Appointments", btnx, btny + (btnh + gap) * 2, btnw, btnh, Color(26, 115, 232));
    drawbutton("View Medical Records", btnx, btny + (btnh + gap) * 3, btnw, btnh, Color(26, 115, 232));
    drawbutton("View Bills", btnx, btny + (btnh + gap) * 4, btnw, btnh, Color(26, 115, 232));
    drawbutton("Pay Bill", btnx, btny + (btnh + gap) * 5, btnw, btnh, Color(26, 115, 232));
    drawbutton("Top Up Balance", btnx, btny + (btnh + gap) * 6, btnw, btnh, Color(26, 115, 232));
    drawbutton("Logout", btnx, btny + (btnh + gap) * 7, btnw, btnh, Color(180, 50, 50));

    if (buttonclicked(btnx, btny, btnw, btnh, mousepos, event)) { setscreen(Screen::BookAppointment); }
    if (buttonclicked(btnx, btny + (btnh + gap), btnw, btnh, mousepos, event)) { setscreen(Screen::CancelAppointment); }
    if (buttonclicked(btnx, btny + (btnh + gap) * 2, btnw, btnh, mousepos, event)) { setscreen(Screen::ViewAppointments); }
    if (buttonclicked(btnx, btny + (btnh + gap) * 3, btnw, btnh, mousepos, event)) { setscreen(Screen::ViewMedicalRecords); }
    if (buttonclicked(btnx, btny + (btnh + gap) * 4, btnw, btnh, mousepos, event)) { setscreen(Screen::ViewBills); }
    if (buttonclicked(btnx, btny + (btnh + gap) * 5, btnw, btnh, mousepos, event)) { setscreen(Screen::PayBill); }
    if (buttonclicked(btnx, btny + (btnh + gap) * 6, btnw, btnh, mousepos, event)) { setscreen(Screen::TopUpBalance); }
    if (buttonclicked(btnx, btny + (btnh + gap) * 7, btnw, btnh, mousepos, event))
    {
        loggedinpatient = nullptr;
        loginfailedattempts = 0;
        loginlocked = false;
        setscreen(Screen::Login);
    }
}

void GUI::drawbookappointment(Event& event, Vector2f mousepos)
{
    drawtext("Book Appointment", 300, 30, 22, Color(26, 115, 232));

    if (event.type == Event::MouseButtonReleased)
    {
        FloatRect box1(300, 100, 400, 40);
        input1active = box1.contains(mousepos);
    }
    handleinput(event, inputbuf1, input1active, false);
    drawinputbox("Specialization", 300, 100, 400, 40, inputbuf1, input1active, false);

    drawbutton("Search", 720, 100, 100, 40, Color(26, 115, 232));

    float tablesx = 300;
    float tablesy = 170;
    const char* headers[] = { "ID", "Name", "Specialization", "Fee" };
    float widths[] = { 80, 200, 200, 120 };

    drawtableheader(headers, widths, 4, tablesx, tablesy);

    int row = 0;
    for (int i = 0; i < doctors.size(); i++)
    {
        Doctor& d = doctors.get(i);
        bool match = false;
        if (inputbuf1[0] == '\0')
        {
            match = true;
        }
        else
        {
            match = Validator::strcompignorecase(d.getspecialization(), inputbuf1);
        }

        if (match)
        {
            char idstr[10];
            inttostr(d.getid(), idstr);
            char feestr[20];
            floattostr(d.getfee(), feestr);
            const char* vals[] = { idstr, d.getname(), d.getspecialization(), feestr };
            Color rowcolor;
            if (row % 2 == 0)
            {
                rowcolor = Color::White;
            }
            else
            {
                rowcolor = Color(240, 244, 255);
            }
            drawtablerow(vals, widths, 4, tablesx, tablesy + 35 + row * 32, rowcolor);
            row++;
        }
    }

    if (event.type == Event::MouseButtonReleased)
    {
        FloatRect box2(300, 410, 150, 40);
        FloatRect box3(470, 410, 200, 40);
        FloatRect box4(690, 410, 130, 40);
        input2active = box2.contains(mousepos);
        input3active = box3.contains(mousepos);
        input4active = box4.contains(mousepos);
    }
    handleinput(event, inputbuf2, input2active, false);
    handleinput(event, inputbuf3, input3active, false);
    handleinput(event, inputbuf4, input4active, false);

    drawinputbox("Doctor ID", 300, 410, 150, 40, inputbuf2, input2active, false);
    drawinputbox("Date (DD-MM-YYYY)", 470, 410, 200, 40, inputbuf3, input3active, false);
    drawinputbox("Time Slot", 690, 410, 130, 40, inputbuf4, input4active, false);

    drawtext("Slots: 09:00 10:00 11:00 12:00 13:00 14:00 15:00 16:00", 300, 460, 12, Color(100, 100, 100));

    drawbutton("Book", 300, 490, 150, 45, Color(26, 115, 232));
    drawbutton("Back", 470, 490, 150, 45, Color(150, 150, 150));

    if (buttonclicked(300, 490, 150, 45, mousepos, event))
    {
        char msgerr[200];
        int docid = strtoint(inputbuf2);
        bool result = PatientMenu::bookappointment(*loggedinpatient, patients, doctors, appointments, bills, docid, inputbuf3, inputbuf4, msgerr);
        if (result)
        {
            setsuccess(msgerr);
        }
        else
        {
            seterror(msgerr);
        }
    }

    if (buttonclicked(470, 490, 150, 45, mousepos, event))
    {
        setscreen(Screen::PatientDashboard);
    }
}

void GUI::drawcancelappointment(Event& event, Vector2f mousepos)
{
    drawtext("Cancel Appointment", 300, 30, 22, Color(26, 115, 232));

    const char* headers[] = { "Appt ID", "Doctor ID", "Date", "Time Slot" };
    float widths[] = { 100, 100, 150, 150 };
    drawtableheader(headers, widths, 4, 300, 80);

    int row = 0;
    for (int i = 0; i < appointments.size(); i++)
    {
        Appointment& a = appointments.get(i);
        if (a.getpatientid() == loggedinpatient->getid() && strequal(a.getstatus(), "pending"))
        {
            char apptid[10], docid[10];
            inttostr(a.getappointmentid(), apptid);
            inttostr(a.getdoctorid(), docid);
            const char* vals[] = { apptid, docid, a.getdate(), a.gettimeslot() };
            Color rowcolor;
            if (row % 2 == 0)
            {
                rowcolor = Color::White;
            }
            else
            {
                rowcolor = Color(240, 244, 255);
            }
            drawtablerow(vals, widths, 4, 300, 115 + row * 32, rowcolor);
            row++;
        }
    }

    if (row == 0)
    {
        drawtext("You have no pending appointments.", 300, 130, 14, Color(150, 150, 150));
    }

    if (event.type == Event::MouseButtonReleased)
    {
        FloatRect box1(300, 420, 200, 40);
        input1active = box1.contains(mousepos);
    }
    handleinput(event, inputbuf1, input1active, false);
    drawinputbox("Appointment ID to cancel", 300, 420, 200, 40, inputbuf1, input1active, false);

    drawbutton("Cancel Appointment", 300, 480, 200, 45, Color(220, 53, 69));
    drawbutton("Back", 520, 480, 150, 45, Color(150, 150, 150));

    if (buttonclicked(300, 480, 200, 45, mousepos, event))
    {
        char msgerr[200];
        int apptid = strtoint(inputbuf1);
        bool result = PatientMenu::cancelappointment(*loggedinpatient, patients, appointments, bills, apptid, msgerr);
        if (result)
        {
            setsuccess(msgerr);
        }
        else
        {
            seterror(msgerr);
        }
    }

    if (buttonclicked(520, 480, 150, 45, mousepos, event))
    {
        setscreen(Screen::PatientDashboard);
    }
}

void GUI::drawviewappointments(Event& event, Vector2f mousepos)
{
    drawtext("My Appointments", 300, 30, 22, Color(26, 115, 232));

    Storage<Appointment> myappts;
    PatientMenu::getpatientappointments(*loggedinpatient, appointments, myappts);

    if (myappts.size() == 0)
    {
        drawtext("No appointments found.", 300, 100, 14, Color(150, 150, 150));
    }
    else
    {
        const char* headers[] = { "ID", "Doctor ID", "Date", "Time Slot", "Status" };
        float widths[] = { 80, 100, 130, 110, 120 };
        drawtableheader(headers, widths, 5, 300, 80);

        for (int i = 0; i < myappts.size(); i++)
        {
            Appointment& a = myappts.get(i);
            char apptid[10], docid[10];
            inttostr(a.getappointmentid(), apptid);
            inttostr(a.getdoctorid(), docid);
            const char* vals[] = { apptid, docid, a.getdate(), a.gettimeslot(), a.getstatus() };
            Color rowcolor;
            if (i % 2 == 0)
            {
                rowcolor = Color::White;
            }
            else
            {
                rowcolor = Color(240, 244, 255);
            }
            drawtablerow(vals, widths, 5, 300, 115 + i * 32, rowcolor);
        }
    }

    drawbutton("Back", 300, 640, 150, 45, Color(150, 150, 150));
    if (buttonclicked(300, 640, 150, 45, mousepos, event))
    {
        setscreen(Screen::PatientDashboard);
    }
}

void GUI::drawviewmedicalrecords(Event& event, Vector2f mousepos)
{
    drawtext("My Medical Records", 300, 30, 22, Color(26, 115, 232));

    Storage<Prescription> mypres;
    PatientMenu::getpatientprescriptions(*loggedinpatient, prescriptions, mypres);

    if (mypres.size() == 0)
    {
        drawtext("No medical records found.", 300, 100, 14, Color(150, 150, 150));
    }
    else
    {
        const char* headers[] = { "Date", "Doctor ID", "Medicines", "Notes" };
        float widths[] = { 120, 100, 300, 280 };
        drawtableheader(headers, widths, 4, 300, 80);

        for (int i = 0; i < mypres.size(); i++)
        {
            Prescription& p = mypres.get(i);
            char docid[10];
            inttostr(p.getdoctorid(), docid);
            const char* vals[] = { p.getdate(), docid, p.getmedicines(), p.getnotes() };
            Color rowcolor;
            if (i % 2 == 0)
            {
                rowcolor = Color::White;
            }
            else
            {
                rowcolor = Color(240, 244, 255);
            }
            drawtablerow(vals, widths, 4, 300, 115 + i * 32, rowcolor);
        }
    }

    drawbutton("Back", 300, 640, 150, 45, Color(150, 150, 150));
    if (buttonclicked(300, 640, 150, 45, mousepos, event))
    {
        setscreen(Screen::PatientDashboard);
    }
}

void GUI::drawviewbills(Event& event, Vector2f mousepos)
{
    drawtext("My Bills", 300, 30, 22, Color(26, 115, 232));

    Storage<Bill> mybills;
    PatientMenu::getpatientbills(*loggedinpatient, bills, mybills);

    if (mybills.size() == 0)
    {
        drawtext("No bills found.", 300, 100, 14, Color(150, 150, 150));
    }
    else
    {
        const char* headers[] = { "Bill ID", "Appt ID", "Amount", "Status", "Date" };
        float widths[] = { 100, 100, 120, 120, 120 };
        drawtableheader(headers, widths, 5, 300, 80);

        float totalunpaid = 0;
        for (int i = 0; i < mybills.size(); i++)
        {
            Bill& b = mybills.get(i);
            char billid[10], apptid[10], amount[20];
            inttostr(b.getbillid(), billid);
            inttostr(b.getappointmentid(), apptid);
            floattostr(b.getamount(), amount);
            const char* vals[] = { billid, apptid, amount, b.getstatus(), b.getdate() };
            Color rowcolor;
            if (i % 2 == 0)
            {
                rowcolor = Color::White;
            }
            else
            {
                rowcolor = Color(240, 244, 255);
            }
            drawtablerow(vals, widths, 5, 300, 115 + i * 32, rowcolor);

            if (strequal(b.getstatus(), "unpaid"))
            {
                totalunpaid += b.getamount();
            }
        }

        char totalstr[50];
        strcopy(totalstr, "Total Outstanding: PKR ", 50);
        int tl = 0;
        while (totalstr[tl])
        {
            tl++;
        }
        char amtbuf[20];
        floattostr(totalunpaid, amtbuf);
        int ai = 0;
        while (amtbuf[ai] && tl < 49)
        {
            totalstr[tl++] = amtbuf[ai++];
        }
        totalstr[tl] = '\0';

        drawtext(totalstr, 300, 600, 15, Color(220, 53, 69));
    }

    drawbutton("Back", 300, 640, 150, 45, Color(150, 150, 150));
    if (buttonclicked(300, 640, 150, 45, mousepos, event))
    {
        setscreen(Screen::PatientDashboard);
    }
}

void GUI::drawpaybill(Event& event, Vector2f mousepos)
{
    drawtext("Pay Bill", 300, 30, 22, Color(26, 115, 232));

    const char* headers[] = { "Bill ID", "Appt ID", "Amount", "Date" };
    float widths[] = { 100, 100, 120, 120 };
    drawtableheader(headers, widths, 4, 300, 80);

    int row = 0;
    for (int i = 0; i < bills.size(); i++)
    {
        Bill& b = bills.get(i);
        if (b.getpatientid() == loggedinpatient->getid() && strequal(b.getstatus(), "unpaid"))
        {
            char billid[10], apptid[10], amount[20];
            inttostr(b.getbillid(), billid);
            inttostr(b.getappointmentid(), apptid);
            floattostr(b.getamount(), amount);
            const char* vals[] = { billid, apptid, amount, b.getdate() };
            Color rowcolor;
            if (row % 2 == 0)
            {
                rowcolor = Color::White;
            }
            else
            {
                rowcolor = Color(240, 244, 255);
            }
            drawtablerow(vals, widths, 4, 300, 115 + row * 32, rowcolor);
            row++;
        }
    }

    if (row == 0)
    {
        drawtext("No unpaid bills.", 300, 130, 14, Color(150, 150, 150));
    }

    if (event.type == Event::MouseButtonReleased)
    {
        FloatRect box1(300, 420, 200, 40);
        input1active = box1.contains(mousepos);
    }
    handleinput(event, inputbuf1, input1active, false);
    drawinputbox("Bill ID to pay", 300, 420, 200, 40, inputbuf1, input1active, false);

    drawbutton("Pay", 300, 480, 150, 45, Color(26, 115, 232));
    drawbutton("Back", 470, 480, 150, 45, Color(150, 150, 150));

    if (buttonclicked(300, 480, 150, 45, mousepos, event))
    {
        char msgerr[200];
        int billid = strtoint(inputbuf1);
        bool result = PatientMenu::paybill(*loggedinpatient, patients, bills, billid, msgerr);
        if (result)
        {
            setsuccess(msgerr);
        }
        else
        {
            seterror(msgerr);
        }
    }

    if (buttonclicked(470, 480, 150, 45, mousepos, event))
    {
        setscreen(Screen::PatientDashboard);
    }
}

void GUI::drawtopupbalance(Event& event, Vector2f mousepos)
{
    drawtext("Top Up Balance", 490, 200, 22, Color(26, 115, 232));

    if (event.type == Event::MouseButtonReleased)
    {
        FloatRect box1(390, 270, 400, 45);
        input1active = box1.contains(mousepos);
    }
    handleinput(event, inputbuf1, input1active, false);
    drawinputbox("Amount (PKR)", 390, 270, 400, 45, inputbuf1, input1active, false);

    drawbutton("Top Up", 490, 340, 200, 45, Color(26, 115, 232));
    drawbutton("Back", 710, 340, 150, 45, Color(150, 150, 150));

    if (buttonclicked(490, 340, 200, 45, mousepos, event))
    {
        char msgerr[200];
        float amount = strtofloat(inputbuf1);
        bool result = PatientMenu::topupbalance(*loggedinpatient, patients, amount, msgerr);
        if (result)
        {
            setsuccess(msgerr);
        }
        else
        {
            seterror(msgerr);
        }
    }

    if (buttonclicked(710, 340, 150, 45, mousepos, event))
    {
        setscreen(Screen::PatientDashboard);
    }
}

void GUI::drawdoctordashboard(Event& event, Vector2f mousepos)
{
    char welcomemsg[100];
    strcopy(welcomemsg, "Welcome, Dr. ", 100);
    int wl = 0;
    while (welcomemsg[wl])
    {
        wl++;
    }
    const char* dname = loggedindoctor->getname();
    int ni = 0;
    while (dname[ni] && wl < 99)
    {
        welcomemsg[wl++] = dname[ni++];
    }
    welcomemsg[wl] = '\0';

    drawtext(welcomemsg, 40, 30, 22, Color(26, 115, 232));
    drawtext(loggedindoctor->getspecialization(), 40, 65, 15, Color(80, 80, 80));

    float btnx = 40;
    float btny = 120;
    float btnw = 250;
    float btnh = 50;
    float gap = 15;

    drawbutton("View Today's Appointments", btnx, btny, btnw, btnh, Color(26, 115, 232));
    drawbutton("Mark Appointment Complete", btnx, btny + (btnh + gap), btnw, btnh, Color(26, 115, 232));
    drawbutton("Mark Appointment No-Show", btnx, btny + (btnh + gap) * 2, btnw, btnh, Color(26, 115, 232));
    drawbutton("Write Prescription", btnx, btny + (btnh + gap) * 3, btnw, btnh, Color(26, 115, 232));
    drawbutton("View Patient History", btnx, btny + (btnh + gap) * 4, btnw, btnh, Color(26, 115, 232));
    drawbutton("Logout", btnx, btny + (btnh + gap) * 5, btnw, btnh, Color(180, 50, 50));

    if (buttonclicked(btnx, btny, btnw, btnh, mousepos, event)) { setscreen(Screen::ViewTodaysAppointments); }
    if (buttonclicked(btnx, btny + (btnh + gap), btnw, btnh, mousepos, event)) { setscreen(Screen::MarkComplete); }
    if (buttonclicked(btnx, btny + (btnh + gap) * 2, btnw, btnh, mousepos, event)) { setscreen(Screen::MarkNoShow); }
    if (buttonclicked(btnx, btny + (btnh + gap) * 3, btnw, btnh, mousepos, event)) { setscreen(Screen::WritePrescription); }
    if (buttonclicked(btnx, btny + (btnh + gap) * 4, btnw, btnh, mousepos, event)) { setscreen(Screen::ViewPatientHistory); }
    if (buttonclicked(btnx, btny + (btnh + gap) * 5, btnw, btnh, mousepos, event))
    {
        loggedindoctor = nullptr;
        loginfailedattempts = 0;
        loginlocked = false;
        setscreen(Screen::Login);
    }
}

void GUI::drawviewtodaysappointments(Event& event, Vector2f mousepos)
{
    drawtext("Today's Appointments", 300, 30, 22, Color(26, 115, 232));

    Storage<Appointment> todaysappts;
    DoctorMenu::gettodaysappointments(*loggedindoctor, appointments, todaysappts);

    if (todaysappts.size() == 0)
    {
        drawtext("No appointments scheduled for today.", 300, 100, 14, Color(150, 150, 150));
    }
    else
    {
        const char* headers[] = { "Appt ID", "Patient ID", "Time Slot", "Status" };
        float widths[] = { 100, 120, 120, 120 };
        drawtableheader(headers, widths, 4, 300, 80);

        for (int i = 0; i < todaysappts.size(); i++)
        {
            Appointment& a = todaysappts.get(i);
            char apptid[10], patid[10];
            inttostr(a.getappointmentid(), apptid);
            inttostr(a.getpatientid(), patid);
            const char* vals[] = { apptid, patid, a.gettimeslot(), a.getstatus() };
            Color rowcolor;
            if (i % 2 == 0)
            {
                rowcolor = Color::White;
            }
            else
            {
                rowcolor = Color(240, 244, 255);
            }
            drawtablerow(vals, widths, 4, 300, 115 + i * 32, rowcolor);
        }
    }

    drawbutton("Back", 300, 640, 150, 45, Color(150, 150, 150));
    if (buttonclicked(300, 640, 150, 45, mousepos, event))
    {
        setscreen(Screen::DoctorDashboard);
    }
}

void GUI::drawmarkcomplete(Event& event, Vector2f mousepos)
{
    drawtext("Mark Appointment Complete", 300, 30, 22, Color(26, 115, 232));

    Storage<Appointment> todaysappts;
    DoctorMenu::gettodaysappointments(*loggedindoctor, appointments, todaysappts);

    const char* headers[] = { "Appt ID", "Patient ID", "Time Slot", "Status" };
    float widths[] = { 100, 120, 120, 120 };
    drawtableheader(headers, widths, 4, 300, 80);

    int row = 0;
    for (int i = 0; i < todaysappts.size(); i++)
    {
        Appointment& a = todaysappts.get(i);
        if (strequal(a.getstatus(), "pending"))
        {
            char apptid[10], patid[10];
            inttostr(a.getappointmentid(), apptid);
            inttostr(a.getpatientid(), patid);
            const char* vals[] = { apptid, patid, a.gettimeslot(), a.getstatus() };
            Color rowcolor;
            if (row % 2 == 0)
            {
                rowcolor = Color::White;
            }
            else
            {
                rowcolor = Color(240, 244, 255);
            }
            drawtablerow(vals, widths, 4, 300, 115 + row * 32, rowcolor);
            row++;
        }
    }

    if (event.type == Event::MouseButtonReleased)
    {
        FloatRect box1(300, 420, 200, 40);
        input1active = box1.contains(mousepos);
    }
    handleinput(event, inputbuf1, input1active, false);
    drawinputbox("Appointment ID", 300, 420, 200, 40, inputbuf1, input1active, false);

    drawbutton("Mark Complete", 300, 480, 180, 45, Color(40, 167, 69));
    drawbutton("Back", 500, 480, 150, 45, Color(150, 150, 150));

    if (buttonclicked(300, 480, 180, 45, mousepos, event))
    {
        char msgerr[200];
        int apptid = strtoint(inputbuf1);
        bool result = DoctorMenu::markcomplete(*loggedindoctor, appointments, apptid, msgerr);
        if (result)
        {
            setsuccess(msgerr);
        }
        else
        {
            seterror(msgerr);
        }
    }

    if (buttonclicked(500, 480, 150, 45, mousepos, event))
    {
        setscreen(Screen::DoctorDashboard);
    }
}

void GUI::drawmarknoshow(Event& event, Vector2f mousepos)
{
    drawtext("Mark Appointment No-Show", 300, 30, 22, Color(26, 115, 232));

    Storage<Appointment> todaysappts;
    DoctorMenu::gettodaysappointments(*loggedindoctor, appointments, todaysappts);

    const char* headers[] = { "Appt ID", "Patient ID", "Time Slot", "Status" };
    float widths[] = { 100, 120, 120, 120 };
    drawtableheader(headers, widths, 4, 300, 80);

    int row = 0;
    for (int i = 0; i < todaysappts.size(); i++)
    {
        Appointment& a = todaysappts.get(i);
        if (strequal(a.getstatus(), "pending"))
        {
            char apptid[10], patid[10];
            inttostr(a.getappointmentid(), apptid);
            inttostr(a.getpatientid(), patid);
            const char* vals[] = { apptid, patid, a.gettimeslot(), a.getstatus() };
            Color rowcolor;
            if (row % 2 == 0)
            {
                rowcolor = Color::White;
            }
            else
            {
                rowcolor = Color(240, 244, 255);
            }
            drawtablerow(vals, widths, 4, 300, 115 + row * 32, rowcolor);
            row++;
        }
    }

    if (event.type == Event::MouseButtonReleased)
    {
        FloatRect box1(300, 420, 200, 40);
        input1active = box1.contains(mousepos);
    }
    handleinput(event, inputbuf1, input1active, false);
    drawinputbox("Appointment ID", 300, 420, 200, 40, inputbuf1, input1active, false);

    drawbutton("Mark No-Show", 300, 480, 180, 45, Color(220, 53, 69));
    drawbutton("Back", 500, 480, 150, 45, Color(150, 150, 150));

    if (buttonclicked(300, 480, 180, 45, mousepos, event))
    {
        char msgerr[200];
        int apptid = strtoint(inputbuf1);
        bool result = DoctorMenu::marknoshow(*loggedindoctor, appointments, bills, apptid, msgerr);
        if (result)
        {
            setsuccess(msgerr);
        }
        else
        {
            seterror(msgerr);
        }
    }

    if (buttonclicked(500, 480, 150, 45, mousepos, event))
    {
        setscreen(Screen::DoctorDashboard);
    }
}

void GUI::drawwriteprescription(Event& event, Vector2f mousepos)
{
    drawtext("Write Prescription", 300, 30, 22, Color(26, 115, 232));

    if (event.type == Event::MouseButtonReleased)
    {
        FloatRect box1(300, 100, 200, 40);
        FloatRect box2(300, 180, 600, 40);
        FloatRect box3(300, 260, 600, 40);
        input1active = box1.contains(mousepos);
        input2active = box2.contains(mousepos);
        input3active = box3.contains(mousepos);
    }
    handleinput(event, inputbuf1, input1active, false);
    handleinput(event, inputbuf2, input2active, false);
    handleinput(event, inputbuf3, input3active, false);

    drawinputbox("Appointment ID", 300, 100, 200, 40, inputbuf1, input1active, false);
    drawinputbox("Medicines", 300, 180, 600, 40, inputbuf2, input2active, false);
    drawinputbox("Notes", 300, 260, 600, 40, inputbuf3, input3active, false);

    drawbutton("Save Prescription", 300, 330, 200, 45, Color(26, 115, 232));
    drawbutton("Back", 520, 330, 150, 45, Color(150, 150, 150));

    if (buttonclicked(300, 330, 200, 45, mousepos, event))
    {
        char msgerr[200];
        int apptid = strtoint(inputbuf1);
        bool result = DoctorMenu::writeprescription(*loggedindoctor, appointments, prescriptions, apptid, inputbuf2, inputbuf3, msgerr);
        if (result)
        {
            setsuccess(msgerr);
        }
        else
        {
            seterror(msgerr);
        }
    }

    if (buttonclicked(520, 330, 150, 45, mousepos, event))
    {
        setscreen(Screen::DoctorDashboard);
    }
}

void GUI::drawviewpatienthistory(Event& event, Vector2f mousepos)
{
    drawtext("View Patient History", 300, 30, 22, Color(26, 115, 232));

    if (event.type == Event::MouseButtonReleased)
    {
        FloatRect box1(300, 80, 200, 40);
        input1active = box1.contains(mousepos);
    }
    handleinput(event, inputbuf1, input1active, false);
    drawinputbox("Patient ID", 300, 80, 200, 40, inputbuf1, input1active, false);

    drawbutton("Search", 520, 80, 120, 40, Color(26, 115, 232));

    if (buttonclicked(520, 80, 120, 40, mousepos, event))
    {
        char msgerr[200];
        int patientid = strtoint(inputbuf1);
        Storage<Prescription> history;
        bool result = DoctorMenu::getpatienthistory(*loggedindoctor, patients, appointments, prescriptions, patientid, history, msgerr);
        if (!result)
        {
            seterror(msgerr);
        }
        else
        {
            const char* headers[] = { "Date", "Medicines", "Notes" };
            float widths[] = { 120, 350, 330 };
            drawtableheader(headers, widths, 3, 300, 150);

            for (int i = 0; i < history.size(); i++)
            {
                Prescription& p = history.get(i);
                const char* vals[] = { p.getdate(), p.getmedicines(), p.getnotes() };
                Color rowcolor;
                if (i % 2 == 0)
                {
                    rowcolor = Color::White;
                }
                else
                {
                    rowcolor = Color(240, 244, 255);
                }
                drawtablerow(vals, widths, 3, 300, 185 + i * 32, rowcolor);
            }
        }
    }

    drawbutton("Back", 300, 640, 150, 45, Color(150, 150, 150));
    if (buttonclicked(300, 640, 150, 45, mousepos, event))
    {
        setscreen(Screen::DoctorDashboard);
    }
}

void GUI::drawadmindashboard(Event& event, Vector2f mousepos)
{
    drawtext("Admin Panel - Medicare", 40, 30, 22, Color(26, 115, 232));

    float btnx = 40;
    float btny = 90;
    float btnw = 250;
    float btnh = 45;
    float gap = 12;

    drawbutton("Add Doctor", btnx, btny, btnw, btnh, Color(26, 115, 232));
    drawbutton("Remove Doctor", btnx, btny + (btnh + gap), btnw, btnh, Color(26, 115, 232));
    drawbutton("View All Patients", btnx, btny + (btnh + gap) * 2, btnw, btnh, Color(26, 115, 232));
    drawbutton("View All Doctors", btnx, btny + (btnh + gap) * 3, btnw, btnh, Color(26, 115, 232));
    drawbutton("View All Appointments", btnx, btny + (btnh + gap) * 4, btnw, btnh, Color(26, 115, 232));
    drawbutton("View Unpaid Bills", btnx, btny + (btnh + gap) * 5, btnw, btnh, Color(26, 115, 232));
    drawbutton("Discharge Patient", btnx, btny + (btnh + gap) * 6, btnw, btnh, Color(26, 115, 232));
    drawbutton("View Security Log", btnx, btny + (btnh + gap) * 7, btnw, btnh, Color(26, 115, 232));
    drawbutton("Generate Daily Report", btnx, btny + (btnh + gap) * 8, btnw, btnh, Color(26, 115, 232));
    drawbutton("Logout", btnx, btny + (btnh + gap) * 9, btnw, btnh, Color(180, 50, 50));

    if (buttonclicked(btnx, btny, btnw, btnh, mousepos, event)) { setscreen(Screen::AddDoctor); }
    if (buttonclicked(btnx, btny + (btnh + gap), btnw, btnh, mousepos, event)) { setscreen(Screen::RemoveDoctor); }
    if (buttonclicked(btnx, btny + (btnh + gap) * 2, btnw, btnh, mousepos, event)) { setscreen(Screen::ViewAllPatients); }
    if (buttonclicked(btnx, btny + (btnh + gap) * 3, btnw, btnh, mousepos, event)) { setscreen(Screen::ViewAllDoctors); }
    if (buttonclicked(btnx, btny + (btnh + gap) * 4, btnw, btnh, mousepos, event)) { setscreen(Screen::ViewAllAppointments); }
    if (buttonclicked(btnx, btny + (btnh + gap) * 5, btnw, btnh, mousepos, event)) { setscreen(Screen::ViewUnpaidBills); }
    if (buttonclicked(btnx, btny + (btnh + gap) * 6, btnw, btnh, mousepos, event)) { setscreen(Screen::DischargePatient); }
    if (buttonclicked(btnx, btny + (btnh + gap) * 7, btnw, btnh, mousepos, event)) { setscreen(Screen::ViewSecurityLog); }
    if (buttonclicked(btnx, btny + (btnh + gap) * 8, btnw, btnh, mousepos, event)) { setscreen(Screen::GenerateDailyReport); }
    if (buttonclicked(btnx, btny + (btnh + gap) * 9, btnw, btnh, mousepos, event))
    {
        loggedinadmin = nullptr;
        loginfailedattempts = 0;
        loginlocked = false;
        setscreen(Screen::Login);
    }
}

void GUI::drawadddoctor(Event& event, Vector2f mousepos)
{
    drawtext("Add Doctor", 300, 30, 22, Color(26, 115, 232));

    if (event.type == Event::MouseButtonReleased)
    {
        FloatRect box1(300, 100, 400, 40);
        FloatRect box2(300, 180, 400, 40);
        FloatRect box3(300, 260, 400, 40);
        FloatRect box4(300, 340, 400, 40);
        FloatRect box5(300, 420, 400, 40);
        input1active = box1.contains(mousepos);
        input2active = box2.contains(mousepos);
        input3active = box3.contains(mousepos);
        input4active = box4.contains(mousepos);
        input5active = box5.contains(mousepos);
    }

    handleinput(event, inputbuf1, input1active, false);
    handleinput(event, inputbuf2, input2active, false);
    handleinput(event, inputbuf3, input3active, false);
    handleinput(event, inputbuf4, input4active, true);
    handleinput(event, inputbuf5, input5active, false);

    drawinputbox("Name", 300, 100, 400, 40, inputbuf1, input1active, false);
    drawinputbox("Specialization", 300, 180, 400, 40, inputbuf2, input2active, false);
    drawinputbox("Contact (11 digits)", 300, 260, 400, 40, inputbuf3, input3active, false);
    drawinputbox("Password (min 6 chars)", 300, 340, 400, 40, inputbuf4, input4active, true);
    drawinputbox("Fee", 300, 420, 400, 40, inputbuf5, input5active, false);

    drawbutton("Add Doctor", 300, 490, 180, 45, Color(26, 115, 232));
    drawbutton("Back", 500, 490, 150, 45, Color(150, 150, 150));

    if (buttonclicked(300, 490, 180, 45, mousepos, event))
    {
        char msgerr[200];
        float fee = strtofloat(inputbuf5);
        bool result = AdminMenu::adddoctor(doctors, inputbuf1, inputbuf2, inputbuf3, inputbuf4, fee, msgerr);
        if (result)
        {
            setsuccess(msgerr);
        }
        else
        {
            seterror(msgerr);
        }
    }

    if (buttonclicked(500, 490, 150, 45, mousepos, event))
    {
        setscreen(Screen::AdminDashboard);
    }
}

void GUI::drawremovedoctor(Event& event, Vector2f mousepos)
{
    drawtext("Remove Doctor", 300, 30, 22, Color(26, 115, 232));

    const char* headers[] = { "ID", "Name", "Specialization", "Fee" };
    float widths[] = { 80, 200, 200, 120 };
    drawtableheader(headers, widths, 4, 300, 80);

    for (int i = 0; i < doctors.size(); i++)
    {
        Doctor& d = doctors.get(i);
        char doctorid[10], doctorfee[20];
        inttostr(d.getid(), doctorid);
        floattostr(d.getfee(), doctorfee);
        const char* vals[] = { doctorid, d.getname(), d.getspecialization(), doctorfee };
        Color rowcolor;
        if (i % 2 == 0)
        {
            rowcolor = Color::White;
        }
        else
        {
            rowcolor = Color(240, 244, 255);
        }
        drawtablerow(vals, widths, 4, 300, 115 + i * 32, rowcolor);
    }

    if (event.type == Event::MouseButtonReleased)
    {
        FloatRect box1(300, 480, 200, 40);
        input1active = box1.contains(mousepos);
    }
    handleinput(event, inputbuf1, input1active, false);
    drawinputbox("Doctor ID to remove", 300, 480, 200, 40, inputbuf1, input1active, false);

    drawbutton("Remove", 300, 540, 150, 45, Color(220, 53, 69));
    drawbutton("Back", 470, 540, 150, 45, Color(150, 150, 150));

    if (buttonclicked(300, 540, 150, 45, mousepos, event))
    {
        char msgerr[200];
        int doctorid = strtoint(inputbuf1);
        bool result = AdminMenu::removedoctor(doctors, appointments, doctorid, msgerr);
        if (result)
        {
            setsuccess(msgerr);
        }
        else
        {
            seterror(msgerr);
        }
    }

    if (buttonclicked(470, 540, 150, 45, mousepos, event))
    {
        setscreen(Screen::AdminDashboard);
    }
}

void GUI::drawviewallpatients(Event& event, Vector2f mousepos)
{
    drawtext("All Patients", 300, 30, 22, Color(26, 115, 232));

    const char* headers[] = { "ID", "Name", "Age", "Gender", "Contact", "Balance" };
    float widths[] = { 70, 160, 60, 80, 140, 120 };
    drawtableheader(headers, widths, 6, 300, 80);

    for (int i = 0; i < patients.size(); i++)
    {
        Patient& p = patients.get(i);
        char patientid[10], patientage[10], patientbalance[20];
        inttostr(p.getid(), patientid);
        inttostr(p.getage(), patientage);
        floattostr(p.getbalance(), patientbalance);
        char gender[2] = { p.getgender()[0], '\0' };
        const char* vals[] = { patientid, p.getname(), patientage, gender, p.getcontact(), patientbalance };
        Color rowcolor;
        if (i % 2 == 0)
        {
            rowcolor = Color::White;
        }
        else
        {
            rowcolor = Color(240, 244, 255);
        }
        drawtablerow(vals, widths, 6, 300, 115 + i * 32, rowcolor);
    }

    drawbutton("Back", 300, 640, 150, 45, Color(150, 150, 150));
    if (buttonclicked(300, 640, 150, 45, mousepos, event))
    {
        setscreen(Screen::AdminDashboard);
    }
}

void GUI::drawviewalldoctors(Event& event, Vector2f mousepos)
{
    drawtext("All Doctors", 300, 30, 22, Color(26, 115, 232));

    const char* headers[] = { "ID", "Name", "Specialization", "Contact", "Fee" };
    float widths[] = { 70, 160, 180, 150, 120 };
    drawtableheader(headers, widths, 5, 300, 80);

    for (int i = 0; i < doctors.size(); i++)
    {
        Doctor& d = doctors.get(i);
        char doctorid[10], doctorfee[20];
        inttostr(d.getid(), doctorid);
        floattostr(d.getfee(), doctorfee);
        const char* vals[] = { doctorid, d.getname(), d.getspecialization(), d.getcontact(), doctorfee };
        Color rowcolor;
        if (i % 2 == 0)
        {
            rowcolor = Color::White;
        }
        else
        {
            rowcolor = Color(240, 244, 255);
        }
        drawtablerow(vals, widths, 5, 300, 115 + i * 32, rowcolor);
    }

    drawbutton("Back", 300, 640, 150, 45, Color(150, 150, 150));
    if (buttonclicked(300, 640, 150, 45, mousepos, event))
    {
        setscreen(Screen::AdminDashboard);
    }
}

void GUI::drawviewallappointments(Event& event, Vector2f mousepos)
{
    drawtext("All Appointments", 300, 30, 22, Color(26, 115, 232));

    Storage<Appointment> sortedappts;
    AdminMenu::getallappointmentssorteddesc(appointments, sortedappts);

    const char* headers[] = { "ID", "Patient ID", "Doctor ID", "Date", "Slot", "Status" };
    float widths[] = { 70, 100, 100, 130, 90, 120 };
    drawtableheader(headers, widths, 6, 300, 80);

    for (int i = 0; i < sortedappts.size(); i++)
    {
        Appointment& a = sortedappts.get(i);
        char apptid[10], patientid[10], doctorid[10];
        inttostr(a.getappointmentid(), apptid);
        inttostr(a.getpatientid(), patientid);
        inttostr(a.getdoctorid(), doctorid);
        const char* vals[] = { apptid, patientid, doctorid, a.getdate(), a.gettimeslot(), a.getstatus() };
        Color rowcolor;
        if (i % 2 == 0)
        {
            rowcolor = Color::White;
        }
        else
        {
            rowcolor = Color(240, 244, 255);
        }
        drawtablerow(vals, widths, 6, 300, 115 + i * 32, rowcolor);
    }

    drawbutton("Back", 300, 640, 150, 45, Color(150, 150, 150));
    if (buttonclicked(300, 640, 150, 45, mousepos, event))
    {
        setscreen(Screen::AdminDashboard);
    }
}

void GUI::drawviewunpaidbills(Event& event, Vector2f mousepos)
{
    drawtext("Unpaid Bills", 300, 30, 22, Color(26, 115, 232));

    Storage<Bill> unpaidbills;
    AdminMenu::getallunpaidbills(bills, unpaidbills);

    const char* headers[] = { "Bill ID", "Patient ID", "Amount", "Date" };
    float widths[] = { 100, 120, 130, 180 };
    drawtableheader(headers, widths, 4, 300, 80);

    for (int i = 0; i < unpaidbills.size(); i++)
    {
        Bill& b = unpaidbills.get(i);
        char billid[10], patientid[10], amount[20];
        inttostr(b.getbillid(), billid);
        inttostr(b.getpatientid(), patientid);
        floattostr(b.getamount(), amount);

        char datewithtag[30];
        strcopy(datewithtag, b.getdate(), 30);
        if (AdminMenu::isoverdue(b.getdate()))
        {
            int dl = 0;
            while (datewithtag[dl])
            {
                dl++;
            }
            char tag[] = " [OVERDUE]";
            int ti = 0;
            while (tag[ti] && dl < 29)
            {
                datewithtag[dl++] = tag[ti++];
            }
            datewithtag[dl] = '\0';
        }

        const char* vals[] = { billid, patientid, amount, datewithtag };
        Color rowcolor;
        if (i % 2 == 0)
        {
            rowcolor = Color::White;
        }
        else
        {
            rowcolor = Color(240, 244, 255);
        }
        drawtablerow(vals, widths, 4, 300, 115 + i * 32, rowcolor);
    }

    drawbutton("Back", 300, 640, 150, 45, Color(150, 150, 150));
    if (buttonclicked(300, 640, 150, 45, mousepos, event))
    {
        setscreen(Screen::AdminDashboard);
    }
}

void GUI::drawdischargepatient(Event& event, Vector2f mousepos)
{
    drawtext("Discharge Patient", 300, 30, 22, Color(26, 115, 232));

    if (event.type == Event::MouseButtonReleased)
    {
        FloatRect box1(300, 100, 200, 40);
        input1active = box1.contains(mousepos);
    }
    handleinput(event, inputbuf1, input1active, false);
    drawinputbox("Patient ID", 300, 100, 200, 40, inputbuf1, input1active, false);

    drawbutton("Discharge", 300, 170, 180, 45, Color(220, 53, 69));
    drawbutton("Back", 500, 170, 150, 45, Color(150, 150, 150));

    if (buttonclicked(300, 170, 180, 45, mousepos, event))
    {
        char msgerr[200];
        int patientid = strtoint(inputbuf1);
        bool result = AdminMenu::dischargepatient(patients, appointments, bills, prescriptions, patientid, msgerr);
        if (result)
        {
            setsuccess(msgerr);
        }
        else
        {
            seterror(msgerr);
        }
    }

    if (buttonclicked(500, 170, 150, 45, mousepos, event))
    {
        setscreen(Screen::AdminDashboard);
    }
}

void GUI::drawviewsecuritylog(Event& event, Vector2f mousepos)
{
    drawtext("Security Log", 300, 30, 22, Color(26, 115, 232));

    char logbuffer[4096];
    FileHandler::loadsecuritylog(logbuffer, 4096);

    if (logbuffer[0] == '\0')
    {
        drawtext("No security events logged.", 300, 100, 14, Color(150, 150, 150));
    }
    else
    {
        float y = 80;
        int i = 0;
        char linebuf[256];
        int li = 0;

        while (logbuffer[i] != '\0' && y < 630)
        {
            if (logbuffer[i] == '\n')
            {
                linebuf[li] = '\0';
                drawtext(linebuf, 300, y, 13, Color(40, 40, 40));
                y += 22;
                li = 0;
            }
            else
            {
                if (li < 255)
                {
                    linebuf[li++] = logbuffer[i];
                }
            }
            i++;
        }
    }

    drawbutton("Back", 300, 650, 150, 45, Color(150, 150, 150));
    if (buttonclicked(300, 650, 150, 45, mousepos, event))
    {
        setscreen(Screen::AdminDashboard);
    }
}

void GUI::drawgeneratedailyreport(Event& event, Vector2f mousepos)
{
    drawtext("Daily Report", 300, 30, 22, Color(26, 115, 232));

    char today[11];
    DoctorMenu::gettodaysdate(today);
    drawtext(today, 500, 30, 16, Color(100, 100, 100));

    int totalpending = 0;
    int totalcompleted = 0;
    int totalnoshow = 0;
    int totalcancelled = 0;
    float revenuecollected = 0;

    for (int i = 0; i < appointments.size(); i++)
    {
        Appointment& a = appointments.get(i);
        if (strequal(a.getdate(), today))
        {
            if (strequal(a.getstatus(), "pending"))
            {
                totalpending++;
            }
            if (strequal(a.getstatus(), "completed"))
            {
                totalcompleted++;
            }
            if (strequal(a.getstatus(), "noshow"))
            {
                totalnoshow++;
            }
            if (strequal(a.getstatus(), "cancelled"))
            {
                totalcancelled++;
            }
        }
    }

    for (int i = 0; i < bills.size(); i++)
    {
        Bill& b = bills.get(i);
        if (strequal(b.getdate(), today) && strequal(b.getstatus(), "paid"))
        {
            revenuecollected += b.getamount();
        }
    }

    char pendingstr[10], completedstr[10], noshowstr[10], cancelledstr[10], revenuestr[20];
    inttostr(totalpending, pendingstr);
    inttostr(totalcompleted, completedstr);
    inttostr(totalnoshow, noshowstr);
    inttostr(totalcancelled, cancelledstr);
    floattostr(revenuecollected, revenuestr);

    char line1[200];
    strcopy(line1, "Total Appointments Today  |  Pending: ", 200);
    int l = 0;
    while (line1[l]) { l++; }
    int k = 0;
    while (pendingstr[k] && l < 199) { line1[l++] = pendingstr[k++]; }
    line1[l] = '\0';
    drawtext(line1, 300, 90, 14, Color(40, 40, 40));

    char line2[100];
    strcopy(line2, "Completed: ", 100);
    l = 0; while (line2[l]) { l++; }
    k = 0; while (completedstr[k] && l < 99) { line2[l++] = completedstr[k++]; } line2[l] = '\0';
    drawtext(line2, 300, 115, 14, Color(40, 40, 40));

    char line3[100];
    strcopy(line3, "No-Show: ", 100);
    l = 0; while (line3[l]) { l++; }
    k = 0; while (noshowstr[k] && l < 99) { line3[l++] = noshowstr[k++]; } line3[l] = '\0';
    drawtext(line3, 300, 140, 14, Color(40, 40, 40));

    char line4[100];
    strcopy(line4, "Cancelled: ", 100);
    l = 0; while (line4[l]) { l++; }
    k = 0; while (cancelledstr[k] && l < 99) { line4[l++] = cancelledstr[k++]; } line4[l] = '\0';
    drawtext(line4, 300, 165, 14, Color(40, 40, 40));

    char revline[100];
    strcopy(revline, "Revenue Collected Today: PKR ", 100);
    l = 0; while (revline[l]) { l++; }
    k = 0; while (revenuestr[k] && l < 99) { revline[l++] = revenuestr[k++]; } revline[l] = '\0';
    drawtext(revline, 300, 210, 14, Color(40, 167, 69));

    drawtext("Patients with Outstanding Unpaid Bills:", 300, 255, 14, Color(220, 53, 69));

    float y = 280;
    for (int i = 0; i < patients.size(); i++)
    {
        float total = 0;
        for (int j = 0; j < bills.size(); j++)
        {
            if (bills.get(j).getpatientid() == patients.get(i).getid() && strequal(bills.get(j).getstatus(), "unpaid"))
            {
                total += bills.get(j).getamount();
            }
        }
        if (total > 0)
        {
            char owedstr[20];
            floattostr(total, owedstr);
            char rowtext[100];
            strcopy(rowtext, patients.get(i).getname(), 100);
            l = 0;
            while (rowtext[l]) { l++; }
            char sep[] = " - PKR ";
            k = 0;
            while (sep[k] && l < 99) { rowtext[l++] = sep[k++]; }
            rowtext[l] = '\0';
            k = 0;
            while (owedstr[k] && l < 99) { rowtext[l++] = owedstr[k++]; }
            rowtext[l] = '\0';
            drawtext(rowtext, 300, y, 13, Color(40, 40, 40));
            y += 22;
        }
    }

    drawbutton("Back", 300, 650, 150, 45, Color(150, 150, 150));
    if (buttonclicked(300, 650, 150, 45, mousepos, event))
    {
        setscreen(Screen::AdminDashboard);
    }
}

void GUI::run()
{
    Clock clock;

    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds();

        if (msgtimer > 0)
        {
            msgtimer -= dt;
            if (msgtimer <= 0)
            {
                showerror = false;
                showsuccess = false;
            }
        }

        Event event;
        event.type = Event::Count;
        Vector2f mousepos = window.mapPixelToCoords(Mouse::getPosition(window));

        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                window.close();
            }

            if (currentscreen == Screen::Login)                       drawlogin(event, mousepos);
            else if (currentscreen == Screen::PatientDashboard)       drawpatientdashboard(event, mousepos);
            else if (currentscreen == Screen::BookAppointment)        drawbookappointment(event, mousepos);
            else if (currentscreen == Screen::CancelAppointment)      drawcancelappointment(event, mousepos);
            else if (currentscreen == Screen::ViewAppointments)       drawviewappointments(event, mousepos);
            else if (currentscreen == Screen::ViewMedicalRecords)     drawviewmedicalrecords(event, mousepos);
            else if (currentscreen == Screen::ViewBills)              drawviewbills(event, mousepos);
            else if (currentscreen == Screen::PayBill)                drawpaybill(event, mousepos);
            else if (currentscreen == Screen::TopUpBalance)           drawtopupbalance(event, mousepos);
            else if (currentscreen == Screen::DoctorDashboard)        drawdoctordashboard(event, mousepos);
            else if (currentscreen == Screen::ViewTodaysAppointments) drawviewtodaysappointments(event, mousepos);
            else if (currentscreen == Screen::MarkComplete)           drawmarkcomplete(event, mousepos);
            else if (currentscreen == Screen::MarkNoShow)             drawmarknoshow(event, mousepos);
            else if (currentscreen == Screen::WritePrescription)      drawwriteprescription(event, mousepos);
            else if (currentscreen == Screen::ViewPatientHistory)     drawviewpatienthistory(event, mousepos);
            else if (currentscreen == Screen::AdminDashboard)         drawadmindashboard(event, mousepos);
            else if (currentscreen == Screen::AddDoctor)              drawadddoctor(event, mousepos);
            else if (currentscreen == Screen::RemoveDoctor)           drawremovedoctor(event, mousepos);
            else if (currentscreen == Screen::ViewAllPatients)        drawviewallpatients(event, mousepos);
            else if (currentscreen == Screen::ViewAllDoctors)         drawviewalldoctors(event, mousepos);
            else if (currentscreen == Screen::ViewAllAppointments)    drawviewallappointments(event, mousepos);
            else if (currentscreen == Screen::ViewUnpaidBills)        drawviewunpaidbills(event, mousepos);
            else if (currentscreen == Screen::DischargePatient)       drawdischargepatient(event, mousepos);
            else if (currentscreen == Screen::ViewSecurityLog)        drawviewsecuritylog(event, mousepos);
            else if (currentscreen == Screen::GenerateDailyReport)    drawgeneratedailyreport(event, mousepos);
        }

        event.type = Event::Count;

        drawbackground();

        if (currentscreen == Screen::Login)                       drawlogin(event, mousepos);
        else if (currentscreen == Screen::PatientDashboard)       drawpatientdashboard(event, mousepos);
        else if (currentscreen == Screen::BookAppointment)        drawbookappointment(event, mousepos);
        else if (currentscreen == Screen::CancelAppointment)      drawcancelappointment(event, mousepos);
        else if (currentscreen == Screen::ViewAppointments)       drawviewappointments(event, mousepos);
        else if (currentscreen == Screen::ViewMedicalRecords)     drawviewmedicalrecords(event, mousepos);
        else if (currentscreen == Screen::ViewBills)              drawviewbills(event, mousepos);
        else if (currentscreen == Screen::PayBill)                drawpaybill(event, mousepos);
        else if (currentscreen == Screen::TopUpBalance)           drawtopupbalance(event, mousepos);
        else if (currentscreen == Screen::DoctorDashboard)        drawdoctordashboard(event, mousepos);
        else if (currentscreen == Screen::ViewTodaysAppointments) drawviewtodaysappointments(event, mousepos);
        else if (currentscreen == Screen::MarkComplete)           drawmarkcomplete(event, mousepos);
        else if (currentscreen == Screen::MarkNoShow)             drawmarknoshow(event, mousepos);
        else if (currentscreen == Screen::WritePrescription)      drawwriteprescription(event, mousepos);
        else if (currentscreen == Screen::ViewPatientHistory)     drawviewpatienthistory(event, mousepos);
        else if (currentscreen == Screen::AdminDashboard)         drawadmindashboard(event, mousepos);
        else if (currentscreen == Screen::AddDoctor)              drawadddoctor(event, mousepos);
        else if (currentscreen == Screen::RemoveDoctor)           drawremovedoctor(event, mousepos);
        else if (currentscreen == Screen::ViewAllPatients)        drawviewallpatients(event, mousepos);
        else if (currentscreen == Screen::ViewAllDoctors)         drawviewalldoctors(event, mousepos);
        else if (currentscreen == Screen::ViewAllAppointments)    drawviewallappointments(event, mousepos);
        else if (currentscreen == Screen::ViewUnpaidBills)        drawviewunpaidbills(event, mousepos);
        else if (currentscreen == Screen::DischargePatient)       drawdischargepatient(event, mousepos);
        else if (currentscreen == Screen::ViewSecurityLog)        drawviewsecuritylog(event, mousepos);
        else if (currentscreen == Screen::GenerateDailyReport)    drawgeneratedailyreport(event, mousepos);

        drawpopup();
        window.display();
    }
}