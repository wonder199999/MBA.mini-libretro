/*************************************************************************
    Irem M92 hardware
*************************************************************************/

typedef struct _pf_layer_info pf_layer_info;
struct _pf_layer_info
{
	tilemap_t	*tmap;
	tilemap_t	*wide_tmap;
	UINT16		vram_base;
	UINT16		control[4];
};

class m92_state : public driver_data_t
{
public:
	static driver_data_t *alloc(running_machine &machine) { return auto_alloc_clear(&machine, m92_state(machine)); }

	m92_state(running_machine &machine)
		: driver_data_t(machine) { }

	UINT32		bank_address;
	UINT32		raster_irq_position;
	INT32		sprite_list;

	/* memory pointers */
	UINT16		*vram_data;
	UINT16		*sprite_control;

	/* device */
	running_device	*nec_maincpu;
	running_device	*nec_sndcpu;

	UINT16		pf_master_control[4];
	UINT16		sound_status;
	UINT16		video_control;
	pf_layer_info	pf_layer[3];

	UINT8		game_kludge;
	UINT8		palette_bank;
	UINT8		irq_vector;
	UINT8		irq_vector_base;
	UINT8		sprite_buffer_busy;
};


/*----------- defined in drivers/irem/m92.c -----------*/

void m92_sprite_interrupt(running_machine *machine);


/*----------- defined in video/m92.c -----------*/

READ16_HANDLER( m92_paletteram_r );

WRITE16_HANDLER( m92_spritecontrol_w );
WRITE16_HANDLER( m92_videocontrol_w );
WRITE16_HANDLER( m92_paletteram_w );
WRITE16_HANDLER( m92_vram_w );
WRITE16_HANDLER( m92_pf1_control_w );
WRITE16_HANDLER( m92_pf2_control_w );
WRITE16_HANDLER( m92_pf3_control_w );
WRITE16_HANDLER( m92_master_control_w );

VIDEO_START( m92 );
VIDEO_UPDATE( m92 );

VIDEO_START( ppan );
VIDEO_UPDATE( ppan );
