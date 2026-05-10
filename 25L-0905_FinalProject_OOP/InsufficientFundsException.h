#pragma once
#include "HospitalException.h"

class InsufficientFundsException : public HospitalException
{
public:
	InsufficientFundsException();
};
