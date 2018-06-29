/*************************************************************************

    Neo-Geo hardware

*************************************************************************/

#define NEOGEO_MASTER_CLOCK                     (24000000)
#define NEOGEO_MAIN_CPU_CLOCK                   (NEOGEO_MASTER_CLOCK / 2)
#define NEOGEO_AUDIO_CPU_CLOCK                  (NEOGEO_MASTER_CLOCK / 6)
#define NEOGEO_YM2610_CLOCK                     (NEOGEO_MASTER_CLOCK / 3)
#define NEOGEO_PIXEL_CLOCK                      (NEOGEO_MASTER_CLOCK / 4)
#define NEOGEO_HTOTAL                           (0x180)
#define NEOGEO_HBEND                            (0x01e)	/* this should really be 29.5 */
#define NEOGEO_HBSTART                          (0x15e)	/* this should really be 349.5 */
#define NEOGEO_VTOTAL                           (0x108)
#define NEOGEO_VBEND                            (0x010)
#define NEOGEO_VBSTART                          (0x0f0)
#define NEOGEO_VSSTART                          (0x000)
#define NEOGEO_VBLANK_RELOAD_HPOS               (0x11f)

#define NEOGEO_BANK_AUDIO_CPU_CART_BANK         "audio_cart"
/* do not use 2, 3 and 4 */
#define NEOGEO_BANK_CARTRIDGE                   "cartridge"
#define NEOGEO_BANK_BIOS                        "bios"
#define NEOGEO_BANK_VECTORS                     "vectors"
#define NEOGEO_BANK_AUDIO_CPU_MAIN_BANK         "audio_main"



class neogeo_state
{
public:
	static void *alloc(running_machine &machine) { return auto_alloc_clear(&machine, neogeo_state(machine)); }
	neogeo_state(running_machine &machine) { }

	/* devices */
	running_device	*maincpu;
	running_device	*audiocpu;
	running_device	*upd4990a;

	/* palette */
	double		rgb_weights_normal[5];
	double		rgb_weights_normal_bit15[5];
	double		rgb_weights_dark[5];
	double		rgb_weights_dark_bit15[5];

	/* video-related */
	UINT32		sprite_gfx_address_mask;
	UINT16		*videoram;
	UINT16		*palettes[2];		/* 0x100*16 2 byte palette entries */
	pen_t		*pens;
	UINT8		*sprite_gfx;
	const UINT8	*region_zoomy;
	UINT16		videoram_read_buffer;
	UINT16		videoram_modulo;
	UINT16		videoram_offset;
	UINT8		palette_bank;
	UINT8		screen_dark;
	UINT8		fixed_layer_source;
	UINT8		auto_animation_speed;
	UINT8		auto_animation_disabled;
	UINT8		auto_animation_counter;
	UINT8		auto_animation_frame_counter;
	UINT8		display_position_interrupt_control;

	/* timers */
	emu_timer	*display_position_interrupt_timer;
	emu_timer	*display_position_vblank_timer;
	emu_timer	*vblank_interrupt_timer;
	emu_timer	*auto_animation_timer;
	emu_timer	*sprite_line_timer;
	UINT32		display_counter;
	UINT32		vblank_interrupt_pending;
	UINT32		display_position_interrupt_pending;
	UINT32		irq3_pending;

	/* misc */
	UINT32		main_cpu_bank_address;
	UINT8		main_cpu_vector_table_source;
	UINT8		controller_select;
	UINT8		save_ram_unlocked;
	UINT8		output_data;
	UINT8		output_latch;
	UINT8		el_value;
	UINT8		led1_value;
	UINT8		led2_value;
	UINT8		recurse;
	UINT8		audio_result;
	UINT8		audio_cpu_rom_source;
	UINT8		audio_cpu_rom_source_last;
	UINT8		audio_cpu_banks[4];

	/* protection */
	UINT32		fatfury2_prot_data;
	INT32		fixed_layer_bank_type;
	UINT16		*pvc_cartridge_ram;
	UINT16		neogeo_rng;
};


/*----------- defined in drivers/neogeo.c -----------*/

void neogeo_set_display_position_interrupt_control(running_machine *machine, UINT16 data);
void neogeo_set_display_counter_msb(const address_space *space, UINT16 data);
void neogeo_set_display_counter_lsb(const address_space *space, UINT16 data);
void neogeo_acknowledge_interrupt(running_machine *machine, UINT16 data);
void neogeo_set_main_cpu_bank_address(const address_space *space, UINT32 bank_address);
READ16_HANDLER( neogeo_unmapped_r );


/*----------- defined in machine/neocrypt.c -----------*/

void kof99_neogeo_gfx_decrypt(running_machine *machine, int extra_xor);
void kof2000_neogeo_gfx_decrypt(running_machine *machine, int extra_xor);
void cmc42_neogeo_gfx_decrypt(running_machine *machine, int extra_xor);
void cmc50_neogeo_gfx_decrypt(running_machine *machine, int extra_xor);
void neogeo_cmc50_m1_decrypt(running_machine *machine);
void neo_pcm2_snk_1999(running_machine *machine, int value);
void neo_pcm2_swap(running_machine *machine, int value);
void neogeo_sfix_decrypt(running_machine *machine);
void kof99_decrypt_68k(running_machine *machine);
void garou_decrypt_68k(running_machine *machine);
void garouo_decrypt_68k(running_machine *machine);
void mslug3_decrypt_68k(running_machine *machine);
void kof2000_decrypt_68k(running_machine *machine);
void kof98_decrypt_68k(running_machine *machine);
void kof2002_decrypt_68k(running_machine *machine);
void matrim_decrypt_68k(running_machine *machine);
void mslug5_decrypt_68k(running_machine *machine);
void svc_px_decrypt(running_machine *machine);
void svcpcb_gfx_decrypt(running_machine *machine);
void svcpcb_s1data_decrypt(running_machine *machine);
void samsho5_decrypt_68k(running_machine *machine);
void kf2k3pcb_gfx_decrypt(running_machine *machine);
void kf2k3pcb_decrypt_68k(running_machine *machine);
void kf2k3pcb_decrypt_s1data(running_machine *machine);
void kof2003_decrypt_68k(running_machine *machine);
void kof2003h_decrypt_68k(running_machine *machine);
void kof2003biosdecode(running_machine *machine);
void samsh5sp_decrypt_68k(running_machine *machine);


/*----------- defined in machine/neoprot.c -----------*/

void neogeo_reset_rng(running_machine *machine);
void fatfury2_install_protection(running_machine *machine);
void mslugx_install_protection(running_machine *machine);
void kof99_install_protection(running_machine *machine);
void garou_install_protection(running_machine *machine);
void garouo_install_protection(running_machine *machine);
void mslug3_install_protection(running_machine *machine);
void kof2000_install_protection(running_machine *machine);
void install_kof98_protection(running_machine *machine);
void install_pvc_protection(running_machine *machine);


/*----------- defined in machine/neoboot.c -----------*/

void kog_px_decrypt(running_machine *machine);
void neogeo_bootleg_cx_decrypt(running_machine *machine);
void install_kof10th_protection(running_machine *machine);
void decrypt_kof10th(running_machine *machine);
void decrypt_kf10thep(running_machine *machine);
void decrypt_kf2k5uni(running_machine *machine);
void neogeo_bootleg_sx_decrypt(running_machine *machine, int value);
void kof2002b_gfx_decrypt(running_machine *machine, UINT8 *src, int size);
void kf2k2mp_decrypt(running_machine *machine);
void kf2k2mp2_px_decrypt(running_machine *machine);
void decrypt_cthd2003(running_machine *machine);
void patch_cthd2003(running_machine *machine);
void decrypt_ct2k3sp(running_machine *machine);
void decrypt_ct2k3sa(running_machine *machine);
void patch_ct2k3sa(running_machine *machine);
void decrypt_kof2k4se_68k(running_machine *machine);
void lans2004_decrypt_68k(running_machine *machine);
void lans2004_vx_decrypt(running_machine *machine);
void install_ms5plus_protection(running_machine *machine);
void svcboot_px_decrypt(running_machine *machine);
void svcboot_cx_decrypt(running_machine *machine);
void svcplus_px_decrypt(running_machine *machine);
void svcplus_px_hack(running_machine *machine);
void svcplusa_px_decrypt(running_machine *machine);
void svcsplus_px_decrypt(running_machine *machine);
void svcsplus_px_hack(running_machine *machine);
void kf2k3bl_px_decrypt(running_machine *machine);
void kf2k3bl_install_protection(running_machine *machine);
void kf2k3pl_px_decrypt(running_machine *machine);
void kf2k3upl_px_decrypt(running_machine *machine);
void kf2k3upl_install_protection(running_machine *machine);
void kf2k3pl_install_protection(running_machine *machine);
void samsho5b_px_decrypt(running_machine *machine);
void samsho5b_vx_decrypt(running_machine *machine);
void matrimbl_decrypt(running_machine *machine);


/*----------- defined in video/neogeo.c -----------*/

READ16_HANDLER( neogeo_video_register_r );
READ16_HANDLER( neogeo_paletteram_r );
WRITE16_HANDLER( neogeo_video_register_w );
WRITE16_HANDLER( neogeo_paletteram_w );

void neogeo_set_palette_bank(running_machine *machine, UINT8 data);
void neogeo_set_screen_dark(running_machine *machine, UINT8 data);
void neogeo_set_fixed_layer_source(running_machine *machine, UINT8 data);

UINT8 neogeo_get_auto_animation_counter(running_machine *machine);

VIDEO_START( neogeo );
VIDEO_RESET( neogeo );
VIDEO_UPDATE( neogeo );


/* -------------------------------------------------------------------------------- */
/*
		Neo Geo bios

    These are the known Bios Roms, Set options.bios to the one you want.

    The Universe bios roms are supported because they're now used on enough PCBs
    to be considered 'in active arcade use' rather than just homebrew hacks.
    Some may be missing, there have been multiple CRCs reported for the same
    revision in some cases (the Universe bios has an option for entering / displaying
    a serial number; these should be noted as such if they're added).

    The 'japan-hotel' BIOS is a dump of an MVS which could be found in some japanese
    hotels. it is a custom MVS mobo which uses MVS carts but it hasn't jamma
    connector and it's similar to a console with a coin mechanism, so it's a sort
    of little coin op console installed in hotels.

    The sp-45.sp1 bios is the latest 'ASIA' revision. Japan-j3.bin is the latest 'JAPAN'
    revision. Both of them are also used in the sp-4x.sp1 bios of the Jamma PCB boards.

    The current Neo Geo MVS system set (SFIX/SM1/000-LO) used is from a NEO-MVH MV1FS board.
    Other boards (MV1xx / MV2x / MV4x /MV6) other system sets?

    Zoom ROM (LO)    128K   TC531000CP      1x 128Kx8   Zoom look-up table ROM
    Fix ROM (SFIX)   128K   27C1000         1x 128Kx8   Text layer graphics ROM
    Sound ROM (SM1)  128K   27C1000/23C1000 1x 128Kx8   Z80 program ROM

*/

#define ROM_LOAD16_WORD_SWAP_BIOS(bios, name, offset, length, hash) \
		ROMX_LOAD(name, offset, length, hash, ROM_GROUPWORD | ROM_REVERSE | ROM_BIOS(bios + 1))		/* Note '+1' */

#define	UNIBIOS_V3	ROM_SYSTEM_BIOS( 0, "uni-bios_3_2", "Universe Bios (Hack, Ver. 3.2)" ) \
			ROM_LOAD16_WORD_SWAP_BIOS( 0, "uni-bios_3_2.rom", 0x00000, 0x020000, CRC(a4e8b9b3) SHA1(c92f18c3f1edda543d264ecd0ea915240e7c8258) ) /* Universe Bios v3.2 (hack) */ \
			ROM_SYSTEM_BIOS( 1, "uni-bios_3_1", "Universe Bios (Hack, Ver. 3.1)" ) \
			ROM_LOAD16_WORD_SWAP_BIOS( 1, "uni-bios_3_1.rom", 0x00000, 0x020000, CRC(0c58093f) SHA1(29329a3448c2505e1ff45ffa75e61e9693165153) ) /* Universe Bios v3.1 (hack) */ \
			ROM_SYSTEM_BIOS( 2, "uni-bios_3_0", "Universe Bios (Hack, Ver. 3.0)" ) \
			ROM_LOAD16_WORD_SWAP_BIOS( 2, "uni-bios_3_0.rom", 0x00000, 0x020000, CRC(a97c89a9) SHA1(97a5eff3b119062f10e31ad6f04fe4b90d366e7f) ) /* Universe Bios v3.0 (hack) */ 

#define UNIBIOS_V2	ROM_SYSTEM_BIOS( 0, "uni-bios_2_3", "Universe Bios (Hack, Ver. 2.3)" ) \
			ROM_LOAD16_WORD_SWAP_BIOS( 0, "uni-bios_2_3.rom", 0x00000, 0x020000, CRC(27664eb5) SHA1(5b02900a3ccf3df168bdcfc98458136fd2b92ac0) ) /* Universe Bios v2.3 (hack) */ \
			ROM_SYSTEM_BIOS( 1, "uni-bios_2_3o", "Universe Bios (Hack, Ver. 2.3, older?)" ) \
			ROM_LOAD16_WORD_SWAP_BIOS( 1, "uni-bios_2_3o.rom", 0x00000, 0x020000, CRC(601720ae) SHA1(1b8a72c720cdb5ee3f1d735bbcf447b09204b8d9) ) /* Universe Bios v2.3 (hack) alt version, withdrawn? */ \
			ROM_SYSTEM_BIOS( 2, "uni-bios_2_2", "Universe Bios (Hack, Ver. 2.2)" ) \
			ROM_LOAD16_WORD_SWAP_BIOS( 2, "uni-bios_2_2.rom", 0x00000, 0x020000, CRC(2d50996a) SHA1(5241a4fb0c63b1a23fd1da8efa9c9a9bd3b4279c) ) /* Universe Bios v2.2 (hack) */ \
			ROM_SYSTEM_BIOS( 3, "uni-bios_2_1", "Universe Bios (Hack, Ver. 2.1)" ) \
			ROM_LOAD16_WORD_SWAP_BIOS( 3, "uni-bios_2_1.rom", 0x00000, 0x020000, CRC(8dabf76b) SHA1(c23732c4491d966cf0373c65c83c7a4e88f0082c) ) /* Universe Bios v2.1 (hack) */ \
			ROM_SYSTEM_BIOS( 4, "uni-bios_2_0", "Universe Bios (Hack, Ver. 2.0)" ) \
			ROM_LOAD16_WORD_SWAP_BIOS( 4, "uni-bios_2_0.rom", 0x00000, 0x020000, CRC(0c12c2ad) SHA1(37bcd4d30f3892078b46841d895a6eff16dc921e) ) /* Universe Bios v2.0 (hack) */ 

#define UNIBIOS_V1	ROM_SYSTEM_BIOS( 0, "uni-bios_1_3", "Universe Bios (Hack, Ver. 1.3)" ) \
			ROM_LOAD16_WORD_SWAP_BIOS( 0, "uni-bios_1_3.rom", 0x00000, 0x020000, CRC(b24b44a0) SHA1(eca8851d30557b97c309a0d9f4a9d20e5b14af4e) ) /* Universe Bios v1.3 (hack) */ \
			ROM_SYSTEM_BIOS( 1, "uni-bios_1_2", "Universe Bios (Hack, Ver. 1.2)" ) \
			ROM_LOAD16_WORD_SWAP_BIOS( 1, "uni-bios_1_2.rom", 0x00000, 0x020000, CRC(4fa698e9) SHA1(682e13ec1c42beaa2d04473967840c88fd52c75a) ) /* Universe Bios v1.2 (hack) */ \
			ROM_SYSTEM_BIOS( 2, "uni-bios_1_2o", "Universe Bios (Hack, Ver. 1.2, older)" ) \
			ROM_LOAD16_WORD_SWAP_BIOS( 2, "uni-bios_1_2o.rom", 0x00000, 0x020000, CRC(e19d3ce9) SHA1(af88ef837f44a3af2d7144bb46a37c8512b67770) ) /* Universe Bios v1.2 (hack) alt version */ \
			ROM_SYSTEM_BIOS( 3, "uni-bios_1_1", "Universe Bios (Hack, Ver. 1.1)" ) \
			ROM_LOAD16_WORD_SWAP_BIOS( 3, "uni-bios_1_1.rom", 0x00000, 0x020000, CRC(5dda0d84) SHA1(4153d533c02926a2577e49c32657214781ff29b7) ) /* Universe Bios v1.1 (hack) */ \
			ROM_SYSTEM_BIOS( 4, "uni-bios_1_0", "Universe Bios (Hack, Ver. 1.0)" ) \
			ROM_LOAD16_WORD_SWAP_BIOS( 4, "uni-bios_1_0.rom", 0x00000, 0x020000, CRC(0ce453a0) SHA1(3b4c0cd26c176fc6b26c3a2f95143dd478f6abf9) ) /* Universe Bios v1.0 (hack) */ 

#define MVSBIOS_JAP	ROM_SYSTEM_BIOS( 0, "japan", "Japan MVS (Ver. 3)" ) \
			ROM_LOAD16_WORD_SWAP_BIOS( 0, "vs-bios.rom", 0x00000, 0x020000, CRC(f0e8f27d) SHA1(ecf01eda815909f1facec62abf3594eaa8d11075) ) /* Japan, Ver 6 VS Bios */ \
			ROM_SYSTEM_BIOS( 1, "japan-s2", "Japan MVS (Ver. 2)" ) \
			ROM_LOAD16_WORD_SWAP_BIOS( 1, "sp-j2.sp1", 0x00000, 0x020000, CRC(acede59c) SHA1(b6f97acd282fd7e94d9426078a90f059b5e9dd91) ) /* Japan, Older */ \
			ROM_SYSTEM_BIOS( 2, "japan-s1", "Japan MVS (Ver. 1)" ) \
			ROM_LOAD16_WORD_SWAP_BIOS( 2, "sp1.jipan.1024",	0x00000, 0x020000, CRC(9fb0abe4) SHA1(18a987ce2229df79a8cf6a84f968f0e42ce4e59d) ) /* Japan, Older */ 


#define	MVSBIOS_US	ROM_SYSTEM_BIOS( 0, "us", "US MVS (Ver. 2?)" ) \
			ROM_LOAD16_WORD_SWAP_BIOS( 0, "usa_2slt.bin", 0x00000, 0x020000, CRC(e72943de) SHA1(5c6bba07d2ec8ac95776aa3511109f5e1e2e92eb) ) /* US, 2 Slot */ \
			ROM_SYSTEM_BIOS( 1, "us-e", "US MVS (Ver. 1)" ) \
			ROM_LOAD16_WORD_SWAP_BIOS( 1, "sp-e.sp1", 0x00000, 0x020000, CRC(2723a5b5) SHA1(5dbff7531cf04886cde3ef022fb5ca687573dcb8) ) /* US, 6 Slot (V5?) */ \
			ROM_SYSTEM_BIOS( 2, "us-v2", "US MVS (4 slot, Ver. 2)" ) \
			ROM_LOAD16_WORD_SWAP_BIOS( 2, "v2.bin", 0x00000, 0x020000, CRC(62f021f4) SHA1(62d372269e1b3161c64ae21123655a0a22ffd1bb) ) /* US, 4 slot */ 


#define MVSBIOS_EURO	ROM_SYSTEM_BIOS( 0, "euro", "Europe MVS (Ver. 2)" ) \
			ROM_LOAD16_WORD_SWAP_BIOS( 0, "sp-s2.sp1", 0x00000, 0x020000, CRC(9036d879) SHA1(4f5ed7105b7128794654ce82b51723e16e389543) ) /* Europe, 1 Slot, has also been found on 2 Slot and 4 Slot (the old hacks were designed for this one) */ \
			ROM_SYSTEM_BIOS( 1, "euro-s1", "Europe MVS (Ver. 1)" ) \
			ROM_LOAD16_WORD_SWAP_BIOS( 1, "sp-s.sp1", 0x00000, 0x020000, CRC(c7f2fa45) SHA1(09576ff20b4d6b365e78e6a5698ea450262697cd) ) /* Europe, 4 Slot */ 


#define MVSBIOS_ASIA	ROM_SYSTEM_BIOS( 0, "asia", "Asia MVS (Ver. 3)" ) \
			ROM_LOAD16_WORD_SWAP_BIOS( 0, "asia-s3.rom", 0x00000, 0x020000, CRC(91b64be3) SHA1(720a3e20d26818632aedf2c2fd16c54f213543e1) ) /* Asia */ 


#define MVSBIOS_MV1C	ROM_SYSTEM_BIOS( 0, "mv1c", "NEO-MVH MV1C" ) \
			ROM_LOAD16_WORD_SWAP_BIOS( 0, "sp-45.sp1", 0x00000, 0x080000, CRC(03cc9f6a) SHA1(cdf1f49e3ff2bac528c21ed28449cf35b7957dc1) ) /* Latest Asia bios */ 


#define MVSBIOS_J3	ROM_SYSTEM_BIOS( 0, "japan-j3", "Japan MVS (J3)" ) \
			ROM_LOAD16_WORD_SWAP_BIOS( 0, "japan-j3.bin", 0x00000, 0x020000, CRC(dff6d41f) SHA1(e92910e20092577a4523a6b39d578a71d4de7085) ) /* Latest Japan bios; correct chip label unknown */ 


#define MVSBIOS_HOTEL	ROM_SYSTEM_BIOS( 0, "japan-hotel", "Custom Japanese Hotel" ) \
			ROM_LOAD16_WORD_SWAP_BIOS( 0, "sp-1v1_3db8c.bin", 0x00000, 0x020000, CRC(162f0ebe) SHA1(fe1c6dd3dfcf97d960065b1bb46c1e11cb7bf271) ) /* 'rare MVS found in japanese hotels' shows v1.3 in test mode */ 

#if defined( USE_UNIBIOS1 )
   #define NEOGEOBIOS	UNIBIOS_V1
#elif defined( USE_UNIBIOS2 )
   #define NEOGEOBIOS	UNIBIOS_V2
#elif defined( USE_UNIBIOS3 )
   #define NEOGEOBIOS	UNIBIOS_V3
#elif defined( USE_MVS_EURO )
   #define NEOGEOBIOS	MVSBIOS_EURO
#elif defined( USE_MVS_ASIA )
   #define NEOGEOBIOS	MVSBIOS_ASIA
#elif defined( USE_MVS_US )
   #define NEOGEOBIOS	MVSBIOS_US
#elif defined( USE_MVS_JAP )
   #define NEOGEOBIOS	MVSBIOS_JAP
#elif defined( USE_MVS_J3 )
   #define NEOGEOBIOS	MVSBIOS_J3
#elif defined( USE_MVS_MV1C )
   #define NEOGEOBIOS	MVSBIOS_MV1C
#else
   #define NEOGEOBIOS	MVSBIOS_HOTEL
#endif

/*  Note you'll have to modify the last for lines of each block to use the extra bios roms,
    they're hacks / homebrew / console bios roms so MAME doesn't list them by default. */

#define NEOGEO_BIOS \
	ROM_REGION16_BE( 0x80000, "mainbios", 0 ) \
	NEOGEOBIOS

#define NEO_BIOS_AUDIO_64K(name, hash)		\
	NEOGEO_BIOS 				\
	\
	ROM_REGION( 0x20000, "audiobios", 0 )	\
	ROM_LOAD( "sm1.sm1", 0x00000, 0x20000, CRC(94416d67) SHA1(42f9d7ddd6c0931fd64226a60dc73602b2819dcf) )	\
	\
	ROM_REGION( 0x20000, "audiocpu", 0 )									\
	ROM_LOAD( name, 	 0x00000, 0x10000, hash )							\
	ROM_RELOAD(          0x10000, 0x10000 )

#define NEO_BIOS_AUDIO_128K(name, hash) 	\
	NEOGEO_BIOS 				\
	\
	ROM_REGION( 0x20000, "audiobios", 0 )	\
	ROM_LOAD( "sm1.sm1", 0x00000, 0x20000, CRC(94416d67) SHA1(42f9d7ddd6c0931fd64226a60dc73602b2819dcf) )	\
	\
	ROM_REGION( 0x30000, "audiocpu", 0 )									\
	ROM_LOAD( name, 	 0x00000, 0x20000, hash )							\
	ROM_RELOAD(          0x10000, 0x20000 )

#define NEO_BIOS_AUDIO_256K(name, hash) 	\
	NEOGEO_BIOS 				\
	\
	ROM_REGION( 0x20000, "audiobios", 0 )	\
	ROM_LOAD( "sm1.sm1", 0x00000, 0x20000, CRC(94416d67) SHA1(42f9d7ddd6c0931fd64226a60dc73602b2819dcf) )	\
	\
	ROM_REGION( 0x50000, "audiocpu", 0 )									\
	ROM_LOAD( name, 	 0x00000, 0x40000, hash )							\
	ROM_RELOAD(          0x10000, 0x40000 )

#define NEO_BIOS_AUDIO_512K(name, hash) 	\
	NEOGEO_BIOS 				\
	\
	ROM_REGION( 0x20000, "audiobios", 0 )	\
	ROM_LOAD( "sm1.sm1", 0x00000, 0x20000, CRC(94416d67) SHA1(42f9d7ddd6c0931fd64226a60dc73602b2819dcf) )	\
	\
	ROM_REGION( 0x90000, "audiocpu", 0 )									\
	ROM_LOAD( name, 	 0x00000, 0x80000, hash )							\
	ROM_RELOAD(          0x10000, 0x80000 )

#define NEO_BIOS_AUDIO_ENCRYPTED_512K(name, hash)	\
	NEOGEO_BIOS					\
	ROM_REGION( 0x20000, "audiobios", 0 )									\
	ROM_LOAD( "sm1.sm1", 0x00000, 0x20000, CRC(94416d67) SHA1(42f9d7ddd6c0931fd64226a60dc73602b2819dcf) )	\
	ROM_REGION( 0x90000, "audiocpu", ROMREGION_ERASEFF )							\
	ROM_REGION( 0x80000, "audiocrypt", 0 )									\
	ROM_LOAD( name, 	 0x00000, 0x80000, hash )							\

#define NEO_BIOS_AUDIO_ENCRYPTED_256K(name, hash)	\
	NEOGEO_BIOS					\
	ROM_REGION( 0x20000, "audiobios", 0 )		\
	ROM_LOAD( "sm1.sm1", 0x00000, 0x20000, CRC(94416d67) SHA1(42f9d7ddd6c0931fd64226a60dc73602b2819dcf) )	\
	ROM_REGION( 0x90000, "audiocpu", ROMREGION_ERASEFF )							\
	ROM_REGION( 0x80000, "audiocrypt", 0 )									\
	ROM_LOAD( name, 	 0x00000, 0x40000, hash )							\

#define NEO_BIOS_AUDIO_ENCRYPTED_128K(name, hash) \
	NEOGEO_BIOS \
	ROM_REGION( 0x20000, "audiobios", 0 ) \
	ROM_LOAD( "sm1.sm1", 0x00000, 0x20000, CRC(94416d67) SHA1(42f9d7ddd6c0931fd64226a60dc73602b2819dcf) ) \
	ROM_REGION( 0x90000, "audiocpu", ROMREGION_ERASEFF ) \
	ROM_REGION( 0x80000, "audiocrypt", 0 ) \
	ROM_LOAD( name, 	 0x00000, 0x20000, hash ) \

#define NO_DELTAT_REGION

#define NEO_SFIX_32K(name, hash)				\
	ROM_REGION( 0x20000, "fixed", 0 )			\
	ROM_LOAD( name,       0x000000, 0x08000, hash ) 	\
	\
	ROM_REGION( 0x20000, "fixedbios", 0 )			\
	ROM_LOAD( "sfix.sfix", 0x000000, 0x20000, CRC(c2ea0cfd) SHA1(fd4a618cdcdbf849374f0a50dd8efe9dbab706c3) ) \
	\
	ROM_Y_ZOOM

#define NEO_SFIX_64K(name, hash)				\
	ROM_REGION( 0x20000, "fixed", 0 )			\
	ROM_LOAD( name,       0x000000, 0x10000, hash ) 	\
	\
	ROM_REGION( 0x20000, "fixedbios", 0 )			\
	ROM_LOAD( "sfix.sfix", 0x000000, 0x20000, CRC(c2ea0cfd) SHA1(fd4a618cdcdbf849374f0a50dd8efe9dbab706c3) ) \
	\
	ROM_Y_ZOOM

#define NEO_SFIX_128K(name, hash)			\
	ROM_REGION( 0x20000, "fixed", 0 )		\
	ROM_LOAD( name,       0x000000, 0x20000, hash ) \
	\
	ROM_REGION( 0x20000, "fixedbios", 0 )		\
	ROM_LOAD( "sfix.sfix", 0x000000, 0x20000, CRC(c2ea0cfd) SHA1(fd4a618cdcdbf849374f0a50dd8efe9dbab706c3) ) \
	\
	ROM_Y_ZOOM

#define ROM_Y_ZOOM				\
	ROM_REGION( 0x20000, "zoomy", 0 )	\
	ROM_LOAD( "000-lo.lo", 0x00000, 0x20000, CRC(5a86cff2) SHA1(5992277debadeb64d1c1c64b0a92d9293eaf7e4a) )

/* ------------------------------------------------------------------------- */
