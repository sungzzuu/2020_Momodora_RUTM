#pragma once
#ifndef __BOMBMONSTER_H__
#define __BOMBMONSTER_H__

#include "Obj.h"
class CBombMonster :
	public CObj
{
public:
	CBombMonster();
	virtual ~CBombMonster();

public:
	enum STATE
	{
		IDLE, THROWBOMB, HURT
	};
	// CObj��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

private:
	void Scene_Change();
	void Frame_Move();
private:

	STATE			m_eCurState;
	STATE			m_ePreState;
	DWORD			m_dwAttack;

	MONSTER::TYPE	m_eType;

};

#endif // !__BOMBMONSTER_H__
