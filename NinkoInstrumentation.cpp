#include "NinkoInstrumentation.h"


VOID ImageLoad( IMG img, VOID *v )
{
	ADDRINT base = IMG_LowAddress(img);
	fprintf(g_outfile, "Loading %s\r\nStart 0x%08x-0x%08x\r\n", IMG_Name(img).c_str(),IMG_LowAddress(img), IMG_HighAddress(img));

	// hook globally.
	if ( g_vars.hook_functions == true )
	{
		HookFunctions( img );
	}
	
	if ( compareFiles( IMG_Name( img ), g_vars.image_name ) == false)
	{
		return;
	}

	// Update vars with base address.
	g_vars.code_start += base;
	g_vars.code_end += base;

	if (g_vars.start_log_on_exec != NULL)
	{
		g_vars.start_log_on_exec += base;
	}
	
	if (g_vars.stop_log_on_exec != NULL)
	{
		g_vars.stop_log_on_exec += base;
	}


	UpdateIgnoredCode( base );
	if (g_vars.code_add != NULL) 
	{
		UpdateCodeAdd( base );
	}
	fprintf(g_outfile, "Monitoring calls from code 0x%lx-0x%lx\r\n", g_vars.code_start, g_vars.code_end);

	g_vars.data_start += base;
	g_vars.data_end += base;			
	UpdateIgnoredData( base );
	if (g_vars.data_add != NULL)
	{
		UpdateDataAdd( base );
	}
	fprintf(g_outfile, "Monitoring data writes from 0x%lx-0x%lx\r\n", g_vars.data_start, g_vars.data_end);
}

/*
 * ObfuscationWriteAndCallLogger - Main function for setting up our analysis routines.
 * First we check if the instructions are in range and that the code isn't ignored.
 *
 * Logging Writes:
 * Next, we check if we care about writes and that the instruction is indeed a write.
 * Setup a generic writer that validates that the target address to be written to is
 * in range of our data_start - data_end and not ignored.
 *
 * Logging Calls:
 * For calls we have two possibilities, one we want to filter out any jmp/calls that
 * jump into the same code block we monitor. Or two, we don't care where the jmps occur
 * and we just want to log all that occur in our monitored code range (code_start - code_end)
 */
VOID ObfuscationWriteAndCallLogger( INS ins, VOID *v, const char *disasm, ADDRINT loc )
{
	if ( g_vars.start_log_on_exec != NULL && IsStartLoggingLoc( loc ) )
	{
		ShouldStartLogging( ins, loc );
	}
	if ( g_vars.stop_log_on_exec != NULL && IsStopLoggingLoc( loc ) )
	{
		ShouldStopLogging( ins, loc );
	}

	// Make sure the call is in the range we care about, and it's not ignored.
	if ( IsInstructionInRange( loc, false ) == 0 || IsCodeIgnored( loc ) == 1 )
	{
		return;
	}

	// make sure we care about even logging writes

	if ( g_vars.disable_log_writes == false && INS_IsMemoryWrite( ins ) )
	{	
		SimpleWriteLogger( ins, v, disasm, loc );
	}

	// For reads
	if ( g_vars.disable_log_reads == false )
	{
		if ( INS_IsMemoryRead(ins) && !INS_IsPrefetch(ins) )
		{
			SimpleReadLogger( ins, v, disasm, loc, false );
		} 
		else if (INS_HasMemoryRead2(ins) )
		{
			SimpleReadLogger( ins, v, disasm, loc, true );
		}
	}


	// make sure we care about logging calls
	if ( g_vars.disable_log_calls == false && INS_IsBranchOrCall( ins ) )
	{
		if ( g_vars.ignore_internal_calls )
		{
			FilteredCallLogger( ins, v, disasm, loc );
		}
		else 
		{
			SimpleCallLogger( ins, v, disasm, loc );
		}
	}	
}

VOID ShouldStartLogging( INS ins, ADDRINT loc )
{
	INS_InsertCall(ins,
		IPOINT_BEFORE,
		AFUNPTR(StartLogging),
		IARG_INST_PTR,
		IARG_END);
}

VOID ShouldStopLogging( INS ins, ADDRINT loc )
{
	INS_InsertCall(ins,
		IPOINT_BEFORE,
		AFUNPTR(StopLogging),
		IARG_INST_PTR,
		IARG_END);
}

VOID SimpleWriteLogger( INS ins, VOID *v, const char *disasm, ADDRINT loc )
{

	INS_InsertIfCall(ins,
		IPOINT_BEFORE, 
		AFUNPTR(IsWriteInRange), 
		IARG_THREAD_ID, 
		IARG_MEMORYWRITE_EA, 
		IARG_END);
	if (INS_HasFallThrough(ins))
    {
		INS_InsertThenCall(ins, 
			IPOINT_AFTER, // note we want AFTER so like, we can see what was written.
			AFUNPTR(LogMemoryWrite), 
			IARG_THREAD_ID, 
			IARG_MEMORYWRITE_SIZE, 
			IARG_PTR, disasm, // disassembled string
			IARG_INST_PTR, // address of instruction
			IARG_END);
	}
	if (INS_IsBranchOrCall(ins))
	{
		INS_InsertThenCall(ins, 
			IPOINT_TAKEN_BRANCH, // note we want AFTER so like, we can see what was written.
			AFUNPTR(LogMemoryWrite), 
			IARG_THREAD_ID, 
			IARG_MEMORYWRITE_SIZE, 
			IARG_PTR, disasm, // disassembled string
			IARG_INST_PTR, // address of instruction
			IARG_END);
	}
}

VOID SimpleReadLogger( INS ins, VOID *v, const char *disasm, ADDRINT loc, bool read_2 )
{
	int read_ea = IARG_MEMORYREAD_EA;

	if ( read_2 )
	{
		read_ea = IARG_MEMORYREAD2_EA;
	}


	INS_InsertIfCall(ins,
		IPOINT_BEFORE, 
		AFUNPTR(IsReadInRange), 
		IARG_THREAD_ID, 
		read_ea, 
		IARG_END);
	
	INS_InsertThenCall(ins, 
			IPOINT_BEFORE, 
			AFUNPTR(LogMemoryRead), 
			IARG_THREAD_ID, 
			read_ea,
			IARG_MEMORYREAD_SIZE, 
			IARG_PTR, disasm, // disassembled string
			IARG_INST_PTR, // address of instruction
			IARG_END);
	
	
}

VOID FilteredCallLogger( INS ins, VOID *v, const char *disasm, ADDRINT loc )
{
	INS_InsertIfCall(ins, 
		IPOINT_BEFORE, 
		(AFUNPTR)IsCallInternal, 
		IARG_BRANCH_TARGET_ADDR,
		IARG_END);

	INS_InsertThenCall(ins,
	       IPOINT_BEFORE,
	       (AFUNPTR)LogCall,
		   IARG_THREAD_ID, // thread ID of the executing thread
		   IARG_INST_PTR, // address of instruction
		   IARG_PTR, disasm, // disassembled string
	       IARG_BRANCH_TARGET_ADDR,	// target
	       IARG_BRANCH_TAKEN,		// Non zero if branch is taken
	       IARG_END);
}

VOID SimpleCallLogger( INS ins, VOID *v, const char *disasm, ADDRINT loc )
{
	INS_InsertCall(ins,
		   IPOINT_BEFORE,
		   (AFUNPTR)LogCall,
		   IARG_THREAD_ID, // thread ID of the executing thread
		   IARG_INST_PTR, // address of instruction
		   IARG_PTR, disasm, // disassembled string
		   IARG_BRANCH_TARGET_ADDR,	// target
		   IARG_BRANCH_TAKEN,			// Non zero if branch is taken
		   IARG_END);
}

VOID Instruction(INS ins, VOID *v)
{
	const char * disasm = dumpInstruction(ins);
	ADDRINT loc = INS_Address(ins);
	ObfuscationWriteAndCallLogger( ins, v, disasm, loc );
 }