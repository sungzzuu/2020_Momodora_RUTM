#pragma once
#ifndef __STRUCT_H__
#define __STRUCT_H__
#include "Enum.h"

typedef struct tagInfo
{
	float	fX;
	float	fY;
	int		iCX;
	int		iCY;
	int		iCollisionCX;
	int		iCollisionCY;
}INFO;

typedef struct tagGauge
{
	int		iHp;
	int		iAtt;
	int		iGold;
	int		iMaxHp;

}GAUGE;

typedef struct tagLinePos
{
	tagLinePos() { ZeroMemory(this, sizeof(tagLinePos)); }
	tagLinePos(float _x, float _y) : fX(_x), fY(_y) {}

	float fX;
	float fY;
}LINEPOS;


typedef struct tagLineInfo
{
	tagLineInfo() { ZeroMemory(this, sizeof(tagLineInfo)); }
	tagLineInfo(LINEPOS& _Left, LINEPOS& _Right) : tLeftPos(_Left), tRightPos(_Right) {}

	LINEPOS tLeftPos;
	LINEPOS tRightPos;
}LINEINFO;

typedef struct tagFrame
{
	bool			bEnd;
	int				iFrameStart;
	int				iFrameEnd;
	int				iPosX;
	int				iPosY;
	int				iSizeX;
	int				iSizeY;
	DWORD			dwFrameSpeed;
	DWORD			dwFrameTime;
}FRAME;


#endif // !__STRUCT_H__
