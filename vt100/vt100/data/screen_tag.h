#pragma once

struct screen_tag
{
	Postion curs;	//keep_cur_pos
	Margin margin;
	bool dec_om;
	bool insert;
	bool wrap;

	//need save
	bool wrapnext;
	int cset;
	bool utf;
	int sco_acs;

	//save tag
	Postion savecurs; //keep_cur_pos
	bool save_wnext;//keep_cur_pos
	int save_cset;//keep_cur_pos
	bool save_utf;//keep_cur_pos
	int save_sco_acs;//keep_cur_pos
	int save_csattr;//keep_cur_pos
	int save_attr;//keep_cur_pos

	truecolour save_truecolour;//keep_cur_pos

	void swap(screen_tag& _Right, bool keep_cur_pos)
	{
		if (keep_cur_pos)
		{
			screen_tag temp = _Right;
			_Right = *this;

			temp.curs = this->curs;
			temp.savecurs = this->savecurs;
			temp.save_wnext = this->save_wnext;
			temp.save_cset = this->save_cset;
			temp.save_utf = this->save_utf;
			temp.save_sco_acs = this->save_sco_acs;
			temp.save_csattr = this->save_csattr;
			temp.save_attr = this->save_attr;

			*this = temp;
		}
		else
		{
			std::swap<screen_tag>(*this, _Right);
		}
	}
};
