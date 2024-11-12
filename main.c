#include <windows.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <tchar.h>

typedef struct s_ {
    char a;
    
    unsigned short int a1;
    unsigned short int a2;
    unsigned int c;

    
} s_type;


int countDigitOne(int n) {
    int s = 0;
    int i = 0;
    long long int digit;
    int number[10];

    for(digit=10, i = 0; ((digit < n) && (digit < 1000000000)); i+=1) {

        number[i] = n % digit;
        printf("%d ",number[i]);
    }
    //n / 100
    if (n / 100) {
        s += 1;
    }

    while(n) {
        if((n % 10) == 1) s += 1;
        if((n / 10) == 1) s += 1;
        n-=1;
    }

    return s;

}

#if 0 
int main( int argc, TCHAR *argv[]){
    

    // printf("Hello, from test2! %d %d %d\n",offsetof(s_type, a),offsetof(s_type, a1),offsetof(s_type, c));

    // int v = countDigitOne(13);
    // printf("%d %d\n",v,v==6);
    // v = countDigitOne(0);
    // printf("%d %d\n",v,v==0);
    // v = countDigitOne(100);
    // printf("%d %d\n",v,v==21);

 

    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );

    if( argc != 2 )
    {
        printf("Usage: %s [cmdline]\n", argv[0]);
        return;
    }

    // Start the child process. 
    if( !CreateProcess( NULL,   // No module name (use command line)
        argv[1],        // Command line
        NULL,           // Process handle not inheritable
        NULL,           // Thread handle not inheritable
        FALSE,          // Set handle inheritance to FALSE
        0,              // No creation flags
        NULL,           // Use parent's environment block
        NULL,           // Use parent's starting directory 
        &si,            // Pointer to STARTUPINFO structure
        &pi )           // Pointer to PROCESS_INFORMATION structure
    ) 
    {
        printf( "CreateProcess failed (%d).\n", GetLastError() );
        return;
    }

    // Wait until child process exits.
    WaitForSingleObject( pi.hProcess, INFINITE );

    printf("finished\n");

    // Close process and thread handles. 
    CloseHandle( pi.hProcess );
    CloseHandle( pi.hThread );    
}
#endif

#define MAX_CONCURRENT_PROCESSES 3  // Max number of concurrent processes
#define TOTAL_PROCESSES 10          // Total number of processes in pool
#define SEMAPHORE_NAME "Global\\MySemaphore"  // Unique name for the semaphore
#define MUTEX_NAME "Global\\MyMutex"   // Unique name for the mutex

void CreateProcessInPool(int processId);

int main() {
    // HANDLE semaphore;

    // // Create a named semaphore with initial and max count as MAX_CONCURRENT_PROCESSES
    // semaphore = CreateSemaphore(NULL, MAX_CONCURRENT_PROCESSES, MAX_CONCURRENT_PROCESSES, SEMAPHORE_NAME);
    // if (semaphore == NULL) {
    //     printf("CreateSemaphore error: %d\n", GetLastError());
    //     return 1;
    // }

    HANDLE mutex;

    mutex = CreateMutex(NULL, FALSE, MUTEX_NAME);
    if (mutex == NULL) {
        printf("CreateMutex error: %d\n", GetLastError());
        return 1;
    }

    // Create a pool of processes
    for (int i = 0; i < TOTAL_PROCESSES; i++) {
        CreateProcessInPool(i + 1);  // Pass process ID to child process
    }

    // Close the local semaphore handle since child processes have their own references
    CloseHandle(mutex);

    printf("All processes have been created.\n");
    return 0;
}

void CreateProcessInPool(int processId) {
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    char commandLine[128];

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    // Prepare command line with process ID
    snprintf(commandLine, 128,"ChildProcess.exe %d", processId);

    // Create child process with command line argument
    if (!CreateProcess(NULL, commandLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        printf("CreateProcess error : %d\n", GetLastError());
        return;
    }

    // Close process and thread handles for this instance as they are no longer needed
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}
