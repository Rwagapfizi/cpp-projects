#include <iostream>
#include <string>
#include <cstring> 
#include <mysql.h>
#include <mysqld_error.h>

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

using namespace std;

char HOST[] = "localhost";
char USER[] = "root";
char PASS[] = "";

int main(int argc, char** argv) {
	
	MYSQL* obj;
	
	int college_ID;
	char Name[20];
	char Birthdate[11];
	float Grade;
	
	bool ProgramIsOpened = true;
	int answer;
	
	char* consult;
	char* sentence;
	string query;
	
	if(!(obj = mysql_init(0))){
		cout << "ERROR: MySql object couldn't be created" << endl;
	} else{
		if(!mysql_real_connect(obj, HOST, USER, PASS, "cpp", 3306, NULL, 0)){
			cout << "ERROR: Some database info is wrong or do not exist" << endl;
			cout << mysql_error(obj) << endl;
		} else {
			cout << "Logged in.\n\n";
			while(ProgramIsOpened){
				cout << "College ID: ";
				cin >> college_ID;
				cin.ignore(100, '\n');
				
				cout << "Name: ";
				cin.getline(Name, 20, '\n');
				
				cout << "Birth Date: ";
				cin.getline(Birthdate, 11, '\n');
				
				cout << "Grade: ";
				cin >> Grade;
				cin.ignore(100, '\n');
				cout <<endl;
				
				query = "INSERT INTO student(College_ID, Name, BirthDate, Grade) VALUES ('%d', '%s', '%s', '%f')";
				sentence = new char[query.length() + 1];
				strcpy(sentence, query.c_str());
				
				consult = new char[strlen(sentence) + sizeof(int) + strlen(Name) + strlen(Birthdate) + sizeof(float)];
				sprintf(consult, sentence, college_ID, Name, Birthdate, Grade);
				
				if(mysql_ping(obj)){
					cout << "ERROR: Impossible to connect.\n";
					cout << mysql_error(obj) << endl;
				} 
				
				if(mysql_query(obj, consult)){
					cout << "ERROR: " << mysql_error(obj) << endl;
					rewind(stdin);
					getchar();
				} else {
					cout << "Info added successfully.\n";
				}
				
				mysql_store_result(obj);
				
				cout << "\nAnother?\n";
				cout << "[1]: Yes\n";
				cout << "[0]: No\n";
				cout << "Answer: ";
				cin >> answer;
				cin.ignore(100, '\n');
				if(answer == 0){
					ProgramIsOpened = false;
				}
				cout << endl;
			}
		}
	}
	
	cout << "Ngaho va kuri code zanjye pumbafu. \n";
	
	return 0;
}
