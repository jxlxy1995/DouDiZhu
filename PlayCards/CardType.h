#ifndef __CardType_h__
#define __CardType_h__

#include <algorithm>
#include <basetsd.h>
#include <vector>

using namespace std;

class CCard;

enum ECardType
{
	CARD_TYPE_SPADE		= 0,			//黑桃
	CARD_TYPE_HEART		= 1,			//红桃
	CARD_TYPE_CLUB		= 2,			//梅花
	CARD_TYPE_DIAMOND	= 3,			//方块
	CARD_TYPE_JOKER		= 4,			//大小王
};

enum ECombType
{
	COMB_TYPE_ERROR			= -1,
	COMB_TYPE_HUOJIAN		= 0,
	COMB_TYPE_ZHADAN		= 1,
	COMB_TYPE_DAN			= 2,
	COMB_TYPE_DUI			= 3,
	COMB_TYPE_SAN			= 4,
	COMB_TYPE_SANDAIYI		= 5,
	COMB_TYPE_SANDAIER		= 6,
	COMB_TYPE_DANSHUN		= 7,
	COMB_TYPE_SHUANGSHUN	= 8,
	COMB_TYPE_SANSHUN		= 9,
	COMB_TYPE_FEIJI			= 10,
	COMB_TYPE_SIDAIER		= 11,
};

enum ECombGradeType
{
	COMB_GRADE_TYPE_NORMAL		= 0,
	COMB_GRADE_TYPE_ZHADAN		= 1,
	COMB_GRADE_TYPE_HUOJIAN		= 2,
};

struct SCardComb
{
	INT8			cCombType;
	INT8			cCount;
	INT8			cMaxPoint;
	INT8			cGrade;
	vector<CCard>	vecCard;
};
#endif