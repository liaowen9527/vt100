#pragma once

/*
* UCSINCOMPLETE is returned from term_translate if it's successfully
* absorbed a byte but not emitted a complete character yet.
* UCSTRUNCATED indicates a truncated multibyte sequence (so the
* caller emits an error character and then calls term_translate again
* with the same input byte). UCSINVALID indicates some other invalid
* multibyte sequence, such as an overlong synonym, or a standalone
* continuation byte, or a completely illegal thing like 0xFE. These
* values are not stored in the terminal data structures at all.
*/
#define UCSINCOMPLETE 0x8000003FU    /* '?' */
#define UCSTRUNCATED  0x80000021U    /* '!' */
#define UCSINVALID    0x8000002AU    /* '*' */

class VtCharTrans
{
public:
	VtCharTrans();
	~VtCharTrans();

public:
	void SetTerm(terminal_tag* p);
	unsigned long Translate(unsigned char c);

protected:
	unsigned long ByUtf(unsigned char c);
	unsigned long ByUtf_0(unsigned char c);
	unsigned long ByUtf_1(unsigned char c);

	unsigned long ByAcs(unsigned char c);
	unsigned long ByCharset(unsigned char c);

protected:
	bool IsAcsMode();
	bool IsUtf();
	int GetCharset();

protected:
	terminal_tag* term;
};

