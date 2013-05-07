#include "NinkoFilters.h"

ADDRINT IsWriteIgnored( ADDRINT ins )
{
	return IsIgnored( ins, g_vars.data_ignore );
}
ADDRINT IsCodeIgnored( ADDRINT ins )
{
	return IsIgnored( ins, g_vars.code_ignore );
}

/*
 * Returns 1 if we should ignore.
 *         0 if we should not ignore
 */ 
ADDRINT IsIgnored( ADDRINT ins, rapidjson::Value *ignore)
{
	for (rapidjson::Value::ConstValueIterator itr = ignore->Begin(); itr != ignore->End(); ++itr)
	{
		if ( itr->IsUint() )
		{
			if ( ins == static_cast<ADDRINT>(itr->GetUint()) )
			{
				return 1;
			}
		}
		else if (itr->IsUint64() )
		{
			if ( ins == static_cast<ADDRINT>(itr->GetUint64()) )
			{
				return 1;
			}
		} 
	}
	return 0;
}
/*
 * This checks to make sure the instruction is in the range of code we are monitoring.
 * Returns 1 if the instruction is in range
 *         0 ins is out of our code range.
 */
ADDRINT IsInstructionInRange( ADDRINT ins )
{
	if ( ins >= g_vars.code_start && ins <= g_vars.code_end )
	{
		return 1;
	}
	return 0;
}

/*
 * This checks to make sure the address that will be written is in the range of data we are monitoring.
 * Returns 1 if the write address is in the range (i.e. we want to ignore the write)
 *         0 the write address is outside of the range we care about.
 */
ADDRINT IsWriteInRange( THREADID threadid, VOID * writeAddress )
{
	if ( ( !IsWriteIgnored((ADDRINT)writeAddress ) ) && ( (ADDRINT)writeAddress >= g_vars.data_start && (ADDRINT)writeAddress <= g_vars.data_end ) )
	{
		CaptureWriteEa( threadid, writeAddress );
		return 1;
	}
	return 0;
}
/*
 * This checks to see if the target for the call is contained in our code monitoring range.
 * You would want to use this if you don't care about calls that occur inside of obfuscated code.
 */
ADDRINT IsCallInternal( ADDRINT callAddress )
{
	return !IsInstructionInRange( callAddress );
}