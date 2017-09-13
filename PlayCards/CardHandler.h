#ifndef __CardHandler_h__
#define __CardHandler_h__

#include "Card.h"
#include <vector>
#include <list>

class CCardPlayer
{
public:
	CCardPlayer(INT8 cIndex) :m_cIndex(cIndex), m_nScore(0){}

	list<CCard>& GetCardList(){ return m_listCard; }
	INT8 GetIndex(){ return m_cIndex; }

private:
	list<CCard> m_listCard;
	INT8 m_cIndex;
	INT8 m_cDizhuIndex;
	INT32 m_nScore;
};

class CCardHandler
{
public:
	static CCardHandler* Instance()
	{
		static CCardHandler m_obInstance;
		return &m_obInstance;
	}

	void Init();
	void BeginGame();
	void ShuffleCard(INT32 nCount = 100);

	vector<string>& GetPointName(){ return m_vecPointName; }
	void PrintCard(vector<CCard>& vecCards);
	void PrintCard(list<CCard>& listCards);

private:
	CCardHandler(){}

private:
	vector<CCard> m_vecAllCards;
	vector<CCardPlayer> m_vecPlayer;

	vector<string> m_vecPointName;
	INT8 m_cDizhuIndex;
};

#endif
