#include "sim/io.h"

#include <string.h>

/* ---- the 51xx: its MB8843 program ($000-$3FF), line by line where it runs
   its loop; X:Y addresses a RAM nibble ------------------------------------ */

#define M (m->ram[(m->x & 3) << 4 | m->y])

/* its R ports: R0-R3 the control panel's nibbles (IN0 low, IN0 high, IN1 low, IN1 high) */
static uint8_t m51_in(const Mcu *m, int port)
{
    uint8_t v = port < 2 ? m->in0 : m->in1;
    return (uint8_t)(port & 1 ? v >> 4 : v & 15);
}

/* $272: JPA into $280-$2BC, a table of LI n / RTS */
static void m51_272(Mcu *m)
{
    static const uint8_t table[16] = { 15, 14, 13, 5, 12, 9, 7, 6, 11, 3, 10, 4, 1, 2, 0, 8 };
    m->a = table[m->a & 15];
    m->zf = m->a == 0;
    m->st = 1;
}

static void m51_187(Mcu *m);
static void m51_1A4(Mcu *m);
static void m51_1C6(Mcu *m);
static void m51_265(Mcu *m);

static void m51_frame(Mcu *m) /* $097 */
{
    m->x = 1; m->zf = 0; m->st = 1; /* LXI */
    m->y = 8; m->zf = 0; m->st = 1; /* LYI */
    m->a = 0; m->zf = 1; m->st = 1; /* LI */
    M = m->a; m->st = 1; /* ST */
    m->x = 0; m->zf = 1; m->st = 1; /* LXI */
    m->y = 9; m->zf = 0; m->st = 1; /* LYI */
    m->a = m->tl; m->zf = m->a == 0; m->st = 1;
    M = m->a; m->st = 1; /* ST */
    m->y = 3; m->zf = 0; m->st = 1; /* LYI */
    m->a = M; m->zf = m->a == 0; m->st = 1; /* L */
    { int v = 1 - m->a; m->cf = v < 0; m->zf = v == 0; m->st = !m->zf; } /* CI */
    if (m->st) goto x0BE; /* JPL */
    m->st = 1;
    m->x = 2; m->zf = 0; m->st = 1; /* LXI */
    m->y = 2; m->zf = 0; m->st = 1; /* LYI */
    m->a = m51_in(m, m->y & 3); m->zf = m->a == 0; m->st = 1; /* IN */
    m->y = 4; m->zf = 0; m->st = 1; /* LYI */
    M = m->a; m->y = (m->y - 1) & 15; m->zf = m->y == 0; m->st = m->y != 15; /* STDC */
    m->a = m51_in(m, m->y & 3); m->zf = m->a == 0; m->st = 1; /* IN */
    m->y = 5; m->zf = 0; m->st = 1; /* LYI */
    M = m->a; m->st = 1; /* ST */
    m->y = 1; m->zf = 0; m->st = 1; /* LYI */
    m->a = m51_in(m, m->y & 3); m->zf = m->a == 0; m->st = 1; /* IN */
    m->y = 7; m->zf = 0; m->st = 1; /* LYI */
    M = m->a; m->st = 1; /* ST */
    m->y = 0; m->zf = 1; m->st = 1; /* LYI */
    m->a = m51_in(m, m->y & 3); m->zf = m->a == 0; m->st = 1; /* IN */
    m->y = 6; m->zf = 0; m->st = 1; /* LYI */
    M = m->a; m->st = 1; /* ST */
    m->a = 15; m->zf = 0; m->st = 1; /* LI */
    m->y = 8; m->zf = 0; m->st = 1; /* LYI */
    M = m->a; m->y = (m->y + 1) & 15; m->zf = m->y == 0; m->st = !m->zf; /* STIC */
    M = m->a; m->y = (m->y + 1) & 15; m->zf = m->y == 0; m->st = !m->zf; /* STIC */
    M = m->a; m->y = (m->y + 1) & 15; m->zf = m->y == 0; m->st = !m->zf; /* STIC */
    M = m->a; m->y = (m->y + 1) & 15; m->zf = m->y == 0; m->st = !m->zf; /* STIC */
    m->a = 0; m->zf = 1; m->st = 1; /* LI */
    m->sb = m->a; m->st = 1;
    if (m->st) return; /* JPL */
    m->st = 1;
x0BE:
    m->y = 3; m->zf = 0; m->st = 1; /* LYI */
    m->a = m51_in(m, m->y & 3); m->zf = m->a == 0; m->st = 1; /* IN */
    m->st = !(m->a >> 3 & 1); /* TBA */
    if (m->st) goto x239; /* JPL */
    m->st = 1;
    m->x = 1; m->zf = 0; m->st = 1; /* LXI */
    m->y = 10; m->zf = 0; m->st = 1; /* LYI */
    { int v = M; M = m->a; m->a = (uint8_t)v; m->zf = v == 0; m->st = 1; } /* X */
    { int v = 10 - m->a; m->cf = v < 0; m->zf = v == 0; m->st = !m->zf; } /* CI */
    if (m->st) goto x0CA; /* JMP */
    m->st = 1;
    { int v = M; M = m->a; m->a = (uint8_t)v; m->zf = v == 0; m->st = 1; } /* X */
    if (m->st) goto x0E7; /* JMP */
    m->st = 1;
x0CA:
    { int v = M; M = m->a; m->a = (uint8_t)v; m->zf = v == 0; m->st = 1; } /* X */
    m->y = 5; m->zf = 0; m->st = 1; /* LYI */
    { int v = M; M = m->a; m->a = (uint8_t)v; m->zf = v == 0; m->st = 1; } /* X */
    m->y = (m->y + 1) & 15; m->zf = m->y == 0; m->st = !m->zf; /* ICY */
    { int v = M; M = m->a; m->a = (uint8_t)v; m->zf = v == 0; m->st = 1; } /* X */
    m->y = (m->y + 1) & 15; m->zf = m->y == 0; m->st = !m->zf; /* ICY */
    { int v = M; M = m->a; m->a = (uint8_t)v; m->zf = v == 0; m->st = 1; } /* X */
    m->a &= M; m->zf = m->a == 0; m->st = !m->zf; /* AND */
    m->a = (uint8_t)(-m->a & 15); m->zf = m->a == 0; m->st = !m->zf; /* NEG */
    { int v = m->a + 15; m->cf = v > 15; m->a = v & 15; m->zf = m->a == 0; m->st = !m->cf; } /* AI */
    m->y = (m->y - 1) & 15; m->st = m->y != 15; /* DCY */
    m->a |= M; m->zf = m->a == 0; m->st = !m->zf; /* OR */
    m->y = (m->y - 1) & 15; m->st = m->y != 15; /* DCY */
    m->a |= M; m->zf = m->a == 0; m->st = !m->zf; /* OR */
    m->y = (m->y - 1) & 15; m->st = m->y != 15; /* DCY */
    M = m->a; m->st = 1; /* ST */
    m->st = !(m->a >> 0 & 1); /* TBA */
    if (m->st) m51_187(m); /* CALL */
    m->st = 1;
    m->y = 4; m->zf = 0; m->st = 1; /* LYI */
    m->a = M; m->zf = m->a == 0; m->st = 1; /* L */
    m->st = !(m->a >> 1 & 1); /* TBA */
    if (m->st) m51_1A4(m); /* CALL */
    m->st = 1;
    m->y = 4; m->zf = 0; m->st = 1; /* LYI */
    m->a = M; m->zf = m->a == 0; m->st = 1; /* L */
    m->st = !(m->a >> 2 & 1); /* TBA */
    if (m->st) m51_1C6(m); /* CALL */
    m->st = 1;
x0E7:
    if (m->st) m51_265(m); /* CALL */
    m->st = 1;
    m->y = 2; m->zf = 0; m->st = 1; /* LYI */
    m->a = m51_in(m, m->y & 3); m->zf = m->a == 0; m->st = 1; /* IN */
    m->x = 2; m->zf = 0; m->st = 1; /* LXI */
    { int v = M; M = m->a; m->a = (uint8_t)v; m->zf = v == 0; m->st = 1; } /* X */
    m->y = (m->y - 1) & 15; m->st = m->y != 15; /* DCY */
    { int v = M; M = m->a; m->a = (uint8_t)v; m->zf = v == 0; m->st = 1; } /* X */
    m->y = (m->y - 1) & 15; m->st = m->y != 15; /* DCY */
    { int v = M; M = m->a; m->a = (uint8_t)v; m->zf = v == 0; m->st = 1; } /* X */
    m->a &= M; m->zf = m->a == 0; m->st = !m->zf; /* AND */
    m->a = (uint8_t)(-m->a & 15); m->zf = m->a == 0; m->st = !m->zf; /* NEG */
    { int v = m->a + 15; m->cf = v > 15; m->a = v & 15; m->zf = m->a == 0; m->st = !m->cf; } /* AI */
    m->y = (m->y + 1) & 15; m->zf = m->y == 0; m->st = !m->zf; /* ICY */
    m->a |= M; m->zf = m->a == 0; m->st = !m->zf; /* OR */
    m->y = (m->y + 1) & 15; m->zf = m->y == 0; m->st = !m->zf; /* ICY */
    m->a |= M; m->zf = m->a == 0; m->st = !m->zf; /* OR */
    m->y = (m->y + 1) & 15; m->zf = m->y == 0; m->st = !m->zf; /* ICY */
    M = m->a; m->st = 1; /* ST */
    m->x = 0; m->zf = 1; m->st = 1; /* LXI */
    m->y = 3; m->zf = 0; m->st = 1; /* LYI */
    m->a = M; m->zf = m->a == 0; m->st = 1; /* L */
    { int v = 0 - m->a; m->cf = v < 0; m->zf = v == 0; m->st = !m->zf; } /* CI */
    if (m->st) goto x1F9; /* JPL */
    m->st = 1;
    m->x = 1; m->zf = 0; m->st = 1; /* LXI */
    m->y = 8; m->zf = 0; m->st = 1; /* LYI */
    m->a = M; m->zf = m->a == 0; m->st = 1; /* L */
    { int v = 0 - m->a; m->cf = v < 0; m->zf = v == 0; m->st = !m->zf; } /* CI */
    if (m->st) goto x120; /* JPL */
    m->st = 1;
    m->y = 10; m->zf = 0; m->st = 1; /* LYI */
    m->a = M; m->zf = m->a == 0; m->st = 1; /* L */
    { int v = 0 - m->a; m->cf = v < 0; m->zf = v == 0; m->st = !m->zf; } /* CI */
    if (m->st) goto x10F; /* JMP */
    m->st = 1;
    m->y = (m->y - 1) & 15; m->st = m->y != 15; /* DCY */
    m->a = M; m->zf = m->a == 0; m->st = 1; /* L */
    { int v = 0 - m->a; m->cf = v < 0; m->zf = v == 0; m->st = !m->zf; } /* CI */
    if (m->st) goto x119; /* JMP */
    m->st = 1;
    if (m->st) goto x120; /* JMP */
    m->st = 1;
x10F:
    m->x = 2; m->zf = 0; m->st = 1; /* LXI */
    m->y = 3; m->zf = 0; m->st = 1; /* LYI */
    m->a = M; m->zf = m->a == 0; m->st = 1; /* L */
    m->st = !(m->a >> 2 & 1); /* TBA */
    if (m->st) goto x1D9; /* JPL */
    m->st = 1;
    m->st = !(m->a >> 3 & 1); /* TBA */
    if (m->st) goto x1D7; /* JPL */
    m->st = 1;
    if (m->st) goto x120; /* JMP */
    m->st = 1;
x119:
    { int v = 1 - m->a; m->cf = v < 0; m->zf = v == 0; m->st = !m->zf; } /* CI */
    if (m->st) goto x10F; /* JMP */
    m->st = 1;
    m->x = 2; m->zf = 0; m->st = 1; /* LXI */
    m->y = 3; m->zf = 0; m->st = 1; /* LYI */
    m->st = !(M >> 2 & 1); /* TBIT 2 */
    if (m->st) goto x1D9; /* JPL */
    m->st = 1;
x120:
    m->x = 1; m->zf = 0; m->st = 1; /* LXI */
    m->y = 15; m->zf = 0; m->st = 1; /* LYI */
    m->a = 12; m->zf = 0; m->st = 1; /* LI */
    m->a &= M; m->zf = m->a == 0; m->st = !m->zf; /* AND */
    M = m->a; m->st = 1; /* ST */
    m->y = 11; m->zf = 0; m->st = 1; /* LYI */
    m->a = M; m->zf = m->a == 0; m->st = 1; /* L */
    { int v = 0 - m->a; m->cf = v < 0; m->zf = v == 0; m->st = !m->zf; } /* CI */
    if (m->st) goto x12B; /* JMP */
    m->st = 1;
    if (m->st) goto x13D; /* JPL */
    m->st = 1;
x12B:
    m->a = m->tl; m->zf = m->a == 0; m->st = 1;
    m->y = 15; m->zf = 0; m->st = 1; /* LYI */
    { int v = 4 - m->a; m->cf = v < 0; m->zf = v == 0; m->st = !m->zf; } /* CI */
    if (m->st) goto x139; /* JPL */
    m->st = 1;
    m->st = !(M >> 3 & 1); /* TBIT 3 */
    if (m->st) goto x134; /* JMP */
    m->st = 1;
    if (m->st) goto x13D; /* JPL */
    m->st = 1;
x134:
    M |= 8; m->st = 1; /* SBIT 3 */
    m->y = 11; m->zf = 0; m->st = 1; /* LYI */
    { int v = (M - 1) & 15; M = (uint8_t)v; m->zf = v == 0; m->st = v != 15; } /* DCM */
    if (m->st) goto x13D; /* JPL */
    m->st = 1;
x139:
    { int v = 0 - m->a; m->cf = v < 0; m->zf = v == 0; m->st = !m->zf; } /* CI */
    if (m->st) goto x13D; /* JPL */
    m->st = 1;
    M &= (uint8_t)~8; m->st = 1; /* RBIT 3 */
x13D:
    m->y = 12; m->zf = 0; m->st = 1; /* LYI */
    m->a = M; m->zf = m->a == 0; m->st = 1; /* L */
    { int v = 0 - m->a; m->cf = v < 0; m->zf = v == 0; m->st = !m->zf; } /* CI */
    if (m->st) goto x143; /* JMP */
    m->st = 1;
    if (m->st) goto x152; /* JPL */
    m->st = 1;
x143:
    m->a = m->tl; m->zf = m->a == 0; m->st = 1;
    m->y = 15; m->zf = 0; m->st = 1; /* LYI */
    { int v = 12 - m->a; m->cf = v < 0; m->zf = v == 0; m->st = !m->zf; } /* CI */
    if (m->st) goto x14F; /* JPL */
    m->st = 1;
    m->st = !(M >> 2 & 1); /* TBIT 2 */
    if (m->st) goto x14B; /* JMP */
    m->st = 1;
    if (m->st) goto x152; /* JMP */
    m->st = 1;
x14B:
    M |= 4; m->st = 1; /* SBIT 2 */
    m->y = 12; m->zf = 0; m->st = 1; /* LYI */
    { int v = (M - 1) & 15; M = (uint8_t)v; m->zf = v == 0; m->st = v != 15; } /* DCM */
    if (m->st) goto x152; /* JMP */
    m->st = 1;
x14F:
    { int v = 8 - m->a; m->cf = v < 0; m->zf = v == 0; m->st = !m->zf; } /* CI */
    if (m->st) goto x152; /* JMP */
    m->st = 1;
    M &= (uint8_t)~4; m->st = 1; /* RBIT 2 */
x152:
    m->x = 0; m->zf = 1; m->st = 1; /* LXI */
    m->y = 3; m->zf = 0; m->st = 1; /* LYI */
    m->a = M; m->zf = m->a == 0; m->st = 1; /* L */
    { int v = 0 - m->a; m->cf = v < 0; m->zf = v == 0; m->st = !m->zf; } /* CI */
    if (m->st) goto x16F; /* JMP */
    m->st = 1;
    m->y = 10; m->zf = 0; m->st = 1; /* LYI */
    m->x = 1; m->zf = 0; m->st = 1; /* LXI */
    m->a = M; m->zf = m->a == 0; m->st = 1; /* L */
    { int v = 0 - m->a; m->cf = v < 0; m->zf = v == 0; m->st = !m->zf; } /* CI */
    if (m->st) goto x161; /* JMP */
    m->st = 1;
    m->y = (m->y - 1) & 15; m->st = m->y != 15; /* DCY */
    m->a = M; m->zf = m->a == 0; m->st = 1; /* L */
    { int v = 1 - m->a; m->cf = v < 0; m->zf = v == 0; m->st = !m->zf; } /* CI */
    if (m->st) goto x161; /* JMP */
    m->st = 1;
    if (m->st) goto x169; /* JMP */
    m->st = 1;
x161:
    { int v = 0 - m->a; m->cf = v < 0; m->zf = v == 0; m->st = !m->zf; } /* CI */
    if (m->st) goto x164; /* JMP */
    m->st = 1;
    if (m->st) goto x16F; /* JMP */
    m->st = 1;
x164:
    m->a = m->th; m->zf = m->a == 0; m->st = 1;
    m->st = !(m->a >> 0 & 1); /* TBA */
    if (m->st) goto x169; /* JMP */
    m->st = 1;
    m->y = 15; m->zf = 0; m->st = 1; /* LYI */
    { int v = (M + 1) & 15; M = (uint8_t)v; m->zf = v == 0; m->st = !m->zf; } /* ICM */
x169:
    m->a = m->th; m->zf = m->a == 0; m->st = 1;
    m->st = !(m->a >> 0 & 1); /* TBA */
    if (m->st) goto x16F; /* JMP */
    m->st = 1;
    m->y = 15; m->zf = 0; m->st = 1; /* LYI */
    { int v = (M + 1) & 15; M = (uint8_t)v; m->zf = v == 0; m->st = !m->zf; } /* ICM */
    { int v = (M + 1) & 15; M = (uint8_t)v; m->zf = v == 0; m->st = !m->zf; } /* ICM */
x16F:
    m->y = 15; m->zf = 0; m->st = 1; /* LYI */
    m->x = 1; m->zf = 0; m->st = 1; /* LXI */
    m->a = M; m->zf = m->a == 0; m->st = 1; /* L */
    m->p = m->a; m->st = 1; /* OUTP */
    m->x = 1; m->zf = 0; m->st = 1; /* LXI */
    m->y = 10; m->zf = 0; m->st = 1; /* LYI */
    m->a = M; m->zf = m->a == 0; m->st = 1; /* L */
    { int v = 0 - m->a; m->cf = v < 0; m->zf = v == 0; m->st = !m->zf; } /* CI */
    if (m->st) goto x17C; /* JPL */
    m->st = 1;
    m->a = 15; m->zf = 0; m->st = 1; /* LI */
    m->sb = m->a; m->st = 1;
    if (m->st) return; /* JMP $17E: the loop waits for the next frame */
    m->st = 1;
x17C:
    m->a = 0; m->zf = 1; m->st = 1; /* LI */
    m->sb = m->a; m->st = 1;
    return; /* on to $17E */
x1D7:
    m->a = 2; m->zf = 0; m->st = 1; /* LI */
    if (m->st) goto x1DA; /* JMP */
    m->st = 1;
x1D9:
    m->a = 1; m->zf = 0; m->st = 1; /* LI */
x1DA:
    m->y = 9; m->zf = 0; m->st = 1; /* LYI */
    m->x = 1; m->zf = 0; m->st = 1; /* LXI */
    m->cf = 0; m->st = 1;
    { int v = M - m->a - m->cf; m->cf = v < 0; m->a = v & 15; m->zf = m->a == 0; m->st = !m->cf; } /* SBC */
    if (m->cf || m->a > 9) { int v = m->a + 10; if (v > 15) m->cf = 1; m->a = v & 15; } m->st = !m->cf; /* DAS */
    M = m->a; m->y = (m->y + 1) & 15; m->zf = m->y == 0; m->st = !m->zf; /* STIC */
    m->a = 0; m->zf = 1; m->st = 1; /* LI */
    { int v = M - m->a - m->cf; m->cf = v < 0; m->a = v & 15; m->zf = m->a == 0; m->st = !m->cf; } /* SBC */
    M = m->a; m->st = 1; /* ST */
    if (m->st) m51_265(m); /* CALL */
    m->st = 1;
    m->a = 2; m->zf = 0; m->st = 1; /* LI */
    m->x = 0; m->zf = 1; m->st = 1; /* LXI */
    m->y = 3; m->zf = 0; m->st = 1; /* LYI */
    M = m->a; m->st = 1; /* ST */
    m->x = 1; m->zf = 0; m->st = 1; /* LXI */
    m->y = 13; m->zf = 0; m->st = 1; /* LYI */
    m->a = 0; m->zf = 1; m->st = 1; /* LI */
    M = m->a; m->y = (m->y + 1) & 15; m->zf = m->y == 0; m->st = !m->zf; /* STIC */
    M = m->a; m->st = 1; /* ST */
    m->y = 3; m->zf = 0; m->st = 1; /* LYI */
    m->a = M; m->zf = m->a == 0; m->st = 1; /* L */
    { int v = 0 - m->a; m->cf = v < 0; m->zf = v == 0; m->st = !m->zf; } /* CI */
    if (m->st) goto x120; /* JPL */
    m->st = 1;
    m->y = 9; m->zf = 0; m->st = 1; /* LYI */
    M = m->a; m->y = (m->y + 1) & 15; m->zf = m->y == 0; m->st = !m->zf; /* STIC */
    m->a = 10; m->zf = 0; m->st = 1; /* LI */
    M = m->a; m->st = 1; /* ST */
    if (m->st) goto x120; /* JPL */
    m->st = 1;
x1F9:
    m->x = 2; m->zf = 0; m->st = 1; /* LXI */
    m->y = 3; m->zf = 0; m->st = 1; /* LYI */
    m->a = 1; m->zf = 0; m->st = 1; /* LI */
    m->a &= M; m->zf = m->a == 0; m->st = !m->zf; /* AND */
    m->y = 7; m->zf = 0; m->st = 1; /* LYI */
    M = m->a; m->st = 1; /* ST */
    m->y = 2; m->zf = 0; m->st = 1; /* LYI */
    m->a = 1; m->zf = 0; m->st = 1; /* LI */
    m->a &= M; m->zf = m->a == 0; m->st = !m->zf; /* AND */
    m->cf = 0; m->st = 1;
    { int v = m->a << 1 | m->cf; m->cf = (v >> 4) & 1; m->a = v & 15; m->zf = m->a == 0; m->st = !m->cf; } /* ROL */
    m->y = 7; m->zf = 0; m->st = 1; /* LYI */
    m->a |= M; m->zf = m->a == 0; m->st = !m->zf; /* OR */
    M = m->a; m->st = 1; /* ST */
    m->x = 0; m->zf = 1; m->st = 1; /* LXI */
    m->y = 2; m->zf = 0; m->st = 1; /* LYI */
    m->a = M; m->zf = m->a == 0; m->st = 1; /* L */
    m->x = 2; m->zf = 0; m->st = 1; /* LXI */
    { int v = 0 - m->a; m->cf = v < 0; m->zf = v == 0; m->st = !m->zf; } /* CI */
    if (m->st) goto x214; /* JMP */
    m->st = 1;
    m->y = 0; m->zf = 1; m->st = 1; /* LYI */
    m->a = m51_in(m, m->y & 3); m->zf = m->a == 0; m->st = 1; /* IN */
    if (m->st) m51_272(m); /* CALL */
    m->st = 1;
    m->y = 6; m->zf = 0; m->st = 1; /* LYI */
    M = m->a; m->st = 1; /* ST */
    if (m->st) goto x218; /* JMP */
    m->st = 1;
x214:
    m->y = 0; m->zf = 1; m->st = 1; /* LYI */
    m->a = m51_in(m, m->y & 3); m->zf = m->a == 0; m->st = 1; /* IN */
    m->y = 6; m->zf = 0; m->st = 1; /* LYI */
    M = m->a; m->st = 1; /* ST */
x218:
    m->x = 2; m->zf = 0; m->st = 1; /* LXI */
    m->y = 3; m->zf = 0; m->st = 1; /* LYI */
    m->a = 2; m->zf = 0; m->st = 1; /* LI */
    m->a &= M; m->zf = m->a == 0; m->st = !m->zf; /* AND */
    m->cf = 0; m->st = 1;
    { int v = m->a >> 1 | m->cf << 3; m->cf = m->a & 1; m->a = v & 15; m->zf = m->a == 0; m->st = !m->cf; } /* ROR */
    m->y = 9; m->zf = 0; m->st = 1; /* LYI */
    M = m->a; m->st = 1; /* ST */
    m->y = 2; m->zf = 0; m->st = 1; /* LYI */
    m->a = 2; m->zf = 0; m->st = 1; /* LI */
    m->a &= M; m->zf = m->a == 0; m->st = !m->zf; /* AND */
    m->y = 9; m->zf = 0; m->st = 1; /* LYI */
    m->a |= M; m->zf = m->a == 0; m->st = !m->zf; /* OR */
    M = m->a; m->st = 1; /* ST */
    m->x = 0; m->zf = 1; m->st = 1; /* LXI */
    m->y = 2; m->zf = 0; m->st = 1; /* LYI */
    m->a = M; m->zf = m->a == 0; m->st = 1; /* L */
    m->x = 2; m->zf = 0; m->st = 1; /* LXI */
    m->y = 1; m->zf = 0; m->st = 1; /* LYI */
    { int v = 0 - m->a; m->cf = v < 0; m->zf = v == 0; m->st = !m->zf; } /* CI */
    if (m->st) goto x234; /* JMP */
    m->st = 1;
    m->a = m51_in(m, m->y & 3); m->zf = m->a == 0; m->st = 1; /* IN */
    if (m->st) m51_272(m); /* CALL */
    m->st = 1;
    m->y = 8; m->zf = 0; m->st = 1; /* LYI */
    M = m->a; m->st = 1; /* ST */
    if (m->st) goto x120; /* JPL */
    m->st = 1;
x234:
    m->a = m51_in(m, m->y & 3); m->zf = m->a == 0; m->st = 1; /* IN */
    m->y = 8; m->zf = 0; m->st = 1; /* LYI */
    M = m->a; m->st = 1; /* ST */
    if (m->st) goto x120; /* JPL */
    m->st = 1;
x239:
    m->x = 1; m->zf = 0; m->st = 1; /* LXI */
    m->y = 9; m->zf = 0; m->st = 1; /* LYI */
    m->a = 0; m->zf = 1; m->st = 1; /* LI */
    m->sb = m->a; m->st = 1;
    M = m->a; m->y = (m->y + 1) & 15; m->zf = m->y == 0; m->st = !m->zf; /* STIC */
    M = m->a; m->st = 1; /* ST */
    m->y = 13; m->zf = 0; m->st = 1; /* LYI */
    M = m->a; m->y = (m->y + 1) & 15; m->zf = m->y == 0; m->st = !m->zf; /* STIC */
    M = m->a; m->st = 1; /* ST */
    m->x = 2; m->zf = 0; m->st = 1; /* LXI */
    m->y = 4; m->zf = 0; m->st = 1; /* LYI */
    m->a = 11; m->zf = 0; m->st = 1; /* LI */
    M = m->a; m->y = (m->y + 1) & 15; m->zf = m->y == 0; m->st = !m->zf; /* STIC */
    M = m->a; m->y = (m->y + 1) & 15; m->zf = m->y == 0; m->st = !m->zf; /* STIC */
    m->x = 0; m->zf = 1; m->st = 1; /* LXI */
    m->y = 3; m->zf = 0; m->st = 1; /* LYI */
    m->a = 1; m->zf = 0; m->st = 1; /* LI */
    M = m->a; m->st = 1; /* ST */
    m->a = 15; m->zf = 0; m->st = 1; /* LI */
    m->p = m->a; m->st = 1; /* OUTP */
    if (m->st) return; /* JPL */
    m->st = 1;
    return; /* on to $24F */
}

static void m51_187(Mcu *m) /* $187 */
{
    m->y = 3; m->zf = 0; m->st = 1; /* LYI */
    m->a = M; m->zf = m->a == 0; m->st = 1; /* L */
    m->y = 13; m->zf = 0; m->st = 1; /* LYI */
    { int v = (M + 1) & 15; M = (uint8_t)v; m->zf = v == 0; m->st = !m->zf; } /* ICM */
    { int v = M - m->a; m->cf = v < 0; m->zf = v == 0; m->st = !m->zf; } /* C */
    if (m->st) goto x19F; /* JMP */
    m->st = 1;
    m->a = 0; m->zf = 1; m->st = 1; /* LI */
    M = m->a; m->st = 1; /* ST */
    m->y = 2; m->zf = 0; m->st = 1; /* LYI */
    m->a = M; m->zf = m->a == 0; m->st = 1; /* L */
    m->y = 8; m->zf = 0; m->st = 1; /* LYI */
    M = m->a; m->y = (m->y + 1) & 15; m->zf = m->y == 0; m->st = !m->zf; /* STIC */
    m->cf = 0; m->st = 1;
    { int v = m->a + M + m->cf; m->cf = v > 15; m->a = v & 15; m->zf = m->a == 0; m->st = !m->cf; } /* ADC */
    if (m->cf || m->a > 9) { int v = m->a + 6; if (v > 15) m->cf = 1; m->a = v & 15; } m->st = !m->cf; /* DAA */
    M = m->a; m->y = (m->y + 1) & 15; m->zf = m->y == 0; m->st = !m->zf; /* STIC */
    m->a = 0; m->zf = 1; m->st = 1; /* LI */
    { int v = m->a + M + m->cf; m->cf = v > 15; m->a = v & 15; m->zf = m->a == 0; m->st = !m->cf; } /* ADC */
    M = m->a; m->st = 1; /* ST */
    { int v = 10 - m->a; m->cf = v < 0; m->zf = v == 0; m->st = !m->zf; } /* CI */
    if (m->st) goto x19F; /* JMP */
    m->st = 1;
    m->a = 9; m->zf = 0; m->st = 1; /* LI */
    M = m->a; m->y = (m->y - 1) & 15; m->zf = m->y == 0; m->st = m->y != 15; /* STDC */
    M = m->a; m->st = 1; /* ST */
x19F:
    m->y = 11; m->zf = 0; m->st = 1; /* LYI */
    { int v = (M + 1) & 15; M = (uint8_t)v; m->zf = v == 0; m->st = !m->zf; } /* ICM */
    if (m->st) goto x1A3; /* JMP */
    m->st = 1;
    { int v = (M - 1) & 15; M = (uint8_t)v; m->zf = v == 0; m->st = v != 15; } /* DCM */
x1A3:
    m->st = 1; return; /* RTS */
}

static void m51_1A4(Mcu *m) /* $1A4 */
{
    m->y = 1; m->zf = 0; m->st = 1; /* LYI */
    m->a = M; m->zf = m->a == 0; m->st = 1; /* L */
    m->y = 14; m->zf = 0; m->st = 1; /* LYI */
    { int v = (M + 1) & 15; M = (uint8_t)v; m->zf = v == 0; m->st = !m->zf; } /* ICM */
    { int v = M - m->a; m->cf = v < 0; m->zf = v == 0; m->st = !m->zf; } /* C */
    if (m->st) goto x1C1; /* JPL */
    m->st = 1;
    m->a = 0; m->zf = 1; m->st = 1; /* LI */
    M = m->a; m->st = 1; /* ST */
    m->y = 0; m->zf = 1; m->st = 1; /* LYI */
    m->a = M; m->zf = m->a == 0; m->st = 1; /* L */
    m->y = 8; m->zf = 0; m->st = 1; /* LYI */
    { int v = M; M = m->a; m->a = (uint8_t)v; m->zf = v == 0; m->st = 1; } /* X */
    m->a |= M; m->zf = m->a == 0; m->st = !m->zf; /* OR */
    { int v = M; M = m->a; m->a = (uint8_t)v; m->zf = v == 0; m->st = 1; } /* X */
    m->y = (m->y + 1) & 15; m->zf = m->y == 0; m->st = !m->zf; /* ICY */
    m->cf = 0; m->st = 1;
    { int v = m->a + M + m->cf; m->cf = v > 15; m->a = v & 15; m->zf = m->a == 0; m->st = !m->cf; } /* ADC */
    if (m->cf || m->a > 9) { int v = m->a + 6; if (v > 15) m->cf = 1; m->a = v & 15; } m->st = !m->cf; /* DAA */
    M = m->a; m->y = (m->y + 1) & 15; m->zf = m->y == 0; m->st = !m->zf; /* STIC */
    m->a = 0; m->zf = 1; m->st = 1; /* LI */
    { int v = m->a + M + m->cf; m->cf = v > 15; m->a = v & 15; m->zf = m->a == 0; m->st = !m->cf; } /* ADC */
    M = m->a; m->st = 1; /* ST */
    { int v = 10 - m->a; m->cf = v < 0; m->zf = v == 0; m->st = !m->zf; } /* CI */
    if (m->st) goto x1C1; /* JPL */
    m->st = 1;
    m->a = 9; m->zf = 0; m->st = 1; /* LI */
    M = m->a; m->y = (m->y - 1) & 15; m->zf = m->y == 0; m->st = m->y != 15; /* STDC */
    M = m->a; m->st = 1; /* ST */
x1C1:
    m->y = 12; m->zf = 0; m->st = 1; /* LYI */
    { int v = (M + 1) & 15; M = (uint8_t)v; m->zf = v == 0; m->st = !m->zf; } /* ICM */
    if (m->st) goto x1C5; /* JMP */
    m->st = 1;
    { int v = (M - 1) & 15; M = (uint8_t)v; m->zf = v == 0; m->st = v != 15; } /* DCM */
x1C5:
    m->st = 1; return; /* RTS */
}

static void m51_1C6(Mcu *m) /* $1C6 */
{
    m->a = 1; m->zf = 0; m->st = 1; /* LI */
    m->y = 8; m->zf = 0; m->st = 1; /* LYI */
    M = m->a; m->st = 1; /* ST */
    m->y = (m->y + 1) & 15; m->zf = m->y == 0; m->st = !m->zf; /* ICY */
    m->cf = 0; m->st = 1;
    { int v = m->a + M + m->cf; m->cf = v > 15; m->a = v & 15; m->zf = m->a == 0; m->st = !m->cf; } /* ADC */
    if (m->cf || m->a > 9) { int v = m->a + 6; if (v > 15) m->cf = 1; m->a = v & 15; } m->st = !m->cf; /* DAA */
    M = m->a; m->y = (m->y + 1) & 15; m->zf = m->y == 0; m->st = !m->zf; /* STIC */
    m->a = 0; m->zf = 1; m->st = 1; /* LI */
    { int v = m->a + M + m->cf; m->cf = v > 15; m->a = v & 15; m->zf = m->a == 0; m->st = !m->cf; } /* ADC */
    M = m->a; m->st = 1; /* ST */
    { int v = 10 - m->a; m->cf = v < 0; m->zf = v == 0; m->st = !m->zf; } /* CI */
    if (m->st) goto x1D6; /* JMP */
    m->st = 1;
    m->a = 9; m->zf = 0; m->st = 1; /* LI */
    M = m->a; m->y = (m->y - 1) & 15; m->zf = m->y == 0; m->st = m->y != 15; /* STDC */
    M = m->a; m->st = 1; /* ST */
x1D6:
    m->st = 1; return; /* RTS */
}

static void m51_265(Mcu *m) /* $265 */
{
    m->x = 1; m->zf = 0; m->st = 1; /* LXI */
    m->y = 9; m->zf = 0; m->st = 1; /* LYI */
    m->a = M; m->zf = m->a == 0; m->st = 1; /* L */
    m->y = 4; m->zf = 0; m->st = 1; /* LYI */
    m->x = 2; m->zf = 0; m->st = 1; /* LXI */
    M = m->a; m->st = 1; /* ST */
    m->x = 1; m->zf = 0; m->st = 1; /* LXI */
    m->y = 10; m->zf = 0; m->st = 1; /* LYI */
    m->a = M; m->zf = m->a == 0; m->st = 1; /* L */
    m->x = 2; m->zf = 0; m->st = 1; /* LXI */
    m->y = 5; m->zf = 0; m->st = 1; /* LYI */
    M = m->a; m->st = 1; /* ST */
    m->st = 1; return; /* RTS */
}

/* $07E: the reset: RAM cleared, the output buffer's end set, switch mode */
static void m51_reset(Mcu *m)
{
    memset(m, 0, sizeof *m);
    m->st = 1;
    m->coinage = -1;
    m->ram[1 << 4 | 15] = 0x0C;
    m->ram[2 << 4 | 10] = 0x0F;
    m->ram[2 << 4 | 11] = 0x0F;
    m->ram[3] = 1; /* switch mode */
    m->pio = 0x64;
    m->in0 = m->in1 = 0xFF;
}

/* $002: the select line's rise in write mode, K the byte's low bits */
void io_51xx_write(Mcu *m, uint8_t v)
{
    m->k = v & 7;
    if (m->coinage >= 0) { /* $0008: command 1's bytes to 1:3, 1:2, 1:1, 1:0 (0: 1:A = $A) */
        uint8_t a = m->k;
        if (!a) m->ram[1 << 4 | 10] = 0x0A;
        m->ram[1 << 4 | m->coinage] = a;
        m->coinage--;
    } else {
        switch (m->k) {
        case 1: m->coinage = 3; return; /* the coinage: four bytes on */
        case 2: m->ram[3] = 0; break;   /* credit mode */
        case 3: m->ram[2] = 1; break;
        case 4: m->ram[2] = 0; break;
        case 5: m->ram[3] = 1; break;   /* switch mode */
        default: break;
        }
    }
    if (m->coinage < 0) m->ram[6] = 4; /* $0022: the output buffer from its start */
}

/* $024F: the rise in read mode: the next byte of the buffer at 2:4-2:B to O */
uint8_t io_51xx_read(Mcu *m)
{
    uint8_t y = m->ram[6];
    m->o = (uint8_t)(m->ram[2 << 4 | y] | m->ram[2 << 4 | ((y + 1) & 15)] << 4);
    y = (uint8_t)((y + 2) & 15);
    if (y == 12) y = 4;
    m->ram[6] = y;
    return m->o;
}

/* ---- the bus ----------------------------------------------------------------- */

void io_reset(Io *io)
{
    memset(io, 0, sizeof *io);
    m51_reset(&io->m51);
    io->ctrl = 0x10;
}

void io_frame(Io *io, uint8_t in0, uint8_t in1)
{
    Mcu *m = &io->m51;
    m->in0 = in0;
    m->in1 = in1;
    if (m->pio & 0x20) { /* the timer counts the vertical blanks */
        m->tl = (m->tl + 1) & 15;
        if (!m->tl) {
            m->th = (m->th + 1) & 15;
            if (!m->th) m->vf = 1;
        }
    }
    m51_frame(m);
}

void io_transfer(Io *io, Game *g, uint8_t ctrl, uint16_t mem, int n)
{
    if (ctrl & 0x10) { /* read: each rise has the 51xx put a byte out, main takes the one before */
        uint8_t v[256];
        for (int i = 0; i <= n && i < 256; i++) v[i] = (ctrl & 1) ? io_51xx_read(&io->m51) : 0xFF;
        for (int i = 0; i < n && i < 256; i++) game_wr(g, (uint16_t)(mem + i), v[i]);
    } else {
        for (int i = 0; i < n; i++) {
            uint8_t v = game_rd(g, (uint16_t)(mem + i));
            if (ctrl & 1) io_51xx_write(&io->m51, v);
            if ((ctrl & 8) && i < 16) io->noise[i] = v;
        }
        if (ctrl & 8) io->noises++;
    }
    io->ctrl = 0x10;
}
