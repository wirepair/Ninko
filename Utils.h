#pragma once
#include "pin.H"
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <list>

#include "NinkoConfig.h"
namespace WINDOWS {
#include "Windows.h"
}
extern std::ostream * out;
extern FILE *g_outfile;
extern ninko_t g_vars;

VOID Fini( INT32 code, VOID *v );
INT32 Usage( const std::string err );

VOID ShowN(THREADID threadid, const char * disasm, ADDRINT eip, UINT32 n, VOID *ea );

void UpdateBaseAddress( ADDRINT base, rapidjson::Value *ignore);
void UpdateIgnoredCode( ADDRINT base );
void UpdateIgnoredData( ADDRINT base );
void UpdateCodeAdd( ADDRINT base );
void UpdateDataAdd( ADDRINT base );


int validateVars();
bool compareFiles( const std::string imageName, const std::string targetImage );
std::list<REG> *listRegisters( INS ins );
const char *dumpInstruction( INS ins );