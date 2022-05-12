#pragma once
#ifndef __FENNEL_H__
#define __FENNEL_H__

#include "Obj.h"
class CFennel :
	public CObj
{
public:
	CFennel();
	virtual ~CFennel();

	enum STATE
	{
		ATTACK, PLUNGE, JUMP, BACKFLIP, DASH, BUFF, DEATH,
		INTRO1, INTRO2, INTRO3, INTRO4
	};

	enum PATTERN
	{
		DASH_ATTACK, BACKFLIP_PLUNGE, BUFFATTACK
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
	void Add_BuffSword();
	void Add_Impact();
private:

	STATE			m_eCurState;
	STATE			m_ePreState;

	int				m_iPattern;
	DWORD			m_dwTurnTime;

};

#endif // !__FENNEL_H__
