#pragma once
#include "pin.H"
#include <iostream>

#include "NinkoConfig.h"


namespace WINDOWS {
#include "Windows.h"
}
extern FILE *g_outfile;

VOID HookFunctions( IMG img );
VOID HookLoadLibrary( RTN rtn );
VOID HookGetProcAddress( RTN rtn );
BOOL IsNotSelfCall( ADDRINT call );

static WINDOWS::FARPROC replacementGetProcAddress( AFUNPTR functionGetProcAddress, WINDOWS::HMODULE hModule, WINDOWS::LPCSTR lpProcName, CONTEXT *ctx, ADDRINT caller );
static WINDOWS::HMODULE replacementLoadLibrary(	AFUNPTR functionLoadLibrary, WINDOWS::LPCTSTR lpFileName, CONTEXT *ctx, ADDRINT eip );