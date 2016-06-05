#include "non_resident_attr.h"



non_resident_attr::non_resident_attr()
{
}

non_resident_attr::non_resident_attr(BYTE* raw) 
	: std_attr_header(raw)
{
	n64InitVCN = *(raw + 16);
	n64FinalVCN = *(raw + 24);
	memcpy(&wDataRunOffset, raw + 32, 4);
	dwPadding = *(raw + 36);
	n64AttrAllocSize = *(raw + 40);
	n64AttrRealSize = *(raw + 48);
	n64InitDataLen = *(raw + 56);
	int offset = 56;
	runs_col = get_runs_col(raw + offset);
	dataRuns = new attr_data_run[runs_col];
	for (int i = 0; i < runs_col; i++)
	{
		dataRuns[i] = attr_data_run(raw + offset);
		offset += dataRuns[i].left + dataRuns[i].right;
	}
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
		raw += cur.left + cur.right;
	}
	return res;
}

