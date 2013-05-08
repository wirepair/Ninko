#include "Utils.h"

using namespace std;
/* ===================================================================== */
// Utilities
/* ===================================================================== */
// Shamelessly taken and slightly modified from debugtrace example. Totally UNTESTED Btw.
VOID ShowN(THREADID threadid, const char * disasm, ADDRINT eip, UINT32 size, VOID *ea )
{
    UINT8 b[512];
    UINT8* x;
    if (size > 512)
	{
        x = new UINT8[size];
	}
    else
	{
        x = b;
	}
    PIN_SafeCopy( x, static_cast<UINT8*>(ea), size );
	fprintf(g_outfile, "WR [tid:%d] eip:0x%lx %s ; [ea:0x%lx] = ", threadid, eip, disasm, ea);
    for ( UINT32 i = 0; i < size; i++ )
    {
		fprintf(g_outfile, "%02x", static_cast<UINT32>(x[size-i-1]));
        if ( ( ( reinterpret_cast<ADDRINT>(ea)+size-i-1 )&0x3 ) ==0 && i<size-1 )
		{
            fprintf(g_outfile, "_");
		}
    }
	if (size > 512)
	{
        delete [] x;
	}
	fprintf(g_outfile, " [size:%d]\r\n", size);
}

void UpdateIgnoreAddress( ADDRINT base, rapidjson::Value *ignore, const std::string type)
{
	for (rapidjson::Value::ValueIterator itr = ignore->Begin(); itr != ignore->End(); ++itr)
	{		
		if ( itr->IsUint() )
		{
			itr->SetUint( base + itr->GetUint() );
			fprintf(g_outfile, "Ignoring %s @ 0x%lx\r\n", type.c_str(), itr->GetUint());
		}
		else if (itr->IsUint64() )
		{
			itr->SetUint64( base + itr->GetUint64() );
			fprintf(g_outfile, "Ignoring 64 %s @ 0x%lx\r\n", type.c_str(), itr->GetUint64());
		} 
	}
}

void UpdateIgnoredCode( ADDRINT base )
{
	UpdateIgnoreAddress( base, g_vars.code_ignore, "code" );
}

void UpdateIgnoredData( ADDRINT base )
{
	UpdateIgnoreAddress( base, g_vars.data_ignore, "data" );
}

bool compareFiles(const std::string imageName, const std::string targetImage)
{
	if ( imageName.compare(targetImage) == 0 )
	{
		return true;
	}
	string fileOnly = imageName.substr(imageName.find_last_of("/\\")+1);
	if ( targetImage.compare(fileOnly) == 0 )
	{
		return true;
	}
	return false;
}

const char *
dumpInstruction(INS ins)
{
	ADDRINT address = INS_Address(ins);
	std::stringstream ss;

	// Generate instruction byte encoding
	for (size_t i=0;i<INS_Size(ins);i++)
	{
		ss << setfill('0') << setw(2) << hex << (((unsigned int) *(unsigned char*)(address + i)) & 0xFF) << " ";
	}

	for (size_t i=INS_Size(ins);i<8;i++)
	{
		ss << "   ";
	}

	// Generate diassembled string
	ss << INS_Disassemble(ins);

	return _strdup(ss.str().c_str());
}

std::list<REG> *
listRegisters(INS ins)
{
	std::list<REG> *registers = new std::list<REG>;

	for (UINT32 i = 0; i < INS_OperandCount(ins); i++) {
		if (INS_OperandIsReg(ins, i)) {
			REG reg = INS_OperandReg(ins, i);
			registers->push_back(reg);
		}
	}

	return registers;
}
int validateVars()
{
	fprintf(g_outfile, "Validating config...\r\n");
	if ( g_vars.code_start >= g_vars.code_end )
	{
		cerr << "ERROR: code start is >= code end.\r\n" << endl;
		return -1;
	}
	if ( g_vars.data_start >= g_vars.data_end )
	{
		cerr << "ERROR: data start is >= data end.\r\n" << endl;
		return -1;
	}
	return 0;
}
/*!
 *  Print out help message.
 */
INT32 Usage(const std::string err)
{
	cerr << err << endl;
    cerr << KNOB_BASE::StringKnobSummary() << endl;

    return -1;
}

/*!
 * Print out analysis results.
 * This function is called when the application exits.
 * @param[in]   code            exit code of the application
 * @param[in]   v               value specified by the tool in the 
 *                              PIN_AddFiniFunction function call
 */
VOID Fini(INT32 code, VOID *v)
{
	fflush( g_outfile );
	fclose( g_outfile );
}
