#include "pch.h"
#include "tools.h"

#define NOP 0x90

void APIENTRY Tools::Patch(BYTE* dst, BYTE* src, DWORD size) {

    DWORD oldProtect;

    VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldProtect);
    memcpy(dst, src, size);
    VirtualProtect(dst, size, oldProtect, &oldProtect);
}

void APIENTRY Tools::PatchNOP(BYTE* dst, DWORD size) {

    DWORD oldProtect;

    VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldProtect);
    memset(dst, 0x90, size);
    VirtualProtect(dst, size, oldProtect, &oldProtect);
}

DWORD* APIENTRY Tools::FindDMA(DWORD ptr, std::vector<uintptr_t> offsets) {

    DWORD addr = ptr;
    for (int i = 0; i < offsets.size(); ++i) {
        addr = *(DWORD*)addr;
        addr += offsets[i];
    }
    return (DWORD*)addr;
}

/*
DWORD APIENTRY getModuleBase(const char* n_func) {

    HMODULE gamelogic = GetModuleHandleA("GameLogic.dll");
    DWORD func = (DWORD)GetProcAddress(gamelogic, n_func);
    if (func == 0) {
        perror("didnt get function address");
    }
    return func;
}
*/

bool APIENTRY Tools::Hook(void* dst, void* src, DWORD size) {

    if (size < 5) {
        perror("at least need 5 bytes space");
        return false;
    }

    DWORD old;
    VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &old);

    memset(dst, NOP, size);

    DWORD offset = ((DWORD)src - (DWORD)dst) - 5;

    *(BYTE *)dst = 0xE9;
    *(DWORD*)((DWORD)dst + 1) = offset;

    DWORD tmp;
    VirtualProtect(dst, size, old, &tmp);
    return true;
}