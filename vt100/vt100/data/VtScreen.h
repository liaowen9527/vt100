#pragma once

class VtCursor;
class VtScreen
{
public:
	VtScreen();
	~VtScreen();

	friend VtCursor;

public:
	Postion GetCursor();
	void SetCursor(const Postion& pos);

	void GetCursor(int& row, int& col);
	void SetCursor(int row, int col);

	Margin GetMargin();
	void SetMargin(const Margin& marg);

	void GetMargin(int& top, int& bottom);
	void SetMargin(int top, int bottom);

	bool CanWrap();
	void SetCanWrap(bool val);

	bool IsWrapNext();
	void SetWrapNext(bool val);

	int GetCharset();
	void SetCharset(int index);

public:
	void Swap(VtScreen& _Right, bool keep_cur_pos);

protected:
	Postion m_curs;	//keep_cur_pos
	Margin m_margin;
	bool m_dec_om;
	bool m_insert;
	bool m_wrap;		//the line can wrapnext if wrap=true

	//need save
	bool m_wrapnext;	//the line and the next line is the same line
	int m_cset;
	bool m_utf;
	int m_sco_acs;

	//save tag
	Postion m_savecurs; //keep_cur_pos
	bool m_save_wnext;//keep_cur_pos
	int m_save_cset;//keep_cur_pos
	bool m_save_utf;//keep_cur_pos
	int m_save_sco_acs;//keep_cur_pos
	int m_save_csattr;//keep_cur_pos
	int m_save_attr;//keep_cur_pos

	truecolour m_save_truecolour;//keep_cur_pos

	
};
