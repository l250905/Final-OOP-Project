#define _CRT_SECURE_NO_WARNINGS
#include "Validator.h"
#include <ctime>

static char tolower(char c)
{
    if (c >= 'A' && c <= 'Z')
    {
        return c + 32;
    }
    else
    {
        return c;
    }
}


bool Validator::isdatevalid(const char* date)
{
   
    int len = 0;
    while (date[len] != '\0')
    {
        len++;
    }
    if (len != 10)
    {
        return false;
    }

 
    if (date[2] != '-')
    {
        return false;
    }
    if (date[5] != '-')
    {
        return false;
    }

 
    for (int i = 0; i < 10; i++) 
    { 
        if (i != 2 && i != 5)
        {
            if (date[i] < '0' || date[i] > '9')
            {
                return false;
            }
        }
    }

  
    int day = (date[0] - '0') * 10 + (date[1] - '0');
    int month = (date[3] - '0') * 10 + (date[4] - '0');
    int year = (date[6] - '0') * 1000 + (date[7] - '0') * 100 + (date[8] - '0') * 10 + (date[9] - '0');


   
    if (day < 1 || day   > 31)
    {
        return false;
    }
    if (month < 1 || month > 12)
    {
        return false;
    }

 
    time_t t = time(nullptr);
    tm* now = localtime(&t);
    int currentYear = now->tm_year + 1900;
    if (year < currentYear)
    {
        return false;
    }
    return true;
}


bool Validator::istimevalid(const char* slot)
{
    const char* validslots[8] = { "09:00","10:00","11:00","12:00", "13:00","14:00","15:00","16:00" };

    for (int i = 0; i < 8; i++) 
    {
        int j = 0;
        bool match = true;
        while (validslots[i][j] != '\0' || slot[j] != '\0') 
        {
            if (validslots[i][j] != slot[j]) 
            {
                match = false;
                break;
            }
            j++;
        }
        if (match)
        {
            return true;
        }
    }
    return false;
}


bool Validator::iscontactvalid(const char* contact)
{
    int len = 0;
    while (contact[len] != '\0') 
    {
       
        if (contact[len] < '0' || contact[len] > '9')
        {
            return false;
        }
        len++;
    }
    
    if (len == 11)
    {
        return true;
    }
    else
    {
        return false;
    }
}


bool Validator::ispassvalid(const char* pass) {
    int len = 0;
    while (pass[len] != '\0')
    {
        len++;
    }

    if (len >= 6)
    {
        return true;
    }
    else
    {
        return false;
    }
}


bool Validator::isfeevalid(float fee)
{
    if(fee > 0.0f)
    {
        return true;
    }
    else
    {
        return false;
    }
}


bool Validator::isidvalid(int id) 
{
    if (id > 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}


bool Validator::ismenuvalid(int choice, int min, int max)
{
    if (choice >= min && choice <= max)
    {
        return true;
    }
    else
    {
        return false;
    }
}


bool Validator::strcompignorecase(const char* a, const char* b) 
{
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0')
    {
        if (tolower(a[i]) != tolower(b[i]))
        {
            return false;
        }
        i++;
    }

    if (a[i] == b[i])
    {
        return true;
    }
    else
    {
        return false;
    }
}