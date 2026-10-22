/*
 * main's subroutines, line by line (sim/cpu.h), generated from the ROM's
 * code and read: each takes and leaves its values in the registers, as
 * the ROM's does, and counts its Z80 cycles in z->t. Named by address.
 */
#ifndef GALAGA_ROUTINES_H
#define GALAGA_ROUTINES_H

#include "sim/cpu.h"

void m_0018(Cpu *z);
void m_0020(Cpu *z);
void m_0028(Cpu *z);
void m_0030(Cpu *z);
void m_003C(Cpu *z);
void m_00D6(Cpu *z);
void m_0160(Cpu *z);
void m_0185(Cpu *z);
void m_01C5(Cpu *z);
void m_043D(Cpu *z);
void m_0466(Cpu *z);
void m_0486(Cpu *z);
void m_0728(Cpu *z);
void m_07D8(Cpu *z);
void m_080B(Cpu *z);
void m_08AD(Cpu *z);
void m_08BE(Cpu *z);
void m_095F(Cpu *z);
void m_0A53(Cpu *z);
void m_0A6E(Cpu *z);
void m_0A72(Cpu *z);
void m_0B06(Cpu *z);
void m_1000(Cpu *z);
void m_104E(Cpu *z);
void m_1061(Cpu *z);
void m_1079(Cpu *z);
void m_1083(Cpu *z);
void m_110C(Cpu *z);
void m_117F(Cpu *z);
void m_11F5(Cpu *z);
void m_11FB(Cpu *z);
void m_1213(Cpu *z);
void m_1228(Cpu *z);
void m_1242(Cpu *z);
void m_127B(Cpu *z);
void m_129E(Cpu *z);
void m_12D5(Cpu *z);
void m_1331(Cpu *z);
void m_133D(Cpu *z);
void m_134C(Cpu *z);
void m_137E(Cpu *z);
void m_1398(Cpu *z);
void m_13B3(Cpu *z);
int m_1C93(Cpu *z, uint16_t ret); /* 1 when it returned from its caller too */
void m_1D09(Cpu *z);
void m_1E49(Cpu *z);
void m_1F19(Cpu *z);
void m_1F9C(Cpu *z);
void m_2188(Cpu *z);
void m_238A(Cpu *z);
void m_23E0(Cpu *z);
void m_25A2(Cpu *z);
void m_2896(Cpu *z);
void m_28E9(Cpu *z);
void m_2C00(Cpu *z);
void m_3000(Cpu *z);
void m_3118(Cpu *z);
void m_3138(Cpu *z);
void m_313B(Cpu *z);
void m_313E(Cpu *z);
void m_3141(Cpu *z);
void m_3180(Cpu *z);
void m_31F7(Cpu *z);
void m_3214(Cpu *z);
void m_321D(Cpu *z);
void m_3231(Cpu *z);
void m_3270(Cpu *z);
void m_3273(Cpu *z);
void m_3275(Cpu *z);
void m_32ED(Cpu *z);
void m_331B(Cpu *z);
void m_3328(Cpu *z);
void m_3489(Cpu *z);
void m_3496(Cpu *z);
void m_352B(Cpu *z);
void m_3770(Cpu *z);
void m_377E(Cpu *z);
void m_37F6(Cpu *z);
void m_37FE(Cpu *z);
void m_38E4(Cpu *z);
void m_3928(Cpu *z);
void m_3962(Cpu *z);
void m_397C(Cpu *z);
void m_3987(Cpu *z);
void m_39A1(Cpu *z);
void m_39AA(Cpu *z);
void m_39B4(Cpu *z);
void m_39B8(Cpu *z);
void m_39C5(Cpu *z);
void m_39E0(Cpu *z);
void m_39FC(Cpu *z);
void m_3A46(Cpu *z);
void m_3A6B(Cpu *z);

#endif
