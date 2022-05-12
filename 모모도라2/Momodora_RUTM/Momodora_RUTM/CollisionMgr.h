#pragma once

#ifndef __COLLISIONMGR_H__
#define __COLLISIONMGR_H__

class CObj;
class CMyRect;
class CCollisionMgr
{
public:
	CCollisionMgr();
	~CCollisionMgr();

public:
	static bool Collision_RectEx(list<CObj*>& _Dst, list<CObj*>& _Src);
	static bool Collision_RectEx(CObj* _Dst, list<CObj*>& _Src);
	static void Collision_Sphere(list<CObj*>& _Dst, list<CObj*>& _Src);
	static void Collision_RectEx_Rect(list<CObj*>& _Src);

	static bool Check_Sphere(CObj* _Dst, CObj* _Src);
	static bool Check_Rect(CObj* _Dst, CObj* _Src, float* _x, float* _y);
	static bool Check_Rect(CMyRect* _Dst, CObj* _Src, float* _x, float* _y);



};

#endif // !__COLLISIONMGR_H__

