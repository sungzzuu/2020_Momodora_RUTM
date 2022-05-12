#pragma once
#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Obj.h"


class CPlayer :
	public CObj
{
public:
	CPlayer();
	virtual ~CPlayer();

public:
	enum STATE
	{
		//1-5
		ATTACK1, ATTACK2, ATTACK3, AIRATTACK, LANDATTCK,
		//6-10
		BOW, AIRBOW, CROUCHBOW, RUN, BRAKE,
		//11-15
		TURN, CROUCH, RISE, DEATH, JUMP,
		//16-20
		FALL, TOPRAYING, PRAYING, FROMPRAYING, HURT,
		//21-25
		IDLE, LOOK, KICKS, YAWN, ITEM,
		//26-30
		LADDERUP, LADDERDOWN, LADDERENTER, LADDERLEAVE, LANDHARD,
		//31-35
		LANDSOFT, PREDASH, DASH, POSTDASH, POSTDASHOLD,
		//36
		ROLL

	};

private:
	void Scene_Change();
	void Frame_Move();
private:
	void Animation_Flow();
	void Add_Leaf();
	void Add_Arrow();
	void Update_Leaf();
public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

private:
	void Check_Player_Pos();

private:
	void Key_Check();
	void Jumping();
	void OffSet();
	void Check_Second_Jump();

public:
	void Set_ItemOn() { m_bItemOn = true; }
	bool Get_ItemOn() { return  m_bItemOn; }
	void Set_ItemCount() { m_iItemCount += 5; }
	int Get_ItemCount() { return  m_iItemCount; }

public:
	bool Get_bGod() const { return m_bGod; }
private:
	float			m_fJumpPower;
	float			m_fJumpAccel;
	bool			m_bJump;
	bool			m_bJumpMax;
	bool			m_bFallOn;
	bool			m_bThinTileOn;
	bool			m_bThinCheck;
	float			m_fSecondJump;
	STATE			m_ePreState;
	STATE			m_eCurState;
	bool			m_bGod;
	DWORD			m_dwGod;
	DWORD			m_dwAttak;
	bool			m_bAttack;
	DWORD			m_dwArrowCol;
	bool			m_bItemOn;
	int				m_iItemCount;
private:
	CObj*			m_pLeaf;
	bool			m_bGodContinue;

private:
	BLENDFUNCTION	m_BlendFunction;
	int				m_iAlpha;
};


#endif // !__PLAYER_H__
