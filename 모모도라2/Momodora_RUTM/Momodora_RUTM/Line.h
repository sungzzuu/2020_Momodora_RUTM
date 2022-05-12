#pragma once
#ifndef __LINE_H__
#define __LINE_H__
#include "Obj.h"

class CLine 
{
public:
	CLine();
	CLine(LINEPOS& _Left, LINEPOS& _Right);
	CLine(LINEINFO& _tInfo, TILE::TYPE _eType);

	~CLine();
public:
	const LINEINFO& Get_Info() const { return m_tInfo; }
	void Set_Info(LINEINFO _tInfo) { m_tInfo = _tInfo; }
	void Set_LineOn(bool _bCheck) { m_bLineOn = _bCheck; }
	void Set_Type(TILE::TYPE _eType) { m_eType = _eType; }

	void Set_LineY(float _y) { m_tInfo.tLeftPos.fY -= _y; m_tInfo.tRightPos.fY -= _y; }
public:
	const TILE::TYPE& Get_Type() const { return m_eType; }
public:
	void	Initialize();
	int		Update();
	void	Late_Update();
	void	Render(HDC _DC);
	void	Release();

private:
	LINEINFO	m_tInfo;
	TILE::TYPE	m_eType;
	bool		m_bLineOn;
};

#endif // !__LINE_H__
