#include "stdafx.h"
#include "MyMenu.h"
#include "SceneMgr.h"
#include "BmpMgr.h"
#include "KeyMgr.h"
#include "MyButton.h"
#include "ObjMgr.h"
#include "SoundMgr.h"

CMyMenu::CMyMenu()
{
}


CMyMenu::~CMyMenu()
{
	Release();
}

void CMyMenu::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/Menu/Menu.bmp", L"Menu");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/Menu/Button/FileSave.bmp", L"½½·Ô1");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/Menu/Button/FileSave.bmp", L"½½·Ô2");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/Menu/Button/¸ÊÅø.bmp", L"¸ÊÅø");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/Menu/Button/Á¶ÀÛÅ°.bmp", L"Á¶ÀÛÅ°");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/Menu/Button/Á¾·á.bmp", L"Á¾·á");

	
	CObj* pObj = CAbstractFactory<CMyButton>::Create(380.f, 50.f);
	pObj->Set_FrameKey(L"½½·Ô1");
	CObjMgr::Get_Instance()->Add_Object(OBJID::UI, pObj);

	pObj = CAbstractFactory<CMyButton>::Create(380.f, 116.f);
	pObj->Set_FrameKey(L"½½·Ô2");
	CObjMgr::Get_Instance()->Add_Object(OBJID::UI, pObj);

	pObj = CAbstractFactory<CMyButton>::Create(130.f, 290.f);
	pObj->Set_FrameKey(L"¸ÊÅø");
	CObjMgr::Get_Instance()->Add_Object(OBJID::UI, pObj);

	pObj = CAbstractFactory<CMyButton>::Create(130.f, 364.f);
	pObj->Set_FrameKey(L"Á¶ÀÛÅ°");
	CObjMgr::Get_Instance()->Add_Object(OBJID::UI, pObj);

	pObj = CAbstractFactory<CMyButton>::Create(130.f, 438.f);
	pObj->Set_FrameKey(L"Á¾·á");
	CObjMgr::Get_Instance()->Add_Object(OBJID::UI, pObj);
	CSoundMgr::Get_Instance()->PlayBGM(L"FileSelect.wav");


}

void CMyMenu::Update()
{
	CObjMgr::Get_Instance()->Update();

}

void CMyMenu::Late_Update()
{
	CObjMgr::Get_Instance()->Late_Update();
}

void CMyMenu::Render(HDC _DC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"Menu");
	BitBlt(_DC, 0, 0, WINCX, WINCY, hMemDC, 0, 0, SRCCOPY);

	for (auto& pUI : CObjMgr::Get_Instance()->Get_Obj(OBJID::UI))
		pUI->Render(_DC);

}

void CMyMenu::Release()
{
	// ¹öÆ° »èÁ¦
	CObjMgr::Get_Instance()->Delete_ID(OBJID::UI);
	

}
