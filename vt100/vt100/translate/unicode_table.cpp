#include "unicode_table.h"
#include "cp_list.h"
#include <windows.h>

bool is_direct_char(wchar_t ch)
{
	return (ch & 0xFFFFFC00) == 0xD800;
}

bool is_direct_font(wchar_t ch)
{
	return (ch & 0xFFFFFE00) == 0xF000;
}


unicode_table::unicode_table()
{
	memset(m_unitab, 0, sizeof(m_unitab));
}

unicode_table::~unicode_table()
{

}

wchar_t unicode_table::get(int index)
{
	return m_unitab[index];
}

void unicode_table::set(int index, wchar_t val)
{
	m_unitab[index] = val;
}

int unicode_table::size()
{
	return 256;
}

void unicode_table::init_128(int codepage, int ftype)
{
	int count = 128;
	int flg = ftype ? (MB_ERR_INVALID_CHARS | MB_USEGLYPHCHARS) : MB_ERR_INVALID_CHARS;

	init_table(codepage, count, ftype);
}

void unicode_table::init_256(int codepage, int ftype)
{
	int count = 256;

	init_table(codepage, count, ftype);
}

void unicode_table::init_some(int begin, int end, wchar_t charset)
{
	for (int i = begin; i < end; i++)
	{
		m_unitab[i] = charset + i;
	}
}

void unicode_table::copy_some(int begin, const wchar_t* p, int count)
{
	wchar_t* dest = m_unitab + begin;
	memcpy(dest, p, count * sizeof(wchar_t));
}

void unicode_table::link_font(unicode_table* font_tbl, wchar_t attr)
{
	for (int line_index = 0; line_index < 256; line_index++) 
	{
		wchar_t line = get(line_index);
		if (is_direct_font(line))
		{
			continue;
		}
			
		for (int i = 0; i < 256; i++) 
		{
			int font_index = ((32 + i) & 0xFF);
			wchar_t font = font_tbl->get(font_index);
			if (line == font)
			{
				wchar_t val = attr + font_index;
				set(line_index, val);
				break;
			}
		}
	}
}

void unicode_table::init_table(int codepage, int count, int ftype)
{
	if (codepage == CP_UTF8)
	{
		init_utf8(count);
		return;
	}

	codepage = get_codepage(codepage);

	if (0 < codepage && codepage < 65536)
	{
		int flg = ftype ? (MB_ERR_INVALID_CHARS | MB_USEGLYPHCHARS) : MB_ERR_INVALID_CHARS;
		init_mb(codepage, count, flg);
	}
	else
	{
		init_other(codepage, count);
	}
}

void unicode_table::init_utf8(int count)
{
	for (int i = 0; i < count; i++)
	{
		m_unitab[i] = i;
	}
}

void unicode_table::init_mb(int codepage, int count, int flgs)
{
	char tbuf[4] = {'\0'};

	for (int i = 0; i < count; i++)
	{
		tbuf[0] = i;

		if (mb_to_wc(codepage, flgs, tbuf, 1, m_unitab + i, 1) != 1)
		{
			m_unitab[i] = 0xFFFD;
		}
	}
}

void unicode_table::init_other(int codepage, int count)
{
	for (int i = 0; i < count; i++)
	{
		m_unitab[i] = i;
	}

	int cp_index = codepage & 0xFFFF;

	cp_list_item cp_item = g_cp_list[cp_index];
	cp_table* table = cp_item.get_cp_table();

	int beg = 256 - cp_item.get_size();
	for (int i = beg; i < count; i++)
	{
		int offset = i - beg;
		m_unitab[i] = table->get_cp(offset);
	}
}

int unicode_table::get_codepage(int codepage)
{
	int ret = codepage;
	if (codepage == CP_ACP)
	{
		ret = GetACP();
	}
	else if (codepage == CP_OEMCP)
	{
		ret = GetOEMCP();
	}

	return ret;
}

int unicode_table::mb_to_wc(int codepage, int flags, const char *mbstr, int mblen, wchar_t *wcstr, int wclen)
{
	return MultiByteToWideChar(codepage, flags, mbstr, mblen, wcstr, wclen);
}
