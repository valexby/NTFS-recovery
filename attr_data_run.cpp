#include "attr_data_run.h"
#include "raw_parser.h"

attr_data_run::attr_data_run()
{
	
}

attr_data_run::attr_data_run(BYTE* buffer)
{
	buff = *buffer;
	value = new BYTE[left + right];
	memcpy(value, buffer + 1, left + right);
}

attr_data_run::~attr_data_run()
{
	delete[] value;
}


attr_data_run::attr_data_run(const attr_data_run& source)
{
	buff = source.buff;
	value = new BYTE[left + right];
	memcpy(value, source.value, left + right);
}

attr_data_run& attr_data_run::operator=(const attr_data_run &source)
{
	buff = source.buff;
	value = new BYTE[left + right];
	memcpy(value, source.value, left + right);
	return *this;
}