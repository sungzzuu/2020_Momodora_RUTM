#pragma once
#ifndef __DEFINE_H__
#define __DEFINE_H__


#define WINCX 800
#define WINCY 600

#define SAFE_DELETE(p) { delete p, p = nullptr;}
#define OBJ_DEAD 0
#define OBJ_NOEVENT 1


#define PI 3.141592f

#define TILECX		40
#define TILECY		40
#define TILEX		30
#define TILEY		20
#define RIGHT		1
#define LEFT		0
#define GRAVITY		6.3f
#define RED			RGB(255,0,0)
#define BLUE		RGB(0,0,255)
#define GREEN		RGB(0,255,0)
#define YELLOW		RGB(255,255,0)
#define BLACK		RGB(0,0,0)
#define PINK		RGB(255,0,255)
#define PURPLE		RGB(200,0,255)
#define WHITE		RGB(255,255,255)
#define SKYBLUE		RGB(100, 200, 255)
#define DEEPGREEN	RGB(128,128,0)
#define EMERALD		RGB(0,255,255)
#define VK_MAX		0xFF
#endif // !__DEFINE_H__
