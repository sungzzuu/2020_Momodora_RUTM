#pragma once
#ifndef __RCETMGR_H__
#define __RECTMGR_H__

class CMyRect;
class CRectMgr
{
private:
	CRectMgr();
	~CRectMgr();

public:
	static CRectMgr* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CRectMgr;
		return m_pInstance;
	}
	static void Destroy_Instance()
	{
		SAFE_DELETE(m_pInstance);
	}
public:
	void initialize();
	int	 Update();
	void Late_Update();
	void Render(HDC _DC);
	void Release();

public:
	const list<CMyRect*>& Get_Rect() {return m_listRect[m_eNowChannel]; }
public:
	void Next_Channel();
	void Previous_Channel();
public:
	void Emplace_Rect(POINT _pt, STAGE::CHANNEL _eChannel, TILE::TYPE _eType);
	void Erase_Rect(POINT _pt, STAGE::CHANNEL _eChannel, TILE::TYPE _eType);
	void Delete_Rect(STAGE::CHANNEL _eChannel);
	bool Collision_Ladder(RECT _rc, float* _x, bool _bUp);
public:
	size_t Get_RectNum(STAGE::CHANNEL _eChannel) { return m_listRect[_eChannel].size(); }
public:
	void Save_Rect(STAGE::CHANNEL _eChannel);
	void Load_Rect(STAGE::CHANNEL _eChannel);
public:
	void Set_Line_On(bool _bLineOn);
public:
	void Set_Channel(STAGE::CHANNEL _eChannel) { m_eNowChannel = _eChannel; }
	
public:
	void Up_MovingTile();
	void Set_dwMovingTime() { m_dwMovingTile = GetTickCount(); m_iMovingCount = 1; };
private:
	static CRectMgr*	m_pInstance;
	list<CMyRect*>		m_listRect[STAGE::END];
	TILE::TYPE			m_eType;
	RECT				m_tRect;
	STAGE::CHANNEL		m_eNowChannel;
	int					m_iChannel;

private:
	DWORD				m_dwMovingTile;
	int					m_iMovingCount;
};

#endif // !__RCETMGR_H__
