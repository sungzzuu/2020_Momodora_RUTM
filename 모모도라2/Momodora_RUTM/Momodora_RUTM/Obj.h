#pragma once

#ifndef __OBJ_H__
#define __OBJ_H__


class CObj
{
public:
	CObj();
	virtual ~CObj();

public:
	virtual void Initialize() = 0;
	virtual int Update() = 0;
	virtual void Late_Update() = 0;
	virtual void Render(HDC _DC) = 0;
	virtual void Release() = 0;

public:

	void Set_Pos(float _x, float _y) { m_tInfo.fX = _x; m_tInfo.fY = _y; }
	void Set_PosX(float _x) { m_tInfo.fX += _x; }
	void Set_PosY(float _y) { m_tInfo.fY += _y; }
	void Set_Dead() { m_bDead = true; }
	void Set_Angle(float _fAngle) { m_fAngle = _fAngle; }
	void Set_Target(CObj* _pTarget) { m_pTarget = _pTarget; }
	void Set_FrameKey(TCHAR* _pFrameKey) { m_pFrameKey = _pFrameKey; }
	void Set_LineOn(bool _bOn) { m_bLineOn = _bOn; }
	void Set_Hp(int _iHp) { m_tGauge.iHp -= _iHp; }
	void Set_Gold(int _iGold) { m_tGauge.iGold += _iGold; }
	void Set_ObjOption(int _iOption) { m_iObjOption = _iOption; }
public:
	TCHAR*	Get_FrameKey() const { return m_pFrameKey; }
	bool	Get_bLine() const { return m_bLineOn; }
	TCHAR*	Get_Name() const { return m_pName; }
	bool	Get_Dead() const { return m_bDead; }
public:
	const RECT& Get_Rect() const { return m_tRect; }
	const INFO& Get_Info() const { return m_tInfo; }
	const RECT& Get_CollisionRect() const { return m_tCollisionRect; }
	const int& Get_ObjOption() const { return m_iObjOption; }
	GROUPID::ID Get_GroupID() { return m_eGroup; }

	bool Get_ChannelChange() { return m_bChannelChange; }
	void Set_ChannelChange() { m_bChannelChange = false; }

	int Get_Att() { return m_tGauge.iAtt; }
	int	Get_Gold() { return m_tGauge.iGold; }
	int Get_Hp() { return m_tGauge.iHp; }
	int Get_MaxHp() { return m_tGauge.iMaxHp; }

protected:
	void Update_Rect();
protected:
	INFO	m_tInfo;
	RECT	m_tRect;
	RECT	m_tCollisionRect;
	FRAME	m_tFrame;
	float	m_fSpeed;

	GAUGE	m_tGauge;
	bool	m_bAttack;

	bool	m_bDead;
	float	m_fAngle;
	float	m_fDis;

	CObj*	m_pTarget;

	bool	m_bLineOn;
	bool	m_bAttackCol;
	DWORD		m_dwCollision;

	TCHAR*	m_pFrameKey;

	TCHAR*	m_pName;

	int		m_iObjOption;
	bool			m_bChannelChange;
	GROUPID::ID		m_eGroup;

};


#endif // !__OBJ_H__
