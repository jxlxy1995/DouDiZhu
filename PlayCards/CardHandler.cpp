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
	m_cLastIndex = 0;
}

void CCardHandler::ShuffleCard(INT32 nCount /*= 100*/)
{
	//UINT32 dwSeed = chrono::system_clock::now().time_since_epoch().count();
	for (INT32 i = 0; i < nCount; ++i)
	{
		random_shuffle(m_vecAllCards.begin(), m_vecAllCards.end());
	}
}

void CCardHandler::BeginGame()
{
	//洗牌
	PrintCard(m_vecAllCards);
	ShuffleCard();
	PrintCard(m_vecAllCards);
	CLogHandler::Instance()->WriteLine("-------------------------------------------");
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
	if (0 >= vecCards.size() || 17 < vecCards.size())
	{
		//log
		return;
	}
	SCardComb stCardComb;
	stCardComb.vecCard = vecCards;
	sort(stCardComb.vecCard.begin(), stCardComb.vecCard.end(), Cmpare());
	
	PrintCard(stCardComb.vecCard);
	

	stCardComb.cCount = vecCards.size();
	INT8 cRet = DealComb(stCardComb);
	if (-1 == cRet)
	{
		//send error
		CLogHandler::Instance()->WriteLine("-1");
	}
	else
	{
		char szOutput[64];
		sprintf_s(szOutput, "Coombo:%d", cRet);
		CLogHandler::Instance()->WriteLine(szOutput);

		m_cLastIndex = cIndex;
		m_stLastComb = stCardComb;
		m_vecPlayer[cIndex].ClearOutCard(stCardComb.vecCard);

	}
	CLogHandler::Instance()->WriteLine("-------------------------------------------");
}

INT8 CCardHandler::DealComb(SCardComb& stCardComb)
{
	if (1 == stCardComb.cCount)
	{
		stCardComb.cCombType = COMB_TYPE_DAN;
		stCardComb.cMaxPoint = stCardComb.vecCard[0].GetCardGrade();
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
		if (vecCard[0].GetCardGrade() > 12)
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
		if (vecCard[stCardComb.cCount - 1].GetCardGrade() == vecCard[stCardComb.cCount - 2].GetCardGrade() && stCardComb.cCount >= 6)
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
		if (vecCard[stCardComb.cCount - 1].GetCardGrade() == vecCard[stCardComb.cCount - 2].GetCardGrade() && vecCard[stCardComb.cCount - 2].GetCardGrade() == vecCard[stCardComb.cCount - 3].GetCardGrade() && stCardComb.cCount >= 6)
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

CCardPlayer* CCardHandler::GetPlayer(INT32 nIndex)
{
	if (nIndex < m_vecPlayer.size() && nIndex >= 0)
	{
		return &(m_vecPlayer[nIndex]);
	}
	else
	{
		return NULL;
	}
}

bool CCardPlayer::AutoPlay(SCardComb& m_stLastComb)
{
	INT32 szSame[16] = { 0 };				//0代表大小王
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
			++szSame[it->GetCardGrade()];
		}
	}

	//i顺的长度szSameCount[i]
	INT32 szSameCount[4] = { 0 };
	INT32 szTempCount[4] = { 0 };
	bool szLast[4] = { false, false, false, false };
	INT32 szBegin[4] = { 0 };
	INT32 szEnd[4] = { 0 };
	INT32 szTempBegin[4] = { 0 };
	INT32 szTempEnd[4] = { 0 };
	for (INT32 i = 1; i <= 12; ++i)
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
					szLast[j] = true;
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

	vector<CCard> vecOutCard;

	if (COMB_TYPE_ERROR == m_stLastComb.cCombType)
	{
		//出最小的
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
			for (INT32 i = szBegin[nMaxIndex]; i <= szEnd[nMaxIndex]; ++i)
			{
// 				INT32 nOutCount = 0;
// 				for (list<CCard>::iterator it = m_listCard.begin(); it != m_listCard.end(); ++it)
// 				{
// 					if (nMaxIndex == nOutCount)
// 					{
// 						break;
// 					}
// 					if (it->GetCardGrade() == i)
// 					{
// 						vecOutCard.push_back(*it);
// 						++nOutCount;
// 					}
// 				}
				ChooseCard(nMaxIndex, i, vecOutCard);
			}
			CCardHandler::Instance()->OnRecvComb(m_cIndex, vecOutCard);
			return true;
		}
		//3dai
		for (INT32 i = 1; i <= 11; ++i)
		{
			if (3 == szSame[i])
			{
				for (INT32 j = 1; j <= 11; ++j)
				{
					//dai k
					for (INT32 k = 2; k >= 0; --k)
					{
						if (j == i)
						{
							continue;
						}
						if (k == szSame[j])
						{
// 							INT32 nOutCounti = 0;
// 							INT32 nOutCountj = 0;
// 							for (list<CCard>::iterator it = m_listCard.begin(); it != m_listCard.end(); ++it)
// 							{
// 								if (3 == nOutCounti && k == nOutCountj)
// 								{
// 									break;
// 								}
// 								if (it->GetCardGrade() == i)
// 								{
// 									vecOutCard.push_back(*it);
// 									++nOutCounti;
// 								}
// 								if (it->GetCardGrade() == j)
// 								{
// 									vecOutCard.push_back(*it);
// 									++nOutCountj;
// 								}
// 							}
							ChooseCard(3, i, vecOutCard);
							ChooseCard(k, j, vecOutCard);
							CCardHandler::Instance()->OnRecvComb(m_cIndex, vecOutCard);
							return true;
						}
					}
				}
			}
		}

		//dui
		for (INT32 i = 1; i <= 13; ++i)
		{
			if (2 == szSame[i])
			{
// 				INT32 nOutCount = 0;
// 				for (list<CCard>::iterator it = m_listCard.begin(); it != m_listCard.end(); ++it)
// 				{
// 					if (2 == nOutCount)
// 					{
// 						break;
// 					}
// 					if (it->GetCardGrade() == i)
// 					{
// 						vecOutCard.push_back(*it);
// 						++nOutCount;
// 					}
// 				}
				ChooseCard(2, i, vecOutCard);
				CCardHandler::Instance()->OnRecvComb(m_cIndex, vecOutCard);
				return true;
			}
		}

		//dan
		for (INT32 i = 1; i <= 15; ++i)
		{
			if (1 == szSame[i])
			{
// 				INT32 nOutCount = 0;
// 				for (list<CCard>::iterator it = m_listCard.begin(); it != m_listCard.end(); ++it)
// 				{
// 					if (1 == nOutCount)
// 					{
// 						break;
// 					}
// 					if (it->GetCardGrade() == i)
// 					{
// 						vecOutCard.push_back(*it);
// 						++nOutCount;
// 					}
// 				}
				ChooseCard(1, i, vecOutCard);
				CCardHandler::Instance()->OnRecvComb(m_cIndex, vecOutCard);
				return true;
			}
		}
	}
	else
	{
		switch (m_stLastComb.cCombType)
		{
		case COMB_TYPE_HUOJIAN:
		{
			CCardHandler::Instance()->OnRecvComb(m_cIndex, vecOutCard);
			return false;
		}
		case COMB_TYPE_ZHADAN:
		{
			if (m_listCard.size() >= ZHADAN_CONDITION)
			{
				CCardHandler::Instance()->OnRecvComb(m_cIndex, vecOutCard);
				return false;
			}
			for (INT32 i = 1; i <= 13; ++i)
			{
				if (4 == szSame[i])
				{
					if (i > m_stLastComb.cMaxPoint)
					{
						ChooseCard(4, i, vecOutCard);
						CCardHandler::Instance()->OnRecvComb(m_cIndex, vecOutCard);
						return true;
					}
				}
			}
			if (1 == m_nHuojianCount)
			{
				ChooseCard(1, 14, vecOutCard);
				ChooseCard(1, 15, vecOutCard);
				CCardHandler::Instance()->OnRecvComb(m_cIndex, vecOutCard);
				return true;
			}
			break;
		}
		case COMB_TYPE_DAN:
		{
			for (INT32 i = 1; i <= 15; ++i)
			{
				if (1 == szSame[i])
				{
					if (i > m_stLastComb.cMaxPoint)
					{
						ChooseCard(1, i, vecOutCard);
						CCardHandler::Instance()->OnRecvComb(m_cIndex, vecOutCard);
						return true;
					}
				}
			}
			break;
		}
		case COMB_TYPE_DUI:
		{
			for (INT32 i = 1; i <= 13; ++i)
			{
				if (2 == szSame[i])
				{
					if (i > m_stLastComb.cMaxPoint)
					{
						ChooseCard(2, i, vecOutCard);
						CCardHandler::Instance()->OnRecvComb(m_cIndex, vecOutCard);
						return true;
					}
				}
			}
			break;
		}
		case COMB_TYPE_SAN:
		{
			for (INT32 i = 1; i <= 13; ++i)
			{
				if (3 == szSame[i])
				{
					if (i > m_stLastComb.cMaxPoint)
					{
						ChooseCard(3, i, vecOutCard);
						CCardHandler::Instance()->OnRecvComb(m_cIndex, vecOutCard);
						return true;
					}
				}
			}
			break;
		}
		case COMB_TYPE_SANDAIYI:
		{
			if (m_listCard.size() < 4)
			{
				break;
			}
			for (INT32 i = 1; i <= 13; ++i)
			{
				if (3 == szSame[i] && i > m_stLastComb.cMaxPoint)
				{
					for (INT32 j = 1; j <= 13; ++i)
					{
						if (j == i)
						{
							continue;
						}
						if (1 == szSame[j])
						{
							ChooseCard(3, i, vecOutCard);
							ChooseCard(1, j, vecOutCard);
							CCardHandler::Instance()->OnRecvComb(m_cIndex, vecOutCard);
							return true;
						}
					}
				}
			}
			break;
		}
		case COMB_TYPE_SANDAIER:
		{
			if (m_listCard.size() < 5)
			{
				break;
			}
			for (INT32 i = 1; i <= 13; ++i)
			{
				if (3 == szSame[i] && i > m_stLastComb.cMaxPoint)
				{
					for (INT32 j = 1; j <= 13; ++i)
					{
						if (j == i)
						{
							continue;
						}
						if (2 == szSame[j])
						{
							ChooseCard(3, i, vecOutCard);
							ChooseCard(2, j, vecOutCard);
							CCardHandler::Instance()->OnRecvComb(m_cIndex, vecOutCard);
							return true;
						}
					}
				}
			}
			break;
		}
		case COMB_TYPE_DANSHUN:
		{
			if (m_listCard.size() < 5)
			{
				break;
			}
			if (szSameCount[1] >= m_stLastComb.cCount && szEnd[1] > m_stLastComb.cMaxPoint)
			{
				INT32 nShunCount = 0;
				for (INT32 i = szEnd[1];; --i)
				{
					if (nShunCount == m_stLastComb.cCount)
					{
						break;
					}
					ChooseCard(1, i, vecOutCard);
					++nShunCount;
				}
				CCardHandler::Instance()->OnRecvComb(m_cIndex, vecOutCard);
				return true;
			}
			break;
		}
		case COMB_TYPE_SHUANGSHUN:
		{
			if (m_listCard.size() < 6)
			{
				break;
			}
			if (szSameCount[2] >= m_stLastComb.cCount && szEnd[2] > m_stLastComb.cMaxPoint)
			{
				INT32 nShunCount = 0;
				for (INT32 i = szEnd[2];; --i)
				{
					if (nShunCount == m_stLastComb.cCount)
					{
						break;
					}
					ChooseCard(2, i, vecOutCard);
					++nShunCount;
				}
				CCardHandler::Instance()->OnRecvComb(m_cIndex, vecOutCard);
				return true;
			}
			break;
		}
		case COMB_TYPE_SANSHUN:
		{
			if (m_listCard.size() < 6)
			{
				break;
			}
			if (szSameCount[3] >= m_stLastComb.cCount && szEnd[3] > m_stLastComb.cMaxPoint)
			{
				INT32 nShunCount = 0;
				for (INT32 i = szEnd[3];; --i)
				{
					if (nShunCount == m_stLastComb.cCount)
					{
						break;
					}
					ChooseCard(3, i, vecOutCard);
					++nShunCount;
				}
				CCardHandler::Instance()->OnRecvComb(m_cIndex, vecOutCard);
				return true;
			}
			break;
		}
		default:
		{
			break;
		}
		}
		//zhadan huojian
		if (m_listCard.size() >= ZHADAN_CONDITION)
		{
			CCardHandler::Instance()->OnRecvComb(m_cIndex, vecOutCard);
			return false;
		}
		for (INT32 i = 1; i <= 13; ++i)
		{
			if (4 == szSame[i])
			{
				ChooseCard(4, i, vecOutCard);
				CCardHandler::Instance()->OnRecvComb(m_cIndex, vecOutCard);
				return true;
			}
		}
		if (1 == m_nHuojianCount)
		{
			ChooseCard(1, 14, vecOutCard);
			ChooseCard(1, 15, vecOutCard);
			CCardHandler::Instance()->OnRecvComb(m_cIndex, vecOutCard);
			return true;
		}
		CCardHandler::Instance()->OnRecvComb(m_cIndex, vecOutCard);
		return false;
	}
	CCardHandler::Instance()->OnRecvComb(m_cIndex, vecOutCard);
	return false;
}

bool CCardPlayer::ClearOutCard(vector<CCard> vecOutCards)
{
	for (size_t i = 0; i < vecOutCards.size(); ++i)
	{
		bool bIsFind = false;
		for (list<CCard>::iterator it = m_listCard.begin(); it != m_listCard.end();)
		{
			if (it->GetCardID() == vecOutCards[i].GetCardID())
			{
				bIsFind = true;
				m_listCard.erase(it);
				break;
			}
			else
			{
				++it;
			}
		}
		if (!bIsFind)
		{
			return false;
		}
	}
	if (0 == m_listCard.size())
	{
		m_bWin = true;
	}
	char cOutput[32];
	sprintf_s(cOutput, "Player %d:", m_cIndex);
	CLogHandler::Instance()->WriteLine(cOutput);
	CCardHandler::Instance()->PrintCard(m_listCard);
	return true;
}

void CCardPlayer::ChooseCard(INT8 cCount, INT32 nTargetGrade, vector<CCard>& vecChooseCard)
{
	INT8 cOutCount = 0;
	for (list<CCard>::iterator it = m_listCard.begin(); it != m_listCard.end(); ++it)
	{
		if (cOutCount == cCount)
		{
			break;
		}
		if (it->GetCardGrade() == nTargetGrade)
		{
			vecChooseCard.push_back(*it);
			++cOutCount;
		}
	}
}
