#pragma once
#ifndef __MAPTOOL_H__
#define __MAPTOOL_H__

#include "Scene.h"
class CTile;

class CMapTool :
	public CScene
{
public:
	CMapTool();
	virtual ~CMapTool();

	// CScene을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

private:
	void Make_TileBar();
	void Make_ObjBar();
	void Pick_TileBar(HDC _TileBar);
	void Pick_ObjBar(HDC _ObjBar);
	
public:
	int	Get_Channel(){ return m_iChannel; }
private:
	void Key_Check();
	DWORD m_dwTime;

	bool	m_bTileBar_On;
	bool	m_bLine_On;
	bool	m_bRect_On;
	bool	m_bLineBar_On;
	bool	m_bObjBar_On;
	typedef struct tagTile
	{
		int			iDrawID_X;
		int			iDrawID_Y;
		TCHAR*		tFrameKey;
		int			iOption;
		RECT		tRect;
		RECT		tDCRect;
		TILE::TYPE	eType;
	}TILEINFO;

	typedef struct tagObj
	{
		int			PosX;
		int			PosY;
		int			iSize_X;
		int			iSize_Y;
		TCHAR*		tFrameKey;
		RECT		tRect;
		RECT		tDCRect;
		int			iOption;
		int			iType;

	}OBJINFO;

	list<TILEINFO*>			m_listTile;
	list<OBJINFO*>			m_listObj;
	STAGE::CHANNEL		m_eNowChannel;
	int					m_iChannel;
	TILEINFO*			m_tNowTile;
	OBJINFO*			m_tNowObj;
	bool				m_bCeiling;
private:
	void Set_NowTile(TILEINFO* _tTile) { m_tNowTile = _tTile; }
	void Set_NowObj(OBJINFO* _tObj) { m_tNowObj = _tObj; }
#define TILESMALL	25
#define OBJSMALL	40
};

#endif // !__MAPTOOL
