#pragma once
#include "pin.H"
#include <iostream>
#include <list>
#include "NinkoConfig.h"
#include "NinkoFilters.h"
#include "Utils.h"

extern FILE *g_outfile;
extern ninko_t g_vars;

VOID StartLogging( ADDRINT address );
VOID StopLogging( ADDRINT address );

VOID LogCall( THREADID threadID, ADDRINT address, const char * disasm, ADDRINT target, BOOL taken );

VOID CaptureWriteEa(THREADID threadid, VOID * addr);
VOID LogMemoryWrite(THREADID threadid, UINT32 size, const char * disasm, ADDRINT eip );