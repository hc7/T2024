#include <windows.h>
#include <stdio.h>

//#define SEMAPHORE_NAME "Global\\MySemaphore"
#define MUTEX_NAME "Global\\MyMutex"

int main(int argc, char *argv[]) {
    //HANDLE semaphore;
    HANDLE mutex;
    int processId;

    if (argc != 2) {
        printf("Usage: ChildProcess.exe <ProcessID>\n");
        return 1;
    }

    processId = atoi(argv[1]);

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
        printf("Process %d is running...\n", processId);
        Sleep(2000);  // Simulate some work with a delay

        // Release the semaphore to allow the next process
        //if (!ReleaseSemaphore(semaphore, 1, NULL)) {
        if (!ReleaseMutex(mutex)) {                
            printf("ReleaseSemaphore error: %d\n", GetLastError());
        }

        printf("Process %d finished.\n", processId);
    } else {
        printf("Process %d encountered an error waiting for the semaphore.\n", processId);
    }

    // Close the semaphore handle
    //CloseHandle(semaphore);
    CloseHandle(mutex);

    return 0;
}
