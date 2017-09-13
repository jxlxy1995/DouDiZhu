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
	CLogHandler::Instance()->CloseFile();
}