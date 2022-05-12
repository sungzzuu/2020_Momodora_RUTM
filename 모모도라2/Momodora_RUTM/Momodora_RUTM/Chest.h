#pragma once
#ifndef __CHEST_H__
#define __CHEST_H__

#include "Obj.h"
class CChest :
	public CObj
{
public:
	CChest();
	virtual ~CChest();
public:
	enum STATE
	{
		CLOSE, OPEN
	};
	// CObj을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

private:
	void Scene_Change();
	void Frame_Move();

private:
	STATE		m_eCurState;
	STATE		m_ePreState;
};

#endif // !__CHEST_H__
