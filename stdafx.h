#pragma once

#include <windows.h>

#include <cstdio>

#include <inttypes.h>

#include <tchar.h>

#include <detours.h>

#include "pattern.h"

void steam_patch_init();

void unload();