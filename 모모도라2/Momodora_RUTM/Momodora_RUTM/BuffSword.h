#pragma once
#ifndef __BUFFSWORD_H__
#define __BUFFSWORD_H__

#include "Obj.h"
class CBuffSword :
	public CObj
{
public:
	CBuffSword();
	virtual ~CBuffSword();
	
public:
	void Scene_Change();


public:
	// CObj을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;
};

#endif // !__BUFFSWORD_H__
