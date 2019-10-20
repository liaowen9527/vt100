#pragma once
#include <vector>
#include "VtData.h"

class VtTermLine : public VtData
{
public:
	VtTermLine();
	~VtTermLine();

public:
	int Cols();
	void Resize(int count);

	void Clear();
	void Clear(int col);

protected:
	std::vector<VtTermChar> m_chars;
};