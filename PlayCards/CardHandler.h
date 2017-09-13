#ifndef __CardHandler_h__
#define __CardHandler_h__

#include "Card.h"
#include <vector>
#include <list>

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
	list<CCard> m_listPlayer[3];
	INT8 m_cDizhuIndex;

	vector<string> m_vecPointName;
};

#endif
