#include <stdio.h>
#include <iostream>
#include <fstream>
#include "GPIB_Connect.h"


using namespace std;

void DataHP4195A(Src &Instr, ofstream &file);
void DataHP3577A(Src &Instr, ofstream &file);
void DataHP(Src &Instr, ofstream &file);
void writeFile(ofstream &file, string str);

int main()
{
	Src Instr; 
	string filename; //text file name
	string name; //value used for text file name
	ofstream file;
	cout << "Enter desired save file name\n";
	cin >> name;
	filename = name + ".txt";	
	file.open(filename);

	//DataHP4195A(Instr, file);
	DataHP3577A(Instr,file); 
	//DataHP(Instr, file);

	//Instr.Send("CLS");
	file.close();
	//getchar(); //just prevents from autoclosing console
}

void DataHP3577A(Src &Instr,ofstream &file)
{
	CString ReadOut;
	Instr.ConnectOldHP(); //Connect to instrument
	Instr.Send("FM1"); //Set data format to ASCII - Done here instead of in connect as source info not visible with instrument
	Instr.Send("TR1; DF7; DT1"); //Dump trace 1 data containing magnitude info
	Instr.Read(ReadOut); //Read data from instrument
	string mag(ReadOut);

	Instr.Send("TR2; DF7; DT2"); //Dump trace 1 data containing magnitude info
	Instr.Read(ReadOut); //Read data from instrument
	string mag2(ReadOut);

	writeFile(file, mag);
	writeFile(file, mag2);

	Instr.Send("TR1; DF5; DT1"); //Dump trace 1 data containing magnitude info
	Instr.Read(ReadOut); //Read data from instrument
	string phase(ReadOut);

	Instr.Send("TR2; DF5; DT2"); //Dump trace 1 data containing magnitude info
	Instr.Read(ReadOut); //Read data from instrument
	string phase2(ReadOut);

	writeFile(file, phase);
	writeFile(file, phase2);
	//Instr.Send("DF7"); //Dump trace 1 data containing magnitude info
}
void DataHP4195A(Src &Instr, ofstream &file)
{
	CString ReadOut;
	Instr.ConnectOldHP(); //Connect to instrument
	Instr.Send("A?");
	Instr.Read(ReadOut);
	string mag(ReadOut);
	writeFile(file,mag);
	Instr.Send("B?");
	Instr.Read(ReadOut);
	string phase(ReadOut);
	writeFile(file, phase);
	Instr.Send("X?");
	Instr.Read(ReadOut);
	string freq(ReadOut);
	writeFile(file,freq);
}
void DataHP(Src &Instr, ofstream &file)
{
	CString ReadOut;
	Instr.Connect(); //Connect to instrument
	//Instr.Send("FORM4");
	Instr.Send("OUTPDATA");
	Instr.Read(ReadOut);
	string mag(ReadOut);
	writeFile(file, mag);

	//Instr.Send("X?");
	//Instr.Read(ReadOut);
	//string freq(ReadOut);
	//writeFile(file, freq);
}

//void writeFile(ofstream &file, string str)
//{	
//	for (int i = 0; i < str.length(); i++)
//	{	
//		if (char(str[i]) != ',') 
//		{
//			file << str[i];
//		}
//		else
//		{
//			file << endl;
//		}
//	}
//	file << endl << endl;
//}

void writeFile(ofstream &file,string str)
{	
	for (int i = 0; i < str.length(); i++)
	{
		if (char(str[i]) != ',')
		{
			file << str[i];
		}
	}
	file << endl;
}


