#include "stdafx.h"
#include "Stage.h"
#include "BmpMgr.h"
#include "Player.h"
#include "ObjMgr.h"
#include "ScrollMgr.h"
#include "LineMgr.h"
#include "TileMgr.h"
#include "CollisionMgr.h"
#include "Leaf.h"
#include "KeyMgr.h"
#include "RectMgr.h"
#include "ObjMgr.h"
#include "SceneMgr.h"
#include "SoundMgr.h"
#include "PlayerUI.h"

CStage::CStage()
	:m_bLineOn(false), m_eChannel(STAGE::STAGE1_1), m_iChannel(1),  m_ibackX(0)
	, m_iPrevious_ScrollX(0)
{
}


CStage::~CStage()
{
	Release();
}

void CStage::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/BackGround/Back1.bmp", L"Back1");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/BackGround/Back2.bmp", L"Back2");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/Map/Tile1.bmp", L"Tile1");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/Map/StoneTile.bmp", L"StoneTile");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/Map/BounceBall.bmp", L"BounceBall");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/Map/ThinTile.bmp", L"ThinTile");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/Map/Ladder.bmp", L"Ladder");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/Map/HardTile.bmp", L"HardTile");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/Map/MovingTile.bmp", L"MovingTile");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/Map/Elevator.bmp", L"Elevator");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/Map/City/CityLadder.bmp", L"CityLadder");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/Map/City/CityTile.bmp", L"CityTile");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/Map/City/CityThinTile.bmp", L"CityThinTile");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/Map/Thorn.bmp", L"Thorn");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/Map/Orb240.bmp", L"Orb");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/Map/StoneTile.bmp", L"StoneTile");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/Map/BounceBall.bmp", L"BounceBall");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/Map/ThinTile.bmp", L"ThinTile");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/Player/Death.bmp", L"DeathBack");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/SavePoint/SavePoint1.bmp", L"SavePoint1");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/Item/Flower.bmp", L"Flower");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/Player/HpUI.bmp", L"HpUI");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/Player/EmptySlot.bmp", L"EmptySlot");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/Player/ItemSlot.bmp", L"ItemSlot");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/Player/MoneyUI.bmp", L"MoneyUI");

	CObjMgr::Get_Instance()->Set_SlotNum(m_iSlotNum);

	if (CObjMgr::Get_Instance()->Get_Empty())
	{
		CObj*	pObj = CAbstractFactory<CPlayer>::Create();
		CObjMgr::Get_Instance()->Add_Object(OBJID::PLAYER, pObj);

		pObj = CAbstractFactory<CPlayerUI>::Create(20+50, 20+50, L"EmptySlot");
		CObjMgr::Get_Instance()->Add_Object(OBJID::PLAYER_UI, pObj);
		pObj = CAbstractFactory<CPlayerUI>::Create(130+200, 20+25, L"HpUI");
		CObjMgr::Get_Instance()->Add_Object(OBJID::PLAYER_UI, pObj);
		pObj = CAbstractFactory<CPlayerUI>::Create(130+20, 80+20, L"MoneyUI");
		CObjMgr::Get_Instance()->Add_Object(OBJID::PLAYER_UI, pObj);
		pObj = CAbstractFactory<CPlayerUI>::Create(400, 550, L"BossHealth");
		CObjMgr::Get_Instance()->Add_Object(OBJID::PLAYER_UI, pObj);
	}
	//m_eChannel = STAGE::STAGE1_1;
	CTileMgr::Get_Instance()->Load_Tile(m_eChannel);
	CTileMgr::Get_Instance()->Initialize();
	CRectMgr::Get_Instance()->Load_Rect(m_eChannel);
	CLineMgr::Get_Instance()->Load_Line(m_eChannel);
	CObjMgr::Get_Instance()->Load_Obj(m_eChannel);
	CSoundMgr::Get_Instance()->StopAll();
	CSoundMgr::Get_Instance()->PlayBGM(L"Stage.wav");

	
}

void CStage::Update()
{
	Key_Check();
	CObjMgr::Get_Instance()->Set_Line(m_bLineOn);
	CTileMgr::Get_Instance()->Set_LineOn(m_bLineOn);
	CLineMgr::Get_Instance()->Set_Line(m_bLineOn);
	CRectMgr::Get_Instance()->Set_Line_On(m_bLineOn);

	CObjMgr::Get_Instance()->Update();
}

void CStage::Late_Update()
{
	CObjMgr::Get_Instance()->Late_Update();
}

void CStage::Render(HDC _DC)
{
	
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();
	TCHAR* backFrameKey = nullptr;
	if (CObjMgr::Get_Instance()->Get_Channel() > 12)
		backFrameKey = L"Back2";
	else
		backFrameKey = L"Back1";
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(backFrameKey);
	HDC hMemDeath = CBmpMgr::Get_Instance()->Find_Bmp(L"DeathBack");
	if (CObjMgr::Get_Instance()->Get_Player()->Get_Hp() <= 0)
	{
		BitBlt(_DC, 0, 0, WINCX, WINCY, hMemDeath, 0, 0, SRCCOPY);
		CObjMgr::Get_Instance()->Get_Player()->Render(_DC);

	}
	else
	{
	
		BitBlt(_DC, 0, 0, WINCX, WINCY, hMemDC, -iScrollX, 0, SRCCOPY);
		if (iScrollX < -1000)
			BitBlt(_DC, 1800 + iScrollX, 0, 900-WINCX - iScrollX, WINCY, hMemDC, 0, 0, SRCCOPY);
		if (iScrollX < -100)
			BitBlt(_DC, 900 + iScrollX, 0, WINCX - iScrollX, WINCY, hMemDC, 0, 0, SRCCOPY);
		
		//TCHAR	szBuff[32] = L"";

		//swprintf_s(szBuff, L"PlayerHp: %d", CObjMgr::Get_Instance()->Get_Player()->Get_Hp());
		//TextOut(_DC, 50, 400, szBuff, lstrlen(szBuff));

		CTileMgr::Get_Instance()->Render(_DC);
		CObjMgr::Get_Instance()->Render(_DC);

		CRectMgr::Get_Instance()->Render(_DC);
		CLineMgr::Get_Instance()->Render(_DC);
	}
	
}

void CStage::Release()
{
	CObjMgr::Get_Instance()->Delete_ID(OBJID::MONSTER);
	CObjMgr::Get_Instance()->Delete_ID(OBJID::PLAER_WEAPON);
	CObjMgr::Get_Instance()->Delete_ID(OBJID::MONSTER_WEAPON);


}

void CStage::Key_Check()
{
	if (CKeyMgr::Get_Instance()->Key_Down(VK_F1))
	{
		if (m_bLineOn)
			m_bLineOn = false;
		else
			m_bLineOn = true;
	}
	if (CKeyMgr::Get_Instance()->Key_Down(VK_SHIFT))
	{

	}
}
