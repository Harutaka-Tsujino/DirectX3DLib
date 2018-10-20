#pragma once
#include <windows.h>
#include <d3dx9.h>
#include "../Draw/Draw.h"
#include"../../FBX/FbxRelated.h"

class Object
{
public:
	Object() {};
	~Object() {};

	virtual VOID Control() = 0;
	virtual VOID Render() = 0;
};
