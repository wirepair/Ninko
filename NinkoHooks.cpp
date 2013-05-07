#include "NinkoHooks.h"


// Add your function hooks here, if you so please.
VOID HookFunctions( IMG img )
{
	RTN rtn;
	if ( (rtn = RTN_FindByName( img, "GetProcAddress" )) != RTN_Invalid() )
	{
		fprintf( g_outfile, "RTN: %s\r\n", RTN_Name( rtn ).c_str());
		HookGetProcAddress( rtn );
	}
	/*
	if ( (rtn = RTN_FindByName( img, "LoadLibrary" )) != RTN_Invalid() )
	{
		fprintf( outFile, "RTN: %s\r\n", RTN_Name( rtn ).c_str());
		HookLoadLibrary( rtn );
	}
	*/
}

// GetProcAddress Hook
static WINDOWS::FARPROC replacementGetProcAddress(
	AFUNPTR functionGetProcAddress,
	WINDOWS::HMODULE hModule,
	WINDOWS::LPCSTR lpProcName,
	CONTEXT *ctx,
	ADDRINT caller)
{
	WINDOWS::FARPROC retFunc = 0;
	char *filename = (char *)calloc(256, sizeof(char));
	PIN_CallApplicationFunction(
		ctx, 
		PIN_ThreadId(),
		CALLINGSTD_STDCALL, functionGetProcAddress,
		PIN_PARG(WINDOWS::FARPROC), &retFunc,
		PIN_PARG(WINDOWS::HMODULE), hModule,
		PIN_PARG(WINDOWS::LPCSTR), lpProcName,
		PIN_PARG_END()
		);
	WINDOWS::GetModuleFileNameA(hModule, filename, 256);
	if (((WINDOWS::DWORD)lpProcName >> 16) == 0)
	{
		fprintf( g_outfile, "%08x = GetProcAddress(%s,ord:%08x) [caller:%08x]\r\n", (unsigned int)retFunc, filename, lpProcName, caller);
	}
	else
	{
		fprintf( g_outfile, "%08x = GetProcAddress(%s,%s) [caller:%08x]\r\n", (unsigned int)retFunc, filename, lpProcName, caller);
	}
	return retFunc;
}

VOID HookGetProcAddress( RTN rtn )
{
	PROTO proto = 
		PROTO_Allocate( PIN_PARG(WINDOWS::FARPROC),
				CALLINGSTD_STDCALL,
				"GetProcAddress",
				PIN_PARG(WINDOWS::HMODULE),			// hModule
				PIN_PARG(WINDOWS::LPCTSTR),			// lpProcName,
				PIN_PARG_END()
				);

	RTN_ReplaceSignature( rtn, (AFUNPTR)replacementGetProcAddress,
			IARG_PROTOTYPE, proto,
			IARG_ORIG_FUNCPTR,
			IARG_FUNCARG_ENTRYPOINT_VALUE, 0,
			IARG_FUNCARG_ENTRYPOINT_VALUE, 1,
			IARG_CONTEXT,
			IARG_RETURN_IP,
			IARG_END
			);

	PROTO_Free( proto );
}

/* LoadLibrary Hook
 * Actually not used, because pin already kinda hooks/lets us know when a module is loaded.
 */
static WINDOWS::HMODULE replacementLoadLibrary(
	AFUNPTR functionLoadLibrary,
	WINDOWS::LPCTSTR lpFileName,
	CONTEXT *ctx,
	ADDRINT eip)
{
	WINDOWS::HMODULE retModule = 0;
	PIN_CallApplicationFunction(
		ctx, 
		PIN_ThreadId(),
		CALLINGSTD_STDCALL, functionLoadLibrary,
		PIN_PARG(WINDOWS::HMODULE), &retModule,
		PIN_PARG(WINDOWS::LPCTSTR), lpFileName,
		PIN_PARG_END()
		);
	fprintf( g_outfile, "%08x = LoadLibrary(%s)\r\n", (unsigned int)retModule, lpFileName);
	return retModule;
}

VOID HookLoadLibrary( RTN rtn )
{
	PROTO proto = 
		PROTO_Allocate( PIN_PARG(WINDOWS::HMODULE),
				CALLINGSTD_STDCALL,
				"LoadLibrary",
				PIN_PARG(WINDOWS::LPCTSTR),			// lpFileName,
				PIN_PARG_END()
				);

	RTN_ReplaceSignature( rtn, (AFUNPTR)replacementLoadLibrary,
			IARG_PROTOTYPE, proto,
			IARG_ORIG_FUNCPTR,
			IARG_FUNCARG_ENTRYPOINT_VALUE, 0,
			IARG_CONTEXT,
			IARG_INST_PTR,
			IARG_END
			);

	PROTO_Free( proto );
}
