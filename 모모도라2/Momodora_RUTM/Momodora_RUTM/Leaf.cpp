#include "stdafx.h"
#include "Leaf.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"

CLeaf::CLeaf()
{
}


CLeaf::~CLeaf()
{
}

void CLeaf::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/Leaf/Stretch.bmp", L"Stretch_Leaf");

	m_tInfo.iCX = 168;
	m_tInfo.iCY = 120;
	m_tInfo.iCollisionCX = 60;
	m_tInfo.iCollisionCY = 60;
	m_eCurState = END;
	m_ePreState = END;


	//°ÔÀÌÁö
	m_tGauge.iHp = 0;
	m_tGauge.iGold = 0;
	m_tGauge.iMaxHp = 0;
	m_tGauge.iAtt = 100;

	m_pName = L"Leaf";

	m_eGroup = GROUPID::OBJECT;

}

int CLeaf::Update()
{
	Update_Rect();
	Scene_Change();
	Frame_Move();


	return OBJ_NOEVENT;
}

void CLeaf::Late_Update()
{
}

void CLeaf::Render(HDC _DC)
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
	if (END != m_eCurState)
	{
		HDC hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(m_pFrameKey);
		if (!lstrcmp(L"Player_LEFT", m_pFrameKey))
		{
			HDC hMemDC2 = CBmpMgr::Get_Instance()->Find_Bmp(L"Player_RIGHT");
			HDC hStretch = CBmpMgr::Get_Instance()->Find_Bmp(L"Stretch_Leaf");
			StretchBlt(hStretch
				, 0, 0
				, m_tInfo.iCX, m_tInfo.iCY
				, hMemDC2
				, m_tFrame.iFrameStart * (m_tFrame.iSizeX + m_tFrame.iPosX) + m_tFrame.iSizeX, m_tFrame.iPosY + m_tFrame.iSizeY
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
				, m_tFrame.iPosX + m_tFrame.iFrameStart * (m_tFrame.iSizeX + m_tFrame.iPosX), m_tFrame.iPosY + m_tFrame.iSizeY
				, m_tFrame.iSizeX, m_tFrame.iSizeY
				, RGB(240, 240, 240));
		}
	}
	
}

void CLeaf::Release()
{
}

int CLeaf::Get_Leaf()
{
	if (ATTACK1 == m_eCurState)
		return 1;
	else if (ATTACK2 == m_eCurState)
		return 2;
	else if (ATTACK3 == m_eCurState)
		return 3;
	else
		return 0;
}

void CLeaf::Frame_Move()
{
	if (m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed < GetTickCount())
	{
		++m_tFrame.iFrameStart;
		m_tFrame.dwFrameTime = GetTickCount();
		if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
		{
			m_eCurState = END;

		}
	}
}

void CLeaf::Set_State(int _state)
{
	if (1 == _state)
		m_eCurState = ATTACK1;
	else if (2 == _state)
		m_eCurState = ATTACK2;
	else if(3 == _state)
		m_eCurState = ATTACK3;

}

void CLeaf::Scene_Change()
{
	if (m_eCurState != m_ePreState)
	{
		switch (m_eCurState)
		{
		case ATTACK1:
			m_tFrame.bEnd = false;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 6;
			m_tFrame.iPosX = 1;
			m_tFrame.iPosY = 390 - 48;
			m_tFrame.iSizeX = 96;
			m_tFrame.iSizeY = 48;
			m_tFrame.dwFrameSpeed = 50;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case ATTACK2:
			m_tFrame.bEnd = false;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 6;
			m_tFrame.iPosX = 1;
			m_tFrame.iPosY = 453 - 48;
			m_tFrame.iSizeX = 96;
			m_tFrame.iSizeY = 48;
			m_tFrame.dwFrameSpeed = 50;
			m_tFrame.dwFrameTime = GetTickCount();
			break; 
		case ATTACK3:
			m_tFrame.bEnd = false;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 6;
			m_tFrame.iPosX = 1;
			m_tFrame.iPosY = 516 - 48;
			m_tFrame.iSizeX = 96;
			m_tFrame.iSizeY = 48;
			m_tFrame.dwFrameSpeed = 50;
			m_tFrame.dwFrameTime = GetTickCount();
			break;

		}
		m_ePreState = m_eCurState;
	}
}
