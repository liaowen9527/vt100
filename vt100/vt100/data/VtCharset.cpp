#include "VtCharset.h"
#include "VtTerm.h"

#define ATTR_INVALID 0x03FFFFU

/* Like Linux use the F000 page for direct to font. */
#define CSET_OEMCP   0x0000F000UL      /* OEM Codepage DTF */
#define CSET_ACP     0x0000F100UL      /* Ansi Codepage DTF */

/* These are internal use overlapping with the UTF-16 surrogates */
#define CSET_ASCII   0x0000D800UL      /* normal ASCII charset ESC ( B */
#define CSET_LINEDRW 0x0000D900UL      /* line drawing charset ESC ( 0 */
#define CSET_SCOACS  0x0000DA00UL      /* SCO Alternate charset */
#define CSET_GBCHR   0x0000DB00UL      /* UK variant   charset ESC ( A */
#define CSET_MASK    0xFFFFFF00UL      /* Character set mask */

VtCharset::VtCharset()
{
	m_utf8linedraw = false;
}

VtCharset::~VtCharset()
{

}

int VtCharset::GetCharset()
{
	return GetCharset(0);
}

int VtCharset::GetCharset(int index)
{
	return m_cset_attr[index];
}

void VtCharset::SetCharset(int index, int val)
{
	m_cset_attr[index] = val;
}

bool VtCharset::IsLineDraw()
{
	return m_utf8linedraw && GetCharset() == CSET_LINEDRW;
}

