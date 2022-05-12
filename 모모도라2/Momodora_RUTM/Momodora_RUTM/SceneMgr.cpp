#include "stdafx.h"
#include "SceneMgr.h"
#include "Logo.h"
#include "Start.h"
#include "MyMenu.h"
#include "ControlType.h"
#include "MapTool.h"
#include "Stage.h"
#include "DeathMenu.h"
CSceneMgr* CSceneMgr::m_pInstance = nullptr;
CSceneMgr::CSceneMgr()
	: m_pScene(nullptr), m_eCurScene(SCENE_END), m_ePreScene(SCENE_END)
{
}


CSceneMgr::~CSceneMgr()
{
	Release();
}

void CSceneMgr::Scene_Change(SCENEID _eID, int _SlotNum)
{
	m_eCurScene = _eID;

	if (m_ePreScene != m_eCurScene)
	{
		Safe_Delete(m_pScene);

		switch (m_eCurScene)
		{
		case CSceneMgr::SCENE_LOGO:
			m_pScene = new CLogo;
			break;
		case CSceneMgr::SCENE_START:
			m_pScene = new CStart;
			break;
		case CSceneMgr::SCENE_MENU:
			m_pScene = new CMyMenu;
			break;
		case CSceneMgr::SCENE_CONTROLTYPE:
			m_pScene = new CControlType;
			break;
		case CSceneMgr::SCENE_STAGE:
			m_pScene = new CStage;
			m_pScene->Initialize();
			m_pScene->Set_SlotNum(_SlotNum);
			m_ePreScene = m_eCurScene;
			return;
		case CSceneMgr::SCENE_MAPTOOL:
			m_pScene = new CMapTool;
			break;
		case CSceneMgr::SCENE_DEATHMENU:
			m_pScene = new CDeathMenu;

		}
		m_pScene->Initialize();
		m_ePreScene = m_eCurScene;
	}
}

void CSceneMgr::Update()
{
	m_pScene->Update();
}

void CSceneMgr::Late_Update()
{
	m_pScene->Late_Update();
}

void CSceneMgr::Render(HDC _DC)
{
	m_pScene->Render(_DC);
}

void CSceneMgr::Release()
{
	Safe_Delete(m_pScene);
}

bool CSceneMgr::Get_CurScene_MapTool()
{
	if (SCENE_MAPTOOL == m_eCurScene)
		return true;
	else
		return false;

}
