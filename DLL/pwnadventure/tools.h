#pragma once

#include <windows.h>
#include <vector>

namespace Tools
{
	void APIENTRY Patch(BYTE* dst, BYTE* src, DWORD size);
	void APIENTRY PatchNOP(BYTE* dst, DWORD size);
	DWORD* APIENTRY FindDMA(DWORD ptr, std::vector<uintptr_t> offsets);
	bool APIENTRY Hook(void* dst, void* src, DWORD size);
}