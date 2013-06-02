#include "NinkoConfig.h"

int NinkoConfig::parse(const std::string fileName)
{
	char * config = NULL;
	ifstream::pos_type size;

	cerr << "Opening configuration file: " << fileName << endl;
	std::ifstream config_file(fileName.c_str(), ios::in|ios::binary|ios::ate);

	if (config_file.is_open())
	{
		size = config_file.tellg();
		config_file.seekg (0, ios::beg);
		config = new char [size];
		config_file.read(config, size);
		config_file.close();
		cerr << "Config file contents: " << config << endl;
	}
	
	if (config == NULL)
	{
		return -1;
	}

	m_doc.Parse<0>(config);
	if (m_doc.IsObject() == false)
	{
		cerr << "Document is not a valid object: " << m_doc.GetParseError() << endl;
		return -1;
	}
	return 0;
}
void NinkoConfig::fillVars( ninko_t *vars )
{
	vars->code_start = getAddr( "code_start" );
	vars->code_end = getAddr( "code_end" );
	vars->code_ignore = getArray( "code_ignore" );
	vars->code_add = getArray( "code_add" );

	vars->ignore_internal_calls = getBool( "ignore_internal_calls" );
	vars->dont_log_calls = getBool( "dont_log_calls" );
	vars->dont_log_writes = getBool( "dont_log_writes" );

	vars->data_start = getAddr( "data_start" );
	vars->data_end = getAddr( "data_end" );
	vars->data_ignore = getArray( "data_ignore" );
	vars->data_add = getArray( "data_add" );

	vars->image_name = getString( "image_name" );
	vars->hook_functions = getBool( "hook_functions" );
	
	vars->stop_log_on_exec = getAddr( "stop_log_on_exec" );
	vars->start_log_on_exec = getAddr( "start_log_on_exec" );
	
	if (vars->start_log_on_exec != NULL)
	{
		vars->logging = false;
	}
	else
	{
		vars->logging = true;
	}

}
bool NinkoConfig::fieldExists(const std::string fieldName)
{
	if (!m_doc.HasMember(fieldName.c_str()))
	{
		return false;
	}
	return true;
}

std::string NinkoConfig::getString(const std::string fieldName)
{
	if (fieldExists(fieldName) && m_doc[fieldName.c_str()].IsString())
	{
		return m_doc[fieldName.c_str()].GetString();
	}
	cerr << "Invalid string specified for " << fieldName << endl;
	return NULL;
}

ADDRINT NinkoConfig::getAddr(const std::string fieldName)
{
	if(fieldExists(fieldName) && m_doc[fieldName.c_str()].IsUint())
	{
		return static_cast<ADDRINT>(m_doc[fieldName.c_str()].GetUint());
	}
	else if (fieldExists(fieldName) && m_doc[fieldName.c_str()].IsUint64())
	{
		return static_cast<ADDRINT>(m_doc[fieldName.c_str()].GetUint64());
	}
	cerr << "Invalid address specified for " << fieldName << endl;
	return NULL;
}

rapidjson::Value* NinkoConfig::getArray(const std::string fieldName)
{
	if(!fieldExists(fieldName) || !m_doc[fieldName.c_str()].IsArray())
	{
		cerr << "Invalid type, expected an array for " << fieldName << endl;
		return NULL;
	}
	
	return &m_doc[fieldName.c_str()];
}

/* Defaults to false.
 */
bool NinkoConfig::getBool(const std::string fieldName)
{
	if(!fieldExists(fieldName) || !m_doc[fieldName.c_str()].IsBool())
	{
		return false;
	}
	return m_doc[fieldName.c_str()].GetBool();	
}