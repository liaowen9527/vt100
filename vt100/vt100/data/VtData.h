#pragma once
#include <algorithm>

struct terminal_tag;

class VtData
{
public:
	VtData();
	virtual ~VtData();

	void SetTerm(terminal_tag* p);

protected:
	terminal_tag* term;
	VtTerm* m_vtTerm;
};

