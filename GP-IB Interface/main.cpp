#include <stdio.h>
#include <iostream>
#include <fstream>
#include "GPIB_Connect.h"


using namespace std;

void DataHP4195A(Src &Instr, ofstream &file);
void DataHP3577A(Src &Instr, ofstream &file);
void writeFile(ofstream &file, string str);

int main()
{
	Src Instr; 
	string filename; //text file name
	string volt; //value used for text file name
	ofstream file;
	cout << "Define voltage of measurement\n";
	cin >> volt;
	filename = volt + "V.txt";	
	file.open(filename);

	DataHP4195A(Instr, file);
	//DataHP3577A(Instr,file); 

	Instr.Send("CLS");
	file.close();
	getchar(); //just prevents from autoclosing console
}


void DataHP3577A(Src &Instr,ofstream &file)
{
	CString ReadOut;
	Instr.ConnectHP3577A(); //Connect to instrument
	Instr.Send("FM1"); //Set data format to ASCII - Done here instead of in connect as source info not visible with instrument
	Instr.Send("DT1"); //Dump trace 1 data containing magnitude info
	Instr.Read(ReadOut); //Read data from instrument
	string mag(ReadOut);
	writeFile(file, mag);
	Instr.Send("X?");
	Instr.Read(ReadOut);
	string freq(ReadOut);
	writeFile(file, freq);
}
void DataHP4195A(Src &Instr, ofstream &file)
{
	CString ReadOut;
	Instr.ConnectHP4195A(); //Connect to instrument
	Instr.Send("A?");
	Instr.Read(ReadOut);
	string mag(ReadOut);
	writeFile(file,mag);
	Instr.Send("X?");
	Instr.Read(ReadOut);
	string freq(ReadOut);
	writeFile(file,freq);
}
void writeFile(ofstream &file,string str)
{
	for (int i = 0; i < str.length() - 2; i++)
	{
		if (char(str[i]) != ',')
		{
			file << str[i];
		}
		else
		{
			file << endl;
		}
	}
	file << endl << endl;
}


