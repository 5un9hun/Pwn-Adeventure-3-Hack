// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <vector>
#include <winnt.h>
#include <cstring>
#include "tools.h"
#include "main.h"

#define _CRT_SECURE_NO_WARNINGS

FILE* f1 = NULL, * f2 = NULL, * f3 = NULL;

DWORD chatJmpBackAddy;
DWORD GameWorld;
DWORD GameAPI;
DWORD IPlayerVFTable;

typedef void* (__thiscall* _GetItemByName)(void* gameAPI, const char* name);
_GetItemByName GetItemByName;

typedef char (__thiscall* _PerformAddItem)(void* player, void* item, unsigned int count, bool allowPartial);
_PerformAddItem PerformAddItem;

typedef void(__thiscall* _SendAddItemEvent)(void* server, void* player, void* item, unsigned int count);
_SendAddItemEvent SendAddItemEvent;

void SetPosition(Vector3 *new_vec) {
    float* vec_x = (float*)Tools::FindDMA(GameWorld, { 0x1C, 0x4, 0x114, 0x90 });
    float* vec_y = (float*)Tools::FindDMA(GameWorld, { 0x1C, 0x4, 0x114, 0x94 });
    float* vec_z = (float*)Tools::FindDMA(GameWorld, { 0x1C, 0x4, 0x114, 0x98 });
    *vec_x = new_vec->x;
    *vec_y = new_vec->y;
    *vec_z = new_vec->z;
    printf("[DEBUG] X: %.2f Y: %.2f Z: %.2f\n", *vec_x, *vec_y, *vec_z);
}

Vector3* GetPosition() {
    Vector3* vec = new Vector3();
    float* vec_x = (float*)Tools::FindDMA(GameWorld, { 0x1C, 0x4, 0x114, 0x90 });
    float* vec_y = (float*)Tools::FindDMA(GameWorld, { 0x1C, 0x4, 0x114, 0x94 });
    float* vec_z = (float*)Tools::FindDMA(GameWorld, { 0x1C, 0x4, 0x114, 0x98 });
    vec->x = *vec_x;
    vec->y = *vec_y;
    vec->z = *vec_z;
    return vec;
}

DWORD check(char* s) {
    if (!memcmp(TP, s, 3)) {
        return 1;
    }
    else if (!memcmp(TPX, s, 4)) {
        return 2;
    }
    else if (!memcmp(TPY, s, 4)) {
        return 3;
    }
    else if (!memcmp(TPZ, s, 4)) {
        return 4;
    }
    /*else if (!memcmp(GIVE, s, 5)) {
        return 5;
    }*/
    return 0;
}

DWORD length_check(char* s) {
    if (strlen(s+0x40) >= 0x10) {
        return 1;
    }
    return 0;
}

void __declspec(naked) client_chat() {

    char* tmp[300]; 
    /*
    if(text[0] == "!") {
        if(memcmp("tp ", text[1:4], 3)) {
            Vector3* vec = new Vector3();
            sscanf(text + 3, "%f %f %f", &(vec->x), &(vec->y), &(vec->z));
            this->SetPosition(*new_pos);
        }
        if(memcmp("tpx ", text[1:5], 4)) {
            Vector3* vec = this->GetPosition();
            sscanf(text + 4, "%f", &(vec->x));
            this->SetPosition(*new_pos);
        }
        if(memcmp("tpy ", text[1:5], 4)) {
            Vector3* vec = this->GetPosition();
            sscanf(text + 4, "%f", &(vec->y));
            this->SetPosition(*new_pos);
        }
        if(memcmp("tpz ", text[1:5], 4)) {
            Vector3* vec = this->GetPosition();
            sscanf(text + 4, "%f", &(vec->z));
            this->SetPosition(*new_pos);
        }
        if(memcmp("give ", text[1:5], 4)) {
            sscanf(text + 5, "%s %d", tmp);
            void *item = GetItemByName(GameAPI, "name")
            AddItem
        }
    }
    */

    __asm {
    GO:
        nop
        nop

        mov ebx, [ebp + 0xc] ; text
        mov esi, [ebp + 0x8] ; Player

        push ebx
        call length_check
        add esp, 4
        cmp eax, 0
        jz ABOVE
        add ebx, 0x40

    ABOVE:
        push ebx
        push DEBUG_PRINT
        call printf
        add esp, 8

        xor ecx, ecx
        mov dl, [ebx+ecx]
        cmp dl, 0x21
        jnz RETURN
        
        inc ebx
        push ebx
        call check
        add esp, 4
        cmp al, 1
        jz TP_branch
        cmp al, 2
        jz TPX_branch
        cmp al, 3
        jz TPY_branch
        cmp al, 4
        jz TPZ_branch
        jmp RETURN

    TP_branch:
        push 12
        call malloc
        add esp, 4
        push eax
        
        lea edx, [eax + 8]
        push edx
        lea edx, [eax + 4]
        push edx
        push eax
        mov edx, FORMAT_XYZ
        push edx
        lea edx, [ebx + 3]
        push edx
        call sscanf
        add esp, 0x14
        
        mov edx, [esp]
        push edx
        call SetPosition
        add esp, 4
        pop edx
        push edx
        call free
        add esp, 4
        jmp RETURN

    TPX_branch:
        call GetPosition
        push eax
        mov edx, FORMAT
        push edx
        lea edx, [ebx + 4]
        push edx
        call sscanf
        add esp, 0x8

        mov edx, [esp]
        push edx
        call SetPosition
        add esp, 4
        pop edx
        push edx
        call free
        add esp, 4
        jmp RETURN

    TPY_branch:
        call GetPosition
        push eax
        lea edx, [eax + 4]
        push edx
        mov edx, FORMAT
        push edx
        lea edx, [ebx + 4]
        push edx
        call sscanf
        add esp, 0xC

        mov edx, [esp]
        push edx
        call SetPosition
        add esp, 4
        pop edx
        push edx
        call free
        add esp, 4
        jmp RETURN

    TPZ_branch:
        call GetPosition
        push eax
        lea edx, [eax + 8]
        push edx
        mov edx, FORMAT
        push edx
        lea edx, [ebx + 4]
        push edx
        call sscanf
        add esp, 0xC

        mov edx, [esp]
        push edx
        call SetPosition
        add esp, 4
        pop edx
        push edx
        call free
        add esp, 4
        jmp RETURN

    RETURN:
        jmp[chatJmpBackAddy]
    }
    /*
    GIVE_branch:
        push 0x34
        call malloc
        add esp, 4
        push eax

        lea edx, [eax + 0x30]
        push edx
        push eax
        mov edx, FORMAT_ITEM
        push edx
        lea edx, [ebx + 5]
        push edx
        call sscanf
        add esp, 0x10

        mov edx, [esp]
        push edx
        mov ecx, GameAPI
        call GetItemByName
        mov edx, [esp]
        lea edx, [edx + 0x30]
        mov edx, [edx]
        cmp edx, 0
        jg COMSUMABLE
 
    EQUIPMENT:
        push 0
        inc edx
        push edx
        push eax
        mov ecx, IPlayerVFTable
        call PerformAddItem
        pop edx
        push edx
        call free
        add esp, 0x4
        jmp RETURN

    COMSUMABLE:
        push 1
        push edx
        push eax
        mov ecx, IPlayerVFTable
        call PerformAddItem
        pop edx
        push edx
        call free
        add esp, 0x4
        jmp RETURN
    */
}

DWORD APIENTRY Hack(HMODULE hModule) {

    // new console, redirect console
    AllocConsole();

    f1 = freopen("CONIN$", "rb", stdin);
    f2 = freopen("CONOUT$", "wb", stdout);
    f3 = freopen("CONOUT$", "wb", stderr);

    bool flyhack = false;
    bool speedhack = false;
    bool god_mode = false;

    std::cout << "==== Hack Test ====" << std::endl;
    std::cout << "[F1] switching fly hack" << std::endl;
    std::cout << "[F2] switching speed hack" << std::endl;
    std::cout << "!tp x y z" << std::endl;
    std::cout << "!tpx x" << std::endl;
    std::cout << "!tpy y" << std::endl;
    std::cout << "!tpz z" << std::endl;
    std::cout << "[`] exit" << std::endl;

    DWORD gamelogic = (DWORD)GetModuleHandleA("GameLogic.dll");
    GameWorld = gamelogic + 0x00097D7C;
    GameAPI = gamelogic + 0x97D80;
    IPlayerVFTable = (DWORD)Tools::FindDMA(GameWorld, { 0x1c, 0x6c, 0x0 }) - 0x70;
    GetItemByName = (_GetItemByName)(gamelogic + 0x1DE20);
    PerformAddItem = (_PerformAddItem)(gamelogic + 0x51C20);
    SendAddItemEvent = (_SendAddItemEvent)(gamelogic + 0x5C150);
    
    // hooking ClientWorld::Chat 
    DWORD Chat = gamelogic + 0xD22F;
    chatJmpBackAddy = Chat + 7;
    Tools::Hook((void*)Chat, client_chat, 7);

    while (true) {

        // exit
        if (GetAsyncKeyState(VK_OEM_3) & 1) {
            break;
        }

        // fly hack
        if (GetAsyncKeyState(VK_F1) & 1) {
            if (!flyhack) {
                std::cout << "[*] enable fly hack" << std::endl;
                Tools::PatchNOP((BYTE *)(gamelogic + 0x51680), 0x10);
                flyhack = true;
            }
            else {
                std::cout << "[*] disable fly hack" << std::endl;
                Tools::Patch((BYTE*)(gamelogic + 0x51680), (BYTE *)"\x8B\x49\x9C\x85\xC9\x74\x07\x8B\x01\x8B\x40\x50\xFF\xE0\x32\xC0", 0x10);
                flyhack = false;
            }
        }

        // speed hack
        if (GetAsyncKeyState(VK_F2) & 1) {
            DWORD* walk_speed = Tools::FindDMA(GameWorld, { 0x1C, 0x6C, 0x120 });
            DWORD* jump_speed = Tools::FindDMA(GameWorld, { 0x1C, 0x6C, 0x124 });
            DWORD* jump_hold_time = Tools::FindDMA(GameWorld, { 0x1C, 0x6C, 0x128 });
            if (!speedhack) {
                std::cout << "[*] enable speed hack" << std::endl;
                *(float*)walk_speed = 4000;
                *(float*)jump_speed = 2000;
                *(float*)jump_hold_time = 30.0;
                speedhack = true;
            }
            else {
                std::cout << "[*] disable speed hack" << std::endl;
                *(float*)walk_speed = 200;
                *(float*)jump_speed = 420;
                *(float*)jump_hold_time = 0.2;
                speedhack = false;
            }
        }
    }

    fclose(f1); fclose(f2); fclose(f3);
    FreeConsole();
    FreeLibraryAndExitThread(hModule, 0);
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        HANDLE thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Hack, hModule, 0, NULL);
        if (thread)
            CloseHandle(thread);
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}