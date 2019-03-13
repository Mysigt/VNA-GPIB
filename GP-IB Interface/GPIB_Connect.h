#pragma once
#ifndef GPIB_CONNECT_H
#define GPIB_CONNECT_H
#include <atlstr.h>
#include <string>

class Src
{
public:
	CString m_strInstrAddr; //instrument address
	CString m_strResult; //Return value

	void ConnectHP3577A(); //initialize connection with HP3577A
	void ConnectHP4195A(); //initialize connection with HP4195A
	void Send(CString m_strCommand); //send command to instrument
	void Read(CString &ReadOut); //read from instrument

private:
	bool InstrWrite(CString strAddr, CString strContent); //used to implement connect/send/read
	bool InstrRead(CString strAddr, CString *pstrResult);
};
#endif