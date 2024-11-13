#include <windows.h>
#include <shlobj.h>
#include <shellapi.h>
#include <stdio.h>

//#define SEMAPHORE_NAME "Global\\MySemaphore"
#define MUTEX_NAME "Global\\MyMutex"

typedef int (*target_func)(void*);  // Define a function pointer type
HMODULE hDll = NULL;
target_func target_call = NULL;

int call_target(char *target){
    char target_file_name[MAX_PATH] = {0,};

    if(target) {
        // Load the DLL
        if(!hDll) {
            
            snprintf(target_file_name,MAX_PATH-1,"%s\\%s.dll",target,target);
            // strncpy(target_file_name,target,MAX_PATH);
            // strncat(target_file_name,"\\",MAX_PATH);
            // strncat(target_file_name,target,MAX_PATH);
            // strncat(target_file_name,".dll",MAX_PATH);

            printf("Loading the DLL '%s' ... \n", target_file_name);

            hDll = LoadLibrary(target_file_name);
            if (hDll == NULL) {
                printf("Could not load the DLL. Error: %d\n", GetLastError());
                return 1;
            } else {
                printf("library loaded ok\n");
            }
        }

        if(!target_call) {
            // Get a pointer to the Add function
            target_call = (target_func)GetProcAddress(hDll, "target");
            if (target_call == NULL) {
                printf("Could not locate the function. Error: %d\n", GetLastError());
                FreeLibrary(hDll);
                hDll = NULL;
                return 1; 
            } else {
                printf("function got ok\n");
            }
        }
        // Call the function
        int result = target_call(NULL);
        printf("Result is: %d\n", result);

        // Unload the DLL
        FreeLibrary(hDll);

    } 
    return 0;
}

int main(int argc, char *argv[]) {
    SHFILEOPSTRUCT fileOp = {0};
    HANDLE mutex;
    char * process_name;
    char target_project_name[MAX_PATH] = {0,};


    if (argc != 2) {
        printf("Usage: ChildProcess.exe <ProcessName>\n");
        return 1;
    }

    process_name = argv[1];

    // Open the named semaphore created by the parent process
    // semaphore = OpenSemaphore(SYNCHRONIZE | SEMAPHORE_MODIFY_STATE, FALSE, SEMAPHORE_NAME);
    // if (semaphore == NULL) {
    //     printf("OpenSemaphore error: %d\n", GetLastError());
    //     return 1;
    // }

    mutex = OpenMutex(SYNCHRONIZE, FALSE, MUTEX_NAME);
    if (mutex == NULL) {
        printf("OpenMutex error: %d\n", GetLastError());
        return 1;
    }
    // Wait on the semaphore to control access to the critical section
    DWORD waitResult = WaitForSingleObject(mutex, INFINITE);
    if (waitResult == WAIT_OBJECT_0) {
        // Critical section - simulate process work
        printf("Process '%s' is running...\n", process_name);

        call_target(process_name);

        // Release the semaphore to allow the next process
        //if (!ReleaseSemaphore(semaphore, 1, NULL)) {
        if (!ReleaseMutex(mutex)) {                
            printf("ReleaseSemaphore error: %d\n", GetLastError());
        }

        printf("Process '%s' finished.\n", process_name);
        // clean up
        //Sleep(1000);
        strncpy(target_project_name,process_name,MAX_PATH-2);
        target_project_name[strlen(target_project_name) + 1] = 0;
        printf("Removing '%s' ..\n", target_project_name);
        fileOp.wFunc = FO_DELETE;
        
        fileOp.pFrom = target_project_name;
        fileOp.fFlags = FOF_NO_UI | FOF_NOCONFIRMATION | FOF_SILENT;
        if (SHFileOperation(&fileOp) == 0) {
            printf("Directory removed successfully.\n");
        } else {
            printf("Failed to delete directory.\n");
        }


    } else {
        printf("Process '%s' encountered an error waiting for the semaphore.\n", process_name);
    }

    // Close the semaphore handle
    //CloseHandle(semaphore);
    CloseHandle(mutex);

    return 0;
}
