#include "stdafx.h"
#include "Bomb.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "LineMgr.h"

CBomb::CBomb()
{
}


CBomb::~CBomb()
{
}

void CBomb::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/WeaponStretch.bmp", L"WeaponStretch");

	m_tInfo.iCX = 40;
	m_tInfo.iCY = 40;
	m_tInfo.iCollisionCX = 30;
	m_tInfo.iCollisionCY = 30;



	m_tFrame.iPosX = 12;
	m_tFrame.iPosY = 657;
	m_tFrame.iSizeX = 16;
	m_tFrame.iSizeY = 16;
	m_fSpeed = 10.f;
	m_pFrameKey = L"Girl_RIGHT";
	m_pName = L"Bomb";
	m_fAngle = 0.f;
	//게이지
	m_tGauge.iHp = 100;
	m_tGauge.iGold = 0;
	m_tGauge.iMaxHp = 100;
	m_tGauge.iAtt = 50;

	m_fSpeed = 8.f;

	m_eGroup = GROUPID::OBJECT;

}

int CBomb::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	m_fAngle += 4.f;
	Update_Rect();
	m_tCollisionRect.bottom = (LONG)(m_tInfo.fY + (m_tInfo.iCollisionCY >> 1));
	if (L"Girl_RIGHT" == m_pFrameKey)
		m_tInfo.fX += m_fSpeed;
	else
		m_tInfo.fX -= m_fSpeed;
	m_tInfo.fY -= 15 * cosf(m_fAngle*PI / 180.f);
		

	float fY = 0.f;
	bool _bCheck = 0;
	Update_Rect();
	//렉트 충돌시 이동안함->박힘->WALL
	if (!m_tGauge.iHp)
		return OBJ_DEAD;

	return OBJ_NOEVENT;
}

void CBomb::Late_Update()
{
}

void CBomb::Render(HDC _DC)
{
	if (m_bDead)
		return;
	Update_Rect();

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

	if (!lstrcmp(L"Girl_LEFT", m_pFrameKey))
	{
		HDC hMemDC2 = CBmpMgr::Get_Instance()->Find_Bmp(L"Girl_RIGHT");
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

void CBomb::Release()
{
}
