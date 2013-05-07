#pragma once

/* ================================================================== */
// Global variables 
/* ================================================================== */

UINT64 insCount = 0;        //number of dynamically executed instructions
UINT64 bblCount = 0;        //number of dynamically executed basic blocks
UINT64 threadCount = 0;     //total number of threads, including main thread

std::ostream * out = &cerr;

/* ===================================================================== */
// Command line switches
/* ===================================================================== */
KNOB<string> KnobOutputFile(KNOB_MODE_WRITEONCE,  "pintool", "o", "", "specify file name for Ninko output");
KNOB<string> KnobConfigFile(KNOB_MODE_WRITEONCE,  "pintool", "c", "ninko.config", "specify config file name for Ninko");
KNOB<BOOL>   KnobCount(KNOB_MODE_WRITEONCE,  "pintool", "count", "1", "count instructions, basic blocks and threads in the application");

