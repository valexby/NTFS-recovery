#pragma once
#include <Windows.h>

class attr_data_run
{
public :
	union
	{
		WORD buff;
		struct
		{
			BYTE right;
			BYTE left;
		};
	};
	BYTE* value = nullptr;
	attr_data_run();
	attr_data_run(BYTE* buffer);
	~attr_data_run();
	attr_data_run(const attr_data_run &);
	attr_data_run& operator=(const attr_data_run &);
};