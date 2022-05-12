#include "stdafx.h"
#include "MainGame.h"
#include "SceneMgr.h"
#include "BmpMgr.h"
#include "KeyMgr.h"
#include "ScrollMgr.h"
#include "TileMgr.h"
#include "LineMgr.h"
#include "ObjMgr.h"
#include "Scene.h"
#include "RectMgr.h"
#include "SoundMgr.h"
CMainGame::CMainGame()
	:m_dwTime(GetTickCount()), m_iFPS(0)
{
}


CMainGame::~CMainGame()
{
	Release();
}

void CMainGame::Initialize()
{
	m_DC = GetDC(g_hWnd);

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/Back.bmp", L"Back");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/BackBuffer.bmp", L"BackBuffer");
	//CBmpMgr::Get_Instance()->Insert_Bmp(__T(""), __T(""));

	CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::SCENE_LOGO);
	CSoundMgr::Get_Instance()->Initialize();

}

void CMainGame::Update()
{
	CSceneMgr::Get_Instance()->Update();
	CTileMgr::Get_Instance()->Update();
	CLineMgr::Get_Instance()->Update();
	CRectMgr::Get_Instance()->Update();
}

void CMainGame::Late_Update()
{
	CSceneMgr::Get_Instance()->Late_Update();
	CKeyMgr::Get_Instance()->Key_Update();
	CScrollMgr::Get_Instance()->Scroll_Lock();
}

void CMainGame::Render()
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"Back");
	HDC hBackBuffer = CBmpMgr::Get_Instance()->Find_Bmp(L"BackBuffer");
	BitBlt(hBackBuffer, 0, 0, WINCX, WINCY, hMemDC, 0, 0, SRCCOPY);

	CSceneMgr::Get_Instance()->Render(hBackBuffer);


	TCHAR	szBuff[32] = L"";
	//swprintf_s(szBuff, L"ScrollX: %f", CScrollMgr::Get_Instance()->Get_ScrollX());
	//TextOut(hBackBuffer, 50, 50, szBuff, lstrlen(szBuff));
	//swprintf_s(szBuff, L"ScrollY: %f", CScrollMgr::Get_Instance()->Get_ScrollY());
	//TextOut(hBackBuffer, 50, 100, szBuff, lstrlen(szBuff));
	//swprintf_s(szBuff, L"TileNum: %d", CTileMgr::Get_Instance()->Get_TileNum(STAGE::STAGE1_1));
	//TextOut(hBackBuffer, 50, 150, szBuff, lstrlen(szBuff));
	//swprintf_s(szBuff, L"LeafNum: %d", CObjMgr::Get_Instance()->Get_ObjSize(OBJID::PLAER_WEAPON));
	//TextOut(hBackBuffer, 50, 200, szBuff, lstrlen(szBuff));

	BitBlt(m_DC, 0, 0, WINCX, WINCY, hBackBuffer, 0, 0, SRCCOPY);

	++m_iFPS;
	if (m_dwTime + 1000 < GetTickCount())
	{
		swprintf_s(m_szFPS, L"FPS: %d", m_iFPS);
		SetWindowText(g_hWnd, m_szFPS);

		m_iFPS = 0;
		m_dwTime = GetTickCount();
	}
	
}

void CMainGame::Release()
{
	CKeyMgr::Destroy_Instance();
	CLineMgr::Destroy_Instance();
	CScrollMgr::Destroy_Instance();
	CBmpMgr::Destroy_Instance();
	CSceneMgr::Destroy_Instance();
	CTileMgr::Destroy_Instance();
	CRectMgr::Destroy_Instance();
	CObjMgr::Destroy_Instance();
	CSoundMgr::Destroy_Instance();

	ReleaseDC(g_hWnd, m_DC);
}
