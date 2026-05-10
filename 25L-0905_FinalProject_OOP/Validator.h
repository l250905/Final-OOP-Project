#pragma once

class Validator {
public:
    static bool isdatevalid(const char* date);

    static bool istimevalid(const char* slot);

    static bool iscontactvalid(const char* contact);

    static bool ispassvalid(const char* password);

    static bool isfeevalid(float fee);

    static bool isidvalid(int id);

    static bool ismenuvalid(int choice, int min, int max);

    static bool strcompignorecase(const char* a, const char* b);

};