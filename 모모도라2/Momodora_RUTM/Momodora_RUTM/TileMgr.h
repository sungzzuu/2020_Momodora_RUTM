#pragma once

#ifndef __TILEMGR_H__
#define __TILEMGR_H__

class CTile;
class CTileMgr
{
private:
	CTileMgr();
	~CTileMgr();

public:
	void Initialize();
	void Update();
	void Late_Update();
	void Render(HDC _DC);
	void Release();

public:
	void Load_Tile(STAGE::CHANNEL _eChannel);
	void Save_Tile(STAGE::CHANNEL _eChannel);
	void Set_LineOn(bool _bLineOn);
	void Next_Channel();
	void Previous_Channel();
	void Delete_Tile(STAGE::CHANNEL _eChannel);
public:
	list<CObj*> Get_Tile(STAGE::CHANNEL _eChannel) const { return m_listTile[_eChannel]; }
	void Get_Begin_Tile(float* _x);
	void Get_End_Tile(float* _x, float* _y);
	size_t Get_TileNum(STAGE::CHANNEL _eChannel){ return m_listTile[_eChannel].size(); }
	bool Get_ThinTile();
	STAGE::CHANNEL Get_Channel() { return m_eNowChannel; }
public:
	void Set_Channel(STAGE::CHANNEL _eChannel) { m_eNowChannel = _eChannel; }
public:
	void Picking_Tile(POINT _pt, int _iDrawID_X, int _iDrawID_Y, int _iOption, TILE::TYPE _eType, STAGE::CHANNEL _eChannel);

public:
	void Up_MovingTile();
	void Set_dwMovingTime() { m_dwMovingTile = GetTickCount(); m_iMovingCount = 1; };

public:
	bool Collision_BounceBall_Player();
public:
	CObj* Get_Orb ();

public:
	static CTileMgr* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CTileMgr;
		return m_pInstance;
	}
	static void Destroy_Instance()
	{
		SAFE_DELETE(m_pInstance);
	}

private:
	static CTileMgr*	m_pInstance;
	STAGE::CHANNEL		m_eNowChannel;
	int					m_iChannel;
	list<CObj*>		m_listTile[STAGE::END];
private:
	DWORD				m_dwMovingTile;
	int					m_iMovingCount;
};


#endif // !__TILEMGR_H__
