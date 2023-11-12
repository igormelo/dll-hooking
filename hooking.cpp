#include <windows.h>
#include <iostream>
#include <string>
#include <stdio.h>

typedef int(__cdecl *CatProc)(LPCTSTR say);

char originalBytes[5];

FARPROC hookedAddress;

int __stdcall myFunc(LPCTSTR say)
{
    HINSTANCE lordDll;
    CatProc catFunc;

    WriteProcessMemory(GetCurrentProcess(), (LPVOID)hookedAddress, originalBytes, 5, NULL);

    lordDll = LoadLibrary("lordDll.dll");
    catFunc = (CatProc)GetProcAddress(lordDll, "Cat");

    return (catFunc)("dLL Hooked");
}

void SuperHook()
{
    HINSTANCE hLib;
    FARPROC myFuncAddress;
    DWORD src;
    DWORD dst;
    CHAR patch[5] = {0};

    hLib = LoadLibraryA("lordDll.dll");
    hookedAddress = GetProcAddress(hLib, "Cat");
    
    ReadProcessMemory(GetCurrentProcess(), (LPCVOID)hookedAddress, originalBytes, 5, NULL);

    myFuncAddress = (FARPROC)&myFunc;
    printf("hookedAddress: %p\n", (LPCVOID)hookedAddress);
    src = (DWORD)hookedAddress + 5;
    printf("hookedAddress + 5: %p\n", src);
    dst = (DWORD)myFuncAddress;

    // Calcula o offset relativo
    DWORD rOffset = dst - src;

    // \xE9 - instrução de JUMP (Assembly)
    memcpy(patch, "\xE9", 1);
    // Copia o offset relativo para os próximos 4 bytes
    memcpy(patch + 1, &rOffset, sizeof(DWORD));

    WriteProcessMemory(GetCurrentProcess(), (LPVOID)hookedAddress, patch, 5, NULL);
}

int main()
{
    HINSTANCE lordDll;
    CatProc catFunc;

    lordDll = LoadLibrary("lordDll.dll");
    catFunc = (CatProc)GetProcAddress(lordDll, "Cat");

    // chama a função Cat original
    (catFunc)("meow-meow");

    // instala o gancho
    SuperHook();

    // chama a função Cat após a instalação do hook
    (catFunc)("meow-meow");

    return 0;
}
