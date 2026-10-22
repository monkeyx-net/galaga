#include "sim/sound.h"

/* sub2's RAM ($9A00-$9AFF) */
#define TICK 0x9A00      /* a byte per track: its note's frames so far */
#define POS 0x9A30       /* a byte per track: its note's offset in its list */
#define SHADOW 0x9A60    /* the WSG's registers $10-$1F, copied out each NMI */
#define WAVE 0x9A70      /* the three voices' waveforms */
#define SOUND 0x9A74     /* the sound being played */
#define TRACK 0x9A75     /* its track, tracks left, voice: from its header */
#define LEFT 0x9A76
#define VOICE 0x9A77
#define ENDED 0x9A78     /* a track reached its end */
#define NOTE 0x9A7A      /* the note's address (a word) */
#define PARAMS 0x9A88    /* the track's envelope, decay start, waveform */
#define BUSY 0x9A8C      /* the NMI is under way */
#define REQUEST 0x9AA0   /* a byte per sound: asked for */
#define PLAYING 0x9AC0   /* a byte per sound: started */

/* sub2's ROM */
#define D(a) game_data(g, GAME_SUB2 + (unsigned)(a))
#define HEADERS 0x0734   /* 3 bytes a sound: first track, tracks, voice */
#define TRACKS 0x0779    /* a word a track: its parameters, then its notes */
#define PITCHES 0x06DA   /* a word a note: the frequency of its top octave */
#define TEMPO 0x07D7     /* a byte a sound: frames a note unit */

/* The register a voice's frequency (0) or volume (1) goes to */
static uint16_t voice_reg(uint8_t voice, int volume)
{
    if (!voice) return volume ? 0x9A65 : 0x9A61;
    if (voice == 1) return volume ? 0x9A6A : 0x9A66;
    return volume ? 0x9A6F : 0x9A6B;
}

/* $06BC: the track has ended: its voice silent */
static void track_end(Game *g)
{
    WR(voice_reg(RD(VOICE), 1), 0);
    WR(ENDED, 1);
}

/* $0581: a frame of the current track (TRACK, VOICE) */
static void track_step(Game *g)
{
    uint8_t track = RD(TRACK);
    uint16_t tick = (uint16_t)(TICK + track);
    WR(tick, RD(tick) + 1);
    uint16_t t = (uint16_t)(D(TRACKS + 2 * track) | D(TRACKS + 2 * track + 1) << 8);
    for (int i = 0; i < 3; i++) WR(PARAMS + i, D(t + i));
    uint16_t note = (uint16_t)(t + 3 + RD(POS + track));
    game_wr16(g, NOTE, note);
    uint8_t n = D(note);
    if (n == 0xFF) {
        track_end(g);
        return;
    }
    uint16_t f = (uint16_t)(D(PITCHES + 2 * (n & 15)) | D(PITCHES + 2 * (n & 15) + 1) << 8);
    for (int octave = n >> 4 & 15; octave; octave--) f >>= 1;
    uint8_t c = (uint8_t)f, b = (uint8_t)(f >> 8);
    uint16_t r = voice_reg(RD(VOICE), 0);
    WR(r, c);
    WR(r + 1, (uint8_t)(c >> 4 | c << 4));
    WR(r + 2, b);
    WR(r + 3, (uint8_t)(b >> 4 | b << 4));
    /* the volume: the envelope from the note's frames */
    uint16_t de = voice_reg(RD(VOICE), 1);
    uint8_t a, frames = RD(tick);
    if (n == 0x0C) { /* a rest */
        a = 0;
    } else {
        uint8_t env = RD(PARAMS);
        if (env == 1 && frames < 6) a = (uint8_t)(frames * 2);        /* rising */
        else if (env > 1 && frames < 6) a = (uint8_t)~frames;          /* falling */
        else {
            uint8_t start = RD(PARAMS + 1);
            if (!start || frames < start) a = 0x0A;
            else if ((uint8_t)(frames - start) >= 0x0A) a = 0;
            else a = (uint8_t)(0x0A - (frames - start));
        }
    }
    WR(de, a);
    WR(WAVE + RD(VOICE), RD(PARAMS + 2));
    /* the note's length: its units x the sound's frames a unit */
    uint8_t unit = D(TEMPO + RD(SOUND));
    uint8_t length = (uint8_t)(unit * D(note + 1));
    if (RD(tick) != length) return;
    WR(POS + track, RD(POS + track) + 2);
    WR(tick, 0);
}

/* The header of the sound in SOUND: its first track, tracks, voice */
static void header(Game *g)
{
    uint8_t s = RD(SOUND);
    for (int i = 0; i < 3; i++) WR(TRACK + i, D(HEADERS + 3 * s + i));
    if (s != 0x0E) return;
    /* $039D: sound $0E's tracks by $9A4C and $9A4D */
    uint8_t a = RD(0x9A4C);
    if (!a) WR(LEFT, 1);
    else if (a == 1 || !RD(0x9A4D)) WR(LEFT, 2);
}

/* Its tracks from their start */
static void restart(Game *g)
{
    uint8_t b = RD(LEFT), track = RD(TRACK);
    game_fill(g, (uint16_t)(POS + track), 0, b ? b : 256);
    game_fill(g, (uint16_t)(TICK + track), 0, b ? b : 256);
}

/* A frame of each of its tracks */
static void tracks(Game *g)
{
    for (;;) {
        track_step(g);
        WR(LEFT, RD(LEFT) - 1);
        if (!RD(LEFT)) break;
        WR(TRACK, RD(TRACK) + 1);
        WR(VOICE, RD(VOICE) + 1);
    }
}

/* Its end: no longer playing */
static int ended(Game *g)
{
    if (!RD(ENDED)) return 0;
    WR(ENDED, 0);
    WR(PLAYING + RD(SOUND), 0);
    return 1;
}

/* $0384: played while asked for: started once */
static void play(Game *g)
{
    header(g);
    uint16_t p = (uint16_t)(PLAYING + RD(SOUND));
    if (!RD(p)) {
        WR(p, RD(p) + 1);
        restart(g);
    }
    tracks(g);
    ended(g);
}

/* $0403: started anew */
static void start(Game *g)
{
    uint16_t p = (uint16_t)(PLAYING + RD(SOUND));
    WR(p, RD(p) + 1);
    header(g);
    restart(g);
    tracks(g);
    ended(g);
}

/* $0459: played on */
static void resume(Game *g)
{
    header(g);
    tracks(g);
    ended(g);
}

/* $04B1: asked for, played to its end, then the request taken back */
static void play_once(Game *g)
{
    header(g);
    uint16_t p = (uint16_t)(PLAYING + RD(SOUND));
    if (!RD(p)) {
        WR(p, RD(p) + 1);
        restart(g);
    }
    tracks(g);
    if (!ended(g)) return;
    uint8_t s = RD(SOUND);
    uint16_t hl = (uint16_t)(REQUEST + s);
    if (s == 0x08) {
        WR(hl, RD(hl) - 1);
    } else if (s == 0x0C) {
        WR(hl, RD(hl) - 1);
        if (!RD(hl) || (RD(hl) & 1)) WR(0x9AB6, 1);
    } else if (s == 0x14) {
        WR(hl, 0);
        WR(0x9AB3, 1);
    } else if (s == 0x07) { /* all but $9AA8, $9AAA and their flags */
        WR(hl, 0);
        game_fill(g, REQUEST, 0, 8);
        WR(REQUEST + 9, 0);
        game_fill(g, REQUEST + 0x0B, 0, 0x0C);
        game_fill(g, PLAYING, 0, 8);
        WR(PLAYING + 9, 0);
        game_fill(g, PLAYING + 0x0B, 0, 0x0C);
    } else {
        WR(hl, 0);
    }
}

/* $00EB: the sound at $9AA0: a glide through the table at $06F4 or $0704,
   restarted when $9211 changes */
static void glide(Game *g)
{
    uint8_t a = RD(0x9211);
    if (a != RD(0x9A80)) {
        WR(0x9A80, a);
        game_wr16(g, 0x9A82, (uint16_t)((uint8_t)(a + 1) ? 0x06F4 : 0x0704));
        WR(TICK, 0);
        WR(0x9A81, 0);
    } else {
        WR(TICK, RD(TICK) + 1);
        if (RD(TICK) != 0x22) goto step;
        WR(TICK, 0);
        WR(0x9A81, RD(0x9A81) + 1);
    }
    {
        uint16_t t = (uint16_t)(game_rd16(g, 0x9A82) + 2 * RD(0x9A81));
        game_wr16(g, 0x9A84, (uint16_t)(D(t) | D(t + 1) << 8));
        game_wr16(g, 0x9A86, (uint16_t)(D(t + 0x20) | D(t + 0x21) << 8));
    }
step:;
    uint16_t at = (uint16_t)(game_rd16(g, 0x9A86) + game_rd16(g, 0x9A84));
    game_wr16(g, 0x9A86, at);
    uint8_t h = (uint8_t)(at >> 8);
    WR(0x9A61, h);
    WR(0x9A62, (uint8_t)(h >> 4 | h << 4));
    WR(0x9A65, 0x0A);
    WR(WAVE, 0);
}

/* A sound by its request flag: started, or played on */
static void startable(Game *g, uint8_t sound)
{
    WR(SOUND, sound);
    if (RD(REQUEST + sound)) {
        WR(REQUEST + sound, 0);
        start(g);
    } else if (RD(PLAYING + sound)) {
        resume(g);
    }
}

/* A sound played while asked for; its flag cleared otherwise */
static int while_asked(Game *g, uint8_t sound)
{
    if (!RD(REQUEST + sound)) {
        WR(PLAYING + sound, 0);
        return 0;
    }
    WR(SOUND, sound);
    play(g);
    return 1;
}

/* A sound played once when asked for */
static void once(Game *g, uint8_t sound)
{
    if (!RD(REQUEST + sound)) return;
    WR(SOUND, sound);
    play_once(g);
}

void sound_work(Game *g)
{
    if (RD(0x9AB8)) { /* $06AB: sub2 starts again */
        game_unported(g, 0x06AB);
        return;
    }
    game_fill(g, SHADOW, 0, 16);
    if (RD(0x9AB7)) { /* $034C: everything stopped */
        game_fill(g, REQUEST, 0, 0x16);
        game_fill(g, PLAYING, 0, 0x17);
    } else {
        uint8_t a = RD(0x9A79);
        if (a) {
            WR(0x9AA8, RD(0x9AA8) + a);
            WR(0x9A79, 0);
        }
        if (RD(REQUEST)) glide(g);
        static const uint8_t startables[6] = { 0x13, 0x0F, 0x03, 0x02, 0x04, 0x01 };
        for (int i = 0; i < 6; i++) startable(g, startables[i]);
        once(g, 0x12);
        if (while_asked(g, 0x05)) {
            WR(0x9A7E, RD(0x9A7E) + 1);
            if (RD(0x9A7E) >= 6) {
                WR(0x9A7E, 0);
                uint8_t v = RD(0x9A7C);
                WR(0x9A7C, v >= 4 ? v - 1 : 0x0C);
            }
            WR(0x9A6F, RD(0x9A7C));
        }
        if (while_asked(g, 0x06)) {
            WR(0x9A7F, RD(0x9A7F) + 1);
            if (RD(0x9A7F) == 0x1C) {
                WR(0x9A7F, 0);
                WR(0x9A7D, RD(0x9A7D) + 1);
            }
            WR(WAVE, RD(0x9A7D));
        }
        while_asked(g, 0x09);
        if (RD(REQUEST + 0x07)) {
            WR(SOUND, 0x07);
            play_once(g);
        } else {
            while_asked(g, 0x11);
            once(g, 0x0D);
            if (RD(REQUEST + 0x0E)) {
                WR(SOUND, 0x0E);
                play_once(g);
                WR(0x9A6A, 9);
                WR(0x9A6F, 6);
            }
            once(g, 0x14);
            once(g, 0x15);
            once(g, 0x0A);
            once(g, 0x0B);
            while_asked(g, 0x10);
            once(g, 0x0C);
            once(g, 0x16);
        }
        once(g, 0x08);
    }
    /* $0366: out to the WSG */
    for (int i = 0; i < 16; i++) WR(0x6810 + i, RD(SHADOW + i));
    WR(0x6805, RD(WAVE));
    WR(0x680A, RD(WAVE + 1));
    WR(0x680F, RD(WAVE + 2));
}

void sound_nmi(Game *g)
{
    if (RD(BUSY)) return;
    WR(0x6822, 1);
    WR(BUSY, 1);
    WR(0x6822, 0);
    sound_work(g);
    WR(BUSY, 0);
}
