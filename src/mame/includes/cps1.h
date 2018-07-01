#ifndef _CPS1_H_
#define _CPS1_H_

/* -------------- MACROS -------------- */
/*	Video raw params
	measured clocks:
	CPS2(Guru): V = 59.6376Hz, H = 15,4445kHz *H is probably measured too low!
	CPS1 GNG: V = 59.61Hz
	CPS1(Charles MacDonald):
	Pixel clock: 8.00 MHz
	Total pixel clocks per scanline: 512 clocks
	Horizontal sync pulse width : 36 clocks
	Horizontal display and blanking period: 476 clocks
	Frame size: 262 scanlines
	Refresh rate: 59.63 MHz.
*/
#define CPS_PIXEL_CLOCK		(XTAL_16MHz / 2)
#define CPS_HTOTAL		(512)
#define CPS_HBEND		(64)
#define CPS_HBSTART		(448)
#define CPS_VTOTAL		(262)
#define CPS_VBEND		(16)
#define CPS_VBSTART		(240)

#define CODE_SIZE		0x400000
#define CPS1_ROWSCROLL_OFFS	(0x20 / 2)    /* base of row scroll offsets in other RAM */

#define CPS1_COINAGE_1(diploc) \
	PORT_DIPNAME( 0x07, 0x07, DEF_STR( Coin_A ) ) PORT_DIPLOCATION(diploc ":1,2,3") \
	PORT_DIPSETTING(    0x00, DEF_STR( 4C_1C ) ) \
	PORT_DIPSETTING(    0x01, DEF_STR( 3C_1C ) ) \
	PORT_DIPSETTING(    0x02, DEF_STR( 2C_1C ) ) \
	PORT_DIPSETTING(    0x07, DEF_STR( 1C_1C ) ) \
	PORT_DIPSETTING(    0x06, DEF_STR( 1C_2C ) ) \
	PORT_DIPSETTING(    0x05, DEF_STR( 1C_3C ) ) \
	PORT_DIPSETTING(    0x04, DEF_STR( 1C_4C ) ) \
	PORT_DIPSETTING(    0x03, DEF_STR( 1C_6C ) ) \
	PORT_DIPNAME( 0x38, 0x38, DEF_STR( Coin_B ) ) PORT_DIPLOCATION(diploc ":4,5,6") \
	PORT_DIPSETTING(    0x00, DEF_STR( 4C_1C ) ) \
	PORT_DIPSETTING(    0x08, DEF_STR( 3C_1C ) ) \
	PORT_DIPSETTING(    0x10, DEF_STR( 2C_1C ) ) \
	PORT_DIPSETTING(    0x38, DEF_STR( 1C_1C ) ) \
	PORT_DIPSETTING(    0x30, DEF_STR( 1C_2C ) ) \
	PORT_DIPSETTING(    0x28, DEF_STR( 1C_3C ) ) \
	PORT_DIPSETTING(    0x20, DEF_STR( 1C_4C ) ) \
	PORT_DIPSETTING(    0x18, DEF_STR( 1C_6C ) )

#define CPS1_COINAGE_2(diploc) \
	PORT_DIPNAME( 0x07, 0x07, DEF_STR( Coinage ) ) PORT_DIPLOCATION(diploc ":1,2,3") \
	PORT_DIPSETTING(    0x00, DEF_STR( 4C_1C ) ) \
	PORT_DIPSETTING(    0x01, DEF_STR( 3C_1C ) ) \
	PORT_DIPSETTING(    0x02, DEF_STR( 2C_1C ) ) \
	PORT_DIPSETTING(    0x07, DEF_STR( 1C_1C ) ) \
	PORT_DIPSETTING(    0x06, DEF_STR( 1C_2C ) ) \
	PORT_DIPSETTING(    0x05, DEF_STR( 1C_3C ) ) \
	PORT_DIPSETTING(    0x04, DEF_STR( 1C_4C ) ) \
	PORT_DIPSETTING(    0x03, DEF_STR( 1C_6C ) )

#define CPS1_COINAGE_3(diploc) \
	PORT_DIPNAME( 0x07, 0x07, DEF_STR( Coin_A ) ) PORT_DIPLOCATION(diploc ":1,2,3") \
	PORT_DIPSETTING(    0x01, DEF_STR( 4C_1C ) ) \
	PORT_DIPSETTING(    0x02, DEF_STR( 3C_1C ) ) \
	PORT_DIPSETTING(    0x03, DEF_STR( 2C_1C ) ) \
	PORT_DIPSETTING(    0x00, "2 Coins/1 Credit (1 to continue)" ) \
	PORT_DIPSETTING(    0x07, DEF_STR( 1C_1C ) ) \
	PORT_DIPSETTING(    0x06, DEF_STR( 1C_2C ) ) \
	PORT_DIPSETTING(    0x05, DEF_STR( 1C_3C ) ) \
	PORT_DIPSETTING(    0x04, DEF_STR( 1C_4C ) ) \
	PORT_DIPNAME( 0x38, 0x38, DEF_STR( Coin_B ) ) PORT_DIPLOCATION(diploc ":4,5,6") \
	PORT_DIPSETTING(    0x08, DEF_STR( 4C_1C ) ) \
	PORT_DIPSETTING(    0x10, DEF_STR( 3C_1C ) ) \
	PORT_DIPSETTING(    0x18, DEF_STR( 2C_1C ) ) \
	PORT_DIPSETTING(    0x00, "2 Coins/1 Credit (1 to continue)" ) \
	PORT_DIPSETTING(    0x38, DEF_STR( 1C_1C ) ) \
	PORT_DIPSETTING(    0x30, DEF_STR( 1C_2C ) ) \
	PORT_DIPSETTING(    0x28, DEF_STR( 1C_3C ) ) \
	PORT_DIPSETTING(    0x20, DEF_STR( 1C_4C ) )

#define CPS1_DIFFICULTY_1(diploc) \
	PORT_DIPNAME( 0x07, 0x04, DEF_STR( Difficulty ) ) PORT_DIPLOCATION(diploc ":1,2,3") \
	PORT_DIPSETTING(    0x07, "1 (Easiest)" ) \
	PORT_DIPSETTING(    0x06, "2" ) \
	PORT_DIPSETTING(    0x05, "3" ) \
	PORT_DIPSETTING(    0x04, "4 (Normal)" ) \
	PORT_DIPSETTING(    0x03, "5" ) \
	PORT_DIPSETTING(    0x02, "6" ) \
	PORT_DIPSETTING(    0x01, "7" ) \
	PORT_DIPSETTING(    0x00, "8 (Hardest)" )


#define CPS1_DIFFICULTY_2(diploc) \
	PORT_DIPNAME( 0x07, 0x07, DEF_STR( Difficulty ) ) PORT_DIPLOCATION(diploc ":1,2,3") \
	PORT_DIPSETTING(    0x04, "1 (Easiest)" ) \
	PORT_DIPSETTING(    0x05, "2" ) \
	PORT_DIPSETTING(    0x06, "3" ) \
	PORT_DIPSETTING(    0x07, "4 (Normal)" ) \
	PORT_DIPSETTING(    0x03, "5" ) \
	PORT_DIPSETTING(    0x02, "6" ) \
	PORT_DIPSETTING(    0x01, "7" ) \
	PORT_DIPSETTING(    0x00, "8 (Hardest)" )


struct gfx_range
{
	/* start and end are as passed by the game (shift adjusted to be all 	*/
	/* in the same scale a 8x8 tiles): they don't necessarily match the 	*/
	/* position in ROM.							*/

	INT32	type;
	INT32	start;
	INT32	end;
	INT32	bank;
};

struct CPS1config
{
	const char	*name;		/* game driver name */

	/* Some games interrogate a couple of registers on bootup. */
	/* These are CPS1 board B self test checks. They wander from game to game. */
	INT32		cpsb_addr;	/* CPS board B test register address */
	INT32		cpsb_value;	/* CPS board B test register expected value */

	/* some games use as a protection check the ability to do 16-bit multiplies */
	/* with a 32-bit result, by writing the factors to two ports and reading the */
	/* result from two other ports. */
	/* It looks like this feature was introduced with 3wonders (CPSB ID = 08xx) */

	INT32		mult_factor1;
	INT32		mult_factor2;
	INT32		mult_result_lo;
	INT32		mult_result_hi;

	/* unknown registers which might be related to the multiply protection */
	INT32		unknown1;
	INT32		unknown2;
	INT32		unknown3;
	INT32		layer_control;
	INT32		priority[4];
	INT32		palette_control;

	/* ideally, the layer enable masks should consist of only one bit, */
	/* but in many cases it is unknown which bit is which. */
	INT32		layer_enable_mask[5];

	/* these depend on the B-board model and PAL */
	INT32		bank_sizes[4];
	const struct	gfx_range *bank_mapper;

	/* some C-boards have additional I/O for extra buttons/extra players */
	INT32		in2_addr;
	INT32		in3_addr;
	INT32		out2_addr;
	INT32		bootleg_kludge;
};


class cps_state
{
public:
	static void *alloc(running_machine &machine) { return auto_alloc_clear(&machine, cps_state(machine)); }
	cps_state(running_machine &machine) { }

	const struct	CPS1config *game_config;

	/* memory pointers */
	/* in the cps1 */
	UINT16		*gfxram;
	UINT16		*cps_a_regs;
	UINT16		*cps_b_regs;
	UINT16		*scroll1;
	UINT16		*scroll2;
	UINT16		*scroll3;
	UINT16		*obj;
	UINT16		*other;
	UINT16		*buffered_obj;
	UINT16		*mainram;
	UINT8		*qsound_sharedram1;
	UINT8		*qsound_sharedram2;
	/* in the cps2 */
	UINT16		*objram1;
	UINT16 		*objram2;
	UINT16 		*output;
	UINT16 		*cps2_buffered_obj;

	/* devices */
	running_device	*maincpu;
	running_device	*audiocpu;
	running_device	*msm_1;		/* fcrash */
	running_device	*msm_2;		/* fcrash */

	/* game-specific */
	UINT16 		*gigaman2_dummyqsound_ram;

	/* video-related */
	tilemap_t	*bg_tilemap[3];
	INT32		scanline1;
	INT32		scanline2;
	INT32		scancalls;
	INT32		scancount;
	INT32		scroll1x;
	INT32		scroll1y;
	INT32		scroll2x;
	INT32		scroll2y;
	INT32		scroll3x;
	INT32		scroll3y;
	INT32		stars_enabled[2];			/* Layer enabled [Y/N] */
	INT32		stars1x;
	INT32		stars1y;
	INT32		stars2x;
	INT32		stars2y;
	INT32		last_sprite_offset;			/* Offset of the last sprite */
	INT32		cps2_last_sprite_offset;		/* Offset of the last sprite */
	INT32		pri_ctrl;				/* Sprite layer priorities */
	INT32		objram_bank;

	/* memory size */
	size_t		gfxram_size;
	size_t		output_size;

	/* misc */
	INT32		dial[2];				/* forgottn */
	INT32		readpaddle;				/* pzloop2 */
	INT32		cps2networkpresent;

	/* fcrash sound hw */
	INT32		sample_buffer1;
	INT32		sample_buffer2;
	INT32		sample_select1;
	INT32		sample_select2;

	/* video config (never changed after VIDEO_START) */
	INT32		scroll_size;
	INT32		obj_size;
	INT32		cps2_obj_size;
	INT32		other_size;
	INT32		palette_align;
	INT32		palette_size;
	INT32		stars_rom_size;
	INT32		cps_version;
	UINT8		empty_tile8x8[64];
	UINT8		empty_tile[512];

	/* fcrash(bootleg) video config */
	UINT16		*bootleg_sprite_ram;
	UINT16		*bootleg_work_ram;
	INT32		sprite_base;
	INT32		sprite_x_offset;
	INT32		sprite_list_end_marker;
	INT32		layer_scroll1x_offset;
	INT32		layer_scroll2x_offset;
	INT32		layer_scroll3x_offset;
	UINT8		layer_enable_reg;
	UINT8		layer_mask_reg[4];
};


/*----------- defined in drivers/cps1.c -----------*/
void cps1_irq_handler_mus(running_device *device, int irq);

ADDRESS_MAP_EXTERN( qsound_sub_map, 8 );
INTERRUPT_GEN( cps1_interrupt );
GFXDECODE_EXTERN( cps1 );

WRITE8_DEVICE_HANDLER( cps1_oki_pin7_w );
WRITE8_HANDLER( qsound_banksw_w );
WRITE8_HANDLER( cps1_snd_bankswitch_w );

READ16_HANDLER( cps1_dsw_r );
READ16_HANDLER( wof_hack_dsw_r );
READ16_HANDLER( cps1_hack_dsw_r );
READ16_HANDLER( qsound_rom_r );
READ16_HANDLER( qsound_sharedram2_r );
READ16_HANDLER( qsound_sharedram1_r );
READ16_HANDLER( cps1_in0_r );
READ16_HANDLER( cps1_in1_r );
READ16_HANDLER( cps1_in2_r );
READ16_HANDLER( cps1_in3_r );

WRITE16_HANDLER( qsound_sharedram1_w );
WRITE16_HANDLER( cps1_coinctrl_w );
WRITE16_HANDLER( cps1_soundlatch_w );
WRITE16_HANDLER( cps1_soundlatch2_w );
WRITE16_HANDLER( cpsq_coinctrl2_w );
WRITE16_HANDLER( qsound_sharedram2_w );

/*----------- defined in video/cps1.c -----------*/
void cps1_get_video_base(running_machine *machine);
void cps2_set_sprite_priorities(running_machine *machine);
void cps2_objram_latch(running_machine *machine);

READ16_HANDLER( cps1_cps_b_r );
READ16_HANDLER( cps2_objram1_r );
READ16_HANDLER( cps2_objram2_r );

WRITE16_HANDLER( cps1_cps_a_w );
WRITE16_HANDLER( cps1_cps_b_w );
WRITE16_HANDLER( cps1_gfxram_w );
WRITE16_HANDLER( cps2_objram_bank_w );
WRITE16_HANDLER( cps2_objram1_w );
WRITE16_HANDLER( cps2_objram2_w );

DRIVER_INIT( cps1 );
DRIVER_INIT( cps2_video );

VIDEO_START( cps1 );
VIDEO_START( cps2 );

VIDEO_UPDATE( cps1 );
VIDEO_EOF( cps1 );

/*************************************
*	Encryption
**************************************/
/*----------- defined in machine/cps2crpt.c -----------*/
DRIVER_INIT( cps2crpt );

/*----------- defined in drivers/cps1.c -----------*/
INPUT_PORTS_EXTERN( cps1_3b );
INPUT_PORTS_EXTERN( cps1_2b );
INPUT_PORTS_EXTERN( cps1_3players );
INPUT_PORTS_EXTERN( cps1_4players );
INPUT_PORTS_EXTERN( knights );
INPUT_PORTS_EXTERN( sf2 );
INPUT_PORTS_EXTERN( punisher );
INPUT_PORTS_EXTERN( cawing );
INPUT_PORTS_EXTERN( dino );
INPUT_PORTS_EXTERN( ffight );
INPUT_PORTS_EXTERN( kod );
INPUT_PORTS_EXTERN( slammast );
INPUT_PORTS_EXTERN( captcomm );
INPUT_PORTS_EXTERN( varth );

/*----------- defined in machine/kabuki.c -----------*/
void mgakuen2_decode(running_machine *machine);
void pang_decode(running_machine *machine);
void cworld_decode(running_machine *machine);
void hatena_decode(running_machine *machine);
void spang_decode(running_machine *machine);
void spangj_decode(running_machine *machine);
void sbbros_decode(running_machine *machine);
void marukin_decode(running_machine *machine);
void qtono1_decode(running_machine *machine);
void qsangoku_decode(running_machine *machine);
void block_decode(running_machine *machine);
void wof_decode(running_machine *machine);
void dino_decode(running_machine *machine);
void punisher_decode(running_machine *machine);
void slammast_decode(running_machine *machine);

/* ----------------------------------------------------*/

#endif
