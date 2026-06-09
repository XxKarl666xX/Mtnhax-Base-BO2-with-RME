#include "stdafx.h"
#include "mtnhax_bo2.h"
DWORD originalInstructions[4] = { 0 };
HMODULE gSelfHandle = NULL;
VOID DLL_PROC_ATTACH() {
    XNotifyUI(L"BO2 RME Menu - Draw Hook Installed!");
    memcpy(originalInstructions, (void*)0x824744E0, sizeof(originalInstructions));
    HookFunctionStart((PDWORD)0x824744E0, (PDWORD)Menu_PaintAllStub, (DWORD)paintallHook);
    // auto unload, good for testing only the draw hook (good when you port this to a other cod)
    //Thread((LPTHREAD_START_ROUTINE)UnloadThread);
}
BOOL WINAPI DllMain(HANDLE hInstDLL, DWORD fdwReason, LPVOID lpReserved) {
    switch (fdwReason) {
    case DLL_PROCESS_ATTACH:
        gSelfHandle = (HMODULE)hInstDLL;
        Thread((LPTHREAD_START_ROUTINE)DLL_PROC_ATTACH);
        break;
    case DLL_PROCESS_DETACH:
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    }
    return TRUE;
}
DWORD WINAPI UnloadThread(LPVOID) {
    //Sleep(5000); // when you wanna have auto unloading you should do a small delay before it unloads, duh
    memcpy((void*)0x824744E0, originalInstructions, sizeof(originalInstructions));
    data::menuOpen = false;
    hax = false;
    shouldUnload = true;
    XNotifyUI(L"Visually unloaded. Need to unload via module loader NOW!");
    return 0;
}