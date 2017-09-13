#include "Card.h"
#include "CardHandler.h"

CCard::CCard(INT32 nID)
{
	m_nID = nID;
	m_nType = nID / 13;
	m_nPoint = nID % 13 + 1;
	InitCardGrade();
	m_strName = "";
	InitCardName();
}

void CCard::InitCardGrade()
{
	//´óÐ¡Íõ
	if (CARD_TYPE_JOKER == m_nType)
	{
		if (1 == m_nPoint)
		{
			m_nGrade = 14;
		}
		else
		{
			m_nGrade = 15;
		}
		return;
	}
	//3~K
	if (m_nPoint >= 3)
	{
		m_nGrade = m_nPoint - 2;
	}
	//A~2
	else
	{
		m_nGrade = m_nPoint + 11;
	}
}

void CCard::InitCardName()
{
	switch (m_nType)
	{
	case CARD_TYPE_SPADE:
	{
		m_strName += "S-";
		break;
	}
	case CARD_TYPE_HEART:
	{
		m_strName += "H-";
		break;
	}
	case CARD_TYPE_CLUB:
	{
		m_strName += "C-";
		break;
	}
	case CARD_TYPE_DIAMOND:
	{
		m_strName += "D-";
		break;
	}
	case CARD_TYPE_JOKER:
	{
		m_strName += "JOKER-";
		break;
	}
	default:
	{
		break;
	}
	}
	vector<string>& vecPointName = CCardHandler::Instance()->GetPointName();
	m_strName += vecPointName[m_nPoint];
}

bool Cmpare::operator()(CCard card1,CCard card2) const
{
	return card1.GetCardGrade() > card2.GetCardGrade();
}
