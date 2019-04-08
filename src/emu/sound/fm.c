/*
**
** File: fm.c -- software implementation of Yamaha FM sound generator
**
** Copyright Jarek Burczynski (bujar at mame dot net)
** Copyright Tatsuyuki Satoh , MultiArcadeMachineEmulator development
**
** Version 1.4.2 (final beta)
**
*/

/*
** History:
**
** 2006-2008 Eke-Eke (Genesis Plus GX), MAME backport by R. Belmont.
**  - implemented PG overflow, aka "detune bug" (Ariel, Comix Zone, Shaq Fu, Spiderman,...), credits to Nemesis
**  - fixed SSG-EG support, credits to Nemesis and additional fixes from Alone Coder
**  - modified EG rates and frequency, tested by Nemesis on real hardware
**  - implemented LFO phase update for CH3 special mode (Warlock birds, Alladin bug sound)
**  - fixed Attack Rate update (Batman & Robin intro)
**  - fixed attenuation level at the start of Substain (Gynoug explosions)
**  - fixed EG decay->substain transition to handle special cases, like SL=0 and Decay rate is very slow (Mega Turrican tracks 03,09...)
**
** 06-23-2007 Zsolt Vasvari:
**  - changed the timing not to require the use of floating point calculations
**
** 03-08-2003 Jarek Burczynski:
**  - fixed YM2608 initial values (after the reset)
**  - fixed flag and irqmask handling (YM2608)
**  - fixed BUFRDY flag handling (YM2608)
**
** 14-06-2003 Jarek Burczynski:
**  - implemented all of the YM2608 status register flags
**  - implemented support for external memory read/write via YM2608
**  - implemented support for deltat memory limit register in YM2608 emulation
**
** 22-05-2003 Jarek Burczynski:
**  - fixed LFO PM calculations (copy&paste bugfix)
**
** 08-05-2003 Jarek Burczynski:
**  - fixed SSG support
**
** 22-04-2003 Jarek Burczynski:
**  - implemented 100% correct LFO generator (verified on real YM2610 and YM2608)
**
** 15-04-2003 Jarek Burczynski:
**  - added support for YM2608's register 0x110 - status mask
**
** 01-12-2002 Jarek Burczynski:
**  - fixed register addressing in YM2608, YM2610, YM2610B chips. (verified on real YM2608)
**    The addressing patch used for early Neo-Geo games can be removed now.
**
** 26-11-2002 Jarek Burczynski, Nicola Salmoria:
**  - recreated YM2608 ADPCM ROM using data from real YM2608's output which leads to:
**  - added emulation of YM2608 drums.
**  - output of YM2608 is two times lower now - same as YM2610 (verified on real YM2608)
**
** 16-08-2002 Jarek Burczynski:
**  - binary exact Envelope Generator (verified on real YM2203);
**    identical to YM2151
**  - corrected 'off by one' error in feedback calculations (when feedback is off)
**  - corrected connection (algorithm) calculation (verified on real YM2203 and YM2610)
**
** 18-12-2001 Jarek Burczynski:
**  - added SSG-EG support (verified on real YM2203)
**
** 12-08-2001 Jarek Burczynski:
**  - corrected sin_tab and tl_tab data (verified on real chip)
**  - corrected feedback calculations (verified on real chip)
**  - corrected phase generator calculations (verified on real chip)
**  - corrected envelope generator calculations (verified on real chip)
**  - corrected FM volume level (YM2610 and YM2610B).
**  - changed YMxxxUpdateOne() functions (YM2203, YM2608, YM2610, YM2610B, YM2612) :
**    this was needed to calculate YM2610 FM channels output correctly.
**    (Each FM channel is calculated as in other chips, but the output of the channel
**    gets shifted right by one *before* sending to accumulator. That was impossible to do
**    with previous implementation).
**
** 23-07-2001 Jarek Burczynski, Nicola Salmoria:
**  - corrected YM2610 ADPCM type A algorithm and tables (verified on real chip)
**
** 11-06-2001 Jarek Burczynski:
**  - corrected end of sample bug in ADPCMA_calc_cha().
**    Real YM2610 checks for equality between current and end addresses (only 20 LSB bits).
**
** 08-12-98 hiro-shi:
** rename ADPCMA -> ADPCMB, ADPCMB -> ADPCMA
** move ROM limit check.(CALC_CH? -> 2610Write1/2)
** test program (ADPCMB_TEST)
** move ADPCM A/B end check.
** ADPCMB repeat flag(no check)
** change ADPCM volume rate (8->16) (32->48).
**
** 09-12-98 hiro-shi:
** change ADPCM volume. (8->16, 48->64)
** replace ym2610 ch0/3 (YM-2610B)
** change ADPCM_SHIFT (10->8) missing bank change 0x4000-0xffff.
** add ADPCM_SHIFT_MASK
** change ADPCMA_DECODE_MIN/MAX.
*/




/************************************************************************/
/*    comment of hiro-shi(Hiromitsu Shioya)                             */
/*    YM2610(B) = OPN-B                                                 */
/*    YM2610  : PSG:3ch FM:4ch ADPCM(18.5KHz):6ch DeltaT ADPCM:1ch      */
/*    YM2610B : PSG:3ch FM:6ch ADPCM(18.5KHz):6ch DeltaT ADPCM:1ch      */
/************************************************************************/

#include "emu.h"
#include "fm.h"
#include "ymdeltat.h"


/* shared function building option */
#define BUILD_OPN	(BUILD_YM2203 || BUILD_YM2610)
#define BUILD_OPN_PRESCALER	(BUILD_YM2203)

/* globals */
#define TYPE_SSG	0x01    /* SSG support          */
#define TYPE_LFOPAN	0x02    /* OPN type LFO and PAN */
#define TYPE_6CH	0x04    /* FM 6CH / 3CH         */
#define TYPE_DAC	0x08    /* YM2612's DAC device  */
#define TYPE_ADPCM	0x10    /* two ADPCM units      */
#define TYPE_2610	0x20    /* bogus flag to differentiate 2608 from 2610 */


#define TYPE_YM2203	(TYPE_SSG)
#define TYPE_YM2610	(TYPE_SSG | TYPE_LFOPAN | TYPE_6CH | TYPE_ADPCM | TYPE_2610)


#define FREQ_SH		16  /* 16.16 fixed point (frequency calculations) */
#define EG_SH		16  /* 16.16 fixed point (envelope generator timing) */
#define LFO_SH		24  /*  8.24 fixed point (LFO calculations)       */
#define TIMER_SH	16  /* 16.16 fixed point (timers calculations)    */

#define FREQ_MASK	((1<<FREQ_SH)-1)

#define ENV_BITS	10
#define ENV_LEN		(1<<ENV_BITS)
#define ENV_STEP	(128.0/ENV_LEN)

#define MAX_ATT_INDEX	(ENV_LEN-1)	/* 1023 */
#define MIN_ATT_INDEX	(0)		/* 0 */

#define EG_ATT		4
#define EG_DEC		3
#define EG_SUS		2
#define EG_REL		1
#define EG_OFF		0

#define SIN_BITS	10
#define SIN_LEN		(1<<SIN_BITS)
#define SIN_MASK	(SIN_LEN-1)

#define TL_RES_LEN	(256)		/* 8 bits addressing (real chip) */

#define FINAL_SH	(0)
#define MAXOUT		(+32767)
#define MINOUT		(-32768)


/*  TL_TAB_LEN is calculated as:
*   13 - sinus amplitude bits     (Y axis)
*   2  - sinus sign bit           (Y axis)
*   TL_RES_LEN - sinus resolution (X axis)
*/
#define TL_TAB_LEN	(13*2*TL_RES_LEN)
static signed int tl_tab[TL_TAB_LEN];

#define ENV_QUIET	(TL_TAB_LEN>>3)

/* sin waveform table in 'decibel' scale */
static unsigned int sin_tab[SIN_LEN];

/* sustain level table (3dB per step) */
/* bit0, bit1, bit2, bit3, bit4, bit5, bit6 */
/* 1,    2,    4,    8,    16,   32,   64   (value)*/
/* 0.75, 1.5,  3,    6,    12,   24,   48   (dB)*/

/* 0 - 15: 0, 3, 6, 9,12,15,18,21,24,27,30,33,36,39,42,93 (dB)*/
#define SC(db)	(UINT32)(db * (4.0/ENV_STEP))
static const UINT32 sl_table[16] = {
	SC( 0), SC( 1), SC( 2), SC(3 ), SC(4 ), SC(5 ), SC(6 ), SC( 7),
	SC( 8), SC( 9), SC(10), SC(11), SC(12), SC(13), SC(14), SC(31)
};
#undef SC


#define RATE_STEPS	(8)
static const UINT8 eg_inc[19*RATE_STEPS] = {

/*cycle:0 1  2 3  4 5  6 7*/

/* 0 */ 0,1, 0,1, 0,1, 0,1, /* rates 00..11 0 (increment by 0 or 1) */
/* 1 */ 0,1, 0,1, 1,1, 0,1, /* rates 00..11 1 */
/* 2 */ 0,1, 1,1, 0,1, 1,1, /* rates 00..11 2 */
/* 3 */ 0,1, 1,1, 1,1, 1,1, /* rates 00..11 3 */

/* 4 */ 1,1, 1,1, 1,1, 1,1, /* rate 12 0 (increment by 1) */
/* 5 */ 1,1, 1,2, 1,1, 1,2, /* rate 12 1 */
/* 6 */ 1,2, 1,2, 1,2, 1,2, /* rate 12 2 */
/* 7 */ 1,2, 2,2, 1,2, 2,2, /* rate 12 3 */

/* 8 */ 2,2, 2,2, 2,2, 2,2, /* rate 13 0 (increment by 2) */
/* 9 */ 2,2, 2,4, 2,2, 2,4, /* rate 13 1 */
/*10 */ 2,4, 2,4, 2,4, 2,4, /* rate 13 2 */
/*11 */ 2,4, 4,4, 2,4, 4,4, /* rate 13 3 */

/*12 */ 4,4, 4,4, 4,4, 4,4, /* rate 14 0 (increment by 4) */
/*13 */ 4,4, 4,8, 4,4, 4,8, /* rate 14 1 */
/*14 */ 4,8, 4,8, 4,8, 4,8, /* rate 14 2 */
/*15 */ 4,8, 8,8, 4,8, 8,8, /* rate 14 3 */

/*16 */ 8,8, 8,8, 8,8, 8,8, /* rates 15 0, 15 1, 15 2, 15 3 (increment by 8) */
/*17 */ 16,16,16,16,16,16,16,16, /* rates 15 2, 15 3 for attack */
/*18 */ 0,0, 0,0, 0,0, 0,0  /* infinity rates for attack and decay(s) */
};


#define O(a)	(a*RATE_STEPS)

/*note that there is no O(17) in this table - it's directly in the code */
static const UINT8 eg_rate_select[32+64+32] = {	/* Envelope Generator rates (32 + 64 rates + 32 RKS) */
/* 32 infinite time rates */
O(18),O(18),O(18),O(18),O(18),O(18),O(18),O(18),
O(18),O(18),O(18),O(18),O(18),O(18),O(18),O(18),
O(18),O(18),O(18),O(18),O(18),O(18),O(18),O(18),
O(18),O(18),O(18),O(18),O(18),O(18),O(18),O(18),

/* rates 00-11 */
O( 0),O( 1),O( 2),O( 3),
O( 0),O( 1),O( 2),O( 3),
O( 0),O( 1),O( 2),O( 3),
O( 0),O( 1),O( 2),O( 3),
O( 0),O( 1),O( 2),O( 3),
O( 0),O( 1),O( 2),O( 3),
O( 0),O( 1),O( 2),O( 3),
O( 0),O( 1),O( 2),O( 3),
O( 0),O( 1),O( 2),O( 3),
O( 0),O( 1),O( 2),O( 3),
O( 0),O( 1),O( 2),O( 3),
O( 0),O( 1),O( 2),O( 3),

/* rate 12 */
O( 4),O( 5),O( 6),O( 7),

/* rate 13 */
O( 8),O( 9),O(10),O(11),

/* rate 14 */
O(12),O(13),O(14),O(15),

/* rate 15 */
O(16),O(16),O(16),O(16),

/* 32 dummy rates (same as 15 3) */
O(16),O(16),O(16),O(16),O(16),O(16),O(16),O(16),
O(16),O(16),O(16),O(16),O(16),O(16),O(16),O(16),
O(16),O(16),O(16),O(16),O(16),O(16),O(16),O(16),
O(16),O(16),O(16),O(16),O(16),O(16),O(16),O(16)

};
#undef O

/*rate  0,    1,    2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15 */
/*shift 11,  10,  9,  8,  7,  6,  5,  4,  3,  2, 1,  0,  0,  0,  0,  0 */
/*mask  2047, 1023, 511, 255, 127, 63, 31, 15, 7,  3, 1,  0,  0,  0,  0,  0 */

#define O(a)	(a*1)
static const UINT8 eg_rate_shift[32+64+32] = {	/* Envelope Generator counter shifts (32 + 64 rates + 32 RKS) */
/* 32 infinite time rates */
O(0),O(0),O(0),O(0),O(0),O(0),O(0),O(0),
O(0),O(0),O(0),O(0),O(0),O(0),O(0),O(0),
O(0),O(0),O(0),O(0),O(0),O(0),O(0),O(0),
O(0),O(0),O(0),O(0),O(0),O(0),O(0),O(0),

/* rates 00-11 */
O(11),O(11),O(11),O(11),
O(10),O(10),O(10),O(10),
O( 9),O( 9),O( 9),O( 9),
O( 8),O( 8),O( 8),O( 8),
O( 7),O( 7),O( 7),O( 7),
O( 6),O( 6),O( 6),O( 6),
O( 5),O( 5),O( 5),O( 5),
O( 4),O( 4),O( 4),O( 4),
O( 3),O( 3),O( 3),O( 3),
O( 2),O( 2),O( 2),O( 2),
O( 1),O( 1),O( 1),O( 1),
O( 0),O( 0),O( 0),O( 0),

/* rate 12 */
O( 0),O( 0),O( 0),O( 0),

/* rate 13 */
O( 0),O( 0),O( 0),O( 0),

/* rate 14 */
O( 0),O( 0),O( 0),O( 0),

/* rate 15 */
O( 0),O( 0),O( 0),O( 0),

/* 32 dummy rates (same as 15 3) */
O( 0),O( 0),O( 0),O( 0),O( 0),O( 0),O( 0),O( 0),
O( 0),O( 0),O( 0),O( 0),O( 0),O( 0),O( 0),O( 0),
O( 0),O( 0),O( 0),O( 0),O( 0),O( 0),O( 0),O( 0),
O( 0),O( 0),O( 0),O( 0),O( 0),O( 0),O( 0),O( 0)

};
#undef O

static const UINT8 dt_tab[4 * 32]={
/* this is YM2151 and YM2612 phase increment data (in 10.10 fixed point format)*/
/* FD=0 */
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
/* FD=1 */
	0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2,
	2, 3, 3, 3, 4, 4, 4, 5, 5, 6, 6, 7, 8, 8, 8, 8,
/* FD=2 */
	1, 1, 1, 1, 2, 2, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5,
	5, 6, 6, 7, 8, 8, 9,10,11,12,13,14,16,16,16,16,
/* FD=3 */
	2, 2, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 6, 6, 7,
	8 , 8, 9,10,11,12,13,14,16,17,19,20,22,22,22,22
};


/* OPN key frequency number -> key code follow table */
/* fnum higher 4bit -> keycode lower 2bit */
static const UINT8 opn_fktable[16] = {
	0,0,0,0,0,0,0,1,2,3,3,3,3,3,3,3
};


/* 8 LFO speed parameters */
/* each value represents number of samples that one LFO level will last for */
static const UINT32 lfo_samples_per_step[8] = {
	108, 77, 71, 67, 62, 44, 8, 5
};


/*There are 4 different LFO AM depths available, they are:
  0 dB, 1.4 dB, 5.9 dB, 11.8 dB
  Here is how it is generated (in EG steps):

  11.8 dB = 0, 2, 4, 6, 8, 10,12,14,16...126,126,124,122,120,118,....4,2,0
   5.9 dB = 0, 1, 2, 3, 4, 5, 6, 7, 8....63, 63, 62, 61, 60, 59,.....2,1,0
   1.4 dB = 0, 0, 0, 0, 1, 1, 1, 1, 2,...15, 15, 15, 15, 14, 14,.....0,0,0

  (1.4 dB is loosing precision as you can see)

  It's implemented as generator from 0..126 with step 2 then a shift
  right N times, where N is:
    8 for 0 dB
    3 for 1.4 dB
    1 for 5.9 dB
    0 for 11.8 dB
*/
static const UINT8 lfo_ams_depth_shift[4] = {
	8, 3, 1, 0
};


/*There are 8 different LFO PM depths available, they are:
  0, 3.4, 6.7, 10, 14, 20, 40, 80 (cents)

  Modulation level at each depth depends on F-NUMBER bits: 4,5,6,7,8,9,10
  (bits 8,9,10 = FNUM MSB from OCT/FNUM register)

  Here we store only first quarter (positive one) of full waveform.
  Full table (lfo_pm_table) containing all 128 waveforms is build
  at run (init) time.

  One value in table below represents 4 (four) basic LFO steps
  (1 PM step = 4 AM steps).

  For example:
   at LFO SPEED=0 (which is 108 samples per basic LFO step)
   one value from "lfo_pm_output" table lasts for 432 consecutive
   samples (4*108=432) and one full LFO waveform cycle lasts for 13824
   samples (32*432=13824; 32 because we store only a quarter of whole
            waveform in the table below)
*/
static const UINT8 lfo_pm_output[7*8][8] = { /* 7 bits meaningful (of F-NUMBER), 8 LFO output levels per one depth (out of 32), 8 LFO depths */
/* FNUM BIT 4: 000 0001xxxx */
/* DEPTH 0 */ {0,   0,   0,   0,   0,   0,   0,   0},
/* DEPTH 1 */ {0,   0,   0,   0,   0,   0,   0,   0},
/* DEPTH 2 */ {0,   0,   0,   0,   0,   0,   0,   0},
/* DEPTH 3 */ {0,   0,   0,   0,   0,   0,   0,   0},
/* DEPTH 4 */ {0,   0,   0,   0,   0,   0,   0,   0},
/* DEPTH 5 */ {0,   0,   0,   0,   0,   0,   0,   0},
/* DEPTH 6 */ {0,   0,   0,   0,   0,   0,   0,   0},
/* DEPTH 7 */ {0,   0,   0,   0,   1,   1,   1,   1},

/* FNUM BIT 5: 000 0010xxxx */
/* DEPTH 0 */ {0,   0,   0,   0,   0,   0,   0,   0},
/* DEPTH 1 */ {0,   0,   0,   0,   0,   0,   0,   0},
/* DEPTH 2 */ {0,   0,   0,   0,   0,   0,   0,   0},
/* DEPTH 3 */ {0,   0,   0,   0,   0,   0,   0,   0},
/* DEPTH 4 */ {0,   0,   0,   0,   0,   0,   0,   0},
/* DEPTH 5 */ {0,   0,   0,   0,   0,   0,   0,   0},
/* DEPTH 6 */ {0,   0,   0,   0,   1,   1,   1,   1},
/* DEPTH 7 */ {0,   0,   1,   1,   2,   2,   2,   3},

/* FNUM BIT 6: 000 0100xxxx */
/* DEPTH 0 */ {0,   0,   0,   0,   0,   0,   0,   0},
/* DEPTH 1 */ {0,   0,   0,   0,   0,   0,   0,   0},
/* DEPTH 2 */ {0,   0,   0,   0,   0,   0,   0,   0},
/* DEPTH 3 */ {0,   0,   0,   0,   0,   0,   0,   0},
/* DEPTH 4 */ {0,   0,   0,   0,   0,   0,   0,   1},
/* DEPTH 5 */ {0,   0,   0,   0,   1,   1,   1,   1},
/* DEPTH 6 */ {0,   0,   1,   1,   2,   2,   2,   3},
/* DEPTH 7 */ {0,   0,   2,   3,   4,   4,   5,   6},

/* FNUM BIT 7: 000 1000xxxx */
/* DEPTH 0 */ {0,   0,   0,   0,   0,   0,   0,   0},
/* DEPTH 1 */ {0,   0,   0,   0,   0,   0,   0,   0},
/* DEPTH 2 */ {0,   0,   0,   0,   0,   0,   1,   1},
/* DEPTH 3 */ {0,   0,   0,   0,   1,   1,   1,   1},
/* DEPTH 4 */ {0,   0,   0,   1,   1,   1,   1,   2},
/* DEPTH 5 */ {0,   0,   1,   1,   2,   2,   2,   3},
/* DEPTH 6 */ {0,   0,   2,   3,   4,   4,   5,   6},
/* DEPTH 7 */ {0,   0,   4,   6,   8,   8, 0xa, 0xc},

/* FNUM BIT 8: 001 0000xxxx */
/* DEPTH 0 */ {0,   0,   0,   0,   0,   0,   0,   0},
/* DEPTH 1 */ {0,   0,   0,   0,   1,   1,   1,   1},
/* DEPTH 2 */ {0,   0,   0,   1,   1,   1,   2,   2},
/* DEPTH 3 */ {0,   0,   1,   1,   2,   2,   3,   3},
/* DEPTH 4 */ {0,   0,   1,   2,   2,   2,   3,   4},
/* DEPTH 5 */ {0,   0,   2,   3,   4,   4,   5,   6},
/* DEPTH 6 */ {0,   0,   4,   6,   8,   8, 0xa, 0xc},
/* DEPTH 7 */ {0,   0,   8, 0xc,0x10,0x10,0x14,0x18},

/* FNUM BIT 9: 010 0000xxxx */
/* DEPTH 0 */ {0,   0,   0,   0,   0,   0,   0,   0},
/* DEPTH 1 */ {0,   0,   0,   0,   2,   2,   2,   2},
/* DEPTH 2 */ {0,   0,   0,   2,   2,   2,   4,   4},
/* DEPTH 3 */ {0,   0,   2,   2,   4,   4,   6,   6},
/* DEPTH 4 */ {0,   0,   2,   4,   4,   4,   6,   8},
/* DEPTH 5 */ {0,   0,   4,   6,   8,   8, 0xa, 0xc},
/* DEPTH 6 */ {0,   0,   8, 0xc,0x10,0x10,0x14,0x18},
/* DEPTH 7 */ {0,   0,0x10,0x18,0x20,0x20,0x28,0x30},

/* FNUM BIT10: 100 0000xxxx */
/* DEPTH 0 */ {0,   0,   0,   0,   0,   0,   0,   0},
/* DEPTH 1 */ {0,   0,   0,   0,   4,   4,   4,   4},
/* DEPTH 2 */ {0,   0,   0,   4,   4,   4,   8,   8},
/* DEPTH 3 */ {0,   0,   4,   4,   8,   8, 0xc, 0xc},
/* DEPTH 4 */ {0,   0,   4,   8,   8,   8, 0xc,0x10},
/* DEPTH 5 */ {0,   0,   8, 0xc,0x10,0x10,0x14,0x18},
/* DEPTH 6 */ {0,   0,0x10,0x18,0x20,0x20,0x28,0x30},
/* DEPTH 7 */ {0,   0,0x20,0x30,0x40,0x40,0x50,0x60},

};


/* all 128 LFO PM waveforms */
static INT32 lfo_pm_table[128*8*32]; /* 128 combinations of 7 bits meaningful (of F-NUMBER), 8 LFO depths, 32 LFO output levels per one depth */


/* register number to channel number , slot offset */
#define OPN_CHAN(N)	(N&3)
#define OPN_SLOT(N)	((N>>2)&3)

/* slot number */
#define SLOT1	0
#define SLOT2	2
#define SLOT3	1
#define SLOT4	3

/* bit0 = Right enable , bit1 = Left enable */
#define OUTD_RIGHT	1
#define OUTD_LEFT	2
#define OUTD_CENTER	3


/* struct describing a single operator (SLOT) */
typedef struct
{
	INT32		*DT;		/* detune          :dt_tab[DT] */
	UINT8		KSR;		/* key scale rate  :3-KSR */
	UINT32		ar;		/* attack rate  */
	UINT32		d1r;		/* decay rate   */
	UINT32		d2r;		/* sustain rate */
	UINT32		rr;		/* release rate */
	UINT8		ksr;		/* key scale rate  :kcode>>(3-KSR) */
	UINT32		mul;		/* multiple        :ML_TABLE[ML] */

	/* Phase Generator */
	UINT32		phase;		/* phase counter */
	INT32		Incr;		/* phase step */

	/* Envelope Generator */
	UINT8		state;		/* phase type */
	UINT32		tl;		/* total level: TL << 3 */
	INT32		volume;		/* envelope counter */
	UINT32		sl;		/* sustain level:sl_table[SL] */
	UINT32		vol_out;	/* current output from EG circuit (without AM from LFO) */

	UINT8		eg_sh_ar;	/*  (attack state) */
	UINT8		eg_sel_ar;	/*  (attack state) */
	UINT8		eg_sh_d1r;	/*  (decay state) */
	UINT8		eg_sel_d1r;	/*  (decay state) */
	UINT8		eg_sh_d2r;	/*  (sustain state) */
	UINT8		eg_sel_d2r;	/*  (sustain state) */
	UINT8		eg_sh_rr;	/*  (release state) */
	UINT8		eg_sel_rr;	/*  (release state) */

	UINT8		ssg;		/* SSG-EG waveform */
	UINT8		ssgn;		/* SSG-EG negated output */

	UINT32		key;		/* 0=last key was KEY OFF, 1=KEY ON */

	/* LFO */
	UINT32		AMmask;		/* AM enable flag */

} FM_SLOT;


typedef struct
{
	FM_SLOT		SLOT[4];	/* four SLOTs (operators) */

	UINT8		ALGO;		/* algorithm */
	UINT8		FB;		/* feedback shift */
	INT32		op1_out[2];	/* op1 output for feedback */

	INT32		*connect1;	/* SLOT1 output pointer */
	INT32		*connect3;	/* SLOT3 output pointer */
	INT32		*connect2;	/* SLOT2 output pointer */
	INT32		*connect4;	/* SLOT4 output pointer */

	INT32		*mem_connect;	/* where to put the delayed sample (MEM) */
	INT32		mem_value;	/* delayed sample (MEM) value */

	INT32		pms;		/* channel PMS */
	UINT8		ams;		/* channel AMS */

	UINT32		fc;		/* fnum,blk:adjusted to sample rate */
	UINT8		kcode;		/* key code:                        */
	UINT32		block_fnum;	/* current blk/fnum value for this slot (can be different betweeen slots of one channel in 3slot mode) */
} FM_CH;


typedef struct
{
	running_device		*device;
	void			*param;			/* this chip parameter  */
	int			clock;			/* master clock  (Hz)   */
	int			rate;			/* sampling rate (Hz)   */
	double			freqbase;		/* frequency base       */
	int			timer_prescaler;	/* timer prescaler      */
#if FM_BUSY_FLAG_SUPPORT
	TIME_TYPE	busy_expiry_time;		/* expiry time of the busy status */
#endif
	UINT8			address;		/* address register     */
	UINT8			irq;			/* interrupt level      */
	UINT8			irqmask;		/* irq mask             */
	UINT8			status;			/* status flag          */
	UINT32			mode;			/* mode  CSM / 3SLOT    */
	UINT8			prescaler_sel;		/* prescaler selector   */
	UINT8			fn_h;			/* freq latch           */
	INT32			TA;			/* timer a              */
	INT32			TAC;			/* timer a counter      */
	UINT8			TB;			/* timer b              */
	INT32			TBC;			/* timer b counter      */
	/* local time tables */
	INT32			dt_tab[8][32];		/* DeTune table         */
	/* Extention Timer and IRQ handler */
	FM_TIMERHANDLER		timer_handler;
	FM_IRQHANDLER		IRQ_Handler;
	const ssg_callbacks	*SSG;
} FM_ST;



/***********************************************************/
/* OPN unit                                                */
/***********************************************************/

/* OPN 3slot struct */
typedef struct
{
	UINT32		fc[3];		/* fnum3,blk3: calculated */
	UINT8		fn_h;		/* freq3 latch */
	UINT8		kcode[3];	/* key code */
	UINT32		block_fnum[3];	/* current fnum value for this slot (can be different betweeen slots of one channel in 3slot mode) */
} FM_3SLOT;


/* OPN/A/B common state */
typedef struct
{
	UINT8		type;		/* chip type */
	FM_ST		ST;		/* general state */
	FM_3SLOT	SL3;		/* 3 slot mode state */
	FM_CH		*P_CH;		/* pointer of CH */
	unsigned int 	pan[6*2];	/* fm channels output masks (0xffffffff = enable) */

	UINT32		eg_cnt;			/* global envelope generator counter */
	UINT32		eg_timer;		/* global envelope generator counter works at frequency = chipclock/64/3 */
	UINT32		eg_timer_add;		/* step of eg_timer */
	UINT32		eg_timer_overflow;	/* envelope generator timer overlfows every 3 samples (on real chip) */

	/* there are 2048 FNUMs that can be generated using FNUM/BLK registers
        but LFO works with one more bit of a precision so we really need 4096 elements */

	UINT32		fn_table[4096];		/* fnumber->increment counter */
	UINT32		fn_max;			/* maximal phase increment (used for phase overflow) */

	/* LFO */
	UINT32		lfo_cnt;
	UINT32		lfo_inc;

	UINT32		lfo_freq[8];		/* LFO FREQ table */
} FM_OPN;


/* current chip state */
static INT32 m2, c1, c2;	/* Phase Modulation input for operators 2,3,4 */
static INT32 mem;		/* one sample delay memory */

static INT32 out_fm[8];		/* outputs of working channels */

static INT32 out_adpcm[4];	/* channel output NONE,LEFT,RIGHT or CENTER for YM2608/YM2610 ADPCM */
static INT32 out_delta[4];	/* channel output NONE,LEFT,RIGHT or CENTER for YM2608/YM2610 DELTAT*/

static UINT32 LFO_AM;		/* runtime LFO calculations helper */
static INT32 LFO_PM;		/* runtime LFO calculations helper */


/* limitter */
#define Limit(val, max, min) {			\
	if ( val > max )      val = max;	\
	else if ( val < min ) val = min;	\
}


/* status set and IRQ handling */
INLINE void FM_STATUS_SET(FM_ST *ST, int flag)
{
	/* set status flag */
	ST->status |= flag;
	if (!(ST->irq) && (ST->status & ST->irqmask))
	{
		ST->irq = 1;
		/* callback user interrupt handler (IRQ is OFF to ON) */
		if (ST->IRQ_Handler)
			(ST->IRQ_Handler)(ST->param, 1);
	}
}

/* status reset and IRQ handling */
INLINE void FM_STATUS_RESET(FM_ST *ST, int flag)
{
	/* reset status flag */
	ST->status &= ~flag;
	if ((ST->irq) && !(ST->status & ST->irqmask))
	{
		ST->irq = 0;
		/* callback user interrupt handler (IRQ is ON to OFF) */
		if (ST->IRQ_Handler)
			(ST->IRQ_Handler)(ST->param, 0);
	}
}

/* IRQ mask set */
INLINE void FM_IRQMASK_SET(FM_ST *ST,int flag)
{
	ST->irqmask = flag;
	/* IRQ handling check */
	FM_STATUS_SET(ST, 0);
	FM_STATUS_RESET(ST, 0);
}

/* OPN Mode Register Write */
INLINE void set_timers( FM_ST *ST, void *n, int v )
{
	/* b7 = CSM MODE */
	/* b6 = 3 slot mode */
	/* b5 = reset b */
	/* b4 = reset a */
	/* b3 = timer enable b */
	/* b2 = timer enable a */
	/* b1 = load b */
	/* b0 = load a */
	ST->mode = v;

	/* reset Timer b flag */
	if (v & 0x20)
		FM_STATUS_RESET(ST, 0x02);
	/* reset Timer a flag */
	if (v & 0x10)
		FM_STATUS_RESET(ST, 0x01);
	/* load b */
	if (v & 0x02)
	{
		if ( ST->TBC == 0 )
		{
			ST->TBC = (256 - ST->TB) << 4;
			/* External timer handler */
			if (ST->timer_handler)
				(ST->timer_handler)(n, 1, ST->TBC * ST->timer_prescaler, ST->clock);
		}
	}
	else
	{	/* stop timer b */
		if ( ST->TBC != 0 )
		{
			ST->TBC = 0;
			if (ST->timer_handler)
				(ST->timer_handler)(n, 1, 0, ST->clock);
		}
	}
	/* load a */
	if (v & 0x01)
	{
		if (ST->TAC == 0)
		{
			ST->TAC = (1024 - ST->TA);
			/* External timer handler */
			if (ST->timer_handler)
				(ST->timer_handler)(n, 0 , ST->TAC * ST->timer_prescaler, ST->clock);
		}
	}
	else
	{	/* stop timer a */
		if (ST->TAC != 0)
		{
			ST->TAC = 0;
			if (ST->timer_handler)
				(ST->timer_handler)(n, 0, 0, ST->clock);
		}
	}
}


/* Timer A Overflow */
INLINE void TimerAOver(FM_ST *ST)
{
	/* set status (if enabled) */
	if (ST->mode & 0x04)
		FM_STATUS_SET(ST, 0x01);

	/* clear or reload the counter */
	ST->TAC = (1024 - ST->TA);
	if (ST->timer_handler)
		(ST->timer_handler)(ST->param, 0, ST->TAC * ST->timer_prescaler, ST->clock);
}
/* Timer B Overflow */
INLINE void TimerBOver(FM_ST *ST)
{
	/* set status (if enabled) */
	if (ST->mode & 0x08)
		FM_STATUS_SET(ST, 0x02);

	/* clear or reload the counter */
	ST->TBC = (256 - ST->TB) << 4;
	if (ST->timer_handler)
		(ST->timer_handler)(ST->param, 1, ST->TBC * ST->timer_prescaler, ST->clock);
}


#define INTERNAL_TIMER_A(ST, CSM_CH)
#define INTERNAL_TIMER_B(ST, step)


#if FM_BUSY_FLAG_SUPPORT

#define FM_BUSY_CLEAR(ST)	((ST)->busy_expiry_time = UNDEFINED_TIME)

INLINE UINT8 FM_STATUS_FLAG(FM_ST *ST)
{
	if ( COMPARE_TIMES(ST->busy_expiry_time, UNDEFINED_TIME) != 0 )
	{
		if (COMPARE_TIMES(ST->busy_expiry_time, FM_GET_TIME_NOW(ST->device->machine)) > 0)
			return ST->status | 0x80;	/* with busy */
		/* expire */
		FM_BUSY_CLEAR(ST);
	}
	return ST->status;
}

INLINE void FM_BUSY_SET(FM_ST *ST,int busyclock )
{
	TIME_TYPE expiry_period = MULTIPLY_TIME_BY_INT(ATTOTIME_IN_HZ(ST->clock), busyclock * ST->timer_prescaler);
	ST->busy_expiry_time = ADD_TIMES(FM_GET_TIME_NOW(ST->device->machine), expiry_period);
}
#endif


INLINE void FM_KEYON(UINT8 type, FM_CH *CH , int s)
{
	FM_SLOT *SLOT = &CH->SLOT[s];
	if ( !SLOT->key )
	{
		SLOT->key = 1;
		SLOT->phase = 0;		/* restart Phase Generator */
		SLOT->ssgn = (SLOT->ssg & 0x04) >> 1;
		SLOT->state = EG_ATT;
	}
}

INLINE void FM_KEYOFF(FM_CH *CH , int s)
{
	FM_SLOT *SLOT = &CH->SLOT[s];
	if ( SLOT->key )
	{
		SLOT->key = 0;
		if (SLOT->state>EG_REL)
			SLOT->state = EG_REL;/* phase -> Release */
	}
}

/* set algorithm connection */
static void setup_connection( FM_CH *CH, int ch)
{
	INT32 *carrier = &out_fm[ch];

	INT32 **om1 = &CH->connect1;
	INT32 **om2 = &CH->connect3;
	INT32 **oc1 = &CH->connect2;

	INT32 **memc = &CH->mem_connect;

	switch (CH->ALGO)
	{
	case 0:
		/* M1---C1---MEM---M2---C2---OUT */
		*om1 = &c1;
		*oc1 = &mem;
		*om2 = &c2;
		*memc= &m2;
		break;
	case 1:
		/* M1------+-MEM---M2---C2---OUT */
		/*      C1-+                     */
		*om1 = &mem;
		*oc1 = &mem;
		*om2 = &c2;
		*memc= &m2;
		break;
	case 2:
		/* M1-----------------+-C2---OUT */
		/*      C1---MEM---M2-+          */
		*om1 = &c2;
		*oc1 = &mem;
		*om2 = &c2;
		*memc= &m2;
		break;
	case 3:
		/* M1---C1---MEM------+-C2---OUT */
		/*                 M2-+          */
		*om1 = &c1;
		*oc1 = &mem;
		*om2 = &c2;
		*memc= &c2;
		break;
	case 4:
		/* M1---C1-+-OUT */
		/* M2---C2-+     */
		/* MEM: not used */
		*om1 = &c1;
		*oc1 = carrier;
		*om2 = &c2;
		*memc= &mem;	/* store it anywhere where it will not be used */
		break;
	case 5:
		/*    +----C1----+     */
		/* M1-+-MEM---M2-+-OUT */
		/*    +----C2----+     */
		*om1 = 0;	/* special mark */
		*oc1 = carrier;
		*om2 = carrier;
		*memc= &m2;
		break;
	case 6:
		/* M1---C1-+     */
		/*      M2-+-OUT */
		/*      C2-+     */
		/* MEM: not used */
		*om1 = &c1;
		*oc1 = carrier;
		*om2 = carrier;
		*memc= &mem;	/* store it anywhere where it will not be used */
		break;
	case 7:
		/* M1-+     */
		/* C1-+-OUT */
		/* M2-+     */
		/* C2-+     */
		/* MEM: not used*/
		*om1 = carrier;
		*oc1 = carrier;
		*om2 = carrier;
		*memc= &mem;	/* store it anywhere where it will not be used */
		break;
	}

	CH->connect4 = carrier;
}

/* set detune & multiple */
INLINE void set_det_mul(FM_ST *ST, FM_CH *CH, FM_SLOT *SLOT, int v)
{
	SLOT->mul = (v & 0x0f) ? ((v & 0x0f) * 2) : 1;
	SLOT->DT  = ST->dt_tab[(v >> 4) & 0x07];
	CH->SLOT[SLOT1].Incr = -1;
}

/* set total level */
INLINE void set_tl(FM_CH *CH, FM_SLOT *SLOT, int v)
{
	SLOT->tl = (v & 0x7f) << (ENV_BITS - 7); /* 7bit TL */
}

/* set attack rate & key scale  */
INLINE void set_ar_ksr(UINT8 type, FM_CH *CH, FM_SLOT *SLOT, int v)
{
	UINT8 old_KSR = SLOT->KSR;

	SLOT->ar = (v & 0x1f) ? (32 + ((v & 0x1f) << 1)) : 0;

	SLOT->KSR = 3 - (v >> 6);
	if (SLOT->KSR != old_KSR)
		CH->SLOT[SLOT1].Incr = -1;

	/* refresh Attack rate */
	if ((SLOT->ar + SLOT->ksr) < (32 + 62))
	{
		SLOT->eg_sh_ar  = eg_rate_shift [SLOT->ar + SLOT->ksr];
		SLOT->eg_sel_ar = eg_rate_select[SLOT->ar + SLOT->ksr];
	}
	else
	{
		SLOT->eg_sh_ar  = 0;
		SLOT->eg_sel_ar = 17 * RATE_STEPS;
	}
}

/* set decay rate */
INLINE void set_dr(UINT8 type, FM_SLOT *SLOT, int v)
{
	SLOT->d1r = (v & 0x1f) ? (32 + ((v & 0x1f) << 1)) : 0;

	SLOT->eg_sh_d1r = eg_rate_shift[SLOT->d1r + SLOT->ksr];
	SLOT->eg_sel_d1r= eg_rate_select[SLOT->d1r + SLOT->ksr];
}

/* set sustain rate */
INLINE void set_sr(UINT8 type, FM_SLOT *SLOT, int v)
{
	SLOT->d2r = (v & 0x1f) ? (32 + ((v & 0x1f) << 1)) : 0;

	SLOT->eg_sh_d2r = eg_rate_shift[SLOT->d2r + SLOT->ksr];
	SLOT->eg_sel_d2r= eg_rate_select[SLOT->d2r + SLOT->ksr];
}

/* set release rate */
INLINE void set_sl_rr(UINT8 type, FM_SLOT *SLOT, int v)
{
	SLOT->sl = sl_table[v >> 4];

	SLOT->rr  = 34 + ((v & 0x0f) << 2);

	SLOT->eg_sh_rr  = eg_rate_shift [SLOT->rr + SLOT->ksr];
	SLOT->eg_sel_rr = eg_rate_select[SLOT->rr + SLOT->ksr];
}

INLINE signed int op_calc(UINT32 phase, unsigned int env, signed int pm)
{
	UINT32 p = (env << 3) + sin_tab[(((signed int)((phase & ~FREQ_MASK) + (pm << 15))) >> FREQ_SH ) & SIN_MASK];

	if (p >= TL_TAB_LEN)
		return 0;

	return tl_tab[p];
}

INLINE signed int op_calc1(UINT32 phase, unsigned int env, signed int pm)
{
	UINT32 p;

	p = (env<<3) + sin_tab[ ( ((signed int)((phase & ~FREQ_MASK) + pm      )) >> FREQ_SH ) & SIN_MASK ];

	if (p >= TL_TAB_LEN)
		return 0;
	return tl_tab[p];
}

/* advance LFO to next sample */
INLINE void advance_lfo(FM_OPN *OPN)
{
	UINT8 pos;

	if (OPN->lfo_inc)	/* LFO enabled ? */
	{
		OPN->lfo_cnt += OPN->lfo_inc;

		pos = (OPN->lfo_cnt >> LFO_SH) & 0x7f;

		/* update AM when LFO output changes */

		/* actually I can't optimize is this way without rewritting chan_calc()
		   to use chip->lfo_am instead of global lfo_am */
		{
			/* triangle */
			/* AM: 0 to 126 step +2, 126 to 0 step -2 */
			if (pos < 64)
				LFO_AM = (pos & 0x3f) * 2;
			else
				LFO_AM = 126 - ((pos & 0x3f) * 2);
		}

		/* PM works with 4 times slower clock */
		pos >>= 2;
		/* update PM when LFO output changes */
		/*if (prev_pos != pos)*/ /* can't use global lfo_pm for this optimization, must be chip->lfo_pm instead*/
		{
			LFO_PM = pos;
		}

	}
	else
	{
		LFO_AM = 0;
		LFO_PM = 0;
	}
}

/* changed from INLINE to static here to work around gcc 4.2.1 codegen bug */
static void advance_eg_channel(FM_OPN *OPN, FM_SLOT *SLOT)
{
	UINT32 out, swap_flag, i;

	i = 4; /* four operators per channel */

	do
	{
		/* reset SSG-EG swap flag */
		swap_flag = 0;

		switch (SLOT->state)
		{
			case EG_ATT:		/* attack phase */
				if (!(OPN->eg_cnt & ((1 << SLOT->eg_sh_ar) - 1)))
				{
					SLOT->volume += (~SLOT->volume * (eg_inc[SLOT->eg_sel_ar + ((OPN->eg_cnt >> SLOT->eg_sh_ar)& 0x07)])) >> 4;

					if (SLOT->volume <= MIN_ATT_INDEX)
					{
						SLOT->volume = MIN_ATT_INDEX;
						SLOT->state = EG_DEC;
					}
				}
			break;

			case EG_DEC:		/* decay phase */
			{
				if (SLOT->ssg & 0x08)	/* SSG EG type envelope selected */
				{
					if (!(OPN->eg_cnt & ((1 << SLOT->eg_sh_d1r) - 1)))
					{
						SLOT->volume += 4 * eg_inc[SLOT->eg_sel_d1r + ((OPN->eg_cnt >> SLOT->eg_sh_d1r) & 0x07)];

						if (SLOT->volume >= (INT32)(SLOT->sl))
							SLOT->state = EG_SUS;
					}
				}
				else
				{
					if (!(OPN->eg_cnt & ((1 << SLOT->eg_sh_d1r) - 1)))
					{
						SLOT->volume += eg_inc[SLOT->eg_sel_d1r + ((OPN->eg_cnt >> SLOT->eg_sh_d1r) & 0x07)];

						if (SLOT->volume >= (INT32)(SLOT->sl))
							SLOT->state = EG_SUS;
					}
				}
			}
			break;

			case EG_SUS:	/* sustain phase */
				if (SLOT->ssg & 0x08)		/* SSG EG type envelope selected */
				{
					if (!(OPN->eg_cnt & ((1 << SLOT->eg_sh_d2r) - 1)))
					{
						SLOT->volume += 4 * eg_inc[SLOT->eg_sel_d2r + ((OPN->eg_cnt >> SLOT->eg_sh_d2r) & 0x07)];

						if (SLOT->volume >= ENV_QUIET)
						{
							SLOT->volume = MAX_ATT_INDEX;

							if (SLOT->ssg & 0x01)		/* bit 0 = hold */
							{
								if (SLOT->ssgn & 0x01)	/* have we swapped once ??? */
								{
									/* yes, so do nothing, just hold current level */
								}
								else
									swap_flag = (SLOT->ssg & 0x02) | 1;	/* bit 1 = alternate */

							}
							else
							{
								/* same as KEY-ON operation */

								/* restart of the Phase Generator should be here */
								SLOT->phase = 0;
								{
									/* phase -> Attack */
									SLOT->volume = 511;
									SLOT->state = EG_ATT;
								}

								swap_flag = SLOT->ssg & 0x02;	/* bit 1 = alternate */
							}
						}
					}
				}
				else
				{
					if (!(OPN->eg_cnt & ((1 << SLOT->eg_sh_d2r) - 1)))
					{
						SLOT->volume += eg_inc[SLOT->eg_sel_d2r + ((OPN->eg_cnt >> SLOT->eg_sh_d2r) & 0x07)];

						if (SLOT->volume >= MAX_ATT_INDEX)
						{
							SLOT->volume = MAX_ATT_INDEX;
							/* do not change SLOT->state (verified on real chip) */
						}
					}

				}
			break;

			case EG_REL:	/* release phase */
				if (!(OPN->eg_cnt & ((1 << SLOT->eg_sh_rr) - 1)))
				{
					/* SSG-EG affects Release phase also (Nemesis) */
					SLOT->volume += eg_inc[SLOT->eg_sel_rr + ((OPN->eg_cnt >> SLOT->eg_sh_rr) & 0x07)];

					if (SLOT->volume >= MAX_ATT_INDEX)
					{
						SLOT->volume = MAX_ATT_INDEX;
						SLOT->state = EG_OFF;
					}
				}
			break;
		}

		out = (UINT32)SLOT->volume;

                /* negate output (changes come from alternate bit, init comes from attack bit) */
		if ((SLOT->ssg & 0x08) && (SLOT->ssgn & 0x02) && (SLOT->state > EG_REL))
			out ^= MAX_ATT_INDEX;

		/* we need to store the result here because we are going to change ssgn
		   in next instruction */
		SLOT->vol_out = out + SLOT->tl;

                /* reverse SLOT inversion flag */
		SLOT->ssgn ^= swap_flag;

		SLOT++;
		i--;
	}
	while (i);
}



#define volume_calc(OP)	((OP)->vol_out + (AM & (OP)->AMmask))

INLINE void update_phase_lfo_slot(FM_OPN *OPN, FM_SLOT *SLOT, INT32 pms, UINT32 block_fnum)
{
	UINT32 fnum_lfo  = ((block_fnum & 0x7f0) >> 4) * 32 * 8;
	INT32  lfo_fn_table_index_offset = lfo_pm_table[ fnum_lfo + pms + LFO_PM ];

	if (lfo_fn_table_index_offset)    /* LFO phase modulation active */
	{
		block_fnum = block_fnum * 2 + lfo_fn_table_index_offset;

		UINT8 blk = (block_fnum & 0x7000) >> 12;
		UINT32 fn = block_fnum & 0xfff;

		/* keyscale code */
		INT32 kc = (blk << 2) | opn_fktable[fn >> 8];

		/* phase increment counter */
		INT32 fc = (OPN->fn_table[fn] >> (7 - blk)) + SLOT->DT[kc];

		/* detects frequency overflow (credits to Nemesis) */
		if (fc < 0)
			fc += OPN->fn_max;

		/* update phase */
		SLOT->phase += (fc * SLOT->mul) >> 1;
	}
	else    /* LFO phase modulation  = zero */
		SLOT->phase += SLOT->Incr;
}

INLINE void update_phase_lfo_channel(FM_OPN *OPN, FM_CH *CH)
{
	UINT32 block_fnum = CH->block_fnum;
	UINT32 fnum_lfo = ((block_fnum & 0x7f0) >> 4) * 32 * 8;
	INT32 lfo_fn_table_index_offset = lfo_pm_table[fnum_lfo + CH->pms + LFO_PM];

	if (lfo_fn_table_index_offset)    /* LFO phase modulation active */
	{
		block_fnum = block_fnum * 2 + lfo_fn_table_index_offset;

		UINT8 blk = (block_fnum & 0x7000) >> 12;
		UINT32 fn = block_fnum & 0xfff;

		/* keyscale code */
		INT32 kc = (blk << 2) | opn_fktable[fn >> 8];

	        /* phase increment counter */
		INT32 fc = (OPN->fn_table[fn] >> (7 - blk));

		/* detects frequency overflow (credits to Nemesis) */
		INT32 finc = fc + CH->SLOT[SLOT1].DT[kc];

		if (finc < 0)
			finc += OPN->fn_max;

		CH->SLOT[SLOT1].phase += (finc * CH->SLOT[SLOT1].mul) >> 1;

		finc = fc + CH->SLOT[SLOT2].DT[kc];
		if (finc < 0)
			finc += OPN->fn_max;

		CH->SLOT[SLOT2].phase += (finc * CH->SLOT[SLOT2].mul) >> 1;

		finc = fc + CH->SLOT[SLOT3].DT[kc];
		if (finc < 0)
			finc += OPN->fn_max;

		CH->SLOT[SLOT3].phase += (finc * CH->SLOT[SLOT3].mul) >> 1;

		finc = fc + CH->SLOT[SLOT4].DT[kc];
		if (finc < 0)
			finc += OPN->fn_max;

		CH->SLOT[SLOT4].phase += (finc*CH->SLOT[SLOT4].mul) >> 1;
	}
	else    /* LFO phase modulation  = zero */
	{
	        CH->SLOT[SLOT1].phase += CH->SLOT[SLOT1].Incr;
	        CH->SLOT[SLOT2].phase += CH->SLOT[SLOT2].Incr;
	        CH->SLOT[SLOT3].phase += CH->SLOT[SLOT3].Incr;
	        CH->SLOT[SLOT4].phase += CH->SLOT[SLOT4].Incr;
	}
}

INLINE void chan_calc(FM_OPN *OPN, FM_CH *CH, int chnum)
{
	UINT32 AM = LFO_AM >> CH->ams;
	UINT32 eg_out;

	m2 = c1 = c2 = mem = 0;
	*CH->mem_connect = CH->mem_value;	/* restore delayed sample (MEM) value to m2 or c2 */

	eg_out = volume_calc(&CH->SLOT[SLOT1]);

	{
		INT32 out = CH->op1_out[0] + CH->op1_out[1];
		CH->op1_out[0] = CH->op1_out[1];

		if (!CH->connect1 )
		{
			/* algorithm 5  */
			mem = c1 = c2 = CH->op1_out[0];
		}
		else
		{
			/* other algorithms */
			*CH->connect1 += CH->op1_out[0];
		}

		CH->op1_out[1] = 0;
		if (eg_out < ENV_QUIET)		/* SLOT 1 */
		{
			if (!CH->FB)
				out = 0;

			CH->op1_out[1] = op_calc1(CH->SLOT[SLOT1].phase, eg_out, (out << CH->FB));
		}
	}

	eg_out = volume_calc(&CH->SLOT[SLOT3]);
	if (eg_out < ENV_QUIET)		/* SLOT 3 */
		*CH->connect3 += op_calc(CH->SLOT[SLOT3].phase, eg_out, m2);

	eg_out = volume_calc(&CH->SLOT[SLOT2]);
	if (eg_out < ENV_QUIET)		/* SLOT 2 */
		*CH->connect2 += op_calc(CH->SLOT[SLOT2].phase, eg_out, c1);

	eg_out = volume_calc(&CH->SLOT[SLOT4]);
	if (eg_out < ENV_QUIET)		/* SLOT 4 */
		*CH->connect4 += op_calc(CH->SLOT[SLOT4].phase, eg_out, c2);

	/* store current MEM */
	CH->mem_value = mem;

	/* update phase counters AFTER output calculations */
	if (CH->pms)
	{
		/* add support for 3 slot mode */
		if ((OPN->ST.mode & 0xC0) && (chnum == 2))
		{
		        update_phase_lfo_slot(OPN, &CH->SLOT[SLOT1], CH->pms, OPN->SL3.block_fnum[1]);
		        update_phase_lfo_slot(OPN, &CH->SLOT[SLOT2], CH->pms, OPN->SL3.block_fnum[2]);
		        update_phase_lfo_slot(OPN, &CH->SLOT[SLOT3], CH->pms, OPN->SL3.block_fnum[0]);
		        update_phase_lfo_slot(OPN, &CH->SLOT[SLOT4], CH->pms, CH->block_fnum);
		}
		else
			update_phase_lfo_channel(OPN, CH);
	}
	else	/* no LFO phase modulation */
	{
		CH->SLOT[SLOT1].phase += CH->SLOT[SLOT1].Incr;
		CH->SLOT[SLOT2].phase += CH->SLOT[SLOT2].Incr;
		CH->SLOT[SLOT3].phase += CH->SLOT[SLOT3].Incr;
		CH->SLOT[SLOT4].phase += CH->SLOT[SLOT4].Incr;
	}
}

/* update phase increment and envelope generator */
INLINE void refresh_fc_eg_slot(FM_OPN *OPN, FM_SLOT *SLOT, int fc, int kc)
{
	int ksr = kc >> SLOT->KSR;

	fc += SLOT->DT[kc];

	/* detects frequency overflow (credits to Nemesis) */
	if (fc < 0)
		fc += OPN->fn_max;

	/* (frequency) phase increment counter */
	SLOT->Incr = (fc * SLOT->mul) >> 1;

	if (SLOT->ksr != ksr)
	{
		SLOT->ksr = ksr;

		/* calculate envelope generator rates */
		if ((SLOT->ar + SLOT->ksr) < (32 + 62))
		{
			SLOT->eg_sh_ar  = eg_rate_shift [SLOT->ar + SLOT->ksr];
			SLOT->eg_sel_ar = eg_rate_select[SLOT->ar + SLOT->ksr];
		}
		else
		{
			SLOT->eg_sh_ar  = 0;
			SLOT->eg_sel_ar = 17 * RATE_STEPS;
		}

		SLOT->eg_sh_d1r = eg_rate_shift [SLOT->d1r + SLOT->ksr];
		SLOT->eg_sh_d2r = eg_rate_shift [SLOT->d2r + SLOT->ksr];
		SLOT->eg_sh_rr  = eg_rate_shift [SLOT->rr  + SLOT->ksr];

		SLOT->eg_sel_d1r= eg_rate_select[SLOT->d1r + SLOT->ksr];
		SLOT->eg_sel_d2r= eg_rate_select[SLOT->d2r + SLOT->ksr];
		SLOT->eg_sel_rr = eg_rate_select[SLOT->rr  + SLOT->ksr];
	}
}

/* update phase increment counters */
/* Changed from INLINE to static to work around gcc 4.2.1 codegen bug */
static void refresh_fc_eg_chan(FM_OPN *OPN, FM_CH *CH )
{
	if ( CH->SLOT[SLOT1].Incr==-1)
	{
		int fc = CH->fc;
		int kc = CH->kcode;

		refresh_fc_eg_slot(OPN, &CH->SLOT[SLOT1], fc, kc);
		refresh_fc_eg_slot(OPN, &CH->SLOT[SLOT2], fc, kc);
		refresh_fc_eg_slot(OPN, &CH->SLOT[SLOT3], fc, kc);
		refresh_fc_eg_slot(OPN, &CH->SLOT[SLOT4], fc, kc);
	}
}

/* initialize time tables */
static void init_timetables( FM_ST *ST , const UINT8 *dttable )
{
	double rate;

	/* DeTune table */
	for (int d = 0; d <= 3; d++)
	{
		for (int i = 0; i <= 31; i++)
		{
			rate = ((double)dttable[d * 32 + i]) * SIN_LEN * ST->freqbase * (1 << FREQ_SH) / ((double)(1 << 20));
			ST->dt_tab[d][i]   = (INT32)rate;
			ST->dt_tab[d + 4][i] = -ST->dt_tab[d][i];
		}
	}
}

static void reset_channels(FM_ST *ST, FM_CH *CH, int num)
{
	ST->mode   = 0;	/* normal mode */
	ST->TA     = 0;
	ST->TAC    = 0;
	ST->TB     = 0;
	ST->TBC    = 0;

	for (int c = 0; c < num; c++)
	{
		CH[c].fc = 0;

		for(int s = 0; s < 4; s++)
		{
			CH[c].SLOT[s].ssg = 0;
			CH[c].SLOT[s].ssgn = 0;
			CH[c].SLOT[s].state= EG_OFF;
			CH[c].SLOT[s].volume = MAX_ATT_INDEX;
			CH[c].SLOT[s].vol_out= MAX_ATT_INDEX;
		}
	}
}

/* initialize generic tables */
static int init_tables(void)
{
	signed int i, x;
	signed int n;
	double o, m;

	for (x = 0; x < TL_RES_LEN; x++)
	{
		m = (1 << 16) / pow(2, (x+1) * (ENV_STEP / 4.0) / 8.0);
		m = floor(m);

		/* we never reach (1<<16) here due to the (x+1) */
		/* result fits within 16 bits at maximum */

		n = (int)m;		/* 16 bits here */
		n >>= 4;		/* 12 bits here */
		if (n & 0x01)		/* round to nearest */
			n = (n >> 1) + 1;
		else
			n >>= 1;
					/* 11 bits here (rounded) */
		n <<= 2;		/* 13 bits here (as in real chip) */

		tl_tab[x * 2 + 0] = n;
		tl_tab[x * 2 + 1] = -tl_tab[x * 2 + 0];

		for (i = 1; i < 13; i++)
		{
			tl_tab[x * 2 + 0 + i * 2 * TL_RES_LEN] =  tl_tab[x * 2 + 0] >> i;
			tl_tab[x * 2 + 1 + i * 2 * TL_RES_LEN] = -tl_tab[x * 2 + 0 + i * 2 * TL_RES_LEN];
		}
	}

	for (i = 0; i < SIN_LEN; i++)
	{
		/* non-standard sinus */
		m = sin(((i * 2) + 1) * M_PI / SIN_LEN); /* checked against the real chip */

		/* we never reach zero here due to ((i*2)+1) */

		if (m > 0.0)
			o = 8 * log( 1.0 / m) / log(2.0);	/* convert to 'decibels' */
		else
			o = 8 * log(-1.0 / m) / log(2.0);	/* convert to 'decibels' */

		o = o / (ENV_STEP / 4);
		n = (int)(2.0 * o);

		if (n & 0x01)					/* round to nearest */
			n = (n >> 1) + 1;
		else
			n >>= 1;

		sin_tab[i] = n * 2 + (m >= 0.0 ? 0 : 1);
	}

	/* build LFO PM modulation table */
	for (i = 0; i < 8; i++)		/* 8 PM depths */
	{
		for (UINT8 fnum = 0; fnum < 128; fnum++)	/* 7 bits meaningful of F-NUMBER */
		{
			UINT8 value;
			UINT32 offset_depth = i;
			UINT32 offset_fnum_bit;
			UINT32 bit_tmp;

			for (UINT8 step = 0; step < 8; step++)
			{
				value = 0;
				for (bit_tmp = 0; bit_tmp < 7; bit_tmp++)	/* 7 bits */
				{
					if (fnum & (1 << bit_tmp))		/* only if bit "bit_tmp" is set */
					{
						offset_fnum_bit = bit_tmp * 8;
						value += lfo_pm_output[offset_fnum_bit + offset_depth][step];
					}
				}
				lfo_pm_table[(fnum * 32 * 8) + (i * 32) + step + 0] = value;
				lfo_pm_table[(fnum * 32 * 8) + (i * 32) + (step ^ 7) + 8] = value;
				lfo_pm_table[(fnum * 32 * 8) + (i * 32) + step + 16] = -value;
				lfo_pm_table[(fnum * 32 * 8) + (i * 32) + (step ^ 7) + 24] = -value;
			}
		}
	}
	return 1;
}



static void FMCloseTable( void )
{
	return;
}


/* CSM Key Controll */
INLINE void CSMKeyControll(UINT8 type, FM_CH *CH)
{
	/* all key on then off (only for operators which were OFF!) */
	if (!CH->SLOT[SLOT1].key)
	{
		FM_KEYON(type, CH,SLOT1);
		FM_KEYOFF(CH, SLOT1);
	}
	if (!CH->SLOT[SLOT2].key)
	{
		FM_KEYON(type, CH,SLOT2);
		FM_KEYOFF(CH, SLOT2);
	}
	if (!CH->SLOT[SLOT3].key)
	{
		FM_KEYON(type, CH,SLOT3);
		FM_KEYOFF(CH, SLOT3);
	}
	if (!CH->SLOT[SLOT4].key)
	{
		FM_KEYON(type, CH,SLOT4);
		FM_KEYOFF(CH, SLOT4);
	}
}

#ifdef __STATE_H__
/* FM channel save , internal state only */
static void FMsave_state_channel(running_device *device,FM_CH *CH,int num_ch)
{
	for (int ch = 0; ch < num_ch; ch++, CH++)
	{
		/* channel */
		state_save_register_device_item_array(device, ch, CH->op1_out);
		state_save_register_device_item(device, ch, CH->fc);
		/* slots */
		for (int slot = 0; slot < 4; slot++)
		{
			FM_SLOT *SLOT = &CH->SLOT[slot];
			state_save_register_device_item(device, ch * 4 + slot, SLOT->phase);
			state_save_register_device_item(device, ch * 4 + slot, SLOT->state);
			state_save_register_device_item(device, ch * 4 + slot, SLOT->volume);
		}
	}
}

static void FMsave_state_st(running_device *device,FM_ST *ST)
{
#if FM_BUSY_FLAG_SUPPORT
	state_save_register_device_item(device, 0, ST->busy_expiry_time.seconds );
	state_save_register_device_item(device, 0, ST->busy_expiry_time.attoseconds );
#endif
	state_save_register_device_item(device, 0, ST->address );
	state_save_register_device_item(device, 0, ST->irq     );
	state_save_register_device_item(device, 0, ST->irqmask );
	state_save_register_device_item(device, 0, ST->status  );
	state_save_register_device_item(device, 0, ST->mode    );
	state_save_register_device_item(device, 0, ST->prescaler_sel );
	state_save_register_device_item(device, 0, ST->fn_h );
	state_save_register_device_item(device, 0, ST->TA   );
	state_save_register_device_item(device, 0, ST->TAC  );
	state_save_register_device_item(device, 0, ST->TB  );
	state_save_register_device_item(device, 0, ST->TBC  );
}
#endif /* _STATE_H */


#if BUILD_OPN
/* prescaler set (and make time tables) */
static void OPNSetPres(FM_OPN *OPN, int pres, int timer_prescaler, int SSGpres)
{
	int i;

	/* frequency base */
	OPN->ST.freqbase = (OPN->ST.rate) ? ((double)OPN->ST.clock / OPN->ST.rate) / pres : 0;
	OPN->eg_timer_add  = (1 << EG_SH) * OPN->ST.freqbase;
	OPN->eg_timer_overflow = 3 * (1 << EG_SH);

	/* Timer base time */
	OPN->ST.timer_prescaler = timer_prescaler;

	/* SSG part  prescaler set */
	if (SSGpres)
		(*OPN->ST.SSG->set_clock)(OPN->ST.param, OPN->ST.clock * 2 / SSGpres);

	/* make time tables */
	init_timetables(&OPN->ST, dt_tab);

	/* there are 2048 FNUMs that can be generated using FNUM/BLK registers
	   but LFO works with one more bit of a precision so we really need 4096 elements */

	/* calculate fnumber -> increment counter table */
	for (i = 0; i < 4096; i++)
	{
		/* freq table for octave 7 */
		/* OPN phase increment counter = 20bit */
		OPN->fn_table[i] = (UINT32)((double)i * 32 * OPN->ST.freqbase * (1 << (FREQ_SH - 10)));	/* -10 because chip works with 10.10 fixed point, while we use 16.16 */
	}

	/* maximal frequency is required for Phase overflow calculation, register size is 17 bits (Nemesis) */
	OPN->fn_max = (UINT32)((double)0x20000 * OPN->ST.freqbase * (1 << (FREQ_SH - 10)));

	/* LFO freq. table */
	for(i = 0; i < 8; i++)
	{
		/* Amplitude modulation: 64 output levels (triangle waveform); 1 level lasts for one of "lfo_samples_per_step" samples */
		/* Phase modulation: one entry from lfo_pm_output lasts for one of 4 * "lfo_samples_per_step" samples  */
		OPN->lfo_freq[i] = (1.0 / lfo_samples_per_step[i]) * (1 << LFO_SH) * OPN->ST.freqbase;
	}
}

/* write a OPN mode register 0x20-0x2f */
static void OPNWriteMode(FM_OPN *OPN, int r, int v)
{
	UINT8 c;
	FM_CH *CH;

	switch(r)
	{
		case 0x21:	/* Test */
			break;

		case 0x22:	/* LFO FREQ (YM2608/YM2610/YM2610B/YM2612) */
			if (OPN->type & TYPE_LFOPAN)
			{
				if (v & 0x08) /* LFO enabled ? */
					OPN->lfo_inc = OPN->lfo_freq[v & 0x07];
				else
					OPN->lfo_inc = 0;
			}
			break;

		case 0x24:	/* timer A High 8*/
			OPN->ST.TA = (OPN->ST.TA & 0x03) | (((int)v) << 2);
			break;

		case 0x25:	/* timer A Low 2*/
			OPN->ST.TA = (OPN->ST.TA & 0x3fc) | (v & 0x03);
			break;

		case 0x26:	/* timer B */
			OPN->ST.TB = v;
			break;

		case 0x27:	/* mode, timer control */
			set_timers(&(OPN->ST), OPN->ST.param, v);
			break;

		case 0x28:	/* key on / off */
			c = v & 0x03;
			if (c == 3 )
				break;
			if ((v & 0x04) && (OPN->type & TYPE_6CH))
				c += 3;

			CH = OPN->P_CH;
			CH = &CH[c];
			if (v&0x10) FM_KEYON(OPN->type,CH,SLOT1); else FM_KEYOFF(CH,SLOT1);
			if (v&0x20) FM_KEYON(OPN->type,CH,SLOT2); else FM_KEYOFF(CH,SLOT2);
			if (v&0x40) FM_KEYON(OPN->type,CH,SLOT3); else FM_KEYOFF(CH,SLOT3);
			if (v&0x80) FM_KEYON(OPN->type,CH,SLOT4); else FM_KEYOFF(CH,SLOT4);
			break;
	}
}

/* write a OPN register (0x30-0xff) */
static void OPNWriteReg(FM_OPN *OPN, int r, int v)
{
	FM_CH *CH;
	FM_SLOT *SLOT;

	UINT8 c = OPN_CHAN(r);

	if (c == 3)
		return;	/* 0xX3,0xX7,0xXB,0xXF */

	if (r >= 0x100)
		c+=3;

	CH = OPN->P_CH;
	CH = &CH[c];

	SLOT = &(CH->SLOT[OPN_SLOT(r)]);

	switch (r & 0xf0)
	{
		case 0x30:	/* DET , MUL */
			set_det_mul(&OPN->ST, CH, SLOT, v);
			break;

		case 0x40:	/* TL */
			set_tl(CH, SLOT, v);
			break;

		case 0x50:	/* KS, AR */
			set_ar_ksr(OPN->type, CH, SLOT, v);
			break;

		case 0x60:	/* bit7 = AM ENABLE, DR */
			set_dr(OPN->type, SLOT, v);

			if (OPN->type & TYPE_LFOPAN) /* YM2608/2610/2610B/2612 */
				SLOT->AMmask = (v & 0x80) ? ~0 : 0;
			break;

		case 0x70:	/*     SR */
			set_sr(OPN->type, SLOT, v);
			break;

		case 0x80:	/* SL, RR */
			set_sl_rr(OPN->type, SLOT, v);
			break;

		case 0x90:	/* SSG-EG */
			SLOT->ssg  =  v& 0x0f;
			SLOT->ssgn = (v& 0x04) >> 1; /* bit 1 in ssgn = attack */

		/* SSG-EG envelope shapes :

        E AtAlH
        1 0 0 0  \\\\

        1 0 0 1  \___

        1 0 1 0  \/\/
                  ___
        1 0 1 1  \

        1 1 0 0  ////
                  ___
        1 1 0 1  /

        1 1 1 0  /\/\

        1 1 1 1  /___


        E = SSG-EG enable


        The shapes are generated using Attack, Decay and Sustain phases.

        Each single character in the diagrams above represents this whole
        sequence:

        - when KEY-ON = 1, normal Attack phase is generated (*without* any
          difference when compared to normal mode),

        - later, when envelope level reaches minimum level (max volume),
          the EG switches to Decay phase (which works with bigger steps
          when compared to normal mode - see below),

        - later when envelope level passes the SL level,
          the EG swithes to Sustain phase (which works with bigger steps
          when compared to normal mode - see below),

        - finally when envelope level reaches maximum level (min volume),
          the EG switches to Attack phase again (depends on actual waveform).

        Important is that when switch to Attack phase occurs, the phase counter
        of that operator will be zeroed-out (as in normal KEY-ON) but not always.
        (I havent found the rule for that - perhaps only when the output level is low)

        The difference (when compared to normal Envelope Generator mode) is
        that the resolution in Decay and Sustain phases is 4 times lower;
        this results in only 256 steps instead of normal 1024.
        In other words:
        when SSG-EG is disabled, the step inside of the EG is one,
        when SSG-EG is enabled, the step is four (in Decay and Sustain phases).

        Times between the level changes are the same in both modes.


        Important:
        Decay 1 Level (so called SL) is compared to actual SSG-EG output, so
        it is the same in both SSG and no-SSG modes, with this exception:

        when the SSG-EG is enabled and is generating raising levels
        (when the EG output is inverted) the SL will be found at wrong level !!!
        For example, when SL=02:
            0 -6 = -6dB in non-inverted EG output
            96-6 = -90dB in inverted EG output
        Which means that EG compares its level to SL as usual, and that the
        output is simply inverted afterall.


        The Yamaha's manuals say that AR should be set to 0x1f (max speed).
        That is not necessary, but then EG will be generating Attack phase.

        */

			break;

		case 0xa0:
			switch (OPN_SLOT(r))
			{
				case 0:		/* 0xa0-0xa2 : FNUM1 */
				{
					UINT32 fn = (((UINT32)((OPN->ST.fn_h) & 0x07)) << 8) + v;
					UINT8 blk = OPN->ST.fn_h >> 3;
					/* keyscale code */
					CH->kcode = (blk << 2) | opn_fktable[fn >> 7];
					/* phase increment counter */
					CH->fc = OPN->fn_table[fn * 2] >> (7 - blk);

					/* store fnum in clear form for LFO PM calculations */
					CH->block_fnum = (blk << 11) | fn;

					CH->SLOT[SLOT1].Incr = -1;
				}
				break;

				case 1:		/* 0xa4-0xa6 : FNUM2,BLK */
					OPN->ST.fn_h = v & 0x3f;
				break;

				case 2:		/* 0xa8-0xaa : 3CH FNUM1 */
					if (r < 0x100)
					{
						UINT32 fn = (((UINT32)(OPN->SL3.fn_h & 0x07)) << 8) + v;
						UINT8 blk = OPN->SL3.fn_h >> 3;
						/* keyscale code */
						OPN->SL3.kcode[c]= (blk << 2) | opn_fktable[fn >> 7];
						/* phase increment counter */
						OPN->SL3.fc[c] = OPN->fn_table[fn * 2] >> (7 - blk);
						OPN->SL3.block_fnum[c] = (blk << 11) | fn;
						(OPN->P_CH)[2].SLOT[SLOT1].Incr = -1;
					}
					break;

				case 3:		/* 0xac-0xae : 3CH FNUM2,BLK */
					if (r < 0x100)
						OPN->SL3.fn_h = v & 0x3f;
				break;
			}
			break;

		case 0xb0:
			switch (OPN_SLOT(r))
			{
				case 0:		/* 0xb0-0xb2 : FB,ALGO */
				{
					int feedback = (v >> 3) & 0x07;
					CH->ALGO = v & 0x07;
					CH->FB   = feedback ? feedback + 6 : 0;
					setup_connection(CH, c);
				}
				break;

				case 1:		/* 0xb4-0xb6 : L , R , AMS , PMS (YM2612/YM2610B/YM2610/YM2608) */
				if (OPN->type & TYPE_LFOPAN)
				{
					/* b0-2 PMS */
					CH->pms = (v & 0x07) * 32; /* CH->pms = PM depth * 32 (index in lfo_pm_table) */

					/* b4-5 AMS */
					CH->ams = lfo_ams_depth_shift[(v >> 4) & 0x03];

					/* PAN :  b7 = L, b6 = R */
					OPN->pan[c * 2] = (v & 0x80) ? ~0 : 0;
					OPN->pan[c * 2 + 1] = (v & 0x40) ? ~0 : 0;
				}
				break;
			}
			break;
	}
}
#endif /* BUILD_OPN */


#if BUILD_OPN_PRESCALER
/*
  prescaler circuit (best guess to verified chip behaviour)

               +--------------+  +-sel2-+
               |              +--|in20  |
         +---+ |  +-sel1-+       |      |
M-CLK -+-|1/2|-+--|in10  | +---+ |   out|--INT_CLOCK
       | +---+    |   out|-|1/3|-|in21  |
       +----------|in11  | +---+ +------+
                  +------+

reg.2d : sel2 = in21 (select sel2)
reg.2e : sel1 = in11 (select sel1)
reg.2f : sel1 = in10 , sel2 = in20 (clear selector)
reset  : sel1 = in11 , sel2 = in21 (clear both)

*/
static void OPNPrescaler_w(FM_OPN *OPN , int addr, int pre_divider)
{
	static const int opn_pres[4] = {
		2*12, 2*12, 6*12, 3*12
	};

	static const int ssg_pres[4] = {
		1, 1, 4, 2
	};

	int sel;

	switch (addr)
	{
		case 0:		/* when reset */
			OPN->ST.prescaler_sel = 2;
			break;
		case 1:		/* when postload */
			break;
		case 0x2d:	/* divider sel : select 1/1 for 1/3line    */
			OPN->ST.prescaler_sel |= 0x02;
			break;
		case 0x2e:	/* divider sel , select 1/3line for output */
			OPN->ST.prescaler_sel |= 0x01;
			break;
		case 0x2f:	/* divider sel , clear both selector to 1/2,1/2 */
			OPN->ST.prescaler_sel = 0;
			break;
	}

	sel = OPN->ST.prescaler_sel & 0x03;
	/* update prescaler */
	OPNSetPres(OPN,	opn_pres[sel] * pre_divider, opn_pres[sel] * pre_divider, ssg_pres[sel] * pre_divider);
}
#endif /* BUILD_OPN_PRESCALER */


#if BUILD_YM2203
/*****************************************************************************/
/*      YM2203 local section                                                 */
/*****************************************************************************/

/* here's the virtual YM2203(OPN) */
typedef struct
{
	UINT8	REGS[256];		/* registers         */
	FM_OPN	OPN;			/* OPN state         */
	FM_CH	CH[3];			/* channel state     */
} YM2203;

/* Generate samples for one of the YM2203s */
void ym2203_update_one(void *chip, FMSAMPLE *buffer, int length)
{
	YM2203 *F2203 = (YM2203 *)chip;
	FM_OPN *OPN = &F2203->OPN;
	FMSAMPLE *buf = buffer;
	FM_CH *cch[3];

	cch[0] = &F2203->CH[0];
	cch[1] = &F2203->CH[1];
	cch[2] = &F2203->CH[2];

	/* refresh PG and EG */
	refresh_fc_eg_chan(OPN, cch[0]);
	refresh_fc_eg_chan(OPN, cch[1]);

	if ((F2203->OPN.ST.mode & 0xc0))
	{
		/* 3SLOT MODE */
		if (cch[2]->SLOT[SLOT1].Incr == -1)
		{
			refresh_fc_eg_slot(OPN, &cch[2]->SLOT[SLOT1], OPN->SL3.fc[1], OPN->SL3.kcode[1]);
			refresh_fc_eg_slot(OPN, &cch[2]->SLOT[SLOT2], OPN->SL3.fc[2], OPN->SL3.kcode[2]);
			refresh_fc_eg_slot(OPN, &cch[2]->SLOT[SLOT3], OPN->SL3.fc[0], OPN->SL3.kcode[0]);
			refresh_fc_eg_slot(OPN, &cch[2]->SLOT[SLOT4], cch[2]->fc, cch[2]->kcode);
		}
	}
	else
		refresh_fc_eg_chan(OPN, cch[2]);

	/* YM2203 doesn't have LFO so we must keep these globals at 0 level */
	LFO_AM = 0;
	LFO_PM = 0;

	/* buffering */
	for (int i = 0; i < length; i++)
	{
		/* clear outputs */
		out_fm[0] = out_fm[1] = out_fm[2] = 0;

		/* advance envelope generator */
		OPN->eg_timer += OPN->eg_timer_add;

		while (OPN->eg_timer >= OPN->eg_timer_overflow)
		{
			OPN->eg_timer -= OPN->eg_timer_overflow;
			OPN->eg_cnt++;

			advance_eg_channel(OPN, &cch[0]->SLOT[SLOT1]);
			advance_eg_channel(OPN, &cch[1]->SLOT[SLOT1]);
			advance_eg_channel(OPN, &cch[2]->SLOT[SLOT1]);
		}

		/* calculate FM */
		chan_calc(OPN, cch[0], 0);
		chan_calc(OPN, cch[1], 1);
		chan_calc(OPN, cch[2], 2);

		/* buffering */
		{
			int lt;

			lt = out_fm[0] + out_fm[1] + out_fm[2];

			lt >>= FINAL_SH;

			Limit(lt, MAXOUT, MINOUT);

			/* buffering */
			buf[i] = lt;
		}

		/* timer A control */
		INTERNAL_TIMER_A(&F2203->OPN.ST, cch[2])
	}
	INTERNAL_TIMER_B(&F2203->OPN.ST, length)
}

/* ---------- reset one of chip ---------- */
void ym2203_reset_chip(void *chip)
{
	YM2203 *F2203 = (YM2203 *)chip;
	FM_OPN *OPN = &F2203->OPN;

	/* Reset Prescaler */
	OPNPrescaler_w(OPN, 0, 1);

	/* reset SSG section */
	(*OPN->ST.SSG->reset)(OPN->ST.param);

	/* status clear */
	FM_IRQMASK_SET(&OPN->ST, 0x03);
	FM_BUSY_CLEAR(&OPN->ST);
	OPNWriteMode(OPN, 0x27, 0x30);	/* mode 0 , timer reset */

	OPN->eg_timer = 0;
	OPN->eg_cnt   = 0;

	FM_STATUS_RESET(&OPN->ST, 0xff);
	reset_channels(&OPN->ST, F2203->CH, 3);

	/* reset OPerator paramater */
	for(int i = 0xb2; i >= 0x30; i--)
		OPNWriteReg(OPN, i, 0);
	for(int i = 0x26; i >= 0x20; i--)
		OPNWriteReg(OPN, i, 0);
}

#ifdef __STATE_H__
void ym2203_postload(void *chip)
{
	if (chip)
	{
		YM2203 *F2203 = (YM2203 *)chip;
		int r;

		/* prescaler */
		OPNPrescaler_w(&F2203->OPN, 1, 1);

		/* SSG registers */
		for(r = 0; r < 16; r++)
		{
			(*F2203->OPN.ST.SSG->write)(F2203->OPN.ST.param, 0, r);
			(*F2203->OPN.ST.SSG->write)(F2203->OPN.ST.param, 1, F2203->REGS[r]);
		}

		/* OPN registers */
		/* DT / MULTI , TL , KS / AR , AMON / DR , SR , SL / RR , SSG-EG */
		for(r = 0x30; r < 0x9e; r++)
			if ((r & 0x03) != 3)
				OPNWriteReg(&F2203->OPN, r, F2203->REGS[r]);
		/* FB / CONNECT , L / R / AMS / PMS */
		for(r = 0xb0; r < 0xb6; r++)
			if ((r & 0x03) != 3)
				OPNWriteReg(&F2203->OPN, r, F2203->REGS[r]);

		/* channels */
		/*FM_channel_postload(F2203->CH,3);*/
	}
}

static void YM2203_save_state(YM2203 *F2203, running_device *device)
{
	state_save_register_device_item_array(device, 0, F2203->REGS);
	FMsave_state_st(device,&F2203->OPN.ST);
	FMsave_state_channel(device, F2203->CH, 3);
	/* 3slots */
	state_save_register_device_item_array(device, 0, F2203->OPN.SL3.fc);
	state_save_register_device_item(device, 0, F2203->OPN.SL3.fn_h);
	state_save_register_device_item_array(device, 0, F2203->OPN.SL3.kcode);
}
#endif /* _STATE_H */

/* ----------  Initialize YM2203 emulator(s) ----------
   'num' is the number of virtual YM2203s to allocate
   'clock' is the chip clock in Hz
   'rate' is sampling rate
*/
void * ym2203_init(void *param, running_device *device, int clock, int rate,
               FM_TIMERHANDLER timer_handler,FM_IRQHANDLER IRQHandler, const ssg_callbacks *ssg)
{
	YM2203 *F2203;

	/* allocate ym2203 state space */
	F2203 = auto_alloc_clear(device->machine, YM2203);

	if (!init_tables())
	{
		auto_free(device->machine, F2203);
		return NULL;
	}

	F2203->OPN.ST.param = param;
	F2203->OPN.type = TYPE_YM2203;
	F2203->OPN.P_CH = F2203->CH;
	F2203->OPN.ST.device = device;
	F2203->OPN.ST.clock = clock;
	F2203->OPN.ST.rate = rate;

	F2203->OPN.ST.timer_handler = timer_handler;
	F2203->OPN.ST.IRQ_Handler   = IRQHandler;
	F2203->OPN.ST.SSG           = ssg;

#ifdef __STATE_H__
	YM2203_save_state(F2203, device);
#endif
	return F2203;
}

/* shut down emulator */
void ym2203_shutdown(void *chip)
{
	YM2203 *FM2203 = (YM2203 *)chip;

	FMCloseTable();
	auto_free(FM2203->OPN.ST.device->machine, FM2203);
}

/* YM2203 I/O interface */
int ym2203_write(void *chip, int a, UINT8 v)
{
	YM2203 *F2203 = (YM2203 *)chip;
	FM_OPN *OPN = &F2203->OPN;

	if (!(a & 0x01))
	{	/* address port */
		OPN->ST.address = (v &= 0xff);

		/* Write register to SSG emulator */
		if (v < 16)
			(*OPN->ST.SSG->write)(OPN->ST.param, 0, v);

		/* prescaler select : 2d,2e,2f  */
		if (v >= 0x2d && v <= 0x2f)
			OPNPrescaler_w(OPN, v, 1);
	}
	else
	{	/* data port */
		int addr = OPN->ST.address;
		F2203->REGS[addr] = v;
		switch (addr & 0xf0)
		{
			case 0x00:	/* 0x00-0x0f : SSG section */
				/* Write data to SSG emulator */
				(*OPN->ST.SSG->write)(OPN->ST.param, a, v);
				break;
			case 0x20:	/* 0x20-0x2f : Mode section */
				ym2203_update_req(OPN->ST.param);
				/* write register */
				OPNWriteMode(OPN, addr, v);
				break;
			default:	/* 0x30-0xff : OPN section */
				ym2203_update_req(OPN->ST.param);
				/* write register */
				OPNWriteReg(OPN, addr, v);
		}
		FM_BUSY_SET(&OPN->ST, 1);
	}

	return OPN->ST.irq;
}

UINT8 ym2203_read(void *chip, int a)
{
	YM2203 *F2203 = (YM2203 *)chip;
	int addr = F2203->OPN.ST.address;
	UINT8 ret = 0;

	if (!(a & 0x01))
	{	/* status port */
		ret = FM_STATUS_FLAG(&F2203->OPN.ST);
	}
	else
	{	/* data port (only SSG) */
		if (addr < 16)
			ret = (*F2203->OPN.ST.SSG->read)(F2203->OPN.ST.param);
	}

	return ret;
}

int ym2203_timer_over(void *chip, int c)
{
	YM2203 *F2203 = (YM2203 *)chip;

	if (c)
	{	/* Timer B */
		TimerBOver(&(F2203->OPN.ST));
	}
	else
	{	/* Timer A */
		ym2203_update_req(F2203->OPN.ST.param);
		/* timer update */
		TimerAOver(&(F2203->OPN.ST));

		/* CSM mode key,TL control */
		if (F2203->OPN.ST.mode & 0x80)
		{	/* CSM mode auto key on */
			CSMKeyControll(F2203->OPN.type, &(F2203->CH[2]));
		}
	}

	return F2203->OPN.ST.irq;
}
#endif /* BUILD_YM2203 */


#if BUILD_YM2610

/* ADPCM type A channel struct */
typedef struct
{
	UINT8		flag;			/* port state               */
	UINT8		flagMask;		/* arrived flag mask        */
	UINT8		now_data;		/* current ROM data         */
	UINT32		now_addr;		/* current ROM address      */
	UINT32		now_step;
	UINT32		step;
	UINT32		start;			/* sample data start address*/
	UINT32		end;			/* sample data end address  */
	UINT8		IL;			/* Instrument Level         */
	INT32		adpcm_acc;		/* accumulator              */
	INT32		adpcm_step;		/* step                     */
	INT32		adpcm_out;		/* (speedup) hiro-shi!!     */
	INT8		vol_mul;		/* volume in "0.75dB" steps */
	UINT8		vol_shift;		/* volume in "-6dB" steps   */
	INT32		*pan;			/* &out_adpcm[OPN_xxxx]     */
} ADPCM_CH;

/* here's the virtual YM2610 */
typedef struct
{
	UINT8		REGS[512];			/* registers            */
	FM_OPN		OPN;				/* OPN state            */
	FM_CH		CH[6];				/* channel state        */
	UINT8		addr_A1;			/* address line A1      */

	/* ADPCM-A unit */
	const UINT8	*pcmbuf;			/* pcm rom buffer       */
	UINT32		pcm_size;			/* size of pcm rom      */
	UINT8		adpcmTL;			/* adpcmA total level   */
	ADPCM_CH	adpcm[6];			/* adpcm channels       */
	UINT32		adpcmreg[0x30];			/* registers            */
	UINT8		adpcm_arrivedEndAddress;
	YM_DELTAT	deltaT;				/* Delta-T ADPCM unit   */

	UINT8		flagmask;			/* YM2608 only */
	UINT8		irqmask;			/* YM2608 only */
} YM2610;

/* here is the virtual YM2608 */
typedef YM2610 YM2608;


/**** YM2610 ADPCM defines ****/
#define ADPCM_SHIFT    (16)      /* frequency step rate   */
#define ADPCMA_ADDRESS_SHIFT 8   /* adpcm A address shift */

static const UINT8 *pcmbufA;
static UINT32 pcmsizeA;


/* Algorithm and tables verified on real YM2608 and YM2610 */

/* usual ADPCM table (16 * 1.1^N) */
static const int steps[49] = {
	 16,  17,   19,   21,   23,   25,   28,
	 31,  34,   37,   41,   45,   50,   55,
	 60,  66,   73,   80,   88,   97,  107,
	118, 130,  143,  157,  173,  190,  209,
	230, 253,  279,  307,  337,  371,  408,
	449, 494,  544,  598,  658,  724,  796,
	876, 963, 1060, 1166, 1282, 1411, 1552
};

/* different from the usual ADPCM table */
static const int step_inc[8] = { -1*16, -1*16, -1*16, -1*16, 2*16, 5*16, 7*16, 9*16 };

/* speedup purposes only */
static int jedi_table[49*16];

static void Init_ADPCMATable(void)
{
	for (int step = 0; step < 49; step++)
	{
		/* loop over all nibbles and compute the difference */
		for (int nib = 0; nib < 16; nib++)
		{
			int value = (2 * (nib & 0x07) + 1) * steps[step] / 8;
			jedi_table[step*16 + nib] = (nib & 0x08) ? -value : value;
		}
	}
}

/* ADPCM A (Non control type) : calculate one channel output */
INLINE void ADPCMA_calc_chan( YM2610 *F2610, ADPCM_CH *ch )
{
	UINT32 step;
	UINT8  data;

	ch->now_step += ch->step;
	if (ch->now_step >= (1 << ADPCM_SHIFT))
	{
		step = ch->now_step >> ADPCM_SHIFT;
		ch->now_step &= (1 << ADPCM_SHIFT) - 1;

		do
		{
			/* end check */
			/* 11-06-2001 JB: corrected comparison. Was > instead of == */
			/* YM2610 checks lower 20 bits only, the 4 MSB bits are sample bank */
			/* Here we use 1<<21 to compensate for nibble calculations */

			if ((ch->now_addr & ((1 << 21) - 1)) == ((ch->end << 1) & ((1 << 21) - 1)))
			{
				ch->flag = 0;
				F2610->adpcm_arrivedEndAddress |= ch->flagMask;
				return;
			}

			if (ch->now_addr & 0x01)
				data = ch->now_data & 0x0f;
			else
			{
				ch->now_data = *(pcmbufA + (ch->now_addr >> 1));
				data = (ch->now_data >> 4) & 0x0f;
			}

			ch->now_addr++;

			ch->adpcm_acc += jedi_table[ch->adpcm_step + data];

			/* extend 12-bit signed int */
			if (ch->adpcm_acc & ~0x7ff)
				ch->adpcm_acc |= ~0xfff;
			else
				ch->adpcm_acc &= 0xfff;

			ch->adpcm_step += step_inc[data & 0x07];
			Limit(ch->adpcm_step, 48*16, 0*16);

		}
		while (--step);

		/* calc pcm * volume data */
		ch->adpcm_out = ((ch->adpcm_acc * ch->vol_mul) >> ch->vol_shift) & ~3;	/* multiply, shift and mask out 2 LSB bits */
	}

	/* output for work of output channels (out_adpcm[OPNxxxx])*/
	*(ch->pan) += ch->adpcm_out;
}

/* ADPCM type A Write */
static void FM_ADPCMAWrite(YM2610 *F2610, int r, int v)
{
	ADPCM_CH *adpcm = F2610->adpcm;
	UINT8 c = r & 0x07;
	F2610->adpcmreg[r] = v & 0xff; /* stock data */

	switch (r)
	{
		case 0x00: /* DM,--,C5,C4,C3,C2,C1,C0 */
			if (!(v & 0x80))
			{
				/* KEY ON */
				for (c = 0; c < 6; c++)
				{
					if ((v >> c) & 0x01)
					{
						/**** start adpcm ****/
						adpcm[c].step      = (UINT32)((float)(1 << ADPCM_SHIFT) * ((float)F2610->OPN.ST.freqbase) / 3.0);
						adpcm[c].now_addr  = adpcm[c].start << 1;
						adpcm[c].now_step  = 0;
						adpcm[c].adpcm_acc = 0;
						adpcm[c].adpcm_step= 0;
						adpcm[c].adpcm_out = 0;
						adpcm[c].flag      = 1;

						if (F2610->pcmbuf == NULL)
							adpcm[c].flag = 0;

						if (adpcm[c].start >= F2610->pcm_size)	/* Check Start in Range */
							adpcm[c].flag = 0;
					}
				}
			}
			else
			{
				/* KEY OFF */
				for(c = 0; c < 6; c++)
					if ((v >> c) & 0x01)
						adpcm[c].flag = 0;
			}
			break;

		case 0x01:	/* B0-5 = TL */
			F2610->adpcmTL = (v & 0x3f) ^ 0x3f;
			for (c = 0; c < 6; c++)
			{
				int volume = F2610->adpcmTL + adpcm[c].IL;

				if ( volume >= 63 )	/* This is correct, 63 = quiet */
				{
					adpcm[c].vol_mul   = 0;
					adpcm[c].vol_shift = 0;
				}
				else
				{
					adpcm[c].vol_mul   = 15 - (volume & 0x07);		/* so called 0.75 dB */
					adpcm[c].vol_shift =  1 + (volume >> 3);	/* Yamaha engineers used the approximation: each -6 dB is close to divide by two (shift right) */
				}

				/* calc pcm * volume data */
				adpcm[c].adpcm_out = ((adpcm[c].adpcm_acc * adpcm[c].vol_mul) >> adpcm[c].vol_shift) & ~3;	/* multiply, shift and mask out low 2 bits */
			}
			break;

		default:
		{
			c = r & 0x07;
			if (c >= 6)
				return;

			switch (r & 0x38)
			{
				case 0x08:	/* B7=L,B6=R, B4-0=IL */
				{
					adpcm[c].IL = (v & 0x1f) ^ 0x1f;
					int volume = F2610->adpcmTL + adpcm[c].IL;

					if (volume >= 63)	/* This is correct, 63 = quiet */
					{
						adpcm[c].vol_mul   = 0;
						adpcm[c].vol_shift = 0;
					}
					else
					{
						adpcm[c].vol_mul   = 15 - (volume & 0x07);		/* so called 0.75 dB */
						adpcm[c].vol_shift =  1 + (volume >> 3);	/* Yamaha engineers used the approximation: each -6 dB is close to divide by two (shift right) */
					}

					adpcm[c].pan = &out_adpcm[(v >> 6) & 0x03];

					/* calc pcm * volume data */
					adpcm[c].adpcm_out = ((adpcm[c].adpcm_acc * adpcm[c].vol_mul) >> adpcm[c].vol_shift) & ~3;	/* multiply, shift and mask out low 2 bits */
				}
				break;

				case 0x10:
				case 0x18:
					adpcm[c].start = ((F2610->adpcmreg[0x18 + c] * 0x0100 | F2610->adpcmreg[0x10 + c]) << ADPCMA_ADDRESS_SHIFT);
				break;

				case 0x20:
				case 0x28:
					adpcm[c].end = ((F2610->adpcmreg[0x28 + c] * 0x0100 | F2610->adpcmreg[0x20 + c]) << ADPCMA_ADDRESS_SHIFT);
					adpcm[c].end += (1 << ADPCMA_ADDRESS_SHIFT) - 1;
				break;
			}
		}
	}
}

#ifdef __STATE_H__
/* FM channel save , internal state only */
static void FMsave_state_adpcma(running_device *device, ADPCM_CH *adpcm)
{
	for (int ch = 0; ch < 6; ch++, adpcm++)
	{
		state_save_register_device_item(device, ch, adpcm->flag);
		state_save_register_device_item(device, ch, adpcm->now_data);
		state_save_register_device_item(device, ch, adpcm->now_addr);
		state_save_register_device_item(device, ch, adpcm->now_step);
		state_save_register_device_item(device, ch, adpcm->adpcm_acc);
		state_save_register_device_item(device, ch, adpcm->adpcm_step);
		state_save_register_device_item(device, ch, adpcm->adpcm_out);
	}
}
#endif /* _STATE_H */

#endif /* BUILD_YM2610 */


#if BUILD_YM2610
/* YM2610(OPNB) */

/* Generate samples for one of the YM2610s */
void ym2610_update_one(void *chip, FMSAMPLE **buffer, int length)
{
	YM2610 *F2610 = (YM2610 *)chip;
	FM_OPN *OPN = &F2610->OPN;
	YM_DELTAT *DELTAT = &F2610->deltaT;
	FMSAMPLE *bufL, *bufR;
	FM_CH *cch[4];
	int i, j;

	/* buffer setup */
	bufL = buffer[0];
	bufR = buffer[1];

	cch[0] = &F2610->CH[1];
	cch[1] = &F2610->CH[2];
	cch[2] = &F2610->CH[4];
	cch[3] = &F2610->CH[5];
	/* setup adpcm rom address */
	pcmbufA  = F2610->pcmbuf;
	pcmsizeA = F2610->pcm_size;

	/* refresh PG and EG */
	refresh_fc_eg_chan(OPN, cch[0]);

	if ((OPN->ST.mode & 0xc0))
	{
		/* 3SLOT MODE */
		if (cch[1]->SLOT[SLOT1].Incr == -1)
		{
			refresh_fc_eg_slot(OPN, &cch[1]->SLOT[SLOT1], OPN->SL3.fc[1], OPN->SL3.kcode[1]);
			refresh_fc_eg_slot(OPN, &cch[1]->SLOT[SLOT2], OPN->SL3.fc[2], OPN->SL3.kcode[2]);
			refresh_fc_eg_slot(OPN, &cch[1]->SLOT[SLOT3], OPN->SL3.fc[0], OPN->SL3.kcode[0]);
			refresh_fc_eg_slot(OPN, &cch[1]->SLOT[SLOT4], cch[1]->fc, cch[1]->kcode);
		}
	}
	else
		refresh_fc_eg_chan(OPN, cch[1]);

	refresh_fc_eg_chan(OPN, cch[2]);
	refresh_fc_eg_chan(OPN, cch[3]);

	/* buffering */
	for (i = 0; i < length; i++)
	{
		advance_lfo(OPN);

		/* clear output acc. */
		out_adpcm[OUTD_LEFT] = out_adpcm[OUTD_RIGHT]= out_adpcm[OUTD_CENTER] = 0;
		out_delta[OUTD_LEFT] = out_delta[OUTD_RIGHT]= out_delta[OUTD_CENTER] = 0;
		/* clear outputs */
		out_fm[1] = 0;
		out_fm[2] = 0;
		out_fm[4] = 0;
		out_fm[5] = 0;

		/* advance envelope generator */
		OPN->eg_timer += OPN->eg_timer_add;

		while (OPN->eg_timer >= OPN->eg_timer_overflow)
		{
			OPN->eg_timer -= OPN->eg_timer_overflow;
			OPN->eg_cnt++;

			advance_eg_channel(OPN, &cch[0]->SLOT[SLOT1]);
			advance_eg_channel(OPN, &cch[1]->SLOT[SLOT1]);
			advance_eg_channel(OPN, &cch[2]->SLOT[SLOT1]);
			advance_eg_channel(OPN, &cch[3]->SLOT[SLOT1]);
		}

		/* calculate FM */
		chan_calc(OPN, cch[0], 1);	/*remapped to 1*/
		chan_calc(OPN, cch[1], 2);	/*remapped to 2*/
		chan_calc(OPN, cch[2], 4);	/*remapped to 4*/
		chan_calc(OPN, cch[3], 5);	/*remapped to 5*/

		/* deltaT ADPCM */
		if (DELTAT->portstate & 0x80)
			YM_DELTAT_ADPCM_CALC(DELTAT);

		/* ADPCMA */
		for (j = 0; j < 6; j++)
		{
			if (F2610->adpcm[j].flag)
				ADPCMA_calc_chan(F2610, &F2610->adpcm[j]);
		}

		/* buffering */
		{
			int lt, rt;

			lt = out_adpcm[OUTD_LEFT] + out_adpcm[OUTD_CENTER];
			rt = out_adpcm[OUTD_RIGHT] + out_adpcm[OUTD_CENTER];
			lt += (out_delta[OUTD_LEFT] + out_delta[OUTD_CENTER]) >> 9;
			rt += (out_delta[OUTD_RIGHT] + out_delta[OUTD_CENTER]) >> 9;

			lt += ((out_fm[1] >> 1) & OPN->pan[2]);	/* the shift right was verified on real chip */
			rt += ((out_fm[1] >> 1) & OPN->pan[3]);
			lt += ((out_fm[2] >> 1) & OPN->pan[4]);
			rt += ((out_fm[2] >> 1) & OPN->pan[5]);

			lt += ((out_fm[4] >> 1) & OPN->pan[8]);
			rt += ((out_fm[4] >> 1) & OPN->pan[9]);
			lt += ((out_fm[5] >> 1) & OPN->pan[10]);
			rt += ((out_fm[5] >> 1) & OPN->pan[11]);


			lt >>= FINAL_SH;
			rt >>= FINAL_SH;

			Limit(lt, MAXOUT, MINOUT);
			Limit(rt, MAXOUT, MINOUT);

			/* buffering */
			bufL[i] = lt;
			bufR[i] = rt;
		}

		/* timer A control */
		INTERNAL_TIMER_A(&OPN->ST, cch[1])
	}
	INTERNAL_TIMER_B(&OPN->ST, length)
}


#ifdef __STATE_H__
void ym2610_postload(void *chip)
{
	if (chip)
	{
		YM2610 *F2610 = (YM2610 *)chip;
		int r;

		/* SSG registers */
		for (r = 0; r < 16; r++)
		{
			(*F2610->OPN.ST.SSG->write)(F2610->OPN.ST.param, 0, r);
			(*F2610->OPN.ST.SSG->write)(F2610->OPN.ST.param, 1, F2610->REGS[r]);
		}

		/* OPN registers */
		/* DT / MULTI , TL , KS / AR , AMON / DR , SR , SL / RR , SSG-EG */
		for (r = 0x30; r<0x9e; r++)
		{
			if ((r & 0x03) != 3)
			{
				OPNWriteReg(&F2610->OPN, r, F2610->REGS[r]);
				OPNWriteReg(&F2610->OPN, r | 0x100, F2610->REGS[r | 0x100]);
			}
		}

		/* FB / CONNECT , L / R / AMS / PMS */
		for (r = 0xb0; r < 0xb6; r++)
		{
			if ((r & 0x03) != 3)
			{
				OPNWriteReg(&F2610->OPN, r, F2610->REGS[r]);
				OPNWriteReg(&F2610->OPN, r | 0x100, F2610->REGS[r | 0x100]);
			}
		}

		/* FM channels */
		/*FM_channel_postload(F2610->CH,6);*/

		/* rhythm(ADPCMA) */
		FM_ADPCMAWrite(F2610,1,F2610->REGS[0x101]);

		for (r = 0; r < 6; r++)
		{
			FM_ADPCMAWrite(F2610, r + 0x08, F2610->REGS[r + 0x108]);
			FM_ADPCMAWrite(F2610, r + 0x10, F2610->REGS[r + 0x110]);
			FM_ADPCMAWrite(F2610, r + 0x18, F2610->REGS[r + 0x118]);
			FM_ADPCMAWrite(F2610, r + 0x20, F2610->REGS[r + 0x120]);
			FM_ADPCMAWrite(F2610, r + 0x28, F2610->REGS[r + 0x128]);
		}

		/* Delta-T ADPCM unit */
		YM_DELTAT_postload(&F2610->deltaT, &F2610->REGS[0x010]);
	}
}

static void YM2610_save_state(YM2610 *F2610, running_device *device)
{
	state_save_register_device_item_array(device, 0, F2610->REGS);
	FMsave_state_st(device, &F2610->OPN.ST);
	FMsave_state_channel(device, F2610->CH, 6);
	/* 3slots */
	state_save_register_device_item_array(device, 0, F2610->OPN.SL3.fc);
	state_save_register_device_item(device, 0, F2610->OPN.SL3.fn_h);
	state_save_register_device_item_array(device, 0, F2610->OPN.SL3.kcode);
	/* address register1 */
	state_save_register_device_item(device, 0, F2610->addr_A1);

	state_save_register_device_item(device, 0, F2610->adpcm_arrivedEndAddress);
	/* rythm(ADPCMA) */
	FMsave_state_adpcma(device, F2610->adpcm);
	/* Delta-T ADPCM unit */
	YM_DELTAT_savestate(device, &F2610->deltaT);
}
#endif /* _STATE_H */

static void YM2610_deltat_status_set(void *chip, UINT8 changebits)
{
	YM2610 *F2610 = (YM2610 *)chip;
	F2610->adpcm_arrivedEndAddress |= changebits;
}
static void YM2610_deltat_status_reset(void *chip, UINT8 changebits)
{
	YM2610 *F2610 = (YM2610 *)chip;
	F2610->adpcm_arrivedEndAddress &= (~changebits);
}

void *ym2610_init(void *param, running_device *device, int clock, int rate,
               void *pcmroma, int pcmsizea, void *pcmromb, int pcmsizeb,
               FM_TIMERHANDLER timer_handler, FM_IRQHANDLER IRQHandler, const ssg_callbacks *ssg)

{
	YM2610 *F2610;

	/* allocate extend state space */
	F2610 = auto_alloc_clear(device->machine, YM2610);

	/* allocate total level table (128kb space) */
	if (!init_tables())
	{
		auto_free(device->machine, F2610);
		return NULL;
	}

	/* FM */
	F2610->OPN.ST.param = param;
	F2610->OPN.type = TYPE_YM2610;
	F2610->OPN.P_CH = F2610->CH;
	F2610->OPN.ST.device = device;
	F2610->OPN.ST.clock = clock;
	F2610->OPN.ST.rate = rate;
	/* Extend handler */
	F2610->OPN.ST.timer_handler = timer_handler;
	F2610->OPN.ST.IRQ_Handler   = IRQHandler;
	F2610->OPN.ST.SSG           = ssg;
	/* ADPCM */
	F2610->pcmbuf   = (const UINT8 *)pcmroma;
	F2610->pcm_size = pcmsizea;
	/* DELTA-T */
	F2610->deltaT.memory = (UINT8 *)pcmromb;
	F2610->deltaT.memory_size = pcmsizeb;

	F2610->deltaT.status_set_handler = YM2610_deltat_status_set;
	F2610->deltaT.status_reset_handler = YM2610_deltat_status_reset;
	F2610->deltaT.status_change_which_chip = F2610;
	F2610->deltaT.status_change_EOS_bit = 0x80;	/* status flag: set bit7 on End Of Sample */

	Init_ADPCMATable();
#ifdef __STATE_H__
	YM2610_save_state(F2610, device);
#endif
	return F2610;
}

/* shut down emulator */
void ym2610_shutdown(void *chip)
{
	YM2610 *F2610 = (YM2610 *)chip;

	FMCloseTable();
	auto_free(F2610->OPN.ST.device->machine, F2610);
}

/* reset one of chip */
void ym2610_reset_chip(void *chip)
{
	YM2610 *F2610 = (YM2610 *)chip;
	FM_OPN *OPN = &F2610->OPN;
	YM_DELTAT *DELTAT = &F2610->deltaT;

	astring name;
	running_device *dev = F2610->OPN.ST.device;

	/* setup PCM buffers again */
	name.printf("%s",dev->tag());
	F2610->pcmbuf = (const UINT8 *)memory_region(dev->machine, name);
	F2610->pcm_size = memory_region_length(dev->machine, name);

	name.printf("%s.deltat",dev->tag());
	F2610->deltaT.memory = (UINT8 *)memory_region(dev->machine, name);

	if (F2610->deltaT.memory == NULL)
	{
		F2610->deltaT.memory = (UINT8*)F2610->pcmbuf;
		F2610->deltaT.memory_size = F2610->pcm_size;
	}
	else
		F2610->deltaT.memory_size = memory_region_length(dev->machine, name);

	/* Reset Prescaler */
	OPNSetPres(OPN, 6*24, 6*24, 4*2); /* OPN 1/6 , SSG 1/4 */
	/* reset SSG section */
	(*OPN->ST.SSG->reset)(OPN->ST.param);
	/* status clear */
	FM_IRQMASK_SET(&OPN->ST, 0x03);
	FM_BUSY_CLEAR(&OPN->ST);
	OPNWriteMode(OPN, 0x27, 0x30); /* mode 0 , timer reset */

	OPN->eg_timer = 0;
	OPN->eg_cnt = 0;

	FM_STATUS_RESET(&OPN->ST, 0xff);

	reset_channels(&OPN->ST, F2610->CH, 6);

	UINT32 i;
	/* reset OPerator paramater */
	for (i = 0xb6; i >= 0xb4; i--)
	{
		OPNWriteReg(OPN, i, 0xc0);
		OPNWriteReg(OPN, i | 0x100, 0xc0);
	}

	for (i = 0xb2; i >= 0x30; i--)
	{
		OPNWriteReg(OPN, i, 0);
		OPNWriteReg(OPN, i | 0x100, 0);
	}

	for (i = 0x26; i >= 0x20; i--)
		OPNWriteReg(OPN, i, 0);

	/**** ADPCM work initial ****/
	for (i = 0; i < 6; i++)
	{
		F2610->adpcm[i].step      = (UINT32)((float)(1 << ADPCM_SHIFT) * ((float)F2610->OPN.ST.freqbase) / 3.0);
		F2610->adpcm[i].now_addr  = 0;
		F2610->adpcm[i].now_step  = 0;
		F2610->adpcm[i].start     = 0;
		F2610->adpcm[i].end       = 0;
		/* F2610->adpcm[i].delta     = 21866; */
		F2610->adpcm[i].vol_mul   = 0;
		F2610->adpcm[i].pan       = &out_adpcm[OUTD_CENTER]; /* default center */
		F2610->adpcm[i].flagMask  = 1 << i;
		F2610->adpcm[i].flag      = 0;
		F2610->adpcm[i].adpcm_acc = 0;
		F2610->adpcm[i].adpcm_step= 0;
		F2610->adpcm[i].adpcm_out = 0;
	}
	F2610->adpcmTL = 0x3f;
	F2610->adpcm_arrivedEndAddress = 0;

	/* DELTA-T unit */
	DELTAT->freqbase = OPN->ST.freqbase;
	DELTAT->output_pointer = out_delta;
	DELTAT->portshift = 8;		/* allways 8bits shift */
	DELTAT->output_range = 1 << 23;
	YM_DELTAT_ADPCM_Reset(DELTAT, OUTD_CENTER, YM_DELTAT_EMULATION_MODE_YM2610);
}

/* YM2610 write */
/* n = number  */
/* a = address */
/* v = value   */
int ym2610_write(void *chip, int a, UINT8 v)
{
	YM2610 *F2610 = (YM2610 *)chip;
	FM_OPN *OPN   = &F2610->OPN;
	int addr, ch;

	v &= 0xff;	/* adjust to 8 bit bus */

	switch (a & 0x03)
	{
		case 0:	/* address port 0 */
			OPN->ST.address = v;
			F2610->addr_A1 = 0;

			/* Write register to SSG emulator */
			if ( v < 16 )
				(*OPN->ST.SSG->write)(OPN->ST.param, 0, v);
		break;

		case 1:	/* data port 0    */
			if (F2610->addr_A1 != 0)
				break;	/* verified on real YM2608 */

			addr = OPN->ST.address;
			F2610->REGS[addr] = v;
			switch (addr & 0xf0)
			{
				case 0x00:	/* SSG section */
					/* Write data to SSG emulator */
					(*OPN->ST.SSG->write)(OPN->ST.param, a, v);
					break;

				case 0x10: /* DeltaT ADPCM */
					ym2610_update_req(OPN->ST.param);

					switch(addr)
					{
						case 0x10:	/* control 1 */
						case 0x11:	/* control 2 */
						case 0x12:	/* start address L */
						case 0x13:	/* start address H */
						case 0x14:	/* stop address L */
						case 0x15:	/* stop address H */
						case 0x19:	/* delta-n L */
						case 0x1a:	/* delta-n H */
						case 0x1b:	/* volume */
							YM_DELTAT_ADPCM_Write(&F2610->deltaT, addr - 0x10, v);
						break;

						case 0x1c: /*  FLAG CONTROL : Extend Status Clear/Mask */
						{
							UINT8 statusmask = ~v;
							/* set arrived flag mask */
							for(ch = 0; ch < 6; ch++)
								F2610->adpcm[ch].flagMask = statusmask & (1 << ch);

							F2610->deltaT.status_change_EOS_bit = statusmask & 0x80;	/* status flag: set bit7 on End Of Sample */

							/* clear arrived flag */
							F2610->adpcm_arrivedEndAddress &= statusmask;
						}
						break;

						default:
							logerror("YM2610: write to unknown deltat register %02x val=%02x\n", addr, v);
						break;
					}
				break;

				case 0x20:	/* Mode Register */
					ym2610_update_req(OPN->ST.param);
					OPNWriteMode(OPN, addr, v);
				break;

				default:	/* OPN section */
					ym2610_update_req(OPN->ST.param);
					/* write register */
					OPNWriteReg(OPN, addr, v);
				break;
			}
			break;

		case 2:	/* address port 1 */
			OPN->ST.address = v;
			F2610->addr_A1 = 1;
		break;

		case 3:	/* data port 1    */
			if (F2610->addr_A1 != 1)
				break;	/* verified on real YM2608 */

			ym2610_update_req(OPN->ST.param);
			addr = OPN->ST.address;
			F2610->REGS[addr | 0x100] = v;
			if (addr < 0x30)
				/* 100-12f : ADPCM A section */
				FM_ADPCMAWrite(F2610, addr, v);
			else
				OPNWriteReg(OPN, addr | 0x100, v);
		break;
	}

	return OPN->ST.irq;
}

UINT8 ym2610_read(void *chip,int a)
{
	YM2610 *F2610 = (YM2610 *)chip;
	int addr = F2610->OPN.ST.address;
	UINT8 ret = 0;

	switch(a & 0x03)
	{
		case 0:	/* status 0 : YM2203 compatible */
			ret = FM_STATUS_FLAG(&F2610->OPN.ST) & 0x83;
		break;
		case 1:	/* data 0 */
			if (addr < 16)
				ret = (*F2610->OPN.ST.SSG->read)(F2610->OPN.ST.param);
			if (addr == 0xff)
				ret = 0x01;
		break;
		case 2:	/* status 1 : ADPCM status */
			/* ADPCM STATUS (arrived End Address) */
			/* B,--,A5,A4,A3,A2,A1,A0 */
			/* B     = ADPCM-B(DELTA-T) arrived end address */
			/* A0-A5 = ADPCM-A          arrived end address */
			ret = F2610->adpcm_arrivedEndAddress;
		break;
		case 3:
			ret = 0;
		break;
	}

	return ret;
}

int ym2610_timer_over(void *chip,int c)
{
	YM2610 *F2610 = (YM2610 *)chip;

	if (c)
	{	/* Timer B */
		TimerBOver(&(F2610->OPN.ST));
	}
	else
	{	/* Timer A */
		ym2610_update_req(F2610->OPN.ST.param);
		/* timer update */
		TimerAOver(&(F2610->OPN.ST));
		/* CSM mode key,TL controll */
		if (F2610->OPN.ST.mode & 0x80)
		{	/* CSM mode total level latch and auto key on */
			CSMKeyControll(F2610->OPN.type, &(F2610->CH[2]));
		}
	}

	return F2610->OPN.ST.irq;
}

#endif /* BUILD_YM2610 */
