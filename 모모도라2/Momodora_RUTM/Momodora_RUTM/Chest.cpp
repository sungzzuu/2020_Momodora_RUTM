#include "stdafx.h"
#include "Chest.h"
#include "ScrollMgr.h"
#include "BmpMgr.h"
#include "ObjMgr.h"
#include "LineMgr.h"

CChest::CChest()
{
}


CChest::~CChest()
{
}

void CChest::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/Monster/Chest.bmp", L"Chest");

	//°ÔÀÌÁö
	m_tInfo.fX = 200.f;
	m_tInfo.fY = 200.f;
	m_tInfo.iCX = 40;
	m_tInfo.iCY = 40;
	m_eCurState = CLOSE;

	m_tInfo.iCollisionCX = 40;
	m_tInfo.iCollisionCY = 40;

	m_pFrameKey = L"Chest";
	m_tGauge.iHp = 1000;
	m_tGauge.iGold = 100;
	m_tGauge.iMaxHp = 1000;
	m_tGauge.iAtt = 100;


	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 0;
	m_tFrame.iPosX = 0;
	m_tFrame.iPosY = 0;
	m_tFrame.iSizeX = 64;
	m_tFrame.iSizeY = 64;

	float fY = 0.f;
	bool _bCheck = false;
	if (CLineMgr::Get_Instance()->Collision_Surface(m_tInfo.fX, m_tInfo.fY + (m_tInfo.iCY >> 1), &fY, &_bCheck))
		m_tInfo.fY = fY - (m_tInfo.iCY >> 1);
}

int CChest::Update()
{

	if (m_tGauge.iHp <= 0 && m_eCurState == CLOSE)
	{
		m_eCurState = OPEN;

	}

		return OBJ_NOEVENT;
}

void CChest::Late_Update()
{
	CObjMgr::Get_Instance()->Collision_Monster_Weapon(this, &m_bAttack);

}

void CChest::Render(HDC _DC)
{
	Update_Rect();
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	if (m_bLineOn)
	{
		HPEN hpen = CreatePen(PS_SOLID, 2, DEEPGREEN);
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
		, m_tFrame.iPosX , m_tFrame.iPosY
		, m_tFrame.iSizeX, m_tFrame.iSizeY
		, RGB(240, 240, 240));
}

void CChest::Release()
{
}

void CChest::Scene_Change()
{
	if (m_eCurState != m_ePreState)
	{
		switch (m_eCurState)
		{
		case CLOSE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iPosX = 0;
			m_tFrame.iPosY = 0;
			m_tFrame.iSizeX = 64;
			m_tFrame.iSizeY = 64;
			break;
		case OPEN:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iPosX = 64;
			m_tFrame.iPosY = 0;
			m_tFrame.iSizeX = 64;
			m_tFrame.iSizeY = 64;
			break;
		}
		m_ePreState = m_eCurState;
	}
}

void CChest::Frame_Move()
{
}
