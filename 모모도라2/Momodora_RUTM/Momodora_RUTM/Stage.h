#pragma once
#ifndef __STAGE_H__
#define __STAGE_H__

#include "Scene.h"
class CStage :
	public CScene
{
public:
	CStage();
	virtual ~CStage();

	// CScene을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

private:
	void Key_Check();
private:
	bool			m_bLineOn;
	STAGE::CHANNEL	m_eChannel;
	int				m_iChannel;
	int				m_ibackX;
	int				m_iPrevious_ScrollX;
};

#endif // !__STAGE_H__
