#pragma once

#include <Psapi.h>

template <typename T>
class Pattern
{
public:
	Pattern(const BYTE* bMask, const char* szMask, const char * szModule) : bMask(bMask), szMask(szMask)
	{
		bSuccess = findPattern(szModule);
	}

	Pattern(const BYTE* bMask, const char* szMask) : Pattern(bMask, szMask, NULL) {}

	T get(int offset = 0)
	{
		return pResult + offset;
	}

	bool bSuccess;

private:
	bool findPattern(const char * moduleName)
	{
		MODULEINFO module = {};

		GetModuleInformation(GetCurrentProcess(), GetModuleHandle(moduleName), &module, sizeof(MODULEINFO));

		auto *address = reinterpret_cast<BYTE*>(module.lpBaseOfDll);

		auto address_end = address + module.SizeOfImage;

		for (; address < address_end; address++)
		{
			if (bCompare((BYTE*)address, bMask, szMask))
			{
				pResult = reinterpret_cast<T>(address);
				return true;
			}
		}

		pResult = NULL;
		return false;
	}

	static bool bCompare(const BYTE* pData, const BYTE* bMask, const char* szMask)
	{
		for (; *szMask; ++szMask, ++pData, ++bMask)
			if (*szMask == 'x' && *pData != *bMask)
				return false;
		return (*szMask) == NULL;
	}

	const BYTE *bMask; const char* szMask;

	T pResult;
};

class BytePattern : public Pattern<BYTE*>
{
public:
	BytePattern(const BYTE* bMask, const char* szMask) :
		Pattern<BYTE*>(bMask, szMask) {}

	BytePattern(const BYTE* bMask, const char* szMask, const char * szModule) :
		Pattern<BYTE*>(bMask, szMask, szModule) {}
};