#include "stdafx.h"
#include "MapTool.h"
#include "ScrollMgr.h"
#include "BmpMgr.h"
#include "TileMgr.h"
#include "KeyMgr.h"
#include "SceneMgr.h"
#include "LineMgr.h"
#include "RectMgr.h"
#include "ObjMgr.h"
#include "SoundMgr.h"

CMapTool::CMapTool()
	:m_dwTime(GetTickCount()), m_bTileBar_On(false), m_bLine_On(false), m_tNowTile(nullptr),
	m_eNowChannel(STAGE::STAGE1_1), m_iChannel(1), m_bLineBar_On(false), m_bRect_On(false)
	,m_bCeiling(false),m_bObjBar_On(false), m_tNowObj(nullptr)
{
}


CMapTool::~CMapTool()
{
	Release();
}

void CMapTool::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/MapToolBack.bmp", L"MTBack");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/MapToolBack.bmp", L"LineBuffer");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/MapTool/TileBar.bmp", L"TileBar");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/MapTool/ObjBar.bmp", L"ObjBar");

	Make_TileBar();
	Make_ObjBar();
	CTileMgr::Get_Instance()->Initialize();
	CSoundMgr::Get_Instance()->StopAll();
	CSoundMgr::Get_Instance()->PlayBGM(L"FileSelect.wav");
}

void CMapTool::Update()
{
	Key_Check();


}

void CMapTool::Late_Update()
{
}

void CMapTool::Render(HDC _DC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"MTBack");
	HDC hLineBuffer = CBmpMgr::Get_Instance()->Find_Bmp(L"LineBuffer");

	HDC hTileBar = CBmpMgr::Get_Instance()->Find_Bmp(L"TileBar");
	HDC hObjBar = CBmpMgr::Get_Instance()->Find_Bmp(L"ObjBar");
	int iScrollX = (int)(CScrollMgr::Get_Instance()->Get_ScrollX());
	int iScrollY = (int)(CScrollMgr::Get_Instance()->Get_ScrollY());

	

	// 맵툴 라인

	if (m_bLine_On)
	{
		for (int i = 0; i < 100; ++i)
		{

			MoveToEx(hMemDC, iScrollX, i*TILECY + iScrollY, nullptr);
			LineTo(hMemDC, 100 * TILECY + iScrollX, i*TILECY + iScrollY);

		}

		for (int i = 0; i < 100; ++i)
		{
			MoveToEx(hMemDC, i*TILECX + iScrollX, iScrollY, nullptr);
			LineTo(hMemDC, i*TILECX + iScrollX, 100 * TILECX + iScrollY);

		}
	}
	BitBlt(_DC, 0, 0
		, 650, 2000
		, hMemDC, 0, 0, SRCCOPY);

	BitBlt(hMemDC, 0, 0
		, 650, 2000
		, hLineBuffer, 0, 0, SRCCOPY);


	if (m_bObjBar_On)
	{
		for (auto& pObj : m_listObj)
		{
			HDC hObjDC = CBmpMgr::Get_Instance()->Find_Bmp(pObj->tFrameKey);
			GdiTransparentBlt(hObjBar
				, pObj->tRect.left, pObj->tRect.top
				, pObj->tRect.right - pObj->tRect.left, pObj->tRect.bottom - pObj->tRect.top
				, hObjDC
				, pObj->PosX, pObj->PosY
				, pObj->iSize_X, pObj->iSize_Y
				, RGB(240, 240, 240));
			Pick_ObjBar(hObjBar);
		}
		BitBlt(_DC, 650, 0
			, 800, 600
			, hObjBar, 0, 0, SRCCOPY);
		
	}
	else if (m_bTileBar_On)
	{
		for (auto& pTile : m_listTile)
		{
			HDC hTileDC = CBmpMgr::Get_Instance()->Find_Bmp(pTile->tFrameKey);
			if (pTile->tFrameKey == L"Ladder")
			{
				GdiTransparentBlt(hTileBar
					, pTile->tRect.left, pTile->tRect.top
					, pTile->tRect.right - pTile->tRect.left, pTile->tRect.bottom - pTile->tRect.top
					, hTileDC
					, pTile->iDrawID_X * 16, pTile->iDrawID_Y * 16
					, 16, 16
					, RGB(255, 255, 255));
			}
			else
			{
				GdiTransparentBlt(hTileBar
					, pTile->tRect.left, pTile->tRect.top
					, pTile->tRect.right - pTile->tRect.left, pTile->tRect.bottom - pTile->tRect.top
					, hTileDC
					, pTile->iDrawID_X*TILECX, pTile->iDrawID_Y *TILECY
					, TILECX, TILECY
					, RGB(255, 255, 255));

			}
			Pick_TileBar(hTileBar);

		}
		BitBlt(_DC, 650, 0
			, 800, 600
			, hTileBar, 0, 0, SRCCOPY);

		
	}
	CTileMgr::Get_Instance()->Render(_DC);
	CObjMgr::Get_Instance()->Render(_DC);
	CRectMgr::Get_Instance()->Render(_DC);
	CLineMgr::Get_Instance()->Render(_DC);

	TCHAR szBuff[20] = {};
	swprintf_s(szBuff, L"CHANNEL: %d", m_iChannel);
	TextOut(_DC, 50, 250, szBuff, lstrlen(szBuff));
	swprintf_s(szBuff, L"LineNum: %d", CLineMgr::Get_Instance()->Get_LineNum(m_eNowChannel));
	TextOut(_DC, 50, 300, szBuff, lstrlen(szBuff));
	swprintf_s(szBuff, L"LineOn: %d", m_bLineBar_On);
	TextOut(_DC, 50, 350, szBuff, lstrlen(szBuff));
	swprintf_s(szBuff, L"RectNum: %d", CRectMgr::Get_Instance()->Get_RectNum(m_eNowChannel));
	TextOut(_DC, 50, 400, szBuff, lstrlen(szBuff));
	swprintf_s(szBuff, L"RectOn: %d", m_bRect_On);
	TextOut(_DC, 50, 450, szBuff, lstrlen(szBuff));
	swprintf_s(szBuff, L"Monster: %d", CObjMgr::Get_Instance()->Get_ObjSize(OBJID::MONSTER));
	TextOut(_DC, 50, 500, szBuff, lstrlen(szBuff));
}

void CMapTool::Release()
{
	for_each(m_listTile.begin(), m_listTile.end(), Safe_Delete<TILEINFO*>);
	m_listTile.clear();
	
	CTileMgr::Get_Instance()->Release();
	CLineMgr::Get_Instance()->Release();
	CRectMgr::Get_Instance()->Release();
	CObjMgr::Get_Instance()->Release();

	for_each(m_listObj.begin(), m_listObj.end(), Safe_Delete<OBJINFO*>);
	m_listTile.clear();
}

void CMapTool::Make_TileBar()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/Map/Tile1.bmp", L"Tile1");
	POINT	tStart = { 0, 0 };

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			TILEINFO* tTile = new TILEINFO;
			tTile->iDrawID_X = i;
			tTile->iDrawID_Y = j;
			if (tTile->iDrawID_Y == 0 && tTile->iDrawID_X == 1)
				tTile->eType = TILE::SURFACE;
			else if (tTile->iDrawID_Y == 0 && (tTile->iDrawID_X == 0 || tTile->iDrawID_X == 2))
				tTile->eType = TILE::HARD;
			else
				tTile->eType = TILE::WALL;
			tTile->tRect.left = TILESMALL*i;
			tTile->tRect.top = TILESMALL*j;
			tTile->tRect.right = TILESMALL*i + TILESMALL;
			tTile->tRect.bottom = TILESMALL*j + TILESMALL;
			tTile->tFrameKey = L"Tile1";
			tTile->iOption = 1;
			m_listTile.emplace_back(tTile);
		}
	}
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/Map/StoneTile.bmp", L"StoneTile");
	tStart = { 75, 0 };
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			TILEINFO* tTile = new TILEINFO;
			tTile->iDrawID_X = i;
			tTile->iDrawID_Y = j;
			if (tTile->iDrawID_Y == 0 && tTile->iDrawID_X == 1)
				tTile->eType = TILE::SURFACE;
			else if (tTile->iDrawID_Y == 0 && (tTile->iDrawID_X == 0 || tTile->iDrawID_X == 2))
				tTile->eType = TILE::HARD;
			else
				tTile->eType = TILE::WALL;
			tTile->tRect.left = tStart.x + TILESMALL*i;
			tTile->tRect.top = tStart.y + TILESMALL*j;
			tTile->tRect.right = tStart.x + TILESMALL*i + TILESMALL;
			tTile->tRect.bottom = tStart.y + TILESMALL*j + TILESMALL;
			tTile->tFrameKey = L"StoneTile";
			tTile->iOption = 2;
			m_listTile.emplace_back(tTile);
		}
	}
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/Map/Ladder.bmp", L"Ladder");
	tStart = { 50, 75 };

	for (int i = 0; i < 3; i++)
	{
		TILEINFO* tTile = new TILEINFO;
		
		
		tTile->iDrawID_X = i;
		tTile->iDrawID_Y = 0;
		tTile->eType = TILE::LADDER;
		tTile->tRect.left = tStart.x + TILESMALL*i;
		tTile->tRect.top = tStart.y;
		tTile->tRect.right = tStart.x + TILESMALL*i + TILESMALL;
		tTile->tRect.bottom = tStart.y + TILESMALL;
		tTile->tFrameKey = L"Ladder";
		tTile->iOption = 5;
		m_listTile.emplace_back(tTile);
	}
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/Map/BounceBall.bmp", L"BounceBall");
	tStart = { 0, 75 };
	TILEINFO* tTile = new TILEINFO;
	tTile->eType = TILE::BOUNCE;
	tTile->iDrawID_X = 0;
	tTile->iDrawID_Y = 0;
	tTile->tRect.left = tStart.x;
	tTile->tRect.top = tStart.y;
	tTile->tRect.right = tStart.x + TILESMALL;
	tTile->tRect.bottom = tStart.y + TILESMALL;
	tTile->tFrameKey = L"BounceBall";
	tTile->iOption = 3;
	m_listTile.emplace_back(tTile);

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/Map/ThinTile.bmp", L"ThinTile");
	tStart = { 25, 75 };

	tTile = new TILEINFO;
	tTile->eType = TILE::THIN;
	tTile->iDrawID_X = 0;
	tTile->iDrawID_Y = 0;
	tTile->tRect.left = tStart.x;
	tTile->tRect.top = tStart.y;
	tTile->tRect.right = tStart.x + TILESMALL;
	tTile->tRect.bottom = tStart.y + TILESMALL;
	tTile->tFrameKey = L"ThinTile";
	tTile->iOption = 4;

	m_listTile.emplace_back(tTile);

	
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/Map/HardTile.bmp", L"HardTile");
	tStart = { 0, 100 };

	tTile = new TILEINFO;
	tTile->eType = TILE::HARD;
	tTile->iDrawID_X = 0;
	tTile->iDrawID_Y = 0;
	tTile->tRect.left = tStart.x;
	tTile->tRect.top = tStart.y;
	tTile->tRect.right = tStart.x + TILESMALL * 2;
	tTile->tRect.bottom = tStart.y + TILESMALL;
	tTile->tFrameKey = L"HardTile";
	tTile->iOption = 6;

	m_listTile.emplace_back(tTile);


	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/Map/MovingTile.bmp", L"MovingTile");
	tStart = { 0, 225 };

	tTile = new TILEINFO;
	tTile->eType = TILE::MOVING;
	tTile->iDrawID_X = 0;
	tTile->iDrawID_Y = 0;
	tTile->tRect.left = tStart.x;
	tTile->tRect.top = tStart.y;
	tTile->tRect.right = tStart.x + TILESMALL*3;
	tTile->tRect.bottom = tStart.y + TILESMALL*3;
	tTile->tFrameKey = L"MovingTile";
	tTile->iOption = 7;
	m_listTile.emplace_back(tTile);

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/Map/Elevator.bmp", L"Elevator");
	tStart = { 0, 125 };

	tTile = new TILEINFO;
	tTile->eType = TILE::SURFACE;
	tTile->iDrawID_X = 0;
	tTile->iDrawID_Y = 0;
	tTile->tRect.left = tStart.x;
	tTile->tRect.top = tStart.y;
	tTile->tRect.right = tStart.x + TILESMALL * 3;
	tTile->tRect.bottom = tStart.y + TILESMALL;
	tTile->tFrameKey = L"Elevator";
	tTile->iOption = 8;
	m_listTile.emplace_back(tTile);

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/Map/City/CityLadder.bmp", L"CityLadder");
	tStart = { 0, 150 };

	for (int i = 0; i < 3; i++)
	{
		tTile = new TILEINFO;
		tTile->iDrawID_X = i;
		tTile->iDrawID_Y = 0;
		tTile->eType = TILE::LADDER;
		tTile->tRect.left = tStart.x + TILESMALL*i;
		tTile->tRect.top = tStart.y;
		tTile->tRect.right = tStart.x + TILESMALL*i + TILESMALL;
		tTile->tRect.bottom = tStart.y + TILESMALL;
		tTile->tFrameKey = L"CityLadder";
		tTile->iOption = 9;
		m_listTile.emplace_back(tTile);
	}

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/Map/City/CityTile.bmp", L"CityTile");
	tStart = { 75, 150 };

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			tTile = new TILEINFO;
			tTile->iDrawID_X = i;
			tTile->iDrawID_Y = j;
			if (tTile->iDrawID_Y == 0 && tTile->iDrawID_X == 1)
				tTile->eType = TILE::SURFACE;
			else if (tTile->iDrawID_Y == 0 && (tTile->iDrawID_X == 0 || tTile->iDrawID_X == 2))
				tTile->eType = TILE::HARD;
			else
				tTile->eType = TILE::WALL;
			tTile->tRect.left = tStart.x+ TILESMALL*i;
			tTile->tRect.top = tStart.y + TILESMALL*j;
			tTile->tRect.right = tStart.x + TILESMALL*i + TILESMALL;
			tTile->tRect.bottom = tStart.y + TILESMALL*j + TILESMALL;
			tTile->tFrameKey = L"CityTile";
			tTile->iOption = 10;
			m_listTile.emplace_back(tTile);
		}
	}

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/Map/City/CityThinTile.bmp", L"CityThinTile");
	tStart = { 0, 200 };

	tTile = new TILEINFO;
	tTile->eType = TILE::THIN;
	tTile->iDrawID_X = 0;
	tTile->iDrawID_Y = 0;
	tTile->tRect.left = tStart.x;
	tTile->tRect.top = tStart.y;
	tTile->tRect.right = tStart.x + TILESMALL;
	tTile->tRect.bottom = tStart.y + TILESMALL;
	tTile->tFrameKey = L"CityThinTile";
	tTile->iOption = 11;

	m_listTile.emplace_back(tTile);
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/Map/Grass.bmp", L"Grass");
	tStart = { 0, 300 };
	for (int i = 0; i < 4; i++)
	{
		tTile = new TILEINFO;
		tTile->iDrawID_X = i;
		tTile->iDrawID_Y = 0;
		tTile->eType = TILE::OBJECT;
		tTile->tRect.left = tStart.x + TILESMALL*i;
		tTile->tRect.top = tStart.y;
		tTile->tRect.right = tStart.x + TILESMALL*i + TILESMALL;
		tTile->tRect.bottom = tStart.y + TILESMALL;
		tTile->tFrameKey = L"Grass";
		tTile->iOption = 12;
		m_listTile.emplace_back(tTile);
	}

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/Map/Orb240.bmp", L"Orb");
	tStart = { 0, 400 };
	for (int i = 0; i < 4; i++)
	{
		tTile = new TILEINFO;
		tTile->iDrawID_X = i;
		tTile->iDrawID_Y = 0;
		tTile->eType = TILE::ORB;
		tTile->tRect.left = tStart.x + TILESMALL*i;
		tTile->tRect.top = tStart.y;
		tTile->tRect.right = tStart.x + TILESMALL*i + TILESMALL;
		tTile->tRect.bottom = tStart.y + TILESMALL;
		tTile->tFrameKey = L"Orb";
		tTile->iOption = 13;
		m_listTile.emplace_back(tTile);
	}

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/Map/Thorn.bmp", L"Thorn");
	tStart = { 0, 500 };
	tTile = new TILEINFO;
	tTile->eType = TILE::THORN;
	tTile->iDrawID_X = 0;
	tTile->iDrawID_Y = 0;
	tTile->tRect.left = tStart.x;
	tTile->tRect.top = tStart.y;
	tTile->tRect.right = tStart.x + TILESMALL * 2;
	tTile->tRect.bottom = tStart.y + TILESMALL;
	tTile->tFrameKey = L"Thorn";
	tTile->iOption = 14;
	m_listTile.emplace_back(tTile);
	// DC의 위치를 알아야 마우스 충돌 가능
	for (auto& pTile : m_listTile)
	{
		pTile->tDCRect.left = pTile->tRect.left + 650;
		pTile->tDCRect.top = pTile->tRect.top;
		pTile->tDCRect.right = pTile->tRect.right + 650;
		pTile->tDCRect.bottom = pTile->tRect.bottom;
	}
	
}

void CMapTool::Make_ObjBar()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/Monster/GirlMonster/Girl_RIGHT.bmp", L"Girl_RIGHT");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/Monster/MonkeyMonster/Monkey_RIGHT.bmp", L"Monkey_RIGHT");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/Monster/BigPlant_RIGHT.bmp", L"BigPlant_RIGHT");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/Monster/Chest.bmp", L"Chest_LEFT");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/SavePoint/SavePoint1.bmp", L"SavePoint1");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/SavePoint/SavePoint2.bmp", L"SavePoint2");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/Item/Flower.bmp", L"Flower");


	//Girl 3개
	
	POINT	tStart = { 0, 0 };
	OBJINFO* pObj = new OBJINFO;
	pObj->iType = 0;
	pObj->iOption =1;
	pObj->PosX = 70;
	pObj->PosY = 286;
	pObj->iSize_X = 32;
	pObj->iSize_Y = 32;
	pObj->tRect.left = tStart.x;
	pObj->tRect.top = tStart.y;
	pObj->tRect.right = tStart.x + OBJSMALL;
	pObj->tRect.bottom = tStart.y + OBJSMALL;
	pObj->tFrameKey = L"Girl_RIGHT";
	m_listObj.emplace_back(pObj);

	tStart = { 50, 0 };
	pObj = new OBJINFO;
	pObj->iOption = 2;
	pObj->PosX = 70;
	pObj->PosY = 392;
	pObj->iSize_X = 32;
	pObj->iSize_Y = 32;
	pObj->iType = 0;

	pObj->tRect.left = tStart.x;
	pObj->tRect.top = tStart.y;
	pObj->tRect.right = tStart.x + OBJSMALL;
	pObj->tRect.bottom = tStart.y + OBJSMALL;
	pObj->tFrameKey = L"Girl_RIGHT";
	m_listObj.emplace_back(pObj);

	tStart = { 100, 0 };
	pObj = new OBJINFO;
	pObj->iOption = 3;
	pObj->iType = 0;

	pObj->PosX = 70;
	pObj->PosY = 551;
	pObj->iSize_X = 32;
	pObj->iSize_Y = 32;
	pObj->tRect.left = tStart.x;
	pObj->tRect.top = tStart.y;
	pObj->tRect.right = tStart.x + OBJSMALL;
	pObj->tRect.bottom = tStart.y + OBJSMALL;
	pObj->tFrameKey = L"Girl_RIGHT";
	m_listObj.emplace_back(pObj);

	//MonKey
	tStart = { 0, 50 };
	pObj = new OBJINFO;
	pObj->iOption = 4;
	pObj->iType = 0;

	pObj->PosX = 2;
	pObj->PosY = 17;
	pObj->iSize_X = 44;
	pObj->iSize_Y = 32;
	pObj->tRect.left = tStart.x;
	pObj->tRect.top = tStart.y;
	pObj->tRect.right = tStart.x + OBJSMALL;
	pObj->tRect.bottom = tStart.y + OBJSMALL;
	pObj->tFrameKey = L"Monkey_RIGHT";
	m_listObj.emplace_back(pObj);


	//BigPlant
	tStart = { 50, 50 };
	pObj = new OBJINFO;
	pObj->iOption = 5;
	pObj->iType = 0;

	pObj->PosX = 2;
	pObj->PosY = 17;
	pObj->iSize_X = 120;
	pObj->iSize_Y = 100;
	pObj->tRect.left = tStart.x;
	pObj->tRect.top = tStart.y;
	pObj->tRect.right = tStart.x + OBJSMALL;
	pObj->tRect.bottom = tStart.y + OBJSMALL;
	pObj->tFrameKey = L"BigPlant_RIGHT";
	m_listObj.emplace_back(pObj);
	//Chest
	tStart = { 100, 50 };
	pObj = new OBJINFO;
	pObj->iOption = 6;
	pObj->iType = 0;

	pObj->PosX = 0;
	pObj->PosY = 0;
	pObj->iSize_X = 64;
	pObj->iSize_Y = 64;
	pObj->tRect.left = tStart.x;
	pObj->tRect.top = tStart.y;
	pObj->tRect.right = tStart.x + OBJSMALL;
	pObj->tRect.bottom = tStart.y + OBJSMALL;
	pObj->tFrameKey = L"Chest_LEFT";
	m_listObj.emplace_back(pObj);

	//Save
	tStart = { 0, 100 };
	pObj = new OBJINFO;
	pObj->iOption = 7;
	pObj->iType = 1;

	pObj->PosX = 0;
	pObj->PosY = 0;
	pObj->iSize_X = 128;
	pObj->iSize_Y = 128;
	pObj->tRect.left = tStart.x;
	pObj->tRect.top = tStart.y;
	pObj->tRect.right = tStart.x + OBJSMALL;
	pObj->tRect.bottom = tStart.y + OBJSMALL;
	pObj->tFrameKey = L"SavePoint1";
	m_listObj.emplace_back(pObj);

	//Save2
	tStart = { 50, 100 };
	pObj = new OBJINFO;
	pObj->iOption = 8;
	pObj->iType = 1;

	pObj->PosX = 0;
	pObj->PosY = 0;
	pObj->iSize_X = 128;
	pObj->iSize_Y = 128;
	pObj->tRect.left = tStart.x;
	pObj->tRect.top = tStart.y;
	pObj->tRect.right = tStart.x + OBJSMALL;
	pObj->tRect.bottom = tStart.y + OBJSMALL;
	pObj->tFrameKey = L"SavePoint2";
	m_listObj.emplace_back(pObj);

	//Flower
	tStart = { 100, 100 };
	pObj = new OBJINFO;
	pObj->iOption = 9;
	pObj->iType = 2;

	pObj->PosX = 0;
	pObj->PosY = 0;
	pObj->iSize_X = 72;
	pObj->iSize_Y = 100;
	pObj->tRect.left = tStart.x;
	pObj->tRect.top = tStart.y;
	pObj->tRect.right = tStart.x + OBJSMALL;
	pObj->tRect.bottom = tStart.y + OBJSMALL;
	pObj->tFrameKey = L"Flower";
	m_listObj.emplace_back(pObj);

	for (auto& pObj : m_listObj)
	{
		pObj->tDCRect.left = pObj->tRect.left + 650;
		pObj->tDCRect.top = pObj->tRect.top;
		pObj->tDCRect.right = pObj->tRect.right + 650;
		pObj->tDCRect.bottom = pObj->tRect.bottom;
	}
}

void CMapTool::Pick_TileBar(HDC _TileBar)
{
	POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	for (auto& pTile : m_listTile)
	{
		if (PtInRect(&pTile->tDCRect, pt))
		{
			HBRUSH myBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
			HBRUSH oldBrush = (HBRUSH)SelectObject(_TileBar, myBrush);
			Rectangle(_TileBar, pTile->tRect.left, pTile->tRect.top, pTile->tRect.right, pTile->tRect.bottom);
			SelectObject(_TileBar, oldBrush);
			DeleteObject(myBrush);

			if (CKeyMgr::Get_Instance()->Key_Up(VK_LBUTTON))
			{
				Set_NowTile(pTile);
			}
		}

	}

	
}

void CMapTool::Pick_ObjBar(HDC _ObjBar)
{
	POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	for (auto& pObj : m_listObj)
	{
		if (PtInRect(&pObj->tDCRect, pt))
		{
			HBRUSH myBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
			HBRUSH oldBrush = (HBRUSH)SelectObject(_ObjBar, myBrush);
			Rectangle(_ObjBar, pObj->tRect.left, pObj->tRect.top, pObj->tRect.right, pObj->tRect.bottom);
			SelectObject(_ObjBar, oldBrush);
			DeleteObject(myBrush);

			if (CKeyMgr::Get_Instance()->Key_Up(VK_LBUTTON))
			{
				Set_NowObj(pObj);
			}
		}

	}
}

void CMapTool::Key_Check()
{
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LEFT))
		CScrollMgr::Get_Instance()->Set_ScrollX(5.f);
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_RIGHT))
		CScrollMgr::Get_Instance()->Set_ScrollX(-5.f);
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_UP))
		CScrollMgr::Get_Instance()->Set_ScrollY(5.f);
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_DOWN))
		CScrollMgr::Get_Instance()->Set_ScrollY(-5.f);

	POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);
	if (pt.x < 630)
	{
		if (m_bObjBar_On)
		{
			if (m_tNowObj)
			{
				if (CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON))
					CObjMgr::Get_Instance()->Picking_Obj(pt, m_tNowObj->iOption, m_tNowObj->iType, m_eNowChannel);
				if (CKeyMgr::Get_Instance()->Key_Down(VK_RBUTTON))
					CObjMgr::Get_Instance()->Picking_Obj(pt, -1, m_tNowObj->iType, m_eNowChannel);
			}
		}
		else if (!m_bLineBar_On && !m_bRect_On)
		{
			if (m_tNowTile)
			{
				if (CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON))
					CTileMgr::Get_Instance()->Picking_Tile(pt, m_tNowTile->iDrawID_X, m_tNowTile->iDrawID_Y, m_tNowTile->iOption, m_tNowTile->eType, m_eNowChannel);
				else if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LBUTTON))
					CTileMgr::Get_Instance()->Picking_Tile(pt, m_tNowTile->iDrawID_X, m_tNowTile->iDrawID_Y, m_tNowTile->iOption, m_tNowTile->eType, m_eNowChannel);
				if (CKeyMgr::Get_Instance()->Key_Down(VK_RBUTTON))
					CTileMgr::Get_Instance()->Picking_Tile(pt, 0, 0, 0, TILE::END, m_eNowChannel);
				else if (CKeyMgr::Get_Instance()->Key_Pressing(VK_RBUTTON))
					CTileMgr::Get_Instance()->Picking_Tile(pt, 0, 0, 0, TILE::END, m_eNowChannel);
			}
		}
		else if(m_bLineBar_On)
		{
			if (m_tNowTile)
			{
				if (m_bCeiling)
				{
					if (CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON))
						CLineMgr::Get_Instance()->Emplace_Line(pt, m_eNowChannel, TILE::CEILING);
					if (CKeyMgr::Get_Instance()->Key_Down(VK_RBUTTON))
						CLineMgr::Get_Instance()->Erase_Line(pt, m_eNowChannel, TILE::CEILING);
				}
				else
				{
					if (CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON))
						CLineMgr::Get_Instance()->Emplace_Line(pt, m_eNowChannel, m_tNowTile->eType);
					if (CKeyMgr::Get_Instance()->Key_Down(VK_RBUTTON))
						CLineMgr::Get_Instance()->Erase_Line(pt, m_eNowChannel, m_tNowTile->eType);

				}
			
			}
			
		}
		else if (m_bRect_On)
		{
			if (m_tNowTile)
			{
				

				if (CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON))
					CRectMgr::Get_Instance()->Emplace_Rect(pt, m_eNowChannel, m_tNowTile->eType);
				if (CKeyMgr::Get_Instance()->Key_Down(VK_RBUTTON))
					CRectMgr::Get_Instance()->Erase_Rect(pt, m_eNowChannel, m_tNowTile->eType);
			}
		}
	
	}
	
	

	if (CKeyMgr::Get_Instance()->Key_Down('T'))
	{
		CTileMgr::Get_Instance()->Save_Tile(m_eNowChannel);
		CLineMgr::Get_Instance()->Save_Line(m_eNowChannel);
		CRectMgr::Get_Instance()->Save_Rect(m_eNowChannel);
		CObjMgr::Get_Instance()->Save_Obj(m_eNowChannel);
	}
		
	if (CKeyMgr::Get_Instance()->Key_Down('Y'))
	{
		CTileMgr::Get_Instance()->Load_Tile(m_eNowChannel);
		CLineMgr::Get_Instance()->Load_Line(m_eNowChannel);
		CRectMgr::Get_Instance()->Load_Rect(m_eNowChannel);
		CObjMgr::Get_Instance()->Load_Obj(m_eNowChannel);

	}
	if (CKeyMgr::Get_Instance()->Key_Down('B'))
	{
		CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::SCENE_MENU);

	}
	if (CKeyMgr::Get_Instance()->Key_Down(VK_TAB))
	{
		if (STAGE::STAGE1_1 == m_eNowChannel)
		{
			m_eNowChannel = STAGE::STAGE1_2;
			m_iChannel = 2;
		}
		else if (STAGE::STAGE1_2 == m_eNowChannel)
		{
			m_eNowChannel = STAGE::STAGE1_3;
			m_iChannel = 3;
		}
		else if (STAGE::STAGE1_3 == m_eNowChannel)
		{
			m_eNowChannel = STAGE::STAGE1_4;
			m_iChannel = 4;
		}
		else if (STAGE::STAGE1_4 == m_eNowChannel)
		{
			m_eNowChannel = STAGE::STAGE1_5;
			m_iChannel = 5;
		}
		else if (STAGE::STAGE1_5 == m_eNowChannel)
		{
			m_eNowChannel = STAGE::STAGE1_6;
			m_iChannel = 6;
		}
		else if (STAGE::STAGE1_6 == m_eNowChannel)
		{
			m_eNowChannel = STAGE::STAGE1_7;
			m_iChannel = 7;
		}
		else if (STAGE::STAGE1_7 == m_eNowChannel)
		{
			m_eNowChannel = STAGE::STAGE1_8;
			m_iChannel = 8;
		}
		else if (STAGE::STAGE1_8 == m_eNowChannel)
		{
			m_eNowChannel = STAGE::STAGE1_9;
			m_iChannel = 9;
		}
		else if (STAGE::STAGE1_9 == m_eNowChannel)
		{
			m_eNowChannel = STAGE::STAGE1_10;
			m_iChannel = 10;
		}
		else if (STAGE::STAGE1_10 == m_eNowChannel)
		{
			m_eNowChannel = STAGE::STAGE1_11;
			m_iChannel = 11;
		}
		else if (STAGE::STAGE1_11 == m_eNowChannel)
		{
			m_eNowChannel = STAGE::STAGE1_12;
			m_iChannel = 12;
		}
		else if (STAGE::STAGE1_12 == m_eNowChannel)
		{
			m_eNowChannel = STAGE::STAGE1_13;
			m_iChannel = 13;
		}
		else if (STAGE::STAGE1_13 == m_eNowChannel)
		{
			m_eNowChannel = STAGE::STAGE1_14;
			m_iChannel = 14;
		}
		else if (STAGE::STAGE1_14 == m_eNowChannel)
		{
			m_eNowChannel = STAGE::STAGE1_15;
			m_iChannel = 15;
		}
		else if (STAGE::STAGE1_15 == m_eNowChannel)
		{
			m_eNowChannel = STAGE::STAGE1_16;
			m_iChannel = 16;
		}
		else if (STAGE::STAGE1_16 == m_eNowChannel)
		{
			m_eNowChannel = STAGE::STAGE1_1;
			m_iChannel = 1;
		}
		CLineMgr::Get_Instance()->Set_Channel(m_eNowChannel);
		CRectMgr::Get_Instance()->Set_Channel(m_eNowChannel);
		CTileMgr::Get_Instance()->Set_Channel(m_eNowChannel);

	}
	if (CKeyMgr::Get_Instance()->Key_Down(VK_F1))
	{
		if (!m_bTileBar_On)
			m_bTileBar_On = true;
		else
			m_bTileBar_On = false;
	}
	if (CKeyMgr::Get_Instance()->Key_Down(VK_F3))
	{
		if (!m_bLineBar_On)
		{
			m_bRect_On = false;
			m_bLineBar_On = true;
		}
		else
			m_bLineBar_On = false;
	}
	if (CKeyMgr::Get_Instance()->Key_Down(VK_F4))
	{
		if (!m_bRect_On)
		{
			m_bLineBar_On = false;
			m_bRect_On = true;;
		}
		else
			m_bRect_On = false;
	}
	if (CKeyMgr::Get_Instance()->Key_Down(VK_F2))
	{
		if (m_bLine_On)
			m_bLine_On = false;
		else
			m_bLine_On = true;
	}
	if (CKeyMgr::Get_Instance()->Key_Down('C'))
	{
		if (m_bCeiling)
			m_bCeiling = false;
		else
			m_bCeiling = true;
	}
	if (CKeyMgr::Get_Instance()->Key_Down(VK_F6))
	{
		if (m_bObjBar_On)
			m_bObjBar_On = false;
		else
		{
			m_bObjBar_On = true;
			m_bTileBar_On = false;
		}
	}
}
