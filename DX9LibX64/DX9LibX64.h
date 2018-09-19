#pragma once
#include <windows.h>
#include "ClassWindow.h"
#include "ClassFPS.h"
#include "ClassDirectX.h"

#define DISALLOW_DEFAULT_CONSTRACTA_AND_DESTRACTA(ClassName)\
ClassName(const ClassName&) = delete;\
~ClassName() = delete;\
ClassName& operator=(const ClassName&) = delete;

#define CREATE_CONSTRACTA_AND_DESTRACTA(ClassName)\
ClassName(){};\
~ClassName(){};\

class Window;
class DirectX;

INT LoopMainFunc(VOID(*func)(), Window* pWindow, DirectX* pDirectX);
VOID MainFunc();
