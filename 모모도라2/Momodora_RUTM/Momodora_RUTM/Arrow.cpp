#include "stdafx.h"
#include "Arrow.h"
#include "ScrollMgr.h"
#include "BmpMgr.h"
#include "ObjMgr.h"

CArrow::CArrow()
{
}


CArrow::~CArrow()
{
}

void CArrow::Initialize()
{

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/WeaponStretch.bmp", L"WeaponStretch");

	m_tInfo.iCX = 50;
	m_tInfo.iCY = 50;
	m_tInfo.iCollisionCX = 30;
	m_tInfo.iCollisionCY = 20;



	m_tFrame.iPosX = 86;
	m_tFrame.iPosY = 3788;
	m_tFrame.iSizeX = 32;
	m_tFrame.iSizeY = 32;
	m_fSpeed = 10.f;
	m_pFrameKey = L"Player_RIGHT";
	m_eCurState = LIVE;

	m_pName = L"Arrow";

	//게이지
	m_tGauge.iHp = 100;
	m_tGauge.iGold = 0;
	m_tGauge.iMaxHp = 100;
	m_tGauge.iAtt = 50;

	if (m_tInfo.fX > CObjMgr::Get_Instance()->Get_Player()->Get_Info().fX)
	{
		m_pFrameKey = L"Player_LEFT";
	}
	else
	{
		m_pFrameKey = L"Player_RIGHT";
	}
	m_eGroup = GROUPID::OBJECT;

}

int CArrow::Update()
{
	Update_Rect();
	m_tCollisionRect.bottom = (LONG)(m_tInfo.fY + (m_tInfo.iCollisionCY >> 1));
	if (WALL != m_eCurState)
	{
		if (L"Player_RIGHT" == m_pFrameKey)
			m_tInfo.fX += m_fSpeed;
		else
			m_tInfo.fX -= m_fSpeed;
	}

	//렉트 충돌시 이동안함->박힘->WALL

	if (m_bDead)
		return OBJ_DEAD;
	return OBJ_NOEVENT;
}

void CArrow::Late_Update()
{
}

void CArrow::Render(HDC _DC)
{
	//세트, 칼은 CollisionRect가 중간임
	Update_Rect();
	m_tCollisionRect.bottom = (LONG)(m_tInfo.fY + (m_tInfo.iCollisionCY >> 1));


	int iScrollX = (int)(CScrollMgr::Get_Instance()->Get_ScrollX());
	int iScrollY = (int)(CScrollMgr::Get_Instance()->Get_ScrollY());

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

	if (!lstrcmp(L"Player_LEFT", m_pFrameKey))
	{
		HDC hMemDC2 = CBmpMgr::Get_Instance()->Find_Bmp(L"Player_RIGHT");
		HDC hStretch = CBmpMgr::Get_Instance()->Find_Bmp(L"WeaponStretch");
		StretchBlt(hStretch
			, 0, 0
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC2
			, m_tFrame.iPosX + m_tFrame.iSizeX - 1, m_tFrame.iPosY
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
			, m_tFrame.iPosX, m_tFrame.iPosY
			, m_tFrame.iSizeX, m_tFrame.iSizeY
			, RGB(240, 240, 240));
	}
}

void CArrow::Release()
{
}
