#pragma once
#ifndef __DEATHMENU_H__
#define __DEATHMENU_H__


#include "Scene.h"
class CDeathMenu :
	public CScene
{
public:
	CDeathMenu();
	virtual ~CDeathMenu();

	// CScene을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

	TCHAR*		m_pFrameKey;
};

#endif // !__DEATHMENU_H__
