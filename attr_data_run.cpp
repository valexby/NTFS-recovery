#include "attr_data_run.h"
#include "raw_parser.h"

attr_data_run::attr_data_run()
{
	
}

attr_data_run::attr_data_run(BYTE* buffer)
{
	int low = *buffer & 0x0F;
	int high = *buffer >> 4;
	memset(&n64AttrSizeClust, 0, 16);
	memcpy(&n64AttrSizeClust, buffer + 1, low);
	memcpy(&n64AttrOffsetClust, buffer + 1 + low, high);
	size = high + low + 1;
}

attr_data_run::~attr_data_run()
{
}


attr_data_run::attr_data_run(const attr_data_run& source)
{
	size = source.size;
	n64AttrSizeClust = source.n64AttrSizeClust;
	n64AttrOffsetClust = source.n64AttrOffsetClust;
}

attr_data_run& attr_data_run::operator=(const attr_data_run &source)
{
	size = source.size;
	n64AttrSizeClust = source.n64AttrSizeClust;
	n64AttrOffsetClust = source.n64AttrOffsetClust;
	return *this;
}