#include "NinkoInstrumentation.h"


/*
 * CallLoggers
 */
VOID LogCall( THREADID threadID, ADDRINT address, const char * disasm, 
			  ADDRINT target, BOOL taken )
{
	// make sure this jump is outside of the obfuscated (preferably into .text)
	//0x431dab <= 0x42f330 && 0x431dab >= 0x004633ef
	if ( taken )
	{
		fprintf(g_outfile, "JOC [tid:%d] addr:0x%lx %s ; target:0x%lx addr_name:%s\r\n",
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
				fprintf(g_outfile, "WR [%d] 0x%lx %s ; [0x%lx] = '%c' [%d]\r\n", threadid, eip, disasm, ea, x, size);
			}
			else
			{
				fprintf(g_outfile, "WR [%d] 0x%lx %s ; [0x%lx] = 0x%x [%d]\r\n", threadid, eip, disasm, ea, x, size);
			}
        }
        break;
      case 2:
        {
            UINT16 x;
            PIN_SafeCopy(&x, static_cast<UINT16*>(ea), 2);
			fprintf(g_outfile, "WR [%d] 0x%lx %s ; [0x%lx] = 0x%02x [%d]\r\n", threadid, eip, disasm, ea, x, size);
        }
        break;
      case 4:
        {
            UINT32 x;
            PIN_SafeCopy(&x, static_cast<UINT32*>(ea), 4);
			fprintf(g_outfile, "WR [%d] 0x%lx %s ; [0x%lx] = 0x%08x [%d]\r\n", threadid, eip, disasm, ea, x, size);
        }
        break;
      case 8:
        {
            UINT64 x;
            PIN_SafeCopy(&x, static_cast<UINT64*>(ea), 8);
			fprintf(g_outfile, "WR [%d] 0x%lx %s ; [0x%lx] = 0x%16x [%d]\r\n", threadid, eip, disasm, ea, x, size);
        }
        break;
      default:
        ShowN( threadid, disasm, eip, size, ea );
        break;
    }

	fflush(g_outfile);
}