#pragma once
#include "pin.H"
#include "NinkoAnalysisRoutines.h"
#include "NinkoFilters.h"
#include "NinkoHooks.h"
#include "Utils.h"
#include "NinkoConfig.h"

extern FILE *g_outfile;
extern ninko_t g_vars;

VOID ImageLoad( IMG img, VOID *v );
VOID Instruction( INS ins, VOID *v );

VOID ShouldStartLogging( INS ins, ADDRINT loc );
VOID ShouldStopLogging( INS ins, ADDRINT loc );

VOID ObfuscationWriteAndCallLogger( INS ins, VOID *v, const char *disasm, ADDRINT loc );

VOID FilteredCallLogger( INS ins, VOID *v, const char *disasm, ADDRINT loc );
VOID SimpleCallLogger( INS ins, VOID *v, const char *disasm, ADDRINT loc );

VOID SimpleWriteLogger( INS ins, VOID *v, const char *disasm, ADDRINT loc );
