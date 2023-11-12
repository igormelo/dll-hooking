#include <windows.h>
#include <iostream>
#include <string>
#include <stdio.h>

typedef int (__cdecl *CatProc)(LPCTSTR say);
typedef int (__cdecl *BirdProc)(LPCTSTR say);

int main(void) {
  HINSTANCE petDll;
  CatProc catFunc;
  BirdProc birdFunc;
  BOOL freeRes;

  petDll = LoadLibrary("lordDll.dll");
  
  if (petDll != NULL) {
    printf("Library Loaded at Address: 0x%p\n", petDll);
    catFunc = (CatProc) GetProcAddress(petDll, "Cat");
    birdFunc = (BirdProc) GetProcAddress(petDll, "Bird");
    if ((catFunc != NULL) && (birdFunc != NULL)) {
      (catFunc) ("meow-meow");
      (catFunc) ("mmmmeow");
      (birdFunc) ("tweet-tweet");
    }
    freeRes = FreeLibrary(petDll);
  }

  return 0;
}