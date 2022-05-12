#include "stdafx.h"
#include "ControlType.h"
#include "BmpMgr.h"
#include "KeyMgr.h"
#include "SceneMgr.h"

CControlType::CControlType()
{
}


CControlType::~CControlType()
{
	Release();
}

void CControlType::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/Manual/Manual.bmp", L"ControlType");

}

void CControlType::Update()
{
	
}

void CControlType::Late_Update()
{
	if (CKeyMgr::Get_Instance()->Key_Down('S'))
	{
		CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::SCENE_MENU);
		return;
	}
}

void CControlType::Render(HDC _DC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"ControlType");
	BitBlt(_DC, 0, 0, WINCX, WINCY, hMemDC, 0, 0, SRCCOPY);
}

void CControlType::Release()
{

}
