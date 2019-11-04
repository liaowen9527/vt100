#pragma once
#include "VtData.h"

class VtCharset : public VtData
{
public:
	VtCharset();
	~VtCharset();

public:
	int GetCharset();
	int GetCharset(int index);
	void SetCharset(int index, int val);

	bool IsLineDraw();

protected:
	int m_cset_attr[2];
	bool m_utf8linedraw;
};