#pragma once

#ifndef __OBJMGR_H__
#define __OBJMGR_H__

class CObjMgr
{
private:
	CObjMgr();
	~CObjMgr();

public:
	void Update();
	void Late_Update();
	void Render(HDC _DC);
	void Release();

public:
	void Add_Object(OBJID::ID _eID, CObj* _pObj);
public:
	void Delete_ID(OBJID::ID _eID);
	void Delete_Leaf();
public:
	bool Get_Empty() const { return m_listObj[OBJID::PLAYER][STAGE::STAGE1_1].empty(); }
	CObj* Get_Player() const { return m_listObj[OBJID::PLAYER][STAGE::STAGE1_1].front(); }
	CObj* Get_Target(CObj* _pDst, OBJID::ID _eID);
	list<CObj*> Get_Obj(OBJID::ID _eID) const { return m_listObj[_eID][m_eNowChannel]; }
	size_t Get_ObjSize(OBJID::ID _eID) const { return m_listObj[_eID][m_eNowChannel].size(); }
	CObj* Get_Fennel();
public:
	void Picking_Obj(POINT _pt,int _iOption,int _iType, STAGE::CHANNEL _eChannel);

public:
	STAGE::CHANNEL Get_Channel() { return m_eNowChannel; }
	void Set_Channel(STAGE::CHANNEL _eChannel) { m_eNowChannel = _eChannel; }
	void Set_SlotNum(int _SlotNum) { m_iSlotNum = _SlotNum; }

public:
	bool Collision_Player_Weapon();
	bool Collision_Player_Monster();
	bool Collision_Monster_Weapon(CObj* _pMonster, bool* _bCol);
	bool Collision_SavePoint_Player();
	bool Collision_Orb_Arrow();
	bool Collision_Player_Gold();
	bool Collision_Flower_Player();
public:
	void Next_Channel();
	void Previous_Channel();

public:
	void Save_Obj(STAGE::CHANNEL _eChannel);
	void Load_Obj(STAGE::CHANNEL _eChannel);
	void Player_Save();
	bool Player_Load();

private:
	void Collision_Check();
public:
	void Set_Line(bool _bLineOn);
	// ΩÃ±€≈Ê ∆–≈œ
public:
	static CObjMgr* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CObjMgr;
		return m_pInstance;
	}
	static void Destroy_Instance()
	{
		SAFE_DELETE(m_pInstance);
	}

private:
	list<CObj*>		m_listObj[OBJID::END][STAGE::END];
	list<CObj*>		m_RenderGroup[GROUPID::END][STAGE::END];
	STAGE::CHANNEL		m_eNowChannel;
	int					m_iChannel;

	// ΩÃ±€≈Ê ∆–≈œ
	static CObjMgr*		m_pInstance;

	int				m_iSlotNum;

};


#endif // !__OBJMGR_H__
