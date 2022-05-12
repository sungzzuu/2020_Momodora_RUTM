#pragma once
#ifndef __ITEM_H__
#define __ITEM_H__

#include "Obj.h"
class CItem :
	public CObj
{
public:
	CItem();
	virtual ~CItem();
public:
	enum STATE
	{
		LIVE, DIE
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

#endif // !__ITEM_H__
