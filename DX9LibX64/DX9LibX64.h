#pragma once
#include<windows.h>

#define DISALLOW_DEFAULT_CONSTRACTA_AND_DESTRACTA(ClassName)\
ClassName(const ClassName&) = delete;\
~ClassName() = delete;\
ClassName& operator=(const ClassName&) = delete;

#define CREATE_CONSTRACTA_AND_DESTRACTA(ClassName)\
ClassName(){}\
~ClassName(){}\

//INT LoopMainFunc(VOID(*func)(), Window* pWindow, DirectX* pDirectX);
VOID MainFunc();
