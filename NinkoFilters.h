#pragma once
#include "pin.H"
#include "NinkoConfig.h"
#include "NinkoInstrumentation.h"

extern FILE *g_outfile;
extern ninko_s g_vars;

ADDRINT IsStartLoggingLoc( ADDRINT ins );
ADDRINT IsStopLoggingLoc( ADDRINT ins );
ADDRINT IsNotInExactRange( ADDRINT writeAddress );
ADDRINT IsInstructionInRange( ADDRINT ins );
ADDRINT IsIgnored( ADDRINT ins, rapidjson::Value *ignore );
ADDRINT IsCodeIgnored( ADDRINT ins );
ADDRINT IsCallInternal( ADDRINT callAddress );
ADDRINT IsWriteIgnored( ADDRINT ins );
ADDRINT IsWriteInRange( THREADID threadid, VOID * writeAddress );

