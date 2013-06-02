#include "NinkoInstrumentation.h"

VOID StartLogging( ADDRINT address )
{
	g_vars.logging = true;
	fprintf(g_outfile, "Logging Enabled\r\n");
}

VOID StopLogging( ADDRINT address )
{
	g_vars.logging = false;
	fprintf(g_outfile, "Logging Disabled\r\n");
}

/*
 * CallLoggers
 */
VOID LogCall( THREADID threadID, ADDRINT address, const char * disasm, 
			  ADDRINT target, BOOL taken )
{
	if (g_vars.logging == false)
	{
		return;
	}
	// make sure this jump is outside of the obfuscated (preferably into .text)
	if ( taken )
	{
		fprintf(g_outfile, "CL [tid:%d] eip:0x%lx %s ; target:0x%lx name:%s\r\n",
				threadID, address, disasm, target,  RTN_FindNameByAddress(target).c_str());
	}
}

/*
 * Memory Write Loggers
 */
// Shamelessly taken from debugtrace example.
LOCALVAR VOID *WriteEa[PIN_MAX_THREADS];

VOID CaptureWriteEa(THREADID threadid, VOID * addr)
{
    WriteEa[threadid] = addr;
}
// Shamelessly taken and slightly modified from debugtrace example.
VOID LogMemoryWrite(THREADID threadid, UINT32 size, const char * disasm, ADDRINT eip )
{

	if (g_vars.logging == false)
	{
		return;
	}
    VOID * ea = WriteEa[threadid];
    
    switch(size)
    {
      case 0:
        break;
      case 1:
        {
            UINT8 x;
            PIN_SafeCopy(&x, static_cast<UINT8*>(ea), 1);
			if (x > 0x20 && x < 0x7f)
			{
				fprintf(g_outfile, "WR [tid:%d] eip:0x%lx %s ; [ea:0x%lx] = '%c' [size:%d]\r\n", threadid, eip, disasm, ea, x, size);
			}
			else
			{
				fprintf(g_outfile, "WR [tid:%d] eip:0x%lx %s ; [ea:0x%lx] = 0x%x [size:%d]\r\n", threadid, eip, disasm, ea, x, size);
			}
        }
        break;
      case 2:
        {
            UINT16 x;
            PIN_SafeCopy(&x, static_cast<UINT16*>(ea), 2);
			fprintf(g_outfile, "WR [tid:%d] eip:0x%lx %s ; [ea:0x%lx] = 0x%02x [size:%d]\r\n", threadid, eip, disasm, ea, x, size);
        }
        break;
      case 4:
        {
            UINT32 x;
            PIN_SafeCopy(&x, static_cast<UINT32*>(ea), 4);
			fprintf(g_outfile, "WR [tid:%d] eip:0x%lx %s ; [ea:0x%lx] = 0x%08x [size:%d]\r\n", threadid, eip, disasm, ea, x, size);
        }
        break;
      case 8:
        {
            UINT64 x;
            PIN_SafeCopy(&x, static_cast<UINT64*>(ea), 8);
			fprintf(g_outfile, "WR [tid:%d] eip:0x%lx %s ; [ea:0x%lx] = 0x%16x [size:%d]\r\n", threadid, eip, disasm, ea, x, size);
        }
        break;
      default:
        ShowN( threadid, disasm, eip, size, ea );
        break;
    }

	fflush(g_outfile);
}