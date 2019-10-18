#pragma once
#include <string>
#include <vector>
#include <deque>

class VtPos
{
public:
	VtPos(int row = 0, int col = 0);

	bool lt(const VtPos& _Right);
	bool le(const VtPos& _Right);
	bool eq(const VtPos& _Right);

public:
	int m_row;
	int m_col;
};

class VtSelection
{
public:
	VtSelection();

	void GetString(const std::vector<VtLine>& lines, std::wstring& str);
	void GetString(const std::deque<VtLine>& lines, std::wstring& str);

	void GetString(const std::deque<VtLine>& lines, std::vector<std::wstring>& vecStr);

public:
	VtPos m_from;
	VtPos m_to;
	bool m_bCheckCrLf;
	std::wstring m_strEof;
};

class VtLine
{
public:
	VtLine();

	static void SubStr(const std::wstring& str, std::wstring& strSubStr, size_t beg, size_t end = std::wstring::npos);

public:
	std::wstring str;
	bool hasCrLf;
	bool hasWchar;
};

class VtLines
{
public:
	VtLines(int from = 0, int to = 0);

	void GetString(const std::vector<VtLine>& lines, std::wstring& str);
	void GetString(const std::deque<VtLine>& lines, std::wstring& str);

public:
	int m_from;
	int m_to;
	bool m_bCheckCrLf;
	std::wstring m_strEof;
};
