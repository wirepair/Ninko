#include "Ninko.h"
#include "Utils.h"
#include "NinkoConfig.h"
#include "NinkoAnalysisRoutines.h"

/*!
 * The main procedure of the tool.
 * This function is called when the application image is loaded but not yet started.
 * Parses our json config first then adds our instrumentation functions.
 * @param[in]   argc            total number of elements in the argv array
 * @param[in]   argv            array of command line arguments, 
 *                              including pin -t <toolname> -- ...
 */
int main(int argc, char *argv[])
{
	int ret = 0;
	PIN_InitSymbols();
	if( PIN_Init(argc,argv) )
    {
        return Usage("no args");
    }

    string configFileName = KnobConfigFile.Value();
	if ( !configFileName.empty() ) 
	{
		if ( g_config.parse( configFileName ) != 0 )  
		{
			cerr << ret << endl;
			return Usage( "ERROR: Invalid configuration" );
		}
	}
	else
	{
		return Usage( "ERROR: Configuration file is required." );
	}

	if ( g_config.fieldExists( "outfile" ) == false ) 
	{
		return Usage( "ERROR: outfile parameter is required." );
	}

	if ( ( ret = fopen_s( &g_outfile, g_config.getString( "outfile" ).c_str(), "wb" ) ) != 0 )
	{
		cerr << "ERROR: outfile is invalid. fopen failed: " << ret << endl;
		return ret;
	}
	
	g_config.fillVars( &g_vars );
	if (validateVars() != 0)
	{
		return 0;
	}
	IMG_AddInstrumentFunction(ImageLoad, 0);
	
	INS_AddInstrumentFunction(Instruction, 0);
	
	// Register function to be called when the application exits
    PIN_AddFiniFunction(Fini, 0);

    // Start the program, never returns
    PIN_StartProgram();
	
    return 0;
}