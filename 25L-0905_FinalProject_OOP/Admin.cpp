#include "Admin.h"
#include<iostream>
using namespace std;


Admin::Admin() : Person() {}

Admin::Admin(int id,const char* name, const char* pass) : Person(id,name,pass) {}

void Admin::displaymenu() {}

void Admin::displayinfo()
{
	cout << "Admin ID: " << id << " | Name: " << name << endl;
}