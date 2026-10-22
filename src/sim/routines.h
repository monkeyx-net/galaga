/*
 * main's subroutines, line by line (sim/cpu.h): each takes and leaves its
 * values in the registers, as the ROM's does. Named by their addresses.
 */
#ifndef GALAGA_ROUTINES_H
#define GALAGA_ROUTINES_H

#include "sim/cpu.h"

void m_0018(Cpu *z); /* $0018 */
void m_0020(Cpu *z); /* $0020 */
void m_0028(Cpu *z); /* $0028 */
void m_0030(Cpu *z); /* $0030 */
void m_003C(Cpu *z); /* $003C */
void m_0160(Cpu *z); /* $0160 */
void m_01C5(Cpu *z); /* $01C5 */
void m_0A53(Cpu *z); /* $0A53 */
void m_0A6E(Cpu *z); /* $0A6E */
void m_1000(Cpu *z); /* $1000 */
void m_104E(Cpu *z); /* $104E */
void m_1061(Cpu *z); /* $1061 */
void m_1079(Cpu *z); /* $1079 */
void m_1083(Cpu *z); /* $1083 */
void m_117F(Cpu *z); /* $117F */
void m_11F5(Cpu *z); /* $11F5 */
void m_11FB(Cpu *z); /* $11FB */
void m_1213(Cpu *z); /* $1213 */
void m_1228(Cpu *z); /* $1228 */
void m_1242(Cpu *z); /* $1242 */
void m_127B(Cpu *z); /* $127B */
void m_129E(Cpu *z); /* $129E */
void m_12D5(Cpu *z); /* $12D5 */
void m_134C(Cpu *z); /* $134C */
void m_137E(Cpu *z); /* $137E */
void m_1398(Cpu *z); /* $1398 */
void m_13B3(Cpu *z); /* $13B3 */
int m_1C93(Cpu *z, uint16_t ret); /* $1C93: 1 when it returned from its caller too */
void m_1D09(Cpu *z); /* $1D09 */
void m_1E49(Cpu *z); /* $1E49 */
void m_1F19(Cpu *z); /* $1F19 */
void m_1F9C(Cpu *z); /* $1F9C */
void m_2188(Cpu *z); /* $2188 */
void m_238A(Cpu *z); /* $238A */
void m_23E0(Cpu *z); /* $23E0 */
void m_25A2(Cpu *z); /* $25A2 */
void m_2896(Cpu *z); /* $2896 */
void m_28E9(Cpu *z); /* $28E9 */
void m_2C00(Cpu *z); /* $2C00 */
void m_3214(Cpu *z); /* $3214 */
void m_3231(Cpu *z); /* $3231 */
void m_3270(Cpu *z); /* $3270 */
void m_3273(Cpu *z); /* $3273 */
void m_3275(Cpu *z); /* $3275 */
void m_331B(Cpu *z); /* $331B */
void m_3328(Cpu *z); /* $3328 */

#endif
