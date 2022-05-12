#include "stdafx.h"
#include "BuffSword.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"

CBuffSword::CBuffSword()
{
}


CBuffSword::~CBuffSword()
{
}

void CBuffSword::Scene_Change()
{
	if (m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed < GetTickCount())
	{
		++m_tFrame.iFrameStart;
		m_tFrame.dwFrameTime = GetTickCount();
		if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
		{
			m_bDead = true;
		}
	}
}

void CBuffSword::Initialize()
{

	m_tInfo.iCX = 236;
	m_tInfo.iCY = 156;
	m_tInfo.iCollisionCX = 80;
	m_tInfo.iCollisionCY = 10;

	m_tFrame.iFrameStart = 3;
	m_tFrame.iFrameEnd = 11;
	m_tFrame.iPosX = 2;
	m_tFrame.iPosY = 1012;
	m_tFrame.iSizeX = 118;
	m_tFrame.iSizeY = 78;
	m_tFrame.dwFrameSpeed = 50;
	m_tFrame.dwFrameTime = GetTickCount();

	Update_Rect();

	
	m_fSpeed = 6.f;

	//게이지
	m_tGauge.iHp = 100;
	m_tGauge.iGold = 0;
	m_tGauge.iMaxHp = 100;
	m_tGauge.iAtt = 100;

	m_eGroup = GROUPID::OBJECT;


}

int CBuffSword::Update()
{
	Update_Rect();
	m_tCollisionRect.top = (LONG)(m_tInfo.fY - 20);
	m_tCollisionRect.bottom = (LONG)(m_tCollisionRect.top +20);
	if (L"Boss_LEFT" == m_pFrameKey)
		m_tInfo.fX -= m_fSpeed;
	else
		m_tInfo.fX += m_fSpeed;

	//렉트 충돌시 이동안함->박힘->WALL
	if (m_tGauge.iHp <= 0)
		return OBJ_DEAD;
	if (m_bDead)
		return OBJ_DEAD;
	return OBJ_NOEVENT;
}

void CBuffSword::Late_Update()
{
}

void CBuffSword::Render(HDC _DC)
{
	Update_Rect();
	m_tCollisionRect.top = (LONG)(m_tInfo.fY -20);
	m_tCollisionRect.bottom = (LONG)(m_tCollisionRect.top + 20);
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	if (m_bLineOn)
	{
		HPEN hpen = CreatePen(PS_SOLID, 2, GREEN);
		HGDIOBJ holdpen = SelectObject(_DC, hpen);
		HBRUSH myBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
		HBRUSH oldBrush = (HBRUSH)SelectObject(_DC, myBrush);
		Rectangle(_DC, m_tRect.left + iScrollX, m_tRect.top + iScrollY, m_tRect.right + iScrollX, m_tRect.bottom + iScrollY);
		Rectangle(_DC, m_tCollisionRect.left + iScrollX, m_tCollisionRect.top + iScrollY, m_tCollisionRect.right + iScrollX, m_tCollisionRect.bottom + iScrollY);
		SelectObject(_DC, oldBrush);
		DeleteObject(myBrush);
		DeleteObject(hpen);
	}

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(m_pFrameKey);
	if (!lstrcmp(L"Boss_RIGHT", m_pFrameKey))
	{
		HDC hMemDC2 = CBmpMgr::Get_Instance()->Find_Bmp(L"Boss_LEFT");
		HDC hStretch = CBmpMgr::Get_Instance()->Find_Bmp(L"BossStretch");
		StretchBlt(hStretch
			, 0, 0
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC2
			, 1 + m_tFrame.iFrameStart * (m_tFrame.iSizeX + m_tFrame.iPosX) + m_tFrame.iSizeX, m_tFrame.iPosY
			, -m_tFrame.iSizeX, m_tFrame.iSizeY
			, SRCCOPY);

		GdiTransparentBlt(_DC
			, m_tRect.left + iScrollX, m_tRect.top + iScrollY
			, m_tInfo.iCX, m_tInfo.iCY
			, hStretch
			, 0, 0
			, m_tInfo.iCX, m_tInfo.iCY
			, RGB(240, 240, 240));
	}
	else
	{
		GdiTransparentBlt(_DC
			, m_tRect.left + iScrollX, m_tRect.top + iScrollY
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC
			, m_tFrame.iPosX + m_tFrame.iFrameStart * (m_tFrame.iSizeX + m_tFrame.iPosX), m_tFrame.iPosY
			, m_tFrame.iSizeX, m_tFrame.iSizeY
			, RGB(240, 240, 240));
	}
}

void CBuffSword::Release()
{
}
