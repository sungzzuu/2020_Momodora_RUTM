#pragma once

#ifndef __SCENEMGR_H__
#define __SCENEMGR_H__

class CScene;
class CSceneMgr
{
private:
	CSceneMgr();
	~CSceneMgr();

public:
	enum SCENEID { SCENE_LOGO, SCENE_START, SCENE_MENU, SCENE_STAGE, SCENE_CONTROLTYPE, SCENE_MAPTOOL, SCENE_DEATHMENU, SCENE_END };

public:
	void Scene_Change(SCENEID _eID, int _SlotNum=0);
	void Update();
	void Late_Update();
	void Render(HDC _DC);
	void Release();

public:
	bool Get_CurScene_MapTool();
public:

public:
	static CSceneMgr* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CSceneMgr;
		return m_pInstance;
	}
	static void Destroy_Instance()
	{
		SAFE_DELETE(m_pInstance);
	}

private:
	static CSceneMgr*	m_pInstance;
	CScene*				m_pScene;

	SCENEID				m_eCurScene;
	SCENEID				m_ePreScene;
};


#endif // !__SCENEMGR_H__
