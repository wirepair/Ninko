#pragma once
#include "pin.H"
#include "rapidjson/document.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>

using namespace std;

typedef struct ninko_s {
	string image_name;
	bool logging;
	bool hook_functions;
	bool ignore_internal_calls;
	bool disable_log_calls;
	bool disable_log_writes;
	bool disable_log_reads;
	ADDRINT code_start;
	ADDRINT code_end;
	ADDRINT data_start;
	ADDRINT data_end;
	ADDRINT stop_log_on_exec;
	ADDRINT start_log_on_exec;
	rapidjson::Value* code_ignore;
	rapidjson::Value* data_ignore;
	rapidjson::Value* code_add;
	rapidjson::Value* data_add;
} ninko_t;

class NinkoConfig {
	public:
		int parse(const string fileName);
		void fillVars(ninko_t *vars);
		// doc variable reading functions
		bool fieldExists(const string fieldName);
		ADDRINT getAddr(const string fieldName);
		string getString(const string fieldName);
		rapidjson::Value* getArray(const string fieldName);
		bool getBool(const string fieldName);
		// public vars
	private:
		rapidjson::Document m_doc;
};