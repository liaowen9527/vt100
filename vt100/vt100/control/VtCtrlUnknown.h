#pragma once
#include "VtCtrlNode.h"


class VtCtrlUnknown : public VtCtrlNode
{
public:
	VtCtrlUnknown();
	~VtCtrlUnknown();

	void Init();

public:
#pragma region DEC Private
	/* <Esc>#3	Double - height letters, top half	DECDHL */
	/* <Esc>#4	Double - height letters, bottom half	DECDHL */
	/* <Esc>#5	Single width, single height letters	DECSWL */
	/* <Esc>#6	Double width, single height letters	DECDWL */
	/* <Esc>#8	Screen alignment display	DECALN */

	/* <Esc>[0q	Turn off all four leds	DECLL0 */
	/* <Esc>[1q	Turn on LED #1	DECLL1 */
	/* <Esc>[2q	Turn on LED #2	DECLL2 */
	/* <Esc>[3q	Turn on LED #3	DECLL3 */
	/* <Esc>[4q	Turn on LED #4	DECLL4 */

	/* <Esc>=	Set alternate keypad mode	DECKPAM */
	/* <Esc>>	Set numeric keypad mode	DECKPNM */

#pragma endregion DEC Private
 
#pragma region unknown
	/* <Esc>N	Set single shift 2	SS2 */
	/* <Esc>O	Set single shift 3	SS3 */
#pragma endregion unknown

#pragma region DECTST
	/*
	DECTST -- Invoke Confidence Test
	ESC [ 2 ; Ps y
	Ps is the parameter indicating the test to be done. Ps is computed by taking the weight indicated for each desired test and adding them together. If Ps is 0, no test is performed but the VT100 is reset.

	Test																				Weight
	Power up self-test (ROM check sum, RAM, NVR keyboard and AVO if installed)			1
	Data Loop Back																		2 (loop back connector required)
	EIA modem control test																4 (loop back connector required)
	Repeat Selected Test(s)																8
	indefinitely (until failure or power off)											
	*/
	/* <Esc>[2;1y	Confidence power up test	DECTST                  */
	/* <Esc>[2;2y	Confidence loopback test	DECTST                  */
	/* <Esc>[2;9y	Repeat power up test	DECTST                      */
	/* <Esc>[2;10y	Repeat loopback test	DECTST                  */

#pragma endregion DECTST

protected:
	void DoNothing();
};