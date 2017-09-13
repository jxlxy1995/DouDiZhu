#include "LogHandler.h"
#include <windows.h>
#include <WinBase.h>
#include <ctime>

void CLogHandler::Init()
{
	
}

void CLogHandler::CreateNewFile()
{
	string strFileName = "GameLog";
	INT64 llTime = GetTickCount();
	char cTime[64];
	sprintf_s(cTime, "%lld.txt", llTime);
	strFileName += cTime;
	m_obOutfile.open(strFileName.c_str());
	m_bFileReady = true;
}

void CLogHandler::WriteLine(const char* pLine)
{
	if (!m_bFileReady)
	{
		return;
	}
	m_obOutfile <<  pLine << endl;
}

void CLogHandler::CloseFile()
{
	m_obOutfile.close();
	m_bFileReady = false;
}
