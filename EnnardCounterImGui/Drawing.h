#ifndef DRAWING_H
#define DRAWING_H

#include <chrono>

#include "pch.h"

class Drawing
{
public:
	static bool bDisplay;
	static BOOL bInit;
	static bool bSetPos;
	static ImVec2 vWindowPos;
	static ImVec2 vWindowSize;

	static bool isStarted;
	static int step;
	static std::chrono::time_point<std::chrono::steady_clock> globalTimer;
	static std::chrono::time_point<std::chrono::steady_clock> intervalTimer;

	

	static HRESULT APIENTRY hkEndScene(LPDIRECT3DDEVICE9 D3D9Device);

private:
	static void InitImGui(LPDIRECT3DDEVICE9 pDevice);
};

#endif