#include "stdafx.h"
#include "BigPlant.h"
#include "LineMgr.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "ObjMgr.h"
#include "Gold.h"
#include "SoundMgr.h"

CBigPlant::CBigPlant()
	:m_dwAttack(GetTickCount())
{
}


CBigPlant::~CBigPlant()
{
}

void CBigPlant::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/Monster/BigPlant_RIGHT.bmp", L"BigPlant_RIGHT");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/Monster/BigPlant_Stretch.bmp", L"BigPlant_Stretch");


	m_tInfo.fX = 400.f;
	m_tInfo.fY = 300.f;
	m_tInfo.iCX = 300;
	m_tInfo.iCY = 250;
	m_tInfo.iCollisionCX = 200;
	m_tInfo.iCollisionCY = 180;
	m_eCurState = WALK;
	m_ePreState = WALK;
	m_bRight = RIGHT;

	m_pFrameKey = L"BigPlant_RIGHT";
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 9;
	m_tFrame.iPosX = 2;
	m_tFrame.iPosY = 17;
	m_tFrame.iSizeX = 120;
	m_tFrame.iSizeY = 100;
	m_tFrame.dwFrameSpeed = 100;
	m_tFrame.dwFrameTime = GetTickCount();
	m_fSpeed = 1.f;
	m_iAttackRandom = 0;
	m_iLeafNum = 0;
	float fY = 0.f;
	bool _bCheck = false;
	if (CLineMgr::Get_Instance()->Collision_Surface(m_tInfo.fX, m_tInfo.fY + (m_tInfo.iCY), &fY, &_bCheck))
		m_tInfo.fY = fY - (m_tInfo.iCY >> 1);


	//게이지
	m_tGauge.iHp = 1000;
	m_tGauge.iGold = 50;
	m_tGauge.iMaxHp = 1000;
	m_tGauge.iAtt = 100;

	m_eGroup = GROUPID::OBJECT;

}

int CBigPlant::Update()
{
	
	if (m_dwCollision + 300 < GetTickCount())
	{
		m_dwCollision = GetTickCount();
		m_bAttack = false;

	}
	Frame_Move();
	Update_Rect();
	if (WALK == m_eCurState)
	{
		if (m_pFrameKey == L"BigPlant_RIGHT")
			m_tInfo.fX += m_fSpeed;
		else
			m_tInfo.fX -= m_fSpeed;
		CSoundMgr::Get_Instance()->PlaySound(L"sndBigMonkeyFootstep.wav", CSoundMgr::MONSTER);
	}
	if (m_tInfo.fX > CObjMgr::Get_Instance()->Get_Player()->Get_Info().fX)
	{
		m_pFrameKey = L"BigPlant_LEFT";
	}
	else
	{
		m_pFrameKey = L"BigPlant_RIGHT";
	}

	if (m_dwAttack + 4000 < GetTickCount())
	{
		m_dwAttack = GetTickCount();
		m_eCurState = ATTACK;
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::MONSTER);
		CSoundMgr::Get_Instance()->PlaySound(L"sndBigMonkeySwing.wav", CSoundMgr::MONSTER);
	}


	Scene_Change();

	if (ATTACK == m_eCurState)
	{
		m_tInfo.iCX = 450;
		m_tInfo.iCY = 250;
		m_tInfo.iCollisionCX = 400;
		m_tInfo.iCollisionCY = 100;
	}
	else
	{
		m_tInfo.iCX = 300;
		m_tInfo.iCY = 250;
		m_tInfo.iCollisionCX = 200;
		m_tInfo.iCollisionCY = 100;

	}
	// 크기가 커지면 바닥충돌 안된다,,,,
	float fY = -1.f;
	bool _bCheck = false;

	if (CLineMgr::Get_Instance()->Collision_Surface(m_tInfo.fX, m_tInfo.fY + (m_tInfo.iCY >> 1), &fY, &_bCheck))
		m_tInfo.fY = fY - (m_tInfo.iCY >> 1);

	if (m_tGauge.iHp <= 0)
	{
		for (int i = 0; i < m_tGauge.iGold; i++)
		{
			int _ran = rand() % 2;
			if (0 == _ran)
				_ran = -1;
			else
				_ran = 1;
			CObj* pObj = CAbstractFactory<CGold>::Create(m_tInfo.fX + i*_ran * 20, m_tInfo.fY - (m_tInfo.iCY >> 1));
			CObjMgr::Get_Instance()->Add_Object(OBJID::GOLD, pObj);
			
		}
		return OBJ_DEAD;
	}

	return OBJ_NOEVENT;
}

void CBigPlant::Late_Update()
{
	if(CObjMgr::Get_Instance()->Collision_Monster_Weapon(this, &m_bAttack))
	{
		m_dwCollision = GetTickCount();
	}
}

void CBigPlant::Render(HDC _DC)
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
	if (!lstrcmp(L"BigPlant_LEFT", m_pFrameKey))
	{
		HDC hMemDC2 = CBmpMgr::Get_Instance()->Find_Bmp(L"BigPlant_RIGHT");
		HDC hStretch = CBmpMgr::Get_Instance()->Find_Bmp(L"BigPlant_Stretch");
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

void CBigPlant::Release()
{
}

void CBigPlant::Scene_Change()
{
	if (m_eCurState != m_ePreState)
	{
		switch (m_eCurState)
		{
		case WALK:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 9;
			m_tFrame.iPosX = 2;
			m_tFrame.iPosY = 17;
			m_tFrame.iSizeX = 120;
			m_tFrame.iSizeY = 100;
			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case ATTACK:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 15;
			m_tFrame.iPosX = 2;
			m_tFrame.iPosY = 169;
			m_tFrame.iSizeX = 180;
			m_tFrame.iSizeY = 100;
			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		}

		m_ePreState = m_eCurState;
	}
}

void CBigPlant::Frame_Move()
{
	if (m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed < GetTickCount())
	{
		++m_tFrame.iFrameStart;
		m_tFrame.dwFrameTime = GetTickCount();
		if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
		{
			m_eCurState = WALK;
			m_tFrame.iFrameStart = 0;
		}
	}
}
