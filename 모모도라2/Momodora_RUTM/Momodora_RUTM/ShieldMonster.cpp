#include "stdafx.h"
#include "ShieldMonster.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "LineMgr.h"
#include "ObjMgr.h"
#include "Sword.h"
#include "Gold.h"
#include "SoundMgr.h"
CShieldMonster::CShieldMonster()
	:m_eType(MONSTER::SHIELD)

{
}


CShieldMonster::~CShieldMonster()
{
	Release();
}

void CShieldMonster::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/Monster/GirlMonster/Girl_RIGHT.bmp", L"Girl_RIGHT");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/Monster/GirlMonster/Stretch.bmp", L"StretchGirl");

	m_tInfo.fX = 500.f;
	m_tInfo.fY = 300.f;
	m_tInfo.iCX = 80;
	m_tInfo.iCY = 80;
	m_tInfo.iCollisionCX = 50;
	m_tInfo.iCollisionCY = 30;
	m_eCurState = SHIELDWALK;
	m_ePreState = SHIELDWALK;

	m_pFrameKey = L"Girl_RIGHT";
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 0;
	m_tFrame.iPosX = 2;
	m_tFrame.iPosY = 445;
	m_tFrame.iSizeX = 32;
	m_tFrame.iSizeY = 32;
	m_tFrame.dwFrameSpeed = 300;
	m_tFrame.dwFrameTime = GetTickCount();
	m_fSpeed = 2.f;


	m_dwAttack = GetTickCount();
	m_dwTurnTime = GetTickCount();
	float fY = 0.f;
	bool _bCheck = false;
	if (CLineMgr::Get_Instance()->Collision_Surface(m_tInfo.fX, m_tInfo.fY + (m_tInfo.iCY >> 1), &fY, &_bCheck))
		m_tInfo.fY = fY - (m_tInfo.iCY >> 1);

	//∞‘¿Ã¡ˆ
	m_tGauge.iHp = 200;
	m_tGauge.iGold = 10;
	m_tGauge.iMaxHp = 200;
	m_tGauge.iAtt = 50;
	m_eGroup = GROUPID::OBJECT;


}

int CShieldMonster::Update()
{
	if (m_dwCollision + 300 < GetTickCount())
		m_bAttack = false;

	if (m_tGauge.iHp <= 0)
	{
		for (int i = 0; i < m_tGauge.iGold; i++)
		{
			int _ran = rand() % 2;
			if (0 == _ran)
				_ran = -1;
			else
				_ran = 1;
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::MONSTER);
			CSoundMgr::Get_Instance()->PlaySound(L"sndImpDeath.wav", CSoundMgr::MONSTER);
			CObj* pObj = CAbstractFactory<CGold>::Create(m_tInfo.fX + i*_ran * 20, m_tInfo.fY - (m_tInfo.iCY));
			CObjMgr::Get_Instance()->Add_Object(OBJID::GOLD, pObj);
		
		}
		return OBJ_DEAD;

	}
	float fY = 0.f;
	bool _bCheck = false;
	if (CLineMgr::Get_Instance()->Collision_Surface(m_tInfo.fX, m_tInfo.fY + (m_tInfo.iCY >> 1), &fY, &_bCheck))
		m_tInfo.fY = fY - (m_tInfo.iCY >> 1);
	if (SHIELDWALK == m_eCurState)
	{
		if (m_pFrameKey == L"Girl_RIGHT")
			m_tInfo.fX += m_fSpeed;
		else
			m_tInfo.fX -= m_fSpeed;
	}
	if (m_dwTurnTime + 2000 < GetTickCount() || SHIELDHURT == m_eCurState )
	{
		if (m_tInfo.fX > CObjMgr::Get_Instance()->Get_Player()->Get_Info().fX)
		{
			m_pFrameKey = L"Girl_LEFT";
		}
		else
		{
			m_pFrameKey = L"Girl_RIGHT";
		}
		m_dwTurnTime = GetTickCount();
	}
	
	if (m_dwAttack + 6000 < GetTickCount())
	{
		m_dwAttack = GetTickCount();
		m_eCurState = SHIELDTHROW;
	}
		
	Scene_Change();
	Frame_Move();

	return OBJ_NOEVENT;
}

void CShieldMonster::Late_Update()
{
	if (m_eCurState == SHIELDWALK || m_eCurState == SHIELDILE)
	{
		if (L"Player_RIGHT" == CObjMgr::Get_Instance()->Get_Player()->Get_FrameKey())
		{
			if (m_pFrameKey == L"Girl_LEFT")
			{
				
				return;

			}
		}
		else if (L"Player_LEFT" == CObjMgr::Get_Instance()->Get_Player()->Get_FrameKey())
		{
			if (m_pFrameKey == L"Girl_RIGHT")
			{

			}
				return;
		}
	}
	
	
	if (CObjMgr::Get_Instance()->Collision_Monster_Weapon(this, &m_bAttack))
	{
		if (m_eCurState == SHIELDWALK || m_eCurState == SHIELDILE)
		{
			if (L"Player_RIGHT" == CObjMgr::Get_Instance()->Get_Player()->Get_FrameKey())
			{
				if (m_pFrameKey == L"Girl_LEFT")
				{
					
			//		CSoundMgr::Get_Instance()->PlaySound(L"sndNoDamage.wav", CSoundMgr::MONSTER);
					return;

				}
			}
			else if (L"Player_LEFT" == CObjMgr::Get_Instance()->Get_Player()->Get_FrameKey())
			{
				if (m_pFrameKey == L"Girl_RIGHT")
				{
				
			//		CSoundMgr::Get_Instance()->PlaySound(L"sndNoDamage.wav", CSoundMgr::MONSTER);
					return;

				}
			}
		}
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::MONSTER);
		CSoundMgr::Get_Instance()->PlaySound(L"sndImpDeathAlt.wav", CSoundMgr::MONSTER);
		
		m_eCurState = SHIELDHURT;
		m_tFrame.iFrameStart = 0;
		m_dwCollision = GetTickCount();

	}
}

void CShieldMonster::Render(HDC _DC)
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
	if (!lstrcmp(L"Girl_LEFT", m_pFrameKey))
	{
		HDC hMemDC2 = CBmpMgr::Get_Instance()->Find_Bmp(L"Girl_RIGHT");
		HDC hStretch = CBmpMgr::Get_Instance()->Find_Bmp(L"StretchGirl");
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


void CShieldMonster::Release()
{

}

void CShieldMonster::Scene_Change()
{
	if (m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed < GetTickCount())
	{
		++m_tFrame.iFrameStart;
		m_tFrame.dwFrameTime = GetTickCount();
		if (SHIELDTHROW == m_eCurState && 3 == m_tFrame.iFrameStart)
		{
			CObj* pObj = CAbstractFactory<CSword>::Create(m_tInfo.fX, m_tInfo.fY + 10.f);
			CObjMgr::Get_Instance()->Add_Object(OBJID::MONSTER_WEAPON, pObj);
			pObj->Set_FrameKey(m_pFrameKey);
		}
		
		if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
		{
			if (SHIELDILE == m_eCurState)
				m_eCurState = SHIELDWALK;
			else
			{
				m_eCurState = SHIELDILE;
				m_tFrame.iFrameStart = 0;
			}
		}
	}
}

void CShieldMonster::Frame_Move()
{
	if (m_eCurState != m_ePreState)
	{
		switch (m_eCurState)
		{
		case SHIELDILE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iPosX = 2;
			m_tFrame.iPosY = 445;
			m_tFrame.iSizeX = 32;
			m_tFrame.iSizeY = 32;
			m_tFrame.dwFrameSpeed = 500;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case SHIELDWALK:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 4;
			m_tFrame.iPosX = 2;
			m_tFrame.iPosY = 551;
			m_tFrame.iSizeX = 32;
			m_tFrame.iSizeY = 32;
			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case SHIELDTHROW:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 7;
			m_tFrame.iPosX = 2;
			m_tFrame.iPosY = 604;
			m_tFrame.iSizeX = 32;
			m_tFrame.iSizeY = 32;
			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case SHIELDHURT:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iPosX = 2;
			m_tFrame.iPosY = 498;
			m_tFrame.iSizeX = 32;
			m_tFrame.iSizeY = 32;
			m_tFrame.dwFrameSpeed = 600;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		}

		m_ePreState = m_eCurState;
	}
}
