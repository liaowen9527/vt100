#pragma once
#include <string>

struct terminal_tag;

class VtParserBase
{
public:
	VtParserBase();
	virtual ~VtParserBase();

public:
	void SetTerm(terminal_tag* p);
	unsigned long GetChar();

	void Parse(char ch);
	virtual void Parse();

protected:
	bool CheckCompat(int vttype);

protected:
	int GetCharWidth(char c);
	bool IsChar(char c);
	bool IsFont(char c);

protected:
	unsigned long c;
	terminal_tag* term;
};

