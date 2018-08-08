#include "student.h" 

Student::Student(string number,string name, string gerden, string grade){
	m_Number = number;
	m_Name = name;
	m_Gerden = gerden;
	m_Grade = grade;
}

Student::~Student(){
}

string Student::GetNumber(){
	return m_Number;
}

string Student::GetName(){
	return m_Name; 
}

string Student::GetGerden(){
	return m_Gerden;
}

string Student::GetGrade(){
	return m_Grade;
}
