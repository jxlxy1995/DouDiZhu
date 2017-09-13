#ifndef __CardType_h__
#define __CardType_h__

#include <algorithm>
#include <basetsd.h>

using namespace std;

enum ECardType
{
	CARD_TYPE_SPADE		= 0,			//黑桃
	CARD_TYPE_HEART		= 1,			//红桃
	CARD_TYPE_CLUB		= 2,			//梅花
	CARD_TYPE_DIAMOND	= 3,			//方块
	CARD_TYPE_JOKER		= 4,			//大小王
};



#endif