#pragma once
#ifndef __CONTROLTYPE_H__
#define __CONTROLTYPE_H__

#include "Scene.h"
class CControlType :
	public CScene
{
public:
	CControlType();
	virtual ~CControlType();

	// CScene을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;
};

#endif // !__CONTROLTYPE_H__
