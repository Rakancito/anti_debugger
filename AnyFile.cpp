// Add on any file in your client

void AntiDebugScanProcess(){
	BOOL  HasDebugPort = FALSE;
	CheckRemoteDebuggerPresent(GetCurrentProcess(), &HasDebugPort);
	if (HasDebugPort)
	{
		TraceError("Please close your debugging app and restart the program");
		vKillProcess(APP_PROCESS);
	}
}

bool vKillProcess(const char *filename)
{
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);
	PROCESSENTRY32 pEntry;
	pEntry.dwSize = sizeof(pEntry);
	BOOL hRes = Process32First(hSnapShot, &pEntry);
	while (hRes)
	{
		if (strcmp(pEntry.szExeFile, filename) == 0)
		{
			HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, 0, (DWORD)pEntry.th32ProcessID);
			if (hProcess != NULL)
			{
				TerminateProcess(hProcess, 1);
				CloseHandle(hProcess);
				return true;
			}
		}
		hRes = Process32Next(hSnapShot, &pEntry);
	}
	CloseHandle(hSnapShot);
	return false;
}

void initThreadAntiDebug(){
	CreateThread(NULL, NULL, LPTHREAD_START_ROUTINE(vScan_AntiDebug_thread), NULL, 0, 0);
}

void vScan_AntiDebug_thread(){
	Sleep(2000);
again:
	AntiDebugScanProcess();
	Sleep(2000);
	goto again;
}
