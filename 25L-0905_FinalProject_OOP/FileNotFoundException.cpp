#include "FileNotFoundException.h"
#include<iostream>
using namespace std;

FileNotFoundException::FileNotFoundException(const char* filename) : HospitalException("")
{
	const char* tempmsg = " file is not found!";

	int index = 0;
	while (filename[index] != '\0' && index < 199)
	{
		message[index] = filename[index];
		index++;
	}

	int i = 0;
	while (tempmsg[i] != '\0' && index < 199)
	{
		message[index] = tempmsg[i];
		index++;
		i++;
	}

	message[index] = '\0';

}

