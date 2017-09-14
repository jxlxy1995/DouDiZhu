#ifndef __Card_h__
#define __Card_h__

#include <string>
#include "CardType.h"

class CCard
{
public:
	CCard(INT32 nID);
	~CCard(){}

	INT32 GetCardType(){ return m_nType; }
	INT32 GetCardPoint(){ return m_nPoint; }
	INT32 GetCardGrade(){ return m_nGrade; }
	void InitCardGrade();
	string& GetCardName(){ return m_strName; }
	void InitCardName();

private:
	INT32 m_nID;
	INT32 m_nType;
	INT32 m_nPoint;
	INT32 m_nGrade;
	string m_strName;
};

class Cmpare
{
public:
	bool operator()(CCard card1, CCard card2) const;
};

#endif
