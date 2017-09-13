#include "CardHandler.h"
#include <random>
#include <chrono>
#include "LogHandler.h"

void CCardHandler::Init()
{
	m_vecPointName.resize(14);
	m_vecPointName[1] = "A";
	m_vecPointName[2] = "2";
	m_vecPointName[3] = "3";
	m_vecPointName[4] = "4";
	m_vecPointName[5] = "5";
	m_vecPointName[6] = "6";
	m_vecPointName[7] = "7";
	m_vecPointName[8] = "8";
	m_vecPointName[9] = "9";
	m_vecPointName[10] = "10";
	m_vecPointName[11] = "J";
	m_vecPointName[12] = "Q";
	m_vecPointName[13] = "K";
	for (INT32 i = 0; i < 54; ++i)
	{
		CCard* pCard = new CCard(i);
		m_vecAllCards.push_back(*pCard);
	}
	for (INT32 i = 0; i < 3; ++i)
	{
		m_vecPlayer.push_back(*(new CCardPlayer(i)));
	}
}

void CCardHandler::ShuffleCard(INT32 nCount /*= 100*/)
{
	UINT32 dwSeed = std::chrono::system_clock::now().time_since_epoch().count();
	for (INT32 i = 0; i < nCount; ++i)
	{
		shuffle(m_vecAllCards.begin(), m_vecAllCards.end(), default_random_engine(dwSeed));
	}
}

void CCardHandler::BeginGame()
{
	//Ï´ÅÆ
	PrintCard(m_vecAllCards);
	ShuffleCard();
	PrintCard(m_vecAllCards);

	//·¢ÅÆ
	list<CCard>& listPlayer0 = m_vecPlayer[0].GetCardList();
	list<CCard>& listPlayer1 = m_vecPlayer[1].GetCardList();
	list<CCard>& listPlayer2 = m_vecPlayer[2].GetCardList();

	for (size_t i = 0; i <= 48;)
	{
		listPlayer0.push_back(m_vecAllCards[i]);
		listPlayer1.push_back(m_vecAllCards[i + 1]);
		listPlayer2.push_back(m_vecAllCards[i + 2]);
		i += 3;
	}
	listPlayer0.sort(Cmpare());
	listPlayer1.sort(Cmpare());
	listPlayer2.sort(Cmpare());
	PrintCard(m_vecPlayer[0].GetCardList());
	PrintCard(m_vecPlayer[1].GetCardList());
	PrintCard(m_vecPlayer[2].GetCardList());
}

void CCardHandler::PrintCard(vector<CCard>& vecCards)
{
	string strTemp = "";
	for (size_t i = 0; i < vecCards.size(); ++i)
	{
		strTemp += vecCards[i].GetCardName();
		strTemp += " ";
	}
	CLogHandler::Instance()->WriteLine(strTemp.c_str());
}

void CCardHandler::PrintCard(list<CCard>& listCards)
{
	string strTemp = "";
	for (list<CCard>::iterator it = listCards.begin(); it != listCards.end(); ++it)
	{
		strTemp += it->GetCardName();
		strTemp += " ";
	}
	CLogHandler::Instance()->WriteLine(strTemp.c_str());
}
