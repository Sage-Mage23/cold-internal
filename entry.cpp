#pragma once
#include "Dependencies/Includes.h"

void Main()
{
    Module = (std::uintptr_t(IFH(GetModuleHandleA)(0)));

    InitClasses();

    InitEngine();
}

bool DllMain(HMODULE hModule, DWORD bReason, LPVOID lpReserved)
{
    if (bReason == DLL_PROCESS_ATTACH)
    {
        Main();
    }

    return true;
} 