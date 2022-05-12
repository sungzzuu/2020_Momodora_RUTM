#pragma once
#ifndef __MYRECT_H__
#define __MYRECT_H__


class CMyRect
{
public:
	CMyRect();
	CMyRect(RECT& _rc, TILE::TYPE _eType);
	~CMyRect();


public:
	void	Initialize();
	int		Update();
	void	Late_Update();
	void	Render(HDC _DC);
	void	Release();

public:
	const RECT&			Get_Rect() { return m_tRect; }
	const TILE::TYPE&	Get_Type() { return m_eType; }

public:
	void Set_Line_On(bool _bLineOn) { m_bLineOn = _bLineOn; }

	void Set_Rect(float _y) { m_tRect.top -= _y; m_tRect.bottom -= _y; }
private:
	RECT			m_tRect;
	TILE::TYPE		m_eType;
	bool			m_bLineOn;
};

#endif // !__MYRECT_H__
