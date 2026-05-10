#include "Person.h"

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

Person::Person()
{
	id = 0;
	name[0] = '\0';
	pass[0] = '\0';
}

Person::Person(int id, const char* name, const char* pass)
{
	this->id = id;
	strcopy(this->name, name, 20);
	strcopy(this->pass, pass, 20);
}

int Person::getid()
{
	return id;
}

const char* Person::getname()
{
	return name;
}

const char* Person::getpass()
{
	return pass;
}

void Person::setname(const char* n)
{
	strcopy(name, n, 20);
}

