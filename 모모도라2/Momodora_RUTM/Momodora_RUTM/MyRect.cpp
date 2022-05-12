#include "stdafx.h"
#include "MyRect.h"
#include "ScrollMgr.h"


CMyRect::CMyRect()
{
}

CMyRect::CMyRect(RECT & _rc, TILE::TYPE _eType)
	:m_tRect(_rc), m_eType(_eType), m_bLineOn(true)
{

}


CMyRect::~CMyRect()
{
	Release();
}

void CMyRect::Initialize()
{
}

int CMyRect::Update()
{
	return 0;
}

void CMyRect::Late_Update()
{
}

void CMyRect::Render(HDC _DC)
{
	int iScrollX = (int)(CScrollMgr::Get_Instance()->Get_ScrollX());
	int iScrollY = (int)(CScrollMgr::Get_Instance()->Get_ScrollY());

	if (m_bLineOn)
	{
		int	iColor = 0;
		if (TILE::LADDER == m_eType)
			iColor = PURPLE;
		else if (TILE::MOVING == m_eType)
			iColor = EMERALD;
		else
			iColor = BLUE;
		HPEN hpen = CreatePen(PS_SOLID, 4, iColor);
		HGDIOBJ holdpen = SelectObject(_DC, hpen);
		HBRUSH myBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
		HBRUSH oldBrush = (HBRUSH)SelectObject(_DC, myBrush);
		Rectangle(_DC, m_tRect.left + iScrollX, m_tRect.top + iScrollY, m_tRect.right + iScrollX, m_tRect.bottom + iScrollY);
		SelectObject(_DC, oldBrush);
		DeleteObject(myBrush);
		DeleteObject(hpen);
	}
}

void CMyRect::Release()
{
}
