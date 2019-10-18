#pragma once
#include "../VtCtxNode.h"

class VtCtrlCharset : public VtCtxNode
{
public:
	VtCtrlCharset();
	~VtCtrlCharset();

	void Init();

public:
	//SCS -- Select Character Set
	/* <Esc>(A	Set United Kingdom G0 character set	setukg0 */
	void Set_ukg0();

	/* <Esc>)A	Set United Kingdom G1 character set	setukg1 */
	void Set_ukg1();

	/* <Esc>(B	Set United States G0 character set	setusg0 */
	void Set_usg0();

	/* <Esc>)B	Set United States G1 character set	setusg1 */
	void Set_usg1();

	/* <Esc>(0	Set G0 special chars. & line set	setspecg0 */
	void Set_specg0();

	/* <Esc>)0	Set G1 special chars. & line set	setspecg1 */
	void Set_specg1();

	/* <Esc>(1	Set G0 alternate character ROM	setaltg0 */
	void Set_altg0();

	/* <Esc>)1	Set G1 alternate character ROM	setaltg1 */
	void Set_altg1();

	/* <Esc>(2	Set G0 alt char ROM and spec.graphics	setaltspecg0 */
	void Set_altspecg0();

	/* <Esc>)2	Set G1 alt char ROM and spec.graphics	setaltspecg1 */
	void Set_altspecg1();
};