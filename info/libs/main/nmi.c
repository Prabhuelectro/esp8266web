/******************************************************************************
 * FileName: nmi.c
 * Description: disasm nmi functions SDK 1.3.0
 * Author: PV`
 * (c) PV` 2015
 ******************************************************************************/
#include "user_config.h"
#include "bios.h"
#include "hw/esp8266.h"
#include "hw/specreg.h"
#include "hw/gpio_register.h"
//#include "user_interface.h"
#include "sdk/add_func.h"
#include "os_type.h"
//#include "user_interface.h"


typedef void (*nmi_tim_func_t)(void);

nmi_tim_func_t pNmiTimFunc;

void NmiTimSetFunc(nmi_tim_func_t func)
{
	DPORT_BASE[0] = (DPORT_BASE[0] & 0x60) | 0x0F;
	pNmiTimFunc = func;
}

void NMI_Handler(void)
{
	do {
		DPORT_BASE[0] = (DPORT_BASE[0] & 0x60) | 0x0E;
	} while(DPORT_BASE[0]&1);
	if(pNmiTimFunc != NULL) {
		pNmiTimFunc();
	}
	xthal_set_intclear(8);
	TIMER0_INT &= 0xFFE;
}


/* asm 
.text:40100020
_NMIExceptionVector:
	wsr             0xD3, a0
	call0           _NMILevelVector


a_nmi_store_regs .int nmi_store_regs    
a__Pri_3_HandlerAddress .int _Pri_3_HandlerAddress
a_NMI_Handler   .int NMI_Handler        

; =============== S U B R O U T I N E =======================================
.text:4010009C
_NMILevelVector:                        ; CODE XREF: .text:40100023p
                l32r            a0, a_nmi_store_regs
                s32i.n          a2, a0, 0x28
                l32r            a2, a__Pri_3_HandlerAddress
                s32i.n          a1, a0, 0x24
                l32i.n          a2, a2, 0
                s32i.n          a3, a0, 0x2C
                xsr             0xD3, a2
                s32i.n          a4, a0, 0x30
                s32i.n          a2, a0, 0x20
                rsr.epc1        a3
                rsr.exccause    a4
                s32i.n          a3, a0, 0xC
                s32i.n          a4, a0, 0x10
                rsr.excvaddr    a3
                s32i.n          a3, a0, 0x14
                rsr             0xD1, a4
                s32i.n          a4, a0, 0x18
                s32i.n          a5, a0, 0x34
                s32i.n          a6, a0, 0x38
                s32i.n          a7, a0, 0x3C
                s32i            a8, a0, 0x40
                s32i            a9, a0, 0x44
                s32i            a10, a0, 0x48
                s32i            a11, a0, 0x4C
                s32i            a12, a0, 0x50
                s32i            a13, a0, 0x54
                s32i            a14, a0, 0x58
                s32i            a15, a0, 0x5C
                l32r            a1, a_nmi_store_regs
                movi.n          a0, 0
                movi.n          a2, 0x23
                wsr.ps          a2
                rsync
                rsr.sar         a14
                s32i.n          a14, a1, 0
                l32r            a13, a_NMI_Handler
                callx0          a13
                l32i.n          a15, a1, 0
                wsr.sar         a15
                movi.n          a2, 0x33
                wsr.ps          a2
                rsync
                l32i.n          a4, a1, 0x30
                l32i.n          a5, a1, 0x34
                l32i.n          a6, a1, 0x38
                l32i.n          a7, a1, 0x3C
                l32i            a8, a1, 0x40
                l32i            a9, a1, 0x44
                l32i            a10, a1, 0x48
                l32i            a11, a1, 0x4C
                l32i            a12, a1, 0x50
                l32i            a13, a1, 0x54
                l32i            a14, a1, 0x58
                l32i            a15, a1, 0x5C
                l32i.n          a2, a1, 0xC
                l32i.n          a3, a1, 0x10
                wsr.epc1        a2
                wsr             0xE8, a3
                l32i.n          a2, a1, 0x14
                wsr             0xEE, a2
                l32i.n          a3, a1, 0x18
                wsr             0xD1, a3
                l32i.n          a0, a1, 0x20
                rsr.sar         a3
                movi            a2, 0x3FF
                slli            a2, a2, 0x14
                wsr.sar         a3
                movi.n          a3, 0xF
                s32i.n          a3, a2, 0
                l32i.n          a2, a1, 0x28
                l32i.n          a3, a1, 0x2C
                l32i.n          a1, a1, 0x24
                rfi             3
; End of function _NMILevelVector

*/
