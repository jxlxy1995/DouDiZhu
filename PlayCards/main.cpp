#include <iostream>
#include "Card.h"
#include "CardHandler.h"
#include "LogHandler.h"

void main()
{
	CLogHandler::Instance()->Init();
	CLogHandler::Instance()->CreateNewFile();
	CCardHandler::Instance()->Init();
	CCardHandler::Instance()->BeginGame();

	bool bIsEnd = false;
	SCardComb stLastComb;
	stLastComb.cCombType = COMB_TYPE_ERROR;
	CLogHandler::Instance()->WriteLine("-------------------------------------------");
	for (INT32 nTrun = 1; /*nTrun < 2*/; ++nTrun)
	{
		if (bIsEnd)
		{
			break;
		}
		char cTrunName[32];
		sprintf_s(cTrunName, "Trun %d:", nTrun);
		CLogHandler::Instance()->WriteLine(cTrunName);
		INT8 cLoop = 0;
		for (INT32 i = CCardHandler::Instance()->GetLastIndex(); i < 3;)
		{
			if (bIsEnd)
			{
				break;
			}
			if (CCardHandler::Instance()->GetLastIndex() == i && 2 == cLoop)
			{
				stLastComb.cCombType = COMB_TYPE_ERROR;
				break;
			}
			CCardPlayer* pPlayer = CCardHandler::Instance()->GetPlayer(i);
			if (NULL != pPlayer)
			{
				if (!pPlayer->AutoPlay(stLastComb))
				{
					++cLoop;
				}
				else
				{
					cLoop = 0;
				}
				stLastComb = CCardHandler::Instance()->GetLastComb();
				bIsEnd = pPlayer->IsWin();
				i = (i + 1) % 3;
			}
		}
	}

	CLogHandler::Instance()->CloseFile();

// 	CLogHandler::Instance()->CreateNewFile();
// 	CLogHandler::Instance()->WriteLine("-------------------------------------------");
// 	for (INT32 i = 0; i < 3; ++i)
// 	{
// 		CCardPlayer* pPlayer = CCardHandler::Instance()->GetPlayer(i);
// 		if (NULL != pPlayer)
// 		{
// 			pPlayer->AutoPlay(stTest);
// 		}
// 	}
// 	CLogHandler::Instance()->CloseFile();
}