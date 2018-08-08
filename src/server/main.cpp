#include <iostream>

#include "student.h"

int main(){
	Student stu("001","xxx","male","88");
	
	cout << "Number : " << stu.GetNumber() << endl;
	cout << "Name   : " << stu.GetName() << endl;
	cout << "Gerden : " << stu.GetGerden() << endl;
	cout << "Grade  : " << stu.GetGrade() << endl; 
	return 0;
}
