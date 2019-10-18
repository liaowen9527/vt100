#pragma once
#include <string>
#include <vector>

#define VT_ESC '\x1b'
#define VT_NUM '\x1c'
#define VT_SEPARATOR ';'

#define VT_STR_ESC "\x1b"
#define VT_STR_NUM "\x1c"

#define CL_ANSIMIN	0x0001	       /* Codes in all ANSI like terminals. */
#define CL_VT100	0x0002	       /* VT100 */
#define CL_VT100AVO	0x0004	       /* VT100 +AVO; 132x24 (not 132x14) & attrs */
#define CL_VT102	0x0008	       /* VT102 */
#define CL_VT220	0x0010	       /* VT220 */
#define CL_VT320	0x0020	       /* VT320 */
#define CL_VT420	0x0040	       /* VT420 */
#define CL_VT510	0x0080	       /* VT510, NB VT510 includes ANSI */
#define CL_VT340TEXT	0x0100	       /* VT340 extensions that appear in the VT420 */
#define CL_SCOANSI	0x1000	       /* SCOANSI not in ANSIMIN. */
#define CL_ANSI		0x2000	       /* ANSI ECMA-48 not in the VT100..VT420 */
#define CL_OTHER	0x4000	       /* Others, Xterm, linux, putty, dunno, etc */

class VtBufChain
{
public:
	VtBufChain();
	~VtBufChain();

	void Input(char ch);

	void Reset();
	void Recal();

public:
	int GetAttrsCount();
	int GetIntAttr(int nIndex);

public:
	std::string m_buffer;
	std::string m_fmt;

	std::vector<std::string> m_attrs;
};