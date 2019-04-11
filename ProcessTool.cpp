////////////////////////////////////////////////////////////////////7
// @BrainCodec - 11 de Abril de 2019
//
// Código para comprobar si un proceso está en ejecución, buscándolo
// tanto por nombre como por ID (PID)

#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>

namespace BC
{
	constexpr auto EMPTY_WSTRING = L"";

	struct ProcessTool
	{
		ProcessTool(size_t h)
			: m_Hash{ h } {}

		std::wstring ClassName() const
		{
			return L"ProcessTool";
		}

		[[deprecated]] std::wstring ToString() const
		{
			return L"ProcessTool";
		}

		bool IsRunningProcessName(std::wstring const processName) const
		{
			return IsRunningProcess(processName);
		}

		bool IsRunningProcessPID(DWORD pid) const
		{
			return IsRunningProcess(EMPTY_WSTRING, pid);
		}

	private:

		bool IsRunningProcess(std::wstring const processName, DWORD pid = 0) const
		{
			PROCESSENTRY32 entry;

			auto hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPALL /*TH32CS_SNAPPROCESS*/, 0);
			if (hProcessSnap == INVALID_HANDLE_VALUE)
			{
				return FALSE;
			}

			entry.dwSize = sizeof(PROCESSENTRY32);
			if (!::Process32First(hProcessSnap, &entry))
			{
				CloseHandle(hProcessSnap);
				return false;
			}

			do
			{
				bool result = false;
				if ((processName.empty() && (entry.th32ProcessID == pid)) ||
					(std::wcscmp(entry.szExeFile, processName.c_str()) == 0))
				{
#ifdef _DEBUG
					std::wcout << "Exe file name: " << entry.szExeFile << "\n";
#endif
					CloseHandle(hProcessSnap);
					return true;
				}
			} while (::Process32Next(hProcessSnap, &entry));

			return false;
		}
		size_t m_Hash;

	};
}

int main()
{
	auto hash = std::hash<std::wstring>() (L"ProcessTool");
	auto ptool = BC::ProcessTool{hash};
	ptool.IsRunningProcessName(L"notepad.exe");
	ptool.IsRunningProcessPID(21240);
}