#include "stdafx.h"
#include "Impact.h"
#include "ScrollMgr.h"
#include "BmpMgr.h"
#include "LineMgr.h"

#include "ObjMgr.h"

CImpact::CImpact()
{
}


CImpact::~CImpact()
{
}

void CImpact::Frame_Move()
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

void CImpact::Initialize()
{

	m_tInfo.fX = CObjMgr::Get_Instance()->Get_Player()->Get_Info().fX;
	m_tInfo.fY = 100.f;
	m_tInfo.iCX = 512;
	m_tInfo.iCY = 510;
	m_tInfo.iCollisionCX = 100;
	m_tInfo.iCollisionCY = 300;
	float fY = 0.f;
	bool _bCheck = false;
	if (CLineMgr::Get_Instance()->Collision_Surface(m_tInfo.fX, m_tInfo.fY + (m_tInfo.iCY >> 1), &fY, &_bCheck))
		m_tInfo.fY = fY - (m_tInfo.iCY >> 1);
	
	

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 8;
	m_tFrame.iPosX = 2;
	m_tFrame.iPosY = 1604;
	m_tFrame.iSizeX = 256;
	m_tFrame.iSizeY = 255;
	m_tFrame.dwFrameSpeed = 200;
	m_tFrame.dwFrameTime = GetTickCount();

	Update_Rect();
	m_pFrameKey = L"Boss_LEFT";

	m_fSpeed = 6.f;

	//게이지
	m_tGauge.iHp = 100;
	m_tGauge.iGold = 0;
	m_tGauge.iMaxHp = 100;
	m_tGauge.iAtt = 100;

	m_eGroup = GROUPID::OBJECT;

	

	

}

int CImpact::Update()
{
	Update_Rect();
	Frame_Move();
	//렉트 충돌시 이동안함->박힘->WALL
	if (m_tGauge.iHp <= 0)
		return OBJ_DEAD;
	if (m_bDead)
		return OBJ_DEAD;
	return OBJ_NOEVENT;
}

void CImpact::Late_Update()
{
}

void CImpact::Render(HDC _DC)
{

	Update_Rect();

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
	GdiTransparentBlt(_DC
		, m_tRect.left + iScrollX, m_tRect.top + iScrollY
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_tFrame.iPosX + m_tFrame.iFrameStart * (m_tFrame.iSizeX + m_tFrame.iPosX), m_tFrame.iPosY
		, m_tFrame.iSizeX, m_tFrame.iSizeY
		, RGB(240, 240, 240));
}

void CImpact::Release()
{
}
