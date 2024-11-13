// example.h
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>

__declspec(dllexport) int target(void *a);

static char dllFileName[MAX_PATH]; // Global variable to store the DLL name

// example.c
static int counter = 0;

int target(void *a) {
    a = a;
    printf("target init '%s'\n",dllFileName);
    return 0;
}



BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
        case DLL_PROCESS_ATTACH:
            // Get the file name of the DLL
            if (GetModuleFileName(hModule, dllFileName, MAX_PATH) != 0) {
                // Successfully retrieved the DLL file name
                printf("DLL loaded from: %s\n", dllFileName);
            } else {
                // Handle error if needed
                printf("Failed to retrieve DLL file name. Error: %lu\n", GetLastError());
            }
            break;
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
            break;
    }
    return TRUE;
}
