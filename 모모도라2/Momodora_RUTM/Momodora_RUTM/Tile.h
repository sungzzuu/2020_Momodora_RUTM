#pragma once
#ifndef __TILE_H__
#define __TILE_H__

#include "Obj.h"
class CTile :
	public CObj
{
public:
	CTile();
	virtual ~CTile();

public:
	

	// CObj을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;


public:
	const int& Get_DrawID_X() { return m_iDrawID_X; }
	const int& Get_DrawID_Y() { return m_iDrawID_Y; }
	const int& Get_Option() { return m_iOption; }
	const TILE::TYPE& Get_Type() { return m_eType; }
public:
	void Load_Tile(INFO& _rInfo) { memcpy(&m_tInfo, &_rInfo, sizeof(INFO)); }
public:
	void Set_DrawID_X(int _iDrawID_X) { m_iDrawID_X = _iDrawID_X; }
	void Set_DrawID_Y(int _iDrawID_Y) { m_iDrawID_Y = _iDrawID_Y; }
	void Set_Option(int _iOption) { m_iOption = _iOption; }
	void Set_Type(TILE::TYPE _eType) { m_eType = _eType; }
	


private:
	int		m_iDrawID_X;
	int		m_iDrawID_Y;
	int		m_iOption;

	TILE::TYPE	m_eType;


};


#endif // !__TILE_H__
