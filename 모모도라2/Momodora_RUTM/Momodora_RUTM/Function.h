#pragma once
#ifndef __FUNCTION_H__
#define __FUNCTION_H__



template <typename T>
void Safe_Delete(T& _obj)
{
	if (_obj)
	{
		delete _obj;
		_obj = nullptr;
	}
	
}
#endif // !__FUNCTION_H__
