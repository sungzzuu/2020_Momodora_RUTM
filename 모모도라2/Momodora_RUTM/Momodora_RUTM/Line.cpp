#include "stdafx.h"
#include "Line.h"
#include "ScrollMgr.h"

CLine::CLine()
{
}

CLine::CLine(LINEPOS & _Left, LINEPOS & _Right)
	:m_tInfo(_Left, _Right), m_eType(TILE::END), m_bLineOn(true)
{
}

CLine::CLine(LINEINFO & _tInfo, TILE::TYPE _eType)
	:m_tInfo(_tInfo), m_eType((_eType))
{
}


CLine::~CLine()
{
	Release();
}
int CLine::Update()
{
	return OBJ_NOEVENT;
}

void CLine::Late_Update()
{
}


void CLine::Release()
{
}

void CLine::Initialize()
{
	
}



void CLine::Render(HDC _DC)
{
	int iScrollX = (int)(CScrollMgr::Get_Instance()->Get_ScrollX());
	int iScrollY = (int)(CScrollMgr::Get_Instance()->Get_ScrollY());

	if (m_bLineOn)
	{
		int	iColor = 0;
		if (TILE::THIN == m_eType)
			iColor = YELLOW;
		else if (TILE::CEILING == m_eType)
			iColor = SKYBLUE;
		else if (TILE::MOVING == m_eType)
			iColor = PURPLE;
		else
			iColor = RED;
		HPEN hpen = CreatePen(PS_SOLID, 2, iColor);
		HGDIOBJ holdpen = SelectObject(_DC, hpen);
		MoveToEx(_DC, (int)m_tInfo.tLeftPos.fX+ iScrollX,(int)m_tInfo.tLeftPos.fY+ iScrollY, nullptr);
		LineTo(_DC, (int)m_tInfo.tRightPos.fX+ iScrollX, (int)m_tInfo.tRightPos.fY+ iScrollY);
		DeleteObject(hpen);
	}
	
}
