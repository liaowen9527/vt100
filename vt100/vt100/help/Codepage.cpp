#include "Codepage.h"


struct cp_list_item 
{
	char *name;
	int codepage;
	int cp_size;
	const wchar_t *cp_table;
};

static const std::vector<cp_list_item> s_cp_list = {
	{"UTF-8", CP_UTF8},

	{"ISO-8859-1:1998 (Latin-1, West Europe)", 0, 96, iso_8859_1},
	{"ISO-8859-2:1999 (Latin-2, East Europe)", 0, 96, iso_8859_2},
	{"ISO-8859-3:1999 (Latin-3, South Europe)", 0, 96, iso_8859_3},
	{"ISO-8859-4:1998 (Latin-4, North Europe)", 0, 96, iso_8859_4},
	{"ISO-8859-5:1999 (Latin/Cyrillic)", 0, 96, iso_8859_5},
	{"ISO-8859-6:1999 (Latin/Arabic)", 0, 96, iso_8859_6},
	{"ISO-8859-7:1987 (Latin/Greek)", 0, 96, iso_8859_7},
	{"ISO-8859-8:1999 (Latin/Hebrew)", 0, 96, iso_8859_8},
	{"ISO-8859-9:1999 (Latin-5, Turkish)", 0, 96, iso_8859_9},
	{"ISO-8859-10:1998 (Latin-6, Nordic)", 0, 96, iso_8859_10},
	{"ISO-8859-11:2001 (Latin/Thai)", 0, 96, iso_8859_11},
	{"ISO-8859-13:1998 (Latin-7, Baltic)", 0, 96, iso_8859_13},
	{"ISO-8859-14:1998 (Latin-8, Celtic)", 0, 96, iso_8859_14},
	{"ISO-8859-15:1999 (Latin-9, \"euro\")", 0, 96, iso_8859_15},
	{"ISO-8859-16:2001 (Latin-10, Balkan)", 0, 96, iso_8859_16},

	{"KOI8-U", 0, 128, koi8_u},
	{"KOI8-R", 20866},
	{"HP-ROMAN8", 0, 96, roman8},
	{"VSCII", 0, 256, vscii},
	{"DEC-MCS", 0, 96, dec_mcs},

	{"Win1250 (Central European)", 1250},
	{"Win1251 (Cyrillic)", 1251},
	{"Win1252 (Western)", 1252},
	{"Win1253 (Greek)", 1253},
	{"Win1254 (Turkish)", 1254},
	{"Win1255 (Hebrew)", 1255},
	{"Win1256 (Arabic)", 1256},
	{"Win1257 (Baltic)", 1257},
	{"Win1258 (Vietnamese)", 1258},

	{"CP437", 437},
	{"CP620 (Mazovia)", 0, 128, mazovia},
	{"CP819", 28591},
	{"CP852", 852},
	{"CP878", 20866},

	{"Use font encoding", -1},

	{0, 0}
};


int CodePage::Decode(char *cp_name)
{
	return CP_UTF8;                /* default */
}

std::string CodePage::Name(int codepage)
{
	const struct cp_list_item *cpi, *cpno;
	static char buf[32];

	if (codepage == -1) {
		sprintf(buf, "Use font encoding");
		return buf;
	}

	if (codepage > 0 && codepage < 65536)
		sprintf(buf, "CP%03d", codepage);
	else
		*buf = 0;

	if (codepage >= 65536) {
		cpno = 0;
		for (cpi = s_cp_list; cpi->name; cpi++)
			if (cpi == s_cp_list + (codepage - 65536)) {
				cpno = cpi;
				break;
			}
		if (cpno)
			for (cpi = s_cp_list; cpi->name; cpi++) {
				if (cpno->cp_table == cpi->cp_table)
					return cpi->name;
			}
	}
	else {
		for (cpi = s_cp_list; cpi->name; cpi++) {
			if (codepage == cpi->codepage)
				return cpi->name;
		}
	}
	return buf;
}
