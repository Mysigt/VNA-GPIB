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
	char cont = 'y';
	int VNA,freq1,freq2,pts;
	string measTyp;

	cout << "Select Analyzer: 1 - HP3577A, 2 - HP4195A, 3 - HP8720C \n";
	cin >> VNA;
	cout << "Define frequency range in MHz \n";
	cin >> freq1 >> freq2;
	cout << "Number of measurement points?" << endl;
	cin >> pts;
	cout << "Linear or logarithmic measurement?";
	cin >> measTyp;


	if (VNA == 3)
	{
		Instr.Connect(); //Connect to instrument	
	}
	else 
	{
		Instr.ConnectOldHP(); //Connect to instrument
	}

	while (cont != 'n')
	{
	cout << "Enter desired save file name\n";
	cin >> name;
	filename = name + ".txt";	
	file.open(filename);
	file <<" Freq range:" << freq1 << "-" << freq2 << endl;
	file << pts << endl;
	file << "Data"<<endl;

	switch (VNA)
	{
	case 1: DataHP3577A(Instr, file); break;
	case 2: DataHP4195A(Instr, file); break;
	case 3: DataHP(Instr, file); break; 
	}
	//Instr.Send("CLS");
	file.close();
	}
	cout << "Continue measuring? Enter 'n' to stop otherwise press any other key";
	cin >> cont;
}

void DataHP3577A(Src &Instr,ofstream &file)
{
	CString ReadOut;
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


