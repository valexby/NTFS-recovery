#pragma once
#include <Windows.h>

class attr_data_run
{
public :
	BYTE size;
	LONGLONG n64AttrSizeClust;
	LONGLONG n64AttrOffsetClust;
	attr_data_run();
	attr_data_run(BYTE* buffer);
	~attr_data_run();
	attr_data_run(const attr_data_run &);
	attr_data_run& operator=(const attr_data_run &);
};