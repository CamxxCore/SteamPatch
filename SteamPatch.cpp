#include "stdafx.h"

typedef bool(*IsFAT32Volume_t)(const char * szPath);

IsFAT32Volume_t pOriginalFunc;

bool IsFat32Volume_Hook(const char * szPath)
{
	return false;
}

void steam_patch_init()
{
	auto pattern = BytePattern((BYTE*)"\x55\x8B\xEC\x81\xEC\x00\x00\x00\x00\x8D\x85\x00\x00\x00\x00\x68\x00\x00\x00\x00\xFF\x75\x08", "xxxxx????xx????x????xxx", "steamclient.dll");

	if (!pattern.bSuccess) {

		OutputDebugString(_T("SteamPatch: Failed to initialize."));

		return;
	}

	pOriginalFunc = (IsFAT32Volume_t)DetourFunction((PBYTE)pattern.get(0), (PBYTE)IsFat32Volume_Hook);

	OutputDebugString(_T("SteamPatch: Initialized successfully."));
}

void unload()
{
	if (pOriginalFunc) {

		DetourRemove((PBYTE)pOriginalFunc, (PBYTE)IsFat32Volume_Hook);

		pOriginalFunc = 0;
	}
}