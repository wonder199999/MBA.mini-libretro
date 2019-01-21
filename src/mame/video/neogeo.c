/***************************************************************************

    Neo-Geo hardware

****************************************************************************/

#include "emu.h"
#include "includes/neogeo.h"
#include "video/resnet.h"

#define NUM_PENS	(0x1000)

/*************************************
 *
 *  Video RAM access
 *
 *************************************/

static void set_videoram_offset(running_machine *machine, UINT16 data)
{
	neogeo_state *state = machine->driver_data<neogeo_state>();

	state->videoram_offset = (data & 0x8000 ? data & 0x87ff : data);
	/* the read happens right away */
	state->videoram_read_buffer = state->videoram[state->videoram_offset];
}

static UINT16 get_videoram_data(running_machine *machine)
{
	neogeo_state *state = machine->driver_data<neogeo_state>();
	return state->videoram_read_buffer;
}

static void set_videoram_data(running_machine *machine, UINT16 data)
{
	neogeo_state *state = machine->driver_data<neogeo_state>();

	state->videoram[state->videoram_offset] = data;
	/* auto increment/decrement the current offset - A15 is NOT effected */
	set_videoram_offset(machine, ((state->videoram_offset & 0x8000) | ((state->videoram_offset + state->videoram_modulo) & 0x7fff)));
}

static void set_videoram_modulo( running_machine *machine, UINT16 data)
{
	neogeo_state *state = machine->driver_data<neogeo_state>();
	state->videoram_modulo = data;
}

static UINT16 get_videoram_modulo( running_machine *machine )
{
	neogeo_state *state = machine->driver_data<neogeo_state>();
	return state->videoram_modulo;
}


/*************************************
 *
 *  Palette handling
 *
 *************************************/

static void compute_rgb_weights( running_machine *machine )
{
	neogeo_state *state = machine->driver_data<neogeo_state>();

	static const int resistances[5] = { 220, 470, 1000, 2200, 3900 };

	/* compute four sets of weights - with or without the pulldowns -
	   ensuring that we use the same scaler for all */

	double scaler = compute_resistor_weights( 0, 0xff, -1,
							    5, resistances, state->rgb_weights_normal, 0, 0,
							    0, NULL, NULL, 0, 0,
							    0, NULL, NULL, 0, 0 );

	compute_resistor_weights( 0, 0xff, scaler,
						5, resistances, state->rgb_weights_normal_bit15, 8200, 0,
						0, NULL, NULL, 0, 0,
						0, NULL, NULL, 0, 0 );

	compute_resistor_weights( 0, 0xff, scaler,
						5, resistances, state->rgb_weights_dark, 150, 0,
						0, NULL, NULL, 0, 0,
						0, NULL, NULL, 0, 0 );

	compute_resistor_weights( 0, 0xff, scaler,
						5, resistances, state->rgb_weights_dark_bit15, (8200.0 * 150.0) / (8200.0 + 150.0), 0,
						0, NULL, NULL, 0, 0,
						0, NULL, NULL, 0, 0 );
}

static pen_t get_pen( running_machine *machine, UINT16 data )
{
	neogeo_state *state = machine->driver_data<neogeo_state>();

	double *weights;

	if (state->screen_dark)
		weights = (data & 0x8000) ? state->rgb_weights_dark_bit15 : state->rgb_weights_dark;
	else
		weights = (data & 0x8000) ? state->rgb_weights_normal_bit15 : state->rgb_weights_normal;

	UINT8 r = combine_5_weights( weights,
						(data >> 11) & 0x01,
						(data >> 10) & 0x01,
						(data >>  9) & 0x01,
						(data >>  8) & 0x01,
						(data >> 14) & 0x01 );

	UINT8 g = combine_5_weights( weights,
						(data >>  7) & 0x01,
						(data >>  6) & 0x01,
						(data >>  5) & 0x01,
						(data >>  4) & 0x01,
						(data >> 13) & 0x01 );

	UINT8 b = combine_5_weights( weights,
						(data >>  3) & 0x01,
						(data >>  2) & 0x01,
						(data >>  1) & 0x01,
						(data >>  0) & 0x01,
						(data >> 12) & 0x01 );

	return MAKE_RGB(r, g, b);
}

static STATE_POSTLOAD( regenerate_pens )
{
	neogeo_state *state = machine->driver_data<neogeo_state>();

	for (UINT32 i = 0; i < NUM_PENS; )
	{
		state->pens[i] = get_pen(machine, state->palettes[state->palette_bank][i]); i++;
		state->pens[i] = get_pen(machine, state->palettes[state->palette_bank][i]); i++;
		state->pens[i] = get_pen(machine, state->palettes[state->palette_bank][i]); i++;
		state->pens[i] = get_pen(machine, state->palettes[state->palette_bank][i]); i++;
	}
}

void neogeo_set_palette_bank( running_machine *machine, UINT8 data )
{
	neogeo_state *state = machine->driver_data<neogeo_state>();

	if (data != state->palette_bank)
	{
		state->palette_bank = data;
		regenerate_pens(machine, NULL);
	}
}

void neogeo_set_screen_dark( running_machine *machine, UINT8 data )
{
	neogeo_state *state = machine->driver_data<neogeo_state>();

	if (data != state->screen_dark)
	{
		state->screen_dark = data;
		regenerate_pens(machine, NULL);
	}
}

READ16_HANDLER( neogeo_paletteram_r )
{
	neogeo_state *state = space->machine->driver_data<neogeo_state>();

	return state->palettes[state->palette_bank][offset];
}

WRITE16_HANDLER( neogeo_paletteram_w )
{
	neogeo_state *state = space->machine->driver_data<neogeo_state>();

	UINT16 *addr = &state->palettes[state->palette_bank][offset];
	COMBINE_DATA(addr);

	state->pens[offset] = get_pen(space->machine, *addr);
}


/*************************************
 *
 *  Auto animation
 *
 *************************************/

static void set_auto_animation_speed( running_machine *machine, UINT8 data)
{
	neogeo_state *state = machine->driver_data<neogeo_state>();
	state->auto_animation_speed = data;
}

static void set_auto_animation_disabled( running_machine *machine, UINT8 data)
{
	neogeo_state *state = machine->driver_data<neogeo_state>();
	state->auto_animation_disabled = data;
}

UINT8 neogeo_get_auto_animation_counter( running_machine *machine )
{
	neogeo_state *state = machine->driver_data<neogeo_state>();
	return state->auto_animation_counter;
}

static TIMER_CALLBACK( auto_animation_timer_callback )
{
	neogeo_state *state = machine->driver_data<neogeo_state>();

	if (state->auto_animation_frame_counter == 0)
	{
		state->auto_animation_frame_counter = state->auto_animation_speed;
		state->auto_animation_counter += 1;
	}
	else
		state->auto_animation_frame_counter = state->auto_animation_frame_counter - 1;

	timer_adjust_oneshot(state->auto_animation_timer, machine->primary_screen->time_until_pos(NEOGEO_VSSTART), 0);
}

static void create_auto_animation_timer( running_machine *machine )
{
	neogeo_state *state = machine->driver_data<neogeo_state>();
	state->auto_animation_timer = timer_alloc(machine, auto_animation_timer_callback, NULL);
}

static void start_auto_animation_timer( running_machine *machine )
{
	neogeo_state *state = machine->driver_data<neogeo_state>();
	timer_adjust_oneshot(state->auto_animation_timer, machine->primary_screen->time_until_pos( NEOGEO_VSSTART ), 0);
}


/*************************************
 *
 *  Fixed layer
 *
 *************************************/

void neogeo_set_fixed_layer_source( running_machine *machine, UINT8 data )
{
	neogeo_state *state = machine->driver_data<neogeo_state>();
	state->fixed_layer_source = data;
}


static void draw_fixed_layer( running_machine *machine, bitmap_t *bitmap, int scanline )
{
	neogeo_state *state = machine->driver_data<neogeo_state>();

	UINT8  *gfx_base = memory_region(machine, state->fixed_layer_source ? "fixed" : "fixedbios");
	UINT32 addr_mask = memory_region_length(machine, state->fixed_layer_source ? "fixed" : "fixedbios") - 1;
	UINT16 *video_data = &state->videoram[0x7000 | (scanline >> 3)];
	UINT32 *pixel_addr = BITMAP_ADDR32(bitmap, scanline, NEOGEO_HBEND);

	int garouoffsets[32];
	int banked = state->fixed_layer_source && (addr_mask > 0x01ffff);

	/* thanks to Mr K for the garou & kof2000 banking info */
	/* Build line banking table for Garou & MS3 before starting render */
	if (state->fixed_layer_bank_type == 1)
	{
		if (banked)
		{
			for (int k = 0, y = 0, garoubank = 0; y < 32; k += 2)
			{
				if (state->videoram[0x7500 + k] == 0x0200 && (state->videoram[0x7580 + k] & 0xff00) == 0xff00)
				{
					garoubank = state->videoram[0x7580 + k] & 0x03;
					garouoffsets[y++] = garoubank;
				}
				garouoffsets[y++] = garoubank;
			}
		}
	}

	for (UINT32 x = 0; x < 40; x++)
	{
		UINT16 code_and_palette = *video_data;
		UINT16 code = code_and_palette & 0x0fff;
		pen_t *char_pens = &state->pens[code_and_palette >> 12 << 4];

		if (banked)
		{
			if (state->fixed_layer_bank_type == 2)
				code += 0x1000 * (((state->videoram[0x7500 + (((scanline >> 3) - 1) & 0x1f) + 32 * (x / 6)] >> (5 - (x % 6)) * 2) & 0x03) ^ 0x03);
			else if (state->fixed_layer_bank_type == 1)
				code += 0x1000 * (garouoffsets[((scanline >> 3) - 2) & 0x1f] ^ 0x03);	/* Garou, MSlug 3 */
		}
		{
			UINT8 *gfx = &gfx_base[((code << 5) | (scanline & 0x07)) & addr_mask];
			const int table[8] = {
				gfx[0x10] & 0x0f, gfx[0x10] & 0xf0, gfx[0x18] & 0x0f, gfx[0x18] & 0xf0,
				gfx[0x00] & 0x0f, gfx[0x00] & 0xf0, gfx[0x08] & 0x0f, gfx[0x08] & 0xf0
			};

			if (table[0]) *pixel_addr = char_pens[table[0]];
			pixel_addr++;
			if (table[1]) *pixel_addr = char_pens[table[1] >> 4];
			pixel_addr++;
			if (table[2]) *pixel_addr = char_pens[table[2]];
			pixel_addr++;
			if (table[3]) *pixel_addr = char_pens[table[3] >> 4];
			pixel_addr++;
			if (table[4]) *pixel_addr = char_pens[table[4]];
			pixel_addr++;
			if (table[5]) *pixel_addr = char_pens[table[5] >> 4];
			pixel_addr++;
			if (table[6]) *pixel_addr = char_pens[table[6]];
			pixel_addr++;
			if (table[7]) *pixel_addr = char_pens[table[7] >> 4];
			pixel_addr++;
		}
		video_data += 0x20;
	}
}


/*************************************
 *
 *  Sprite hardware
 *
 *************************************/

#define MAX_SPRITES_PER_SCREEN    (381)
#define MAX_SPRITES_PER_LINE      (96)


/* horizontal zoom table - verified on real hardware */
static const int zoom_x_tables[16][16] =
{
	{ 0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0 },
	{ 0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0 },
	{ 0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0 },
	{ 0,0,1,0,1,0,0,0,1,0,0,0,1,0,0,0 },
	{ 0,0,1,0,1,0,0,0,1,0,0,0,1,0,1,0 },
	{ 0,0,1,0,1,0,1,0,1,0,0,0,1,0,1,0 },
	{ 0,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0 },
	{ 1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0 },
	{ 1,0,1,0,1,0,1,0,1,1,1,0,1,0,1,0 },
	{ 1,0,1,1,1,0,1,0,1,1,1,0,1,0,1,0 },
	{ 1,0,1,1,1,0,1,0,1,1,1,0,1,0,1,1 },
	{ 1,0,1,1,1,0,1,1,1,1,1,0,1,0,1,1 },
	{ 1,0,1,1,1,0,1,1,1,1,1,0,1,1,1,1 },
	{ 1,1,1,1,1,0,1,1,1,1,1,0,1,1,1,1 },
	{ 1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1 },
	{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 }
};


INLINE int rows_to_height(int rows)
{
	if ((rows == 0) || (rows > 0x20))
		rows = 0x20;

	return rows * 0x10;
}


INLINE int sprite_on_scanline(int scanline, int y, int rows)
{
	return (rows == 0) || (rows >= 0x20) || ((scanline - y) & 0x01ff) < (rows << 4);
}


static void draw_sprites( running_machine *machine, bitmap_t *bitmap, int scanline )
{
	neogeo_state *state = machine->driver_data<neogeo_state>();

	/* select the active list */
	UINT16 *sprite_list = (scanline & 0x01) ? &state->videoram[0x8680] : &state->videoram[0x8600];
	INT32 y = 0, zoom_y = 0, x = 0, zoom_x = 0, rows = 0;

	/* optimization -- find last non-zero entry and only draw that many +1
	   sprite.  This is not 100% correct as the hardware will keep drawing
	   the #0 sprite over and over, but we need the speed	*/

	INT32 max_sprite_index;
	for (max_sprite_index = (MAX_SPRITES_PER_LINE - 1); max_sprite_index >= 0; max_sprite_index--)
		if (sprite_list[max_sprite_index] != 0)
			break;

	/* add the +1 now, just in case the 0 at the end is real sprite */
	if (max_sprite_index != (MAX_SPRITES_PER_LINE - 1))
		max_sprite_index += 1;

	for (INT32 sprite_index = 0; sprite_index <= max_sprite_index; sprite_index++)
	{
		UINT16 sprite_number = sprite_list[sprite_index] & 0x01ff;
		UINT16 y_control = state->videoram[0x8200 | sprite_number];
		UINT16 zoom_control = state->videoram[0x8000 | sprite_number];

		/* if chained, go to next X coordinate and get new X zoom */
		if (y_control & 0x40)
		{
			x = (x + zoom_x + 1) & 0x01ff;
			zoom_x = (zoom_control >> 8) & 0x0f;
		}
		/* new block */
		else
		{
			y = 0x200 - (y_control >> 7);
			x = state->videoram[0x8400 | sprite_number] >> 7;
			zoom_y = zoom_control & 0xff;
			zoom_x = (zoom_control >> 8) & 0x0f;
			rows = y_control & 0x3f;
		}

		/* skip if falls completely outside the screen */
		if ((x >= 0x0140) && (x <= 0x01f0))
			continue;

		/* double check the Y coordinate, in case somebody modified the sprite coordinate since we buffered it */
		if (sprite_on_scanline(scanline, y, rows))
		{
			INT32 sprite_line = (scanline - y) & 0x01ff;
			INT32 zoom_line = sprite_line & 0xff;
			INT32 invert = sprite_line & 0x0100;

			if (invert)
				zoom_line ^= 0xff;

			if (rows > 0x20)
			{
				zoom_line %= (zoom_y + 1) << 1;

				if (zoom_line > zoom_y)
				{
					zoom_line = ((zoom_y + 1) << 1) - 1 - zoom_line;
					invert = !invert;
				}
			}

			UINT8 sprite_y_and_tile = state->region_zoomy[(zoom_y << 8) | zoom_line];
			INT32 sprite_y = sprite_y_and_tile & 0x0f;
			INT32 tile = sprite_y_and_tile >> 4;

			if (invert)
			{
				sprite_y ^= 0x0f;
				tile ^= 0x1f;
			}

			offs_t attr_and_code_offs = (sprite_number << 6) | (tile << 1);
			UINT16 attr = state->videoram[attr_and_code_offs + 1];
			UINT32 code = ((attr << 12) & 0x0f0000) | state->videoram[attr_and_code_offs];

			/* substitute auto animation bits */
			if (!state->auto_animation_disabled)
			{
				if (attr & 0x0008)
					code = (code & ~0x07) | (state->auto_animation_counter & 0x07);
				else if (attr & 0x0004)
					code = (code & ~0x03) | (state->auto_animation_counter & 0x03);
			}

			/* vertical flip? */
			if (attr & 0x0002)
				sprite_y ^= 0x0f;

			const int *zoom_x_table = zoom_x_tables[zoom_x];
			/* compute offset in gfx ROM and mask it to the number of bits available */
			UINT8 *gfx = &state->sprite_gfx[((code << 8) | (sprite_y << 4)) & state->sprite_gfx_address_mask];
			pen_t *line_pens = &state->pens[attr >> 8 << 4];

			INT32 x_inc;
			/* horizontal flip? */
			if (attr & 0x0001)
			{
				gfx += 0x0f;
				x_inc = -1;
			}
			else
				x_inc = 1;

			/* draw the line - no wrap-around */
			if (x <= 0x01f0)
			{
				UINT32 *pixel_addr = BITMAP_ADDR32(bitmap, scanline, x + NEOGEO_HBEND);

				for (UINT32 i = 0; i < 0x10; i++)
				{
					if (*zoom_x_table)
					{
						if (*gfx)
							*pixel_addr = line_pens[*gfx];
						pixel_addr++;
					}
					zoom_x_table++;
					gfx += x_inc;
				}
			}
			/* wrap-around */
			else
			{
				INT32 x_save = x;
				UINT32 *pixel_addr = BITMAP_ADDR32(bitmap, scanline, NEOGEO_HBEND);

				for (UINT32 i = 0; i < 0x10; i++)
				{
					if (*zoom_x_table)
					{
						if (x >= 0x200)
						{
							if (*gfx)
								*pixel_addr = line_pens[*gfx];
							pixel_addr++;
						}
						x++;
					}
					zoom_x_table++;
					gfx += x_inc;
				}
				x = x_save;
			}
		}
	}
}


static void parse_sprites( running_machine *machine, int scanline )
{
	neogeo_state *state = machine->driver_data<neogeo_state>();

	/* select the active list */
	UINT16 *sprite_list = (scanline & 0x01) ? &state->videoram[0x8680] : &state->videoram[0x8600];
	UINT32 active_sprite_count = 0;
	INT32 y = 0, rows = 0;

	/* scan all sprites */
	for (UINT16 sprite_number = 0; sprite_number < MAX_SPRITES_PER_SCREEN; sprite_number++)
	{
		UINT16 y_control = state->videoram[0x8200 | sprite_number];

		/* if not chained, get Y position and height, otherwise use previous values */
		if (~y_control & 0x40)
		{
			y = 0x200 - (y_control >> 7);
			rows = y_control & 0x3f;
		}

		/* skip sprites with 0 rows */
		if (rows == 0)
			continue;

		if (!sprite_on_scanline(scanline, y, rows))
			continue;

		/* sprite is on this scanline, add it to active list */
		*sprite_list = sprite_number;
		sprite_list++;

		/* increment sprite count, and if we reached the max, bail out */
		active_sprite_count++;

		if (active_sprite_count == MAX_SPRITES_PER_LINE)
			break;
	}
	/* fill the rest of the sprite list with 0, including one extra entry */
	memset(sprite_list, 0, sizeof(sprite_list[0]) * (MAX_SPRITES_PER_LINE - active_sprite_count + 1));
}


static TIMER_CALLBACK( sprite_line_timer_callback )
{
	neogeo_state *state = machine->driver_data<neogeo_state>();

	int scanline = param;

	/* we are at the beginning of a scanline -
	   we need to draw the previous scanline and parse the sprites on the current one */
	if (scanline != 0)
		machine->primary_screen->update_partial(scanline - 1);

	parse_sprites(machine, scanline);

	/* let's come back at the beginning of the next line */
	scanline = (scanline + 1) % NEOGEO_VTOTAL;		// 264

	timer_adjust_oneshot(state->sprite_line_timer, machine->primary_screen->time_until_pos(scanline), scanline);
}


static void create_sprite_line_timer( running_machine *machine )
{
	neogeo_state *state = machine->driver_data<neogeo_state>();
	state->sprite_line_timer = timer_alloc(machine, sprite_line_timer_callback, NULL);
}


static void start_sprite_line_timer( running_machine *machine )
{
	neogeo_state *state = machine->driver_data<neogeo_state>();
	timer_adjust_oneshot(state->sprite_line_timer, machine->primary_screen->time_until_pos(0), 0);
}


static void optimize_sprite_data( running_machine *machine )
{
	neogeo_state *state = machine->driver_data<neogeo_state>();

	/* convert the sprite graphics data into a format that allows faster blitting */
	UINT8 *src;
	UINT8 *dest;

	/* get mask based on the length rounded up to the nearest power of 2 */
	state->sprite_gfx_address_mask = 0xffffffff;

	UINT32 len = memory_region_length(machine, "sprites");

	for (UINT32 bit = 0x80000000; bit != 0; bit >>= 1)
	{
		if (((len * 2) - 1) & bit)
			break;

		state->sprite_gfx_address_mask >>= 1;
	}

	state->sprite_gfx = auto_alloc_array_clear(machine, UINT8, state->sprite_gfx_address_mask + 1);

	src = memory_region(machine, "sprites");
	dest = state->sprite_gfx;

	for (UINT32 i = 0; i < len; i += 0x80, src += 0x80)
	{
		for (UINT32 y = 0; y < 0x10; y++)
		{
			for (UINT32 x = 0; x < 8; x++)
			{
				*(dest++) = (((src[0x43 | (y << 2)] >> x) & 0x01) << 3) |
						    (((src[0x41 | (y << 2)] >> x) & 0x01) << 2) |
							(((src[0x42 | (y << 2)] >> x) & 0x01) << 1) |
							(((src[0x40 | (y << 2)] >> x) & 0x01) << 0);
			}

			for (UINT32 x = 0; x < 8; x++)
			{
				*(dest++) = (((src[0x03 | (y << 2)] >> x) & 0x01) << 3) |
						    (((src[0x01 | (y << 2)] >> x) & 0x01) << 2) |
							(((src[0x02 | (y << 2)] >> x) & 0x01) << 1) |
							(((src[0x00 | (y << 2)] >> x) & 0x01) << 0);
			}
		}
	}
}


/*************************************
 *
 *  Video control
 *
 *************************************/

static UINT16 get_video_control( running_machine *machine )
{
/*	The format of this very important location is:  AAAA AAAA A??? BCCC

        A is the raster line counter. mosyougi relies solely on this to do the
          raster effects on the title screen; sdodgeb loops waiting for the top
          bit to be 1; zedblade heavily depends on it to work correctly (it
          checks the top bit in the IRQ2 handler).

        B is definitely a PAL/NTSC flag. Evidence:
          1) trally changes the position of the speed indicator depending on
             it (0 = lower 1 = higher).
          2) samsho3 sets a variable to 60 when the bit is 0 and 50 when it's 1.
             This is obviously the video refresh rate in Hz.
          3) samsho3 sets another variable to 256 or 307. This could be the total
             screen height (including vblank), or close to that.

          Some games (e.g. lstbld2, samsho3) do this (or similar):
          bclr    #$0, $3c000e.l
          when the bit is set, so 3c000e (whose function is unknown) has to be
          related

        C animation counter lower 3 bits	*/

	/* the vertical counter chain goes from 0xf8 - 0x1ff */
	UINT16 v_counter = machine->primary_screen->vpos() + 0x0100;

	if (v_counter >= 0x0200)
		v_counter -= NEOGEO_VTOTAL;	// 0x0108

	UINT16 ret = (v_counter << 7) | (neogeo_get_auto_animation_counter(machine) & 0x07);

	return ret;
}

static void set_video_control( running_machine *machine, UINT16 data )
{
	set_auto_animation_speed(machine, data >> 8);
	set_auto_animation_disabled(machine, data & 0x0008);

	neogeo_set_display_position_interrupt_control(machine, data & 0x00f0);
}

READ16_HANDLER( neogeo_video_register_r )
{
	UINT16 ret;

	/* accessing the LSB only is not mapped */
	if (mem_mask == 0x00ff)
		ret = neogeo_unmapped_r(space, 0, 0xffff) & 0x00ff;
	else
	{
		switch (offset)
		{
			default:
			case 0x00:
			case 0x01: ret = get_videoram_data(space->machine); break;
			case 0x02: ret = get_videoram_modulo(space->machine); break;
			case 0x03: ret = get_video_control(space->machine); break;
		}
	}

	return ret;
}

WRITE16_HANDLER( neogeo_video_register_w )
{
	/* accessing the LSB only is not mapped */
	if (mem_mask != 0x00ff)
	{
		/* accessing the MSB only stores same data in MSB and LSB */
		if (mem_mask == 0xff00)
			data = (data & 0xff00) | (data >> 8);

		switch (offset)
		{
			case 0x00: set_videoram_offset(space->machine, data); break;
			case 0x01: set_videoram_data(space->machine, data); break;
			case 0x02: set_videoram_modulo(space->machine, data); break;
			case 0x03: set_video_control(space->machine, data); break;
			case 0x04: neogeo_set_display_counter_msb(space, data); break;
			case 0x05: neogeo_set_display_counter_lsb(space, data); break;
			case 0x06: neogeo_acknowledge_interrupt(space->machine, data); break;
			case 0x07:
			default: break;		/* unknown, see get_video_control */
		}
	}
}

/*************************************
 *
 *  Video system start
 *
 *************************************/

VIDEO_START( neogeo )
{
	neogeo_state *state = machine->driver_data<neogeo_state>();

	/* allocate memory not directly mapped */
	state->palettes[0] = auto_alloc_array(machine, UINT16, NUM_PENS);
	state->palettes[1] = auto_alloc_array(machine, UINT16, NUM_PENS);
	state->pens = auto_alloc_array(machine, pen_t, NUM_PENS);
	state->videoram = auto_alloc_array(machine, UINT16, 0x20000 / 2);

	/* clear allocated memory */
	memset(state->palettes[0], 0x00, NUM_PENS * sizeof(UINT16));
	memset(state->palettes[1], 0x00, NUM_PENS * sizeof(UINT16));
	memset(state->pens, 0x00, NUM_PENS * sizeof(pen_t));
	memset(state->videoram, 0x00, 0x20000);

	compute_rgb_weights(machine);
	create_sprite_line_timer(machine);
	create_auto_animation_timer(machine);
	optimize_sprite_data(machine);

	/* initialize values that are not modified on a reset */
	state->videoram_read_buffer = 0;
	state->videoram_offset = 0;
	state->videoram_modulo = 0;
	state->auto_animation_speed = 0;
	state->auto_animation_disabled = 0;
	state->auto_animation_counter = 0;
	state->auto_animation_frame_counter = 0;

	/* register for state saving */
	state_save_register_global_pointer(machine, state->palettes[0], NUM_PENS);
	state_save_register_global_pointer(machine, state->palettes[1], NUM_PENS);
	state_save_register_global_pointer(machine, state->videoram, 0x20000 / 2);
	state_save_register_global(machine, state->videoram_read_buffer);
	state_save_register_global(machine, state->videoram_modulo);
	state_save_register_global(machine, state->videoram_offset);
	state_save_register_global(machine, state->fixed_layer_source);
	state_save_register_global(machine, state->screen_dark);
	state_save_register_global(machine, state->palette_bank);
	state_save_register_global(machine, state->auto_animation_speed);
	state_save_register_global(machine, state->auto_animation_disabled);
	state_save_register_global(machine, state->auto_animation_counter);
	state_save_register_global(machine, state->auto_animation_frame_counter);

	state_save_register_postload(machine, regenerate_pens, NULL);

	state->region_zoomy = memory_region(machine, "zoomy");
}


/*************************************
 *
 *  Video system reset
 *
 *************************************/

VIDEO_RESET( neogeo )
{
	start_sprite_line_timer(machine);
	start_auto_animation_timer(machine);
	optimize_sprite_data(machine);
}

/*************************************
 *
 *  Video update
 *
 *************************************/

VIDEO_UPDATE( neogeo )
{
	neogeo_state *state = screen->machine->driver_data<neogeo_state>();

	/* fill with background color first */
	bitmap_fill(bitmap, cliprect, state->pens[0x0fff]);

	draw_sprites(screen->machine, bitmap, cliprect->min_y);

	draw_fixed_layer(screen->machine, bitmap, cliprect->min_y);

	return 0;
}
