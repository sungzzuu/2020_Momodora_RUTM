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

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/Menu/Button/FileSave.bmp", L"����1");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/Menu/Button/FileSave.bmp", L"����2");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/Menu/Button/����.bmp", L"����");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/Menu/Button/����Ű.bmp", L"����Ű");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/Menu/Button/����.bmp", L"����");

	
	CObj* pObj = CAbstractFactory<CMyButton>::Create(380.f, 50.f);
	pObj->Set_FrameKey(L"����1");
	CObjMgr::Get_Instance()->Add_Object(OBJID::UI, pObj);

	pObj = CAbstractFactory<CMyButton>::Create(380.f, 116.f);
	pObj->Set_FrameKey(L"����2");
	CObjMgr::Get_Instance()->Add_Object(OBJID::UI, pObj);

	pObj = CAbstractFactory<CMyButton>::Create(130.f, 290.f);
	pObj->Set_FrameKey(L"����");
	CObjMgr::Get_Instance()->Add_Object(OBJID::UI, pObj);

	pObj = CAbstractFactory<CMyButton>::Create(130.f, 364.f);
	pObj->Set_FrameKey(L"����Ű");
	CObjMgr::Get_Instance()->Add_Object(OBJID::UI, pObj);

	pObj = CAbstractFactory<CMyButton>::Create(130.f, 438.f);
	pObj->Set_FrameKey(L"����");
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
	// ��ư ����
	CObjMgr::Get_Instance()->Delete_ID(OBJID::UI);
	

}
