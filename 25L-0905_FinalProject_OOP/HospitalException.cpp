#include "HospitalException.h"

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


HospitalException::HospitalException(const char* msg) 
{
	strcopy(message, msg, 200);
}

const char* HospitalException::what()
{
    return message;
}