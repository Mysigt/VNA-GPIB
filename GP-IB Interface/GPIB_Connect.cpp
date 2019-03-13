#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <Windows.h> //need for accessing sleep function - change for LINUX
#include <atlstr.h> //defines CString
#include "GPIB_Connect.h" 
#include "visa.h" //must download NI-VISA and add to additional library 
#define MAX_REC_SIZE 20000

using namespace std;

//Write function
bool Src::InstrWrite(CString strAddr, CString strContent )
{
	ViSession defaultRM, instr;
	ViStatus status;
	ViUInt32 retCount;
	char * SendBuf = NULL;
	char * SendAddr = NULL;
	bool bWriteOK = false; 

	SendAddr = strAddr.GetBuffer(strAddr.GetLength()); //takes in address as CString and convert to char*
	strcpy(SendAddr, strAddr);
	strAddr.ReleaseBuffer();

	SendBuf = strContent.GetBuffer(strContent.GetLength()); //takes in command data as CString and converts to char*
	strcpy(SendBuf, strContent);
	strContent.ReleaseBuffer();

	status = viOpenDefaultRM(&defaultRM); //check if VI opens
	if (status < VI_SUCCESS) //exit with print out if no instrument found
	{
		cout << "No VISA instrument opened :(";
		return false;
	}

	status = viOpen(defaultRM, SendAddr, VI_NULL, VI_NULL, &instr); //open instrument
	status = viWrite(instr, (unsigned char *)SendBuf, strlen(SendBuf), &retCount); //write command to instrument

	status = viClose(instr); //close instrument
	status = viClose(defaultRM);

	return bWriteOK;
}

//Read from instrument
bool Src::InstrRead(CString strAddr, CString *pstrResult)
{
	ViSession defaultRM, instr;
	ViStatus status;
	ViUInt32 retCount;
	char * SendAddr = NULL;
	unsigned char RecBuf[MAX_REC_SIZE];
	bool bReadOK = false;

	SendAddr = strAddr.GetBuffer(strAddr.GetLength()); //takes in address as CString and convert to char*
	strcpy(SendAddr, strAddr);
	strAddr.ReleaseBuffer();

	memset(RecBuf, 0, MAX_REC_SIZE);

	status = viOpenDefaultRM(&defaultRM); //check if VI opens
	if (status < VI_SUCCESS)
	{
		cout << "No VISA instrument opened :( \n";
		return false;
	}

	status = viOpen(defaultRM, SendAddr, VI_NULL, VI_NULL, &instr); //open instrument
	status = viRead(instr, RecBuf, MAX_REC_SIZE, &retCount); //write command to instrument

	status = viClose(instr);
	status = viClose(defaultRM);

	(*pstrResult).Format("%s", RecBuf);

	return bReadOK;
}


void Src::ConnectHP3577A()
{
	ViSession defaultRM;
	ViStatus status;
	ViString expr = (char*)"?*";
	ViPFindList findList = new unsigned long;
	ViPUInt32 retCount = new unsigned long;
	ViChar instrDesc[1000];
	CString strSrc = "";
	CString strInstr = "";
	unsigned long i = 0;
	bool bFind = false;

	status = viOpenDefaultRM(&defaultRM);

	if (status < VI_SUCCESS)
	{
		cout << "No VISA instrument opened :(";
		return;
	}

	memset(instrDesc, 0, 1000);
	status = viFindRsrc(defaultRM, expr, findList, retCount, instrDesc);

	for (i = 0; i < (*retCount); i++)
	{
		
		strSrc.Format("%s", instrDesc);
		//InstrWrite(strSrc, "*IDN?"); //command to identify VISA description of instrument
		//::Sleep(200);
		//InstrRead(strSrc,&strInstr); //get instrument name 
	
		strInstr.MakeUpper();
		if (strSrc.Find("GPIB0::11::INSTR") >= 0)  //if GPIB instrument is found series breaks out of loop 
		{
			bFind = true;
			m_strInstrAddr = strSrc; //Set instrument address for communication
		}
		status = viFindNext(*findList, instrDesc);
	}

	if (bFind == false)
	{
		cout << "No Instrument found :( \n";
	}
	else
	{
		cout << "Connection successful \n";
		cout << "\nDevices: " << m_strInstrAddr << endl;
	}
}

void Src::ConnectHP4195A()
{
	ViSession defaultRM;
	ViStatus status;
	ViString expr = (char*)"?*";
	ViPFindList findList = new unsigned long;
	ViPUInt32 retCount = new unsigned long;
	ViChar instrDesc[1000];
	CString strSrc = "";
	CString strInstr = "";
	unsigned long i = 0;
	bool bFindDP = false;

	status = viOpenDefaultRM(&defaultRM);

	if (status < VI_SUCCESS)
	{
		cout << "No VISA instrument opened :(";
		return;
	}

	memset(instrDesc, 0, 1000);
	status = viFindRsrc(defaultRM, expr, findList, retCount, instrDesc);

	int instrNum = 0; //to keep track of number of instruments for assigning addresses
	for (i = 0; i < (*retCount); i++)
	{

		strSrc.Format("%s", instrDesc);
		InstrWrite(strSrc, "FMT1; ID?"); //command to identify VISA description of instrument
		::Sleep(200);
		InstrRead(strSrc, &strInstr); //get instrument name 

		strInstr.MakeUpper();
		if (strInstr.Find("HP") >= 0)  //if instrument is found as being in DP series break out of loop 
		{
			bFindDP = true;
			m_strInstrAddr = strSrc; //Set instrument address for communication
			instrNum++;
		}
		status = viFindNext(*findList, instrDesc);
	}

	if (bFindDP == false)
	{
		cout << "HP4195A not found :( \n";
	}
	else
	{
		cout << "Connection successful \n";
		cout << "\nDevice: " << strInstr << endl;
	}
}
//Send command to connected instrument
void Src::Send(CString m_strCommand)
{
	if (m_strInstrAddr.IsEmpty())
	{
		cout << "Please connect to the instrument, no instrument address found \n";
	}
	//cout << "Sent Command: " << m_strCommand << " \n";
	InstrWrite(m_strInstrAddr,m_strCommand);
	m_strResult.Empty();
}

//Read from connected instrument
void Src::Read(CString &ReadOut)
{
	InstrRead(m_strInstrAddr, &ReadOut);
	cout << "Read: Completed \n";
}