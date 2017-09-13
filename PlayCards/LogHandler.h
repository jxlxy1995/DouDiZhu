#ifndef __LogHandler_h__
#define __LogHandler_h__

#include <iostream>  
#include <fstream> 
#include <string>
#include <basetsd.h>
using namespace std;

class CLogHandler
{
public:
	static CLogHandler* Instance()
	{
		static CLogHandler m_obInstance;
		return &m_obInstance;
	}

	void Init();
	void CreateNewFile();
	void WriteLine(const char* pLine);
	void CloseFile();

private:
	CLogHandler() :m_bFileReady(false){}

private:
	ofstream m_obOutfile;
	bool m_bFileReady;
};

#endif