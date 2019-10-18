#pragma once

class VtCharTrans
{
public:
	VtCharTrans();
	~VtCharTrans();

public:
	void SetTerm(terminal_tag* p);
	bool Translate(char c);
	bool IsBreak();

protected:
	void Break();
	void SetResult(unsigned long result);

protected:
	void ByUtf();
	void ByUtf_0();
	void ByUtf_1();

	void ByAcs();
	void ByCharset();

protected:
	bool IsAcsMode();
	bool IsUtf();
	int GetCharset();

protected:
	terminal_tag* term;

	unsigned long m_orgin;
	unsigned long m_result;
	bool m_break;
};

