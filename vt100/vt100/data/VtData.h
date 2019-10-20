#pragma once
#include <algorithm>

struct VtTerm;

class VtData
{
public:
	VtData();
	virtual ~VtData();

	void SetTerm(VtTerm* p);

protected:
	VtTerm * term;
	VtTerm* m_vtTerm;
};

