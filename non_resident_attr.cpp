#include "non_resident_attr.h"
#include "attr_data_run.h"


non_resident_attr::non_resident_attr()
{
}

non_resident_attr::non_resident_attr(BYTE* raw) 
	: std_attr_header(raw)
{
	memcpy(&n64InitVCN, raw + 16, 16);
	memcpy(&wDataRunOffset, raw + 32, 4);
	dwPadding = *(raw + 36);
	memcpy(&n64AttrAllocSize, raw + 40, 24);
	int offset = wDataRunOffset;
	runs_col = get_runs_col(raw + offset);
	dataRuns = new attr_data_run[runs_col];
	for (int i = 0; i < runs_col; i++)
	{
		dataRuns[i] = attr_data_run(raw + offset);
		offset += dataRuns[i].size;
	}
}

non_resident_attr::non_resident_attr(const non_resident_attr& source) : std_attr_header(source)
{
	*this = source;
}


non_resident_attr& non_resident_attr::operator=(const non_resident_attr& source)
{
	if (this == &source) return *this;
	std_attr_header::operator=(source);
	memcpy(&n64InitVCN, &source.n64InitVCN, 16);
	memcpy(&wDataRunOffset, &source.wDataRunOffset, 4);
	dwPadding = source.dwPadding;
	memcpy(&n64AttrAllocSize, &source.n64AttrAllocSize, 24);
	runs_col = source.runs_col;
	dataRuns = new attr_data_run[runs_col];
	for (int i = 0; i < runs_col; i++)
		dataRuns[i] = source.dataRuns[i];
	return *this;
}


non_resident_attr::~non_resident_attr()
{
	delete[] dataRuns;
}

int non_resident_attr::get_runs_col(BYTE* raw)
{
	int res = 0;
	while (*raw != 0)
	{
		res++;
		attr_data_run cur = attr_data_run(raw);
		raw += cur.size;
	}
	return res;
}

