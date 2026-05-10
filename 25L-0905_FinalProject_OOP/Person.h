#pragma once

class Person
{
protected:
	int id;
	char name[20];
	char pass[20];
public:
	Person();
	Person(int id, const char* name, const char* pass);

	int getid();
	const char* getname();
	const char* getpass();

	void setname(const char* n);

	virtual void displaymenu() = 0;
	virtual void displayinfo() = 0;

	virtual ~Person() {};

};