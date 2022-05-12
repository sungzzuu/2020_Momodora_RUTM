#include "stdafx.h"
#include "Fennel.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "LineMgr.h"
#include "ObjMgr.h"
#include "SoundMgr.h"
#include "BuffSword.h"
#include "Impact.h"
#include "PlayerUI.h"

CFennel::CFennel()
{

}


CFennel::~CFennel()
{
}

void CFennel::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/Boss/Boss.bmp", L"Boss_LEFT");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/Boss/BossStretch.bmp", L"BossStretch");


	m_tInfo.fX = 600.f;
	m_tInfo.fY = 400.f;
	m_tInfo.iCX = 236;
	m_tInfo.iCY = 156;
	m_tInfo.iCollisionCX = 100;
	m_tInfo.iCollisionCY = 80;
	m_eCurState = INTRO1;
	m_ePreState = INTRO1;
	m_iPattern = 0;
	m_pFrameKey = L"Boss_LEFT";



	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 18;
	m_tFrame.iPosX = 2;
	m_tFrame.iPosY = 1208;
	m_tFrame.iSizeX = 118;
	m_tFrame.iSizeY = 78;
	m_tFrame.dwFrameSpeed = 50;
	m_tFrame.dwFrameTime = GetTickCount();


	m_fSpeed = 1.f;

	float fY = 0.f;
	bool _bCheck = false;
	if (CLineMgr::Get_Instance()->Collision_Surface(m_tInfo.fX, m_tInfo.fY + (m_tInfo.iCY >> 1), &fY, &_bCheck))
		m_tInfo.fY = fY - (60);


	//게이지
	m_tGauge.iHp = 5200;
	m_tGauge.iGold = 100;
	m_tGauge.iMaxHp = 5200;
	m_tGauge.iAtt = 100;

	m_eGroup = GROUPID::OBJECT;

	m_dwTurnTime = GetTickCount();

	CSoundMgr::Get_Instance()->StopAll();
	CSoundMgr::Get_Instance()->PlayBGM(L"Boss.wav");
	m_pName = L"Fennel";
}

int CFennel::Update()
{
	Update_Rect();
	float fY = 0.f;
	bool _bCheck = false;

	if (m_dwCollision + 100 < GetTickCount())
		m_bAttack = false;
	Frame_Move();
	Update_Rect();
	if (ATTACK == m_eCurState)
	{
		if (m_pFrameKey == L"Boss_RIGHT")
			m_tInfo.fX += m_fSpeed;
		else
			m_tInfo.fX -= m_fSpeed;
	}
	else if (DASH == m_eCurState)
	{
		if (m_pFrameKey == L"Boss_RIGHT")
			m_tInfo.fX += m_fSpeed*8;
		else
			m_tInfo.fX -= m_fSpeed*8;
	}
	else if (BACKFLIP == m_eCurState)
	{
		if (m_pFrameKey == L"Boss_RIGHT")
			m_tInfo.fX -= m_fSpeed * 3;
		else
			m_tInfo.fX += m_fSpeed * 3;
	}
	if (m_dwTurnTime + 400 < GetTickCount())
	{
		if (m_tInfo.fX > CObjMgr::Get_Instance()->Get_Player()->Get_Info().fX)
		{
			m_pFrameKey = L"Boss_LEFT";
		}
		else
		{
			m_pFrameKey = L"Boss_RIGHT";
		}

		m_dwTurnTime = GetTickCount();
	}

	Scene_Change();
	if (m_bDead)
	{
		CSoundMgr::Get_Instance()->StopAll();
		CSoundMgr::Get_Instance()->PlayBGM(L"Clear.wav");
		return OBJ_DEAD;

	}
	if (m_tGauge.iHp <= 0 && m_eCurState != DEATH)
	{
		m_eCurState = DEATH;
	}
	return OBJ_NOEVENT;
}

void CFennel::Late_Update()
{
	//플레이어와 충돌체크!
	if (CObjMgr::Get_Instance()->Collision_Monster_Weapon(this, &m_bAttack))
		m_dwCollision = GetTickCount();

}

void CFennel::Render(HDC _DC)
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

void CFennel::Release()
{
}

void CFennel::Scene_Change()
{

	if (m_eCurState != m_ePreState)
	{
		switch (m_eCurState)
		{
	
		case INTRO1:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 18;
			m_tFrame.iPosX = 2;
			m_tFrame.iPosY = 1208;
			m_tFrame.iSizeX = 118;
			m_tFrame.iSizeY = 78;
			m_tFrame.dwFrameSpeed = 50;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case INTRO2:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 7;
			m_tFrame.iPosX = 2;
			m_tFrame.iPosY = 1307;
			m_tFrame.iSizeX = 118;
			m_tFrame.iSizeY = 78;
			m_tFrame.dwFrameSpeed = 50;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case INTRO3:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 13;
			m_tFrame.iPosX = 2;
			m_tFrame.iPosY = 1406;
			m_tFrame.iSizeX = 118;
			m_tFrame.iSizeY = 78;
			m_tFrame.dwFrameSpeed = 50;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case INTRO4:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 2;
			m_tFrame.iPosX = 2;
			m_tFrame.iPosY = 1505;
			m_tFrame.iSizeX = 118;
			m_tFrame.iSizeY = 78;
			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case ATTACK:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 22;
			m_tFrame.iPosX = 2;
			m_tFrame.iPosY = 220;
			m_tFrame.iSizeX = 118;
			m_tFrame.iSizeY = 78;
			m_tFrame.dwFrameSpeed = 50;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case PLUNGE:
			Add_Impact();
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 12;
			m_tFrame.iPosX = 2;
			m_tFrame.iPosY = 517;
			m_tFrame.iSizeX = 118;
			m_tFrame.iSizeY = 78;
			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case JUMP:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 22;
			m_tFrame.iPosX = 2;
			m_tFrame.iPosY = 220;
			m_tFrame.iSizeX = 118;
			m_tFrame.iSizeY = 78;
			m_tFrame.dwFrameSpeed = 50;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case BACKFLIP:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 12;
			m_tFrame.iPosX = 2;
			m_tFrame.iPosY = 715;
			m_tFrame.iSizeX = 118;
			m_tFrame.iSizeY = 78;
			m_tFrame.dwFrameSpeed = 50;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case DASH:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd =0;
			m_tFrame.iPosX = 2;
			m_tFrame.iPosY = 814;
			m_tFrame.iSizeX = 118;
			m_tFrame.iSizeY = 78;
			m_tFrame.dwFrameSpeed = 300;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case BUFF:
			Add_BuffSword();
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 14;
			m_tFrame.iPosX = 2;
			m_tFrame.iPosY = 913;
			m_tFrame.iSizeX = 118;
			m_tFrame.iSizeY = 78;
			m_tFrame.dwFrameSpeed =100;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case DEATH:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 13;
			m_tFrame.iPosX = 2;
			m_tFrame.iPosY = 1111;
			m_tFrame.iSizeX = 118;
			m_tFrame.iSizeY = 78;
			m_tFrame.dwFrameSpeed = 50;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		}

		m_ePreState = m_eCurState;
	}
}

void CFennel::Frame_Move()
{
	srand(unsigned(time(NULL)));

	if (m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed < GetTickCount())
	{
		++m_tFrame.iFrameStart;
		//if ( == m_eCurState && 5 == m_tFrame.iFrameStart)
		//{
		//	CObj* pObj = CAbstractFactory<CSword>::Create(m_tInfo.fX, m_tInfo.fY + 10.f);
		//	CObjMgr::Get_Instance()->Add_Object(OBJID::MONSTER_WEAPON, pObj);
		//	pObj->Set_FrameKey(m_pFrameKey);
		//}
		m_tFrame.dwFrameTime = GetTickCount();
		if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
		{

			if (DEATH == m_eCurState)
			{
				m_bDead = true;
			}
			else if (INTRO1 == m_eCurState)
			{
				m_eCurState = INTRO2;
			
			}
			else if (INTRO2 == m_eCurState)
			{
				m_eCurState = INTRO3;
			}
			else if (INTRO3 == m_eCurState)
			{
				m_eCurState = INTRO4;
			}
			else if (DASH == m_eCurState)
			{
				m_iPattern = rand() % 4;

				if (0 == m_iPattern)
					m_eCurState = DASH;
				else if (1 == m_iPattern)
					m_eCurState = BACKFLIP;
				else if (2 == m_iPattern)
					m_eCurState = BUFF;
				else
					m_eCurState = ATTACK;
			}
			else if (BACKFLIP == m_eCurState)
			{
				m_eCurState = PLUNGE;
			}
			else
				m_eCurState = DASH;

			m_tFrame.iFrameStart = 0;

		}
	}
}

void CFennel::Add_BuffSword()
{
	CObj* pObj = CAbstractFactory<CBuffSword>::Create(m_tInfo.fX, m_tRect.bottom- 50,m_pFrameKey);
	CObjMgr::Get_Instance()->Add_Object(OBJID::MONSTER_WEAPON, pObj);
}

void CFennel::Add_Impact()
{
	float _fX = CObjMgr::Get_Instance()->Get_Player()->Get_Info().fX;
	float _fY = CObjMgr::Get_Instance()->Get_Player()->Get_Info().fY;

	CObj* pObj = CAbstractFactory<CImpact>::Create();
	CObjMgr::Get_Instance()->Add_Object(OBJID::MONSTER_WEAPON, pObj);
}
