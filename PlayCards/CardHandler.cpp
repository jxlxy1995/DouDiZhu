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
	UINT32 dwSeed = chrono::system_clock::now().time_since_epoch().count();
	for (INT32 i = 0; i < nCount; ++i)
	{
		shuffle(m_vecAllCards.begin(), m_vecAllCards.end(), default_random_engine(dwSeed));
	}
}

void CCardHandler::BeginGame()
{
	//洗牌
	PrintCard(m_vecAllCards);
	ShuffleCard();
	PrintCard(m_vecAllCards);

	//发牌
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

void CCardHandler::OnRecvComb(INT8 cIndex, vector<CCard> vecCards)
{
	if (0 > vecCards.size() || 17 < vecCards.size())
	{
		//log
		return;
	}
	SCardComb stCardComb;
	stCardComb.vecCard = vecCards;
	stCardComb.cCount = vecCards.size();

	if (-1 == DealComb(stCardComb))
	{
		//send error
	}
	else
	{

	}
}

INT8 CCardHandler::DealComb(SCardComb& stCardComb)
{
	if (1 == stCardComb.cCount)
	{
		stCardComb.cCombType = COMB_TYPE_DAN;
		stCardComb.cGrade = COMB_GRADE_TYPE_NORMAL;
		return COMB_TYPE_DAN;
	}
	vector<CCard> vecCard = stCardComb.vecCard;
	if (2 == stCardComb.cCount)
	{
		if (29 == (vecCard[0].GetCardGrade() + vecCard[1].GetCardGrade()))
		{
			stCardComb.cCombType = COMB_TYPE_HUOJIAN;
			stCardComb.cGrade = COMB_GRADE_TYPE_HUOJIAN;
			return COMB_TYPE_HUOJIAN;
		}
		else
		{
			if (vecCard[0].GetCardPoint() == vecCard[1].GetCardPoint())
			{
				stCardComb.cCombType = COMB_TYPE_DUI;
				stCardComb.cGrade = COMB_GRADE_TYPE_NORMAL;
				stCardComb.cMaxPoint = vecCard[0].GetCardGrade();
				return COMB_TYPE_DUI;
			}
		}
	}
	if (3 == stCardComb.cCount)
	{
		if (vecCard[0].GetCardPoint() == vecCard[1].GetCardPoint() && vecCard[1].GetCardPoint() == vecCard[2].GetCardPoint())
		{
			stCardComb.cCombType = COMB_TYPE_SAN;
			stCardComb.cGrade = COMB_GRADE_TYPE_NORMAL;
			stCardComb.cMaxPoint = vecCard[0].GetCardGrade();
			return COMB_TYPE_SAN;
		}
	}
	if (4 == stCardComb.cCount)
	{
		if (vecCard[0].GetCardPoint() == vecCard[1].GetCardPoint() && vecCard[1].GetCardPoint() == vecCard[2].GetCardPoint() && vecCard[2].GetCardPoint() == vecCard[3].GetCardPoint())
		{
			stCardComb.cCombType = COMB_TYPE_ZHADAN;
			stCardComb.cGrade = COMB_GRADE_TYPE_ZHADAN;
			stCardComb.cMaxPoint = vecCard[0].GetCardGrade();
			return COMB_TYPE_ZHADAN;
		}
		else
		{
			for (size_t i = 0; i < 4; i+=3)
			{
				INT32 nSamePoint = vecCard[(i + 1) % 4].GetCardPoint();
				bool bSame = true;
				for (size_t j = 0; j < 4; ++j)
				{
					if (j == i)
					{
						continue;
					}
					if (vecCard[j].GetCardPoint() != nSamePoint)
					{
						bSame = false;
						break;
					}
				}
				if (bSame)
				{
					stCardComb.cCombType = COMB_TYPE_SANDAIYI;
					stCardComb.cGrade = COMB_GRADE_TYPE_NORMAL;
					stCardComb.cMaxPoint = vecCard[(i + 1) % 4].GetCardGrade();
					return COMB_TYPE_SANDAIYI;
				}
			}
		}
	}
	if (5 == stCardComb.cCount)
	{
		if (vecCard[0].GetCardPoint() == vecCard[1].GetCardPoint() 
			&& vecCard[2].GetCardPoint() == vecCard[3].GetCardPoint() 
			&& vecCard[3].GetCardPoint() == vecCard[4].GetCardPoint())
		{
			stCardComb.cCombType = COMB_TYPE_SANDAIER;
			stCardComb.cGrade = COMB_GRADE_TYPE_NORMAL;
			stCardComb.cMaxPoint = vecCard[2].GetCardGrade();
			return COMB_TYPE_SANDAIER;
		}
		if (vecCard[3].GetCardPoint() == vecCard[4].GetCardPoint()
			&& vecCard[0].GetCardPoint() == vecCard[1].GetCardPoint()
			&& vecCard[1].GetCardPoint() == vecCard[2].GetCardPoint())
		{
			stCardComb.cCombType = COMB_TYPE_SANDAIER;
			stCardComb.cGrade = COMB_GRADE_TYPE_NORMAL;
			stCardComb.cMaxPoint = vecCard[0].GetCardGrade();
			return COMB_TYPE_SANDAIER;
		}
	}
	if (6 == stCardComb.cCount)
	{
		if (vecCard[0].GetCardPoint() == vecCard[1].GetCardPoint()
			&& vecCard[2].GetCardPoint() == vecCard[3].GetCardPoint()
			&& vecCard[3].GetCardPoint() == vecCard[4].GetCardPoint()
			&& vecCard[4].GetCardPoint() == vecCard[5].GetCardPoint())
		{
			stCardComb.cCombType = COMB_TYPE_SIDAIER;
			stCardComb.cGrade = COMB_GRADE_TYPE_NORMAL;
			stCardComb.cMaxPoint = vecCard[2].GetCardGrade();
			return COMB_TYPE_SIDAIER;
		}
		if (vecCard[4].GetCardPoint() == vecCard[5].GetCardPoint()
			&& vecCard[0].GetCardPoint() == vecCard[1].GetCardPoint()
			&& vecCard[1].GetCardPoint() == vecCard[2].GetCardPoint()
			&& vecCard[2].GetCardPoint() == vecCard[3].GetCardPoint())
		{
			stCardComb.cCombType = COMB_TYPE_SIDAIER;
			stCardComb.cGrade = COMB_GRADE_TYPE_NORMAL;
			stCardComb.cMaxPoint = vecCard[0].GetCardGrade();
			return COMB_TYPE_SIDAIER;
		}
	}
	if (stCardComb.cCount >= 5)
	{
		if (vecCard[0].GetCardGrade() > 11)
		{
			return COMB_TYPE_ERROR;
		}
		if (vecCard[0].GetCardGrade() != vecCard[1].GetCardGrade())
		{
			bool bShun = true;
			for (size_t i = 1; i < stCardComb.cCount; ++i)
			{
				if ((vecCard[i].GetCardGrade() + 1) != vecCard[i - 1].GetCardGrade())
				{
					bShun = false;
					break;
				}
			}
			if (bShun)
			{
				stCardComb.cCombType = COMB_TYPE_DANSHUN;
				stCardComb.cGrade = COMB_GRADE_TYPE_NORMAL;
				stCardComb.cMaxPoint = vecCard[0].GetCardGrade();
				return COMB_TYPE_DANSHUN;
			}
		}
		if (vecCard[stCardComb.cCount].GetCardGrade() == vecCard[stCardComb.cCount - 1].GetCardGrade() && stCardComb.cCount >= 6)
		{
			bool bShun = true;
			for (size_t i = 0; i < stCardComb.cCount - 2; i+=2)
			{
				if (vecCard[i].GetCardGrade() != vecCard[i + 1].GetCardGrade())
				{
					bShun = false;
					break;
				}
				if ((vecCard[i].GetCardGrade() - 1) != vecCard[i + 2].GetCardGrade())
				{
					bShun = false;
					break;
				}
				if (bShun)
				{
					stCardComb.cCombType = COMB_TYPE_SHUANGSHUN;
					stCardComb.cGrade = COMB_GRADE_TYPE_NORMAL;
					stCardComb.cMaxPoint = vecCard[0].GetCardGrade();
					return COMB_TYPE_SHUANGSHUN;
				}
			}
		}
		if (vecCard[stCardComb.cCount].GetCardGrade() == vecCard[stCardComb.cCount - 1].GetCardGrade() && vecCard[stCardComb.cCount - 1].GetCardGrade() == vecCard[stCardComb.cCount - 2].GetCardGrade() && stCardComb.cCount >= 6)
		{
			bool bShun = true;
			for (size_t i = 0; i < stCardComb.cCount - 3; i += 3)
			{
				if (vecCard[i].GetCardGrade() != vecCard[i + 1].GetCardGrade() 
					|| vecCard[i].GetCardGrade() != vecCard[i + 2].GetCardGrade()
					|| vecCard[i + 1].GetCardGrade() != vecCard[i + 2].GetCardGrade())
				{
					bShun = false;
					break;
				}
				if ((vecCard[i].GetCardGrade() - 1) != vecCard[i + 3].GetCardGrade())
				{
					bShun = false;
					break;
				}
			}
			if (bShun)
			{
				stCardComb.cCombType = COMB_TYPE_SANSHUN;
				stCardComb.cGrade = COMB_GRADE_TYPE_NORMAL;
				stCardComb.cMaxPoint = vecCard[0].GetCardGrade();
				return COMB_TYPE_SANSHUN;
			}
		}
	}
	//feiji
	if (stCardComb.cCount >= 8)
	{
		INT32 szSame[12] = { 0 };
		for (size_t i = 0; i < stCardComb.cCount; ++i)
		{
			++szSame[vecCard[i].GetCardGrade()];
		}
		INT32 nSameCount = 0;
		bool bLastSame = false;
		for (INT32 i = 1; i < 12; ++i)
		{
			if (szSame[i] >= 3)
			{
				if (bLastSame)
				{
					++nSameCount;
				}
				else
				{
					bLastSame = true;
					nSameCount = 1;
				}
			}
		}
		if (nSameCount >= 2)
		{
			if (stCardComb.cCount != 8 && stCardComb.cCount != 10
				&& stCardComb.cCount != 12 && stCardComb.cCount != 15
				&& stCardComb.cCount != 16)
			{
				return COMB_TYPE_ERROR;
			}
			if (10 == stCardComb.cCount)
			{
				INT32 cCount = 0;
				for (INT32 i = 1; i < 12; ++i)
				{
					if (szSame[i] == 2)
					{
						++cCount;
					}
				}
				if (2 != cCount)
				{
					return COMB_TYPE_ERROR;
				}
			}
			if (15 == stCardComb.cCount)
			{
				INT32 cCount = 0;
				for (INT32 i = 1; i < 12; ++i)
				{
					if (szSame[i] == 2)
					{
						++cCount;
					}
				}
				if (3 != cCount)
				{
					return COMB_TYPE_ERROR;
				}
			}
			stCardComb.cCombType = COMB_TYPE_FEIJI;
			stCardComb.cGrade = COMB_GRADE_TYPE_NORMAL;
			for (INT32 i = 11; i >= 1; --i)
			{
				if (szSame[i] >= 3)
				{
					stCardComb.cMaxPoint = i;
				}
			}
			return COMB_TYPE_FEIJI;
		}
	}
	return COMB_TYPE_ERROR;
}

void CCardPlayer::AutoPlay(SCardComb& m_stLastComb)
{
	INT32 szSame[12] = { 0 };				//0代表大小王
	INT32 nCardCount = m_listCard.size();
	for (list<CCard>::iterator it = m_listCard.begin(); it != m_listCard.end(); ++it)
	{
		if (it->GetCardType() != CARD_TYPE_JOKER)
		{
			++szSame[it->GetCardGrade()];
			if (4 == szSame[it->GetCardGrade()])
			{
				++m_nZhadanCount;
			}
		}
		else
		{
			++szSame[0];
			if (2 == szSame[0])
			{
				++m_nHuojianCount;
			}
		}
	}
	vector<CCard> vecOutCard;

	if (COMB_TYPE_ERROR == m_stLastComb.cCombType)
	{
		//出最小的
		INT32 szSameCount[4] = { 0 };
		INT32 szTempCount[4] = { 0 };
		bool szLast[4] = { false, false, false, false };
		INT32 szBegin[4] = { 0 };
		INT32 szEnd[4] = { 0 };
		INT32 szTempBegin[4] = { 0 };
		INT32 szTempEnd[4] = { 0 };
		for (INT32 i = 0; i < 12; ++i)
		{
			for (INT32 j = 1; j < 4; ++j)
			{
				if (szSame[i] >= j)
				{
					if (szLast[j])
					{
						++szTempCount[j];
						szTempEnd[j] = i;
						if (szTempCount[j] > szSameCount[j])
						{
							szSameCount[j] = szTempCount[j];
							szBegin[j] = szTempBegin[j];
							szEnd[j] = szTempEnd[j];
						}
					}
					else
					{
						szTempCount[j] = 1;
						szTempBegin[j] = i;
						szTempEnd[j] = 0;
					}
				}
				else
				{
					szLast[j] = false;
				}
			}
		}
		// 判断是否有单顺 双顺 三顺
		if (szSameCount[1] < 5)
		{
			szSameCount[1] = 0;
		}
		if (szSameCount[2] < 3)
		{
			szSameCount[2] = 0;
		}
		if (szSameCount[3] < 2)
		{
			szSameCount[3] = 0;
		}
		INT32 nMaxCount = 0;
		INT32 nMaxIndex = 0;
		for (INT32 i = 1; i < 4; ++i)
		{
			szTempCount[i] = szSameCount[i] * i;
			if (szTempCount[i] > nMaxCount)
			{
				nMaxCount = szTempCount[i];
				nMaxIndex = i;
			}
		}
		if (0 != nMaxIndex)
		{
			//出这组最大数量的顺子

		}

	}
	else
	{
		
	}
}
