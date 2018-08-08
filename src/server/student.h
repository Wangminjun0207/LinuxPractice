#include <string>
using namespace std; 

class Student{
	string m_Number;
	string m_Name;
	string m_Gerden;
	string m_Grade;
	public:
	Student(string number,string name, string gerden, string grade);
	~Student();
	string GetName();
	string GetNumber();
	string GetGerden();
	string GetGrade();
};


