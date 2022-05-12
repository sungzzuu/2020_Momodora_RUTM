#pragma once
#ifndef __LINEMGR_H__
#define __LINEMGR_H__

class CLine;
class CLineMgr
{
private:
	CLineMgr();
	~CLineMgr();


public:
	void Initialize();
	void Update();
	void Late_Update();
	void Render(HDC _DC);
	void Release();


public:
	void Emplace_Line(POINT _pt, STAGE::CHANNEL _eChannel, TILE::TYPE _eType);
	void Erase_Line(POINT _pt, STAGE::CHANNEL _eChannel, TILE::TYPE _eType);
	void Delete_Line(STAGE::CHANNEL _eChannel);
public:
	void Next_Channel();
	void Previous_Channel();
public:
	bool Collision_Surface(float _x, float _y, float* _pY, bool* _bThinTileOn);
	bool Collision_Ceiling(float _x, float _y, float* _pY);
	bool Collision_Line(float _x, float* _y);

	size_t Get_LineNum(STAGE::CHANNEL _eChannel) { return m_listLine[_eChannel].size(); }

public:
	void Set_Line(bool _bLineOn);
	void Set_Channel(STAGE::CHANNEL _eChannel) { m_eNowChannel = _eChannel; }
public:
	void Load_Line(STAGE::CHANNEL _eChannel);
	void Save_Line(STAGE::CHANNEL _eChannel);

public:
	void Up_MovingTile();
	void Set_dwMovingTime() { m_dwMovingTile = GetTickCount(); m_iMovingCount = 1; };
public:
	static CLineMgr* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CLineMgr;
		return m_pInstance;
	}
	static void Destroy_Instance()
	{
		SAFE_DELETE(m_pInstance);
	}
private:
	static CLineMgr*	m_pInstance;
	list<CLine*>		m_listLine[STAGE::END];
	STAGE::CHANNEL		m_eNowChannel;
	LINEPOS				m_tLinePos[2];
	int					m_iChannel;

private:
	DWORD				m_dwMovingTile;
	int					m_iMovingCount;
};

#endif // !__LINEMGR_H__
