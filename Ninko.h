#pragma once
#include "pin.H"

#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>

#include "NinkoConfig.h"
/* ================================================================== */
// Global variables 
/* ================================================================== */

FILE *g_outfile = NULL;
NinkoConfig g_config = NinkoConfig();
ninko_t g_vars;

/* ===================================================================== */
// Command line switches
/* ===================================================================== */
KNOB<string> KnobConfigFile(KNOB_MODE_WRITEONCE,  "pintool", "c", "ninko.config", "specify config file for Ninko");

