#pragma once
#include <algorithm>

struct VtTerm;

class VtData
{
public:
	VtData();
	virtual ~VtData();

	VtTerm* GetTerm();
	void SetTerm(VtTerm* term);

protected:
	VtTerm* m_vtTerm;
};

