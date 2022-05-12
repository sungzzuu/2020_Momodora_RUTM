#pragma once
#ifndef __START_H__
#define __START_H__

#include "Scene.h"
class CStart :
	public CScene
{
public:
	CStart();
	virtual ~CStart();

	// CScene을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

private:
	WPARAM	m_wParam;
};

#endif // !__START_H__
