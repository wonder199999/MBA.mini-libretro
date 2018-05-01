/* Final Crash & other CPS1 bootlegs */

/*
Final Crash is a bootleg of Final Fight

Final Fight is by Capcom and runs on CPS1 hardware
The bootleg was manufactured by Playmark of Italy

this driver depends heavily on cps1.c, but has been
kept apart in an attempt to keep cps1.c clutter free

Sound is very different from CPS1.

---

Final Crash (bootleg of final fight)

1x 68k
1x z80
2x ym2203
2x oki5205
1x osc 10mhz
1x osc 24mhz

eproms:
1.bin sound eprom
from 2.bin to 9.bin program eproms
10.bin to 25.bin gfx eproms

---

kodb has various graphical issues, mainly with old info not being cleared away.
Also, it should be using a vblank irq value of 4. This triggers the following bootleg read/writes;
 - IN1 is read at 0x992000
 - IN0 is read of 0x992008
 - dips continue to be read at 0x80001a
 - sound command is wrote at 0x992006
 - scroll 1Y is wrote at 0x980000
 - scroll 1X is wrote at 0x980002
 - scroll 2Y is wrote at 0x980004
 - scroll 2X is wrote at 0x980006
 - scroll 3Y is wrote at 0x980008
 - scroll 3X is wrote at 0x98000a
 - the layer enable and layer mask writes continue at 0x98000c and 0x980020-2

These read/writes are identical to those used by a Knights of the Round bootleg which uses the all sf2mdt sound
hardware. This set is currently non-working.

This also prevents the game from toggling the sprite address at m_cps_a_regs[0], similar to other bootlegs.
Currently the game is working somewhat, but only using the code left over from the original. If anyone wants to
do any development work on the set, (eg, find the sprite clearing issue), then this should be changed as the game
likely won't write any sprite clearing values otherwise.

None of this is hooked up currently due to issues with row scroll on the scroll2 layer.
*/

#include "emu.h"
#include "cpu/z80/z80.h"
#include "cpu/m68000/m68000.h"
#include "sound/2203intf.h"
#include "sound/msm5205.h"
#include "sound/okim6295.h"
#include "sound/2151intf.h"

#include "includes/cps1.h"

//#include "cpu/pic16c5x/pic16c5x.h"
//#include "machine/eeprom.h"
//#include "sound/qsound.h"

#define CODE_SIZE 0x400000
#define CPS1_ROWSCROLL_OFFS	(0x20 / 2)    /* base of row scroll offsets in other RAM */


/* -------------- Functions ---------------- */
static WRITE16_HANDLER( fcrash_soundlatch_w )
{
	cps_state *state = (cps_state *)space->machine->driver_data;

	if (ACCESSING_BITS_0_7)
	{
		soundlatch_w(space, 0, data & 0xff);
		cpu_set_input_line(state->audiocpu, 0, HOLD_LINE);
	}
}

static WRITE16_HANDLER( sf2mdt_soundlatch_w )
{
	cps_state *state = (cps_state *)space->machine->driver_data;

	if (ACCESSING_BITS_8_15)
	{
		soundlatch_w(space, 0, data >> 8);
		cpu_set_input_line(state->audiocpu, 0, HOLD_LINE);
		/* cpuexec_boost_interleave( space->machine, attotime_zero, ATTOTIME_IN_USEC(50) ); */
	}
}

static WRITE8_HANDLER( fcrash_snd_bankswitch_w )
{
	cps_state *state = (cps_state *)space->machine->driver_data;

	sound_set_output_gain(state->msm_1, 0, (data & 0x08) ? 0.0 : 1.0);
	sound_set_output_gain(state->msm_2, 0, (data & 0x10) ? 0.0 : 1.0);

	memory_set_bank(space->machine, "bank1", data & 0x07);
}

static WRITE8_HANDLER( sf2mdt_snd_bankswitch_w )
{
	cps_state *state = (cps_state *)space->machine->driver_data;

	sound_set_output_gain(state->msm_1, 0, (data & 0x20) ? 0.0 : 1.0);
	sound_set_output_gain(state->msm_2, 0, (data & 0x10) ? 0.0 : 1.0);

	memory_set_bank(space->machine, "bank1", data & 0x07);
}

static WRITE8_HANDLER( knightsb_snd_bankswitch_w )
{
	cps_state *state = (cps_state *)space->machine->driver_data;

	sound_set_output_gain(state->msm_1, 0, (data & 0x20) ? 0.0 : 1.0);
	sound_set_output_gain(state->msm_2, 0, (data & 0x10) ? 0.0 : 1.0);

	memory_set_bank(space->machine, "bank1", data & 0x0f);
}

static void m5205_int1( running_device *device )
{
	cps_state *state = (cps_state *)device->machine->driver_data;

	msm5205_data_w(device, state->sample_buffer1 & 0x0f);
	state->sample_buffer1 >>= 4;
	state->sample_select1 ^= 1;

	if (state->sample_select1 == 0)
		cpu_set_input_line(state->audiocpu, INPUT_LINE_NMI, PULSE_LINE);
}

static void m5205_int2( running_device *device )
{
	cps_state *state = (cps_state *)device->machine->driver_data;

	msm5205_data_w(device, state->sample_buffer2 & 0x0f);
	state->sample_buffer2 >>= 4;
	state->sample_select2 ^= 1;
}

static WRITE8_HANDLER( fcrash_msm5205_0_data_w )
{
	cps_state *state = (cps_state *)space->machine->driver_data;
	state->sample_buffer1 = data;
}

static WRITE8_HANDLER( fcrash_msm5205_1_data_w )
{
	cps_state *state = (cps_state *)space->machine->driver_data;
	state->sample_buffer2 = data;
}

static const msm5205_interface msm5205_interface1 = { m5205_int1, MSM5205_S96_4B };
static const msm5205_interface msm5205_interface2 = { m5205_int2, MSM5205_S96_4B };
static const ym2151_interface ym2151_config = { cps1_irq_handler_mus };

static WRITE16_HANDLER( sf2mdta_layer_w )
{
	cps_state *state = (cps_state *)space->machine->driver_data;
	switch (offset)
	{
		case 0x06: state->cps_a_regs[0x0c / 2] = data + 0xffbe;	break;		  /* scroll 1x */
		case 0x07: state->cps_a_regs[0x0e / 2] = data; break;			  /* scroll 1y */
		case 0x08: state->cps_a_regs[0x14 / 2] = data + 0xffce;	break;		  /* scroll 3x */
		case 0x09: {
			state->cps_a_regs[0x12 / 2] = data;				  /* scroll 2y */
			state->cps_a_regs[CPS1_ROWSCROLL_OFFS] = data;			  /* row scroll start */
			break; }
		case 0x0a: state->cps_a_regs[0x10 / 2] = data + 0xffce;	break;		  /* scroll 2x */
		case 0x0b: state->cps_a_regs[0x16 / 2] = data;	break;			  /* scroll 3y */
		case 0x26: state->cps_b_regs[state->layer_enable_reg / 2] = data; break;
	}
}

static WRITE16_HANDLER( sf2mdt_layer_w )
{
	cps_state *state = (cps_state *)space->machine->driver_data;
	switch (offset)
	{
		case 0x06: state->cps_a_regs[0x14 / 2] = data + 0xffce; break;		/* scroll 3x */
		case 0x07: state->cps_a_regs[0x16 / 2] = data; break;			/* scroll 3y */
		case 0x08: state->cps_a_regs[0x10 / 2] = data + 0xffce;	break;		/* scroll 2x */
		case 0x09: state->cps_a_regs[0x0c / 2] = data + 0xffca;	break;		/* scroll 1x */
		case 0x0a: {
			state->cps_a_regs[0x12 / 2] = data;				/* scroll 2y */
			state->cps_a_regs[CPS1_ROWSCROLL_OFFS] = data;			/* row scroll start */
			break; }
		case 0x0b: state->cps_a_regs[0x0e / 2] = data; break;			/* scroll 1y */
		case 0x26: state->cps_b_regs[state->layer_enable_reg / 2] = data;
	}
}

static WRITE16_HANDLER( knightsb_layer_w )
{
	cps_state *state = (cps_state *)space->machine->driver_data;
	switch (offset)
	{
		case 0x00: state->cps_a_regs[0x0e / 2] = data; break;
		case 0x01: state->cps_a_regs[0x0c / 2] = data; break;
		case 0x02: {
			state->cps_a_regs[0x12 / 2] = data;
			state->cps_a_regs[CPS1_ROWSCROLL_OFFS] = data;
			break; }
		case 0x03: state->cps_a_regs[0x10 / 2] = data; break;
		case 0x04: state->cps_a_regs[0x16 / 2] = data; break;
		case 0x05: state->cps_a_regs[0x14 / 2] = data; break;
		case 0x06:
		{
			switch (data)
			{
				case 0x0000:
				case 0x001f:
				case 0x00ff:
					data = 0x12f2;
				break;
				case 0x2000:
					data = 0x06f2;
				break;
				case 0xa000:
					data = 0x24d0;
				break;
				default:
					printf ("Unknown control word = %X\n",data);
					data = 0x12c0;
			}
			state->cps_b_regs[state->layer_enable_reg / 2] = data;
			break;
		}
		case 0x10: state->cps_b_regs[state->layer_mask_reg[1] / 2] = data; break;
		case 0x11: state->cps_b_regs[state->layer_mask_reg[2] / 2] = data; break;
		case 0x12: state->cps_b_regs[state->layer_mask_reg[3] / 2] = data;
	}
}

static WRITE16_HANDLER( kodb_layer_w )
{
	cps_state *state = (cps_state *)space->machine->driver_data;

	switch (offset)
	{
		case 0x06: state->cps_b_regs[state->layer_enable_reg / 2] = data; break;
		case 0x10: state->cps_b_regs[state->layer_mask_reg[1] / 2] = data; break;
		case 0x11: state->cps_b_regs[state->layer_mask_reg[2] / 2] = data;
	}
}

/* ---  RENDER HANDLER  --- */
static void bootleg_update_transmasks( running_machine *machine )
{
	cps_state *state = (cps_state *)machine->driver_data;

	UINT32 i;
	for (i = 0; i < 4; i++)
	{
		INT32 mask = state->layer_mask_reg[i] ? (state->cps_b_regs[state->layer_mask_reg[i] / 2] ^ 0xffff) : 0xffff;

		tilemap_set_transmask(state->bg_tilemap[0], i, mask, 0x8000);
		tilemap_set_transmask(state->bg_tilemap[1], i, mask, 0x8000);
		tilemap_set_transmask(state->bg_tilemap[2], i, mask, 0x8000);
	}
}

static void bootleg_render_sprites( running_machine *machine, bitmap_t *bitmap, const rectangle *cliprect )
{
	cps_state *state = (cps_state *)machine->driver_data;

	INT32 pos, base = state->sprite_base / 2;
	INT32 last_sprite_offset = 0x1ffc;
	INT32 num_sprites = machine->gfx[2]->total_elements;
	UINT16 *sprite_ram = state->gfxram;
	UINT16 tileno, color, xpos, ypos, flipx, flipy;

	if (state->bootleg_sprite_ram)			/* if we have separate sprite ram, use it */
		sprite_ram = state->bootleg_sprite_ram;

	for ( pos = 0x1ffc - base; pos >= 0; pos -= 4)	/* get end of sprite list marker */
		if (sprite_ram[base + pos - 1] == state->sprite_list_end_marker) last_sprite_offset = pos;

	if ( ((base + last_sprite_offset) < 0x2000) || (!state->bootleg_sprite_ram) )	/* if we are using bootleg sprite ram, the index must be less than 0x2000 */
	{
		for (pos = last_sprite_offset; pos >= 0; pos -= 4)
		{
			tileno = sprite_ram[base + pos];
			if (tileno >= num_sprites) continue;
			xpos   = sprite_ram[base + pos + 2] & 0x01ff;
			ypos   = sprite_ram[base + pos - 1] & 0x01ff;
			flipx  = sprite_ram[base + pos + 1] & 0x20;
			flipy  = sprite_ram[base + pos + 1] & 0x40;
			color  = sprite_ram[base + pos + 1] & 0x1f;
			ypos   = 256 - 16 - ypos;
			xpos   += state->sprite_x_offset + 49;

			pdrawgfx_transpen(bitmap, cliprect, machine->gfx[2], tileno, color, flipx, flipy, xpos, ypos, machine->priority_bitmap, 0x02, 0x0f);
		}
	}
}

static void bootleg_render_layer( running_machine *machine, bitmap_t *bitmap, const rectangle *cliprect, int layer, int primask )
{
	cps_state *state = (cps_state *)machine->driver_data;

	switch (layer)
	{
		case 0:
			bootleg_render_sprites(machine, bitmap, cliprect);
		break;
		case 1:
		case 2:
		case 3:
			tilemap_draw(bitmap, cliprect, state->bg_tilemap[layer - 1], TILEMAP_DRAW_LAYER1, primask);
		break;
	}
}

static void bootleg_render_high_layer( running_machine *machine, bitmap_t *bitmap, const rectangle *cliprect, int layer )
{
	cps_state *state = (cps_state *)machine->driver_data;

	/* bitmap_t *dummy_bitmap = NULL; */
	switch (layer)
	{
		case 0:
		break;
		case 1:
		case 2:
		case 3:
			tilemap_draw(/*dummy_bitmap*/NULL, cliprect, state->bg_tilemap[layer - 1], TILEMAP_DRAW_LAYER0, 1);
		break;
	}
}

static void bootleg_build_palette( running_machine *machine )
{
	cps_state *state = (cps_state *)machine->driver_data;

	INT32 offset, palettebase = (state->cps_a_regs[0x0a / 2] << 8) & 0x1ffff;	/* all the bootlegs seem to write the palette offset as usual */

	for (offset = 0; offset < 32 * 6 * 16; offset++)
	{
		INT32 palette = state->gfxram[palettebase / 2 + offset];
		INT32 r, g, b, bright;

		/* from my understanding of the schematics, when the 'brightness' component is set to 0 it should reduce brightness to 1/3 */
		bright = 0x0f + ((palette >> 12) << 1);

		r = ((palette >> 8) & 0x0f) * 0x11 * bright / 0x2d;
		g = ((palette >> 4) & 0x0f) * 0x11 * bright / 0x2d;
		b = ((palette >> 0) & 0x0f) * 0x11 * bright / 0x2d;

		palette_set_color( machine, offset, MAKE_RGB(r, g, b) );
	}
}

static VIDEO_UPDATE( bootleg_updatescreen )
{
	cps_state *state = (cps_state *)screen->machine->driver_data;

	INT32 layercontrol = state->cps_b_regs[state->layer_enable_reg / 2];
	INT32 videocontrol = state->cps_a_regs[0x22 / 2];
	flip_screen_set(screen->machine, videocontrol & 0x8000);

	/* Get video memory base registers */
	cps1_get_video_base(screen->machine);
	/* Build palette */
	bootleg_build_palette(screen->machine);
	/* Update masks */
	bootleg_update_transmasks(screen->machine);

	tilemap_set_scrollx(state->bg_tilemap[0], 0, state->scroll1x - state->layer_scroll1x_offset);
	tilemap_set_scrolly(state->bg_tilemap[0], 0, state->scroll1y);

	if (videocontrol & 0x01)	/* linescroll enable */
	{
		UINT32 i;
		INT32 scrly = -state->scroll2y;
		tilemap_set_scroll_rows(state->bg_tilemap[1], 1024);
		INT32 otheroffs = state->cps_a_regs[CPS1_ROWSCROLL_OFFS];

		for (i = 0; i < 256; )
		{
			tilemap_set_scrollx(state->bg_tilemap[1], (i - scrly) & 0x03ff, state->scroll2x + state->other[(i + otheroffs) & 0x03ff]); i++;
			tilemap_set_scrollx(state->bg_tilemap[1], (i - scrly) & 0x03ff, state->scroll2x + state->other[(i + otheroffs) & 0x03ff]); i++;
			tilemap_set_scrollx(state->bg_tilemap[1], (i - scrly) & 0x03ff, state->scroll2x + state->other[(i + otheroffs) & 0x03ff]); i++;
			tilemap_set_scrollx(state->bg_tilemap[1], (i - scrly) & 0x03ff, state->scroll2x + state->other[(i + otheroffs) & 0x03ff]); i++;
		}
	}
	else
	{
		tilemap_set_scroll_rows(state->bg_tilemap[1], 1);
		tilemap_set_scrollx(state->bg_tilemap[1], 0, state->scroll2x - state->layer_scroll2x_offset);
	}
	tilemap_set_scrolly(state->bg_tilemap[1], 0, state->scroll2y);
	tilemap_set_scrollx(state->bg_tilemap[2], 0, state->scroll3x - state->layer_scroll3x_offset);
	tilemap_set_scrolly(state->bg_tilemap[2], 0, state->scroll3y);

	/* turn all tilemaps on regardless of settings in get_video_base() */
	/* write a custom get_video_base for this bootleg hardware? */
	tilemap_set_enable(state->bg_tilemap[0], 1);
	tilemap_set_enable(state->bg_tilemap[1], 1);
	tilemap_set_enable(state->bg_tilemap[2], 1);

	/* Blank screen */
	bitmap_fill(bitmap, cliprect, 0x0bff);
	bitmap_fill(screen->machine->priority_bitmap, cliprect, 0);

	INT32 l0 = (layercontrol >> 0x06) & 0x03;
	INT32 l1 = (layercontrol >> 0x08) & 0x03;
	INT32 l2 = (layercontrol >> 0x0a) & 0x03;
	INT32 l3 = (layercontrol >> 0x0c) & 0x03;

	bootleg_render_layer(screen->machine, bitmap, cliprect, l0, 0);

	if (l1 == 0) bootleg_render_high_layer(screen->machine, bitmap, cliprect, l0);
	bootleg_render_layer(screen->machine, bitmap, cliprect, l1, 0);

	if (l2 == 0) bootleg_render_high_layer(screen->machine, bitmap, cliprect, l1);
	bootleg_render_layer(screen->machine, bitmap, cliprect, l2, 0);

	if (l3 == 0) bootleg_render_high_layer(screen->machine, bitmap, cliprect, l2);
	bootleg_render_layer(screen->machine, bitmap, cliprect, l3, 0);

	return 0;
}


/* --- MEMADDRESS MAP --- */
static ADDRESS_MAP_START( knightsb_map, ADDRESS_SPACE_PROGRAM, 16 )
	AM_RANGE(0x000000, 0x3fffff) AM_ROM
	AM_RANGE(0x800000, 0x800001) AM_READ_PORT("IN1")
	AM_RANGE(0x800002, 0x800003) AM_READ_PORT("IN2")            	/* Player 3 controls */
	AM_RANGE(0x800004, 0x800005) AM_WRITENOP
	AM_RANGE(0x800006, 0x800007) AM_WRITE(fcrash_soundlatch_w)
	AM_RANGE(0x800018, 0x80001f) AM_READ(cps1_dsw_r)
	AM_RANGE(0x800030, 0x800037) AM_WRITENOP
	AM_RANGE(0x800100, 0x80013f) AM_RAM AM_BASE_MEMBER(cps_state, cps_a_regs)
	AM_RANGE(0x800140, 0x80017f) AM_RAM AM_BASE_MEMBER(cps_state, cps_b_regs)
	AM_RANGE(0x800180, 0x800181) AM_WRITENOP
	AM_RANGE(0x880000, 0x880001) AM_WRITENOP
	AM_RANGE(0x900000, 0x93ffff) AM_RAM_WRITE(cps1_gfxram_w) AM_BASE_SIZE_MEMBER(cps_state, gfxram, gfxram_size)
	AM_RANGE(0x980000, 0x98002f) AM_WRITE(knightsb_layer_w)
	AM_RANGE(0x990000, 0x990001) AM_WRITENOP
	AM_RANGE(0xff0000, 0xffffff) AM_RAM
ADDRESS_MAP_END

static ADDRESS_MAP_START( fcrash_map, ADDRESS_SPACE_PROGRAM, 16 )
	AM_RANGE(0x000000, 0x3fffff) AM_ROM
	AM_RANGE(0x800030, 0x800031) AM_WRITE(cps1_coinctrl_w)
	AM_RANGE(0x800100, 0x80013f) AM_RAM AM_BASE_MEMBER(cps_state, cps_a_regs)	/* CPS-A custom */
	AM_RANGE(0x800140, 0x80017f) AM_RAM AM_BASE_MEMBER(cps_state, cps_b_regs)	/* CPS-B custom */
	AM_RANGE(0x880000, 0x880001) AM_READ_PORT("IN1")				/* Player input ports */
	AM_RANGE(0x880006, 0x880007) AM_WRITE(fcrash_soundlatch_w)			/* Sound command */
	AM_RANGE(0x880008, 0x88000f) AM_READ(cps1_dsw_r)				/* System input ports / Dip Switches */
	AM_RANGE(0x890000, 0x890001) AM_WRITENOP					/* palette related? */
	AM_RANGE(0x900000, 0x92ffff) AM_RAM_WRITE(cps1_gfxram_w) AM_BASE_SIZE_MEMBER(cps_state, gfxram, gfxram_size)
	AM_RANGE(0xff0000, 0xffffff) AM_RAM
ADDRESS_MAP_END

static ADDRESS_MAP_START( sf2mdt_map, ADDRESS_SPACE_PROGRAM, 16 )
	AM_RANGE(0x000000, 0x3fffff) AM_ROM
	AM_RANGE(0x708100, 0x7081ff) AM_WRITE(sf2mdta_layer_w)
	AM_RANGE(0x70c000, 0x70c001) AM_READ_PORT("IN1")
	AM_RANGE(0x70c008, 0x70c009) AM_READ_PORT("IN2")
	AM_RANGE(0x70c018, 0x70c01f) AM_READ(cps1_hack_dsw_r)
	AM_RANGE(0x70c106, 0x70c107) AM_WRITE(sf2mdt_soundlatch_w)
	AM_RANGE(0x70d000, 0x70d001) AM_WRITENOP
	AM_RANGE(0x800100, 0x80013f) AM_RAM AM_BASE_MEMBER(cps_state, cps_a_regs)
	AM_RANGE(0x800140, 0x80017f) AM_RAM AM_BASE_MEMBER(cps_state, cps_b_regs)
	AM_RANGE(0x900000, 0x92ffff) AM_RAM_WRITE(cps1_gfxram_w) AM_BASE_SIZE_MEMBER(cps_state, gfxram, gfxram_size)
	AM_RANGE(0xff0000, 0xffffff) AM_RAM
ADDRESS_MAP_END

/* SOUND MAP */
static ADDRESS_MAP_START( sound_map, ADDRESS_SPACE_PROGRAM, 8 )
	AM_RANGE(0x0000, 0x7fff) AM_ROM
	AM_RANGE(0x8000, 0xbfff) AM_ROMBANK("bank1")
	AM_RANGE(0xd000, 0xd7ff) AM_RAM
	AM_RANGE(0xd800, 0xd801) AM_DEVREADWRITE("ym1", ym2203_r, ym2203_w)
	AM_RANGE(0xdc00, 0xdc01) AM_DEVREADWRITE("ym2", ym2203_r, ym2203_w)
	AM_RANGE(0xe000, 0xe000) AM_WRITE(fcrash_snd_bankswitch_w)
	AM_RANGE(0xe400, 0xe400) AM_READ(soundlatch_r)
	AM_RANGE(0xe800, 0xe800) AM_WRITE(fcrash_msm5205_0_data_w)
	AM_RANGE(0xec00, 0xec00) AM_WRITE(fcrash_msm5205_1_data_w)
ADDRESS_MAP_END

static ADDRESS_MAP_START( sf2mdt_z80map, ADDRESS_SPACE_PROGRAM, 8 )
	AM_RANGE(0x0000, 0x7fff) AM_ROM
	AM_RANGE(0x8000, 0xbfff) AM_ROMBANK("bank1")
	AM_RANGE(0xd000, 0xd7ff) AM_RAM
	AM_RANGE(0xd800, 0xd801) AM_DEVREADWRITE("2151", ym2151_r, ym2151_w)
	AM_RANGE(0xdc00, 0xdc00) AM_READ(soundlatch_r)
	AM_RANGE(0xe000, 0xe000) AM_WRITE(sf2mdt_snd_bankswitch_w)
	AM_RANGE(0xe400, 0xe400) AM_WRITE(fcrash_msm5205_0_data_w)
	AM_RANGE(0xe800, 0xe800) AM_WRITE(fcrash_msm5205_1_data_w)
ADDRESS_MAP_END

static ADDRESS_MAP_START( knightsb_z80map, ADDRESS_SPACE_PROGRAM, 8 )
	AM_RANGE(0x0000, 0x7fff) AM_ROM
	AM_RANGE(0x8000, 0xbfff) AM_ROMBANK("bank1")
	AM_RANGE(0xcffe, 0xcfff) AM_WRITENOP
	AM_RANGE(0xd000, 0xd7ff) AM_RAM
	AM_RANGE(0xd800, 0xd801) AM_DEVREADWRITE("2151", ym2151_r, ym2151_w)
	AM_RANGE(0xdc00, 0xdc00) AM_READ(soundlatch_r)
	AM_RANGE(0xe000, 0xe000) AM_WRITE(knightsb_snd_bankswitch_w)
	AM_RANGE(0xe400, 0xe400) AM_WRITE(fcrash_msm5205_0_data_w)
	AM_RANGE(0xe800, 0xe800) AM_WRITE(fcrash_msm5205_1_data_w)
ADDRESS_MAP_END

static ADDRESS_MAP_START( kodb_z80map, ADDRESS_SPACE_PROGRAM, 8 )
	AM_RANGE(0x0000, 0x7fff) AM_ROM
	AM_RANGE(0x8000, 0xbfff) AM_ROMBANK("bank1")
	AM_RANGE(0xd000, 0xd7ff) AM_RAM
	AM_RANGE(0xe000, 0xe001) AM_DEVREADWRITE("2151", ym2151_r, ym2151_w)
	AM_RANGE(0xe400, 0xe400) AM_DEVREADWRITE("oki", okim6295_r, okim6295_w)
	AM_RANGE(0xe800, 0xe800) AM_READ(soundlatch_r)
ADDRESS_MAP_END


/* --- INPUT PORTS --- */
#define CPS1_COINAGE_1 \
	PORT_DIPNAME( 0x07, 0x07, DEF_STR( Coin_A ) ) \
	PORT_DIPSETTING(    0x00, DEF_STR( 4C_1C ) ) \
	PORT_DIPSETTING(    0x01, DEF_STR( 3C_1C ) ) \
	PORT_DIPSETTING(    0x02, DEF_STR( 2C_1C ) ) \
	PORT_DIPSETTING(    0x07, DEF_STR( 1C_1C ) ) \
	PORT_DIPSETTING(    0x06, DEF_STR( 1C_2C ) ) \
	PORT_DIPSETTING(    0x05, DEF_STR( 1C_3C ) ) \
	PORT_DIPSETTING(    0x04, DEF_STR( 1C_4C ) ) \
	PORT_DIPSETTING(    0x03, DEF_STR( 1C_6C ) ) \
	PORT_DIPNAME( 0x38, 0x38, DEF_STR( Coin_B ) ) \
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


static INPUT_PORTS_START( sf2hack )		/* for sf2/sf2ce hackrom */
	PORT_INCLUDE( cps1_3b )

	PORT_MODIFY("IN0")
	PORT_SERVICE_NO_TOGGLE( 0x40, IP_ACTIVE_LOW )

	PORT_MODIFY("IN1")
	PORT_BIT( 0x0010, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME( "P1 Jab Punch" ) PORT_PLAYER(1)
	PORT_BIT( 0x0020, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME( "P1 Strong Punch" ) PORT_PLAYER(1)
	PORT_BIT( 0x0040, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME( "P1 Fierce Punch" ) PORT_PLAYER(1)
	PORT_BIT( 0x1000, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME( "P2 Jab Punch" ) PORT_PLAYER(2)
	PORT_BIT( 0x2000, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME( "P2 Strong Punch" ) PORT_PLAYER(2)
	PORT_BIT( 0x4000, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME( "P2 Fierce Punch" ) PORT_PLAYER(2)

	PORT_START("IN2")
	PORT_BIT( 0x0001, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME( "P1 Short Kick" ) PORT_PLAYER(1)
	PORT_BIT( 0x0002, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME( "P1 Forward Kick" ) PORT_PLAYER(1)
	PORT_BIT( 0x0004, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME( "P1 Roundhouse Kick" ) PORT_PLAYER(1)
	PORT_BIT( 0x0008, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x0010, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME( "P2 Short Kick" ) PORT_PLAYER(2)
	PORT_BIT( 0x0020, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME( "P2 Forward Kick" ) PORT_PLAYER(2)
	PORT_BIT( 0x0040, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME( "P2 Roundhouse Kick" ) PORT_PLAYER(2)
	PORT_BIT( 0x0080, IP_ACTIVE_LOW, IPT_UNKNOWN )

	PORT_START("DSWA")
	CPS1_COINAGE_1
	PORT_DIPNAME( 0x40, 0x40, "2 Coins to Start, 1 to Continue" ) PORT_DIPLOCATION( "SW(A):7" )
	PORT_DIPSETTING(    0x40, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPUNUSED_DIPLOC( 0x80, 0x80, "SW(A):8" )

	PORT_START("DSWB")
	CPS1_DIFFICULTY_1( "SW(B)" )
	PORT_DIPUNUSED_DIPLOC( 0x08, 0x08, "SW(B):4" )
	PORT_DIPUNUSED_DIPLOC( 0x10, 0x10, "SW(B):5" )
	PORT_DIPUNUSED_DIPLOC( 0x20, 0x20, "SW(B):6" )
	PORT_DIPUNUSED_DIPLOC( 0x40, 0x40, "SW(B):7" )
	PORT_DIPUNUSED_DIPLOC( 0x80, 0x80, "SW(B):8" )

	PORT_START("DSWC")
	PORT_DIPUNUSED_DIPLOC( 0x01, 0x01, "SW(C):1" )
	PORT_DIPUNUSED_DIPLOC( 0x02, 0x02, "SW(C):2" )
	PORT_DIPNAME( 0x04, 0x04, DEF_STR( Free_Play ) )	PORT_DIPLOCATION("SW(C):3")
	PORT_DIPSETTING(    0x04, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x08, 0x08, "Freeze" )			PORT_DIPLOCATION("SW(C):4")
	PORT_DIPSETTING(    0x08, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x10, 0x10, DEF_STR( Flip_Screen ) )	PORT_DIPLOCATION("SW(C):5")
	PORT_DIPSETTING(    0x10, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x20, 0x00, DEF_STR( Demo_Sounds ) )	PORT_DIPLOCATION("SW(C):6")
	PORT_DIPSETTING(    0x20, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x40, 0x00, DEF_STR( Allow_Continue ) )	PORT_DIPLOCATION("SW(C):7")
	PORT_DIPSETTING(    0x40, DEF_STR( No ) )
	PORT_DIPSETTING(    0x00, DEF_STR( Yes ) )
	PORT_DIPNAME( 0x80, 0x80, "Game Mode")			PORT_DIPLOCATION("SW(C):8")
	PORT_DIPSETTING(    0x80, "Game" )
	PORT_DIPSETTING(    0x00, DEF_STR( Test ) )
INPUT_PORTS_END

static INPUT_PORTS_START( fcrash )
	PORT_INCLUDE( cps1_3b )

	PORT_MODIFY("IN1")
	PORT_BIT( 0x0040, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_PLAYER(1) PORT_NAME ("P1 Button 3 (Cheat)")
	PORT_BIT( 0x4000, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_PLAYER(2) PORT_NAME ("P2 Button 3 (Cheat)")

	PORT_START("DSWA")
	CPS1_COINAGE_1
	PORT_DIPNAME( 0x40, 0x40, "2 Coins to Start, 1 to Continue" )
	PORT_DIPSETTING(    0x40, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPUNUSED( 0x80, IP_ACTIVE_LOW )

	PORT_START("DSWB")
	PORT_DIPNAME( 0x07, 0x04, "Difficulty Level 1" )
	PORT_DIPSETTING(    0x07, DEF_STR( Easiest ) )
	PORT_DIPSETTING(    0x06, DEF_STR( Easier ) )
	PORT_DIPSETTING(    0x05, DEF_STR( Easy ) )
	PORT_DIPSETTING(    0x04, DEF_STR( Normal ) )
	PORT_DIPSETTING(    0x03, DEF_STR( Medium ) )
	PORT_DIPSETTING(    0x02, DEF_STR( Hard ) )
	PORT_DIPSETTING(    0x01, DEF_STR( Harder ) )
	PORT_DIPSETTING(    0x00, DEF_STR( Hardest ) )
	PORT_DIPNAME( 0x18, 0x10, "Difficulty Level 2" )
	PORT_DIPSETTING(    0x18, DEF_STR( Easy ) )
	PORT_DIPSETTING(    0x10, DEF_STR( Normal ) )
	PORT_DIPSETTING(    0x08, DEF_STR( Hard ) )
	PORT_DIPSETTING(    0x00, DEF_STR( Hardest ) )
	PORT_DIPNAME( 0x60, 0x60, DEF_STR( Bonus_Life ) )
	PORT_DIPSETTING(    0x60, "100k" )
	PORT_DIPSETTING(    0x40, "200k" )
	PORT_DIPSETTING(    0x20, "100k and every 200k" )
	PORT_DIPSETTING(    0x00, DEF_STR( None ) )
	PORT_DIPUNUSED( 0x80, IP_ACTIVE_LOW )

	PORT_START("DSWC")
	PORT_DIPNAME( 0x03, 0x03, DEF_STR( Lives ) )
	PORT_DIPSETTING(    0x00, "1" )
	PORT_DIPSETTING(    0x03, "2" )
	PORT_DIPSETTING(    0x02, "3" )
	PORT_DIPSETTING(    0x01, "4" )
	PORT_DIPNAME( 0x04, 0x04, DEF_STR( Free_Play ) )
	PORT_DIPSETTING(    0x04, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x08, 0x08, "Freeze" )
	PORT_DIPSETTING(    0x08, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x10, 0x10, DEF_STR( Flip_Screen ) )
	PORT_DIPSETTING(    0x10, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x20, 0x00, DEF_STR( Demo_Sounds ) )
	PORT_DIPSETTING(    0x20, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x40, 0x00, DEF_STR( Allow_Continue ) )
	PORT_DIPSETTING(    0x40, DEF_STR( No ) )
	PORT_DIPSETTING(    0x00, DEF_STR( Yes ) )
	PORT_DIPNAME( 0x80, 0x80, "Game Mode")
	PORT_DIPSETTING(    0x80, "Game" )
	PORT_DIPSETTING(    0x00, DEF_STR( Test ) )
INPUT_PORTS_END

static INPUT_PORTS_START( kodb )
	PORT_INCLUDE( cps1_3players )

	PORT_MODIFY("IN0")
	PORT_SERVICE_NO_TOGGLE( 0x40, IP_ACTIVE_LOW )

	PORT_START("DSWA")
	CPS1_COINAGE_2( "SW(A)" )
	PORT_DIPNAME( 0x08, 0x08, "Coin Slots" )	PORT_DIPLOCATION("SW(A):4")
	PORT_DIPSETTING(    0x00, "1" )
	PORT_DIPSETTING(    0x08, "3" )
	PORT_DIPNAME( 0x10, 0x10, "Play Mode" )		PORT_DIPLOCATION("SW(A):5")
	PORT_DIPSETTING(    0x00, "2 Players" )
	PORT_DIPSETTING(    0x10, "3 Players" )
	PORT_DIPUNUSED_DIPLOC( 0x20, 0x20, "SW(A):6" )
	PORT_DIPNAME( 0x40, 0x40, "2 Coins to Start, 1 to Continue" )	PORT_DIPLOCATION("SW(A):7")
	PORT_DIPSETTING(    0x40, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPUNUSED_DIPLOC( 0x80, 0x80, "SW(A):8" )

	PORT_START("DSWB")
	CPS1_DIFFICULTY_1( "SW(B)" )
	PORT_DIPNAME( 0x38, 0x38, DEF_STR( Lives ) )	PORT_DIPLOCATION("SW(B):4,5,6")
	PORT_DIPSETTING(    0x30, "1" )
	PORT_DIPSETTING(    0x38, "2" )
	PORT_DIPSETTING(    0x28, "3" )
	PORT_DIPSETTING(    0x20, "4" )
	PORT_DIPSETTING(    0x18, "5" )
	PORT_DIPSETTING(    0x10, "6" )
	PORT_DIPSETTING(    0x08, "7" )
	PORT_DIPSETTING(    0x00, "8" )
	PORT_DIPNAME( 0xc0, 0xc0, DEF_STR( Bonus_Life ) )	PORT_DIPLOCATION("SW(B):7,8")
	PORT_DIPSETTING(    0x80, "80k and every 400k" )
	PORT_DIPSETTING(    0xc0, "100k and every 450k" )
	PORT_DIPSETTING(    0x40, "160k and every 450k" )
	PORT_DIPSETTING(    0x00, DEF_STR( None ) )

	PORT_START("DSWC")
	PORT_DIPUNUSED_DIPLOC( 0x01, 0x01, "SW(C):1" )
	PORT_DIPUNUSED_DIPLOC( 0x02, 0x02, "SW(C):2" )
	PORT_DIPNAME( 0x04, 0x04, DEF_STR( Free_Play ) )	PORT_DIPLOCATION("SW(C):3")
	PORT_DIPSETTING(    0x04, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x08, 0x08, "Freeze" )		PORT_DIPLOCATION("SW(C):4")
	PORT_DIPSETTING(    0x08, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x10, 0x10, DEF_STR( Flip_Screen ) )	PORT_DIPLOCATION("SW(C):5")
	PORT_DIPSETTING(    0x10, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x20, 0x00, DEF_STR( Demo_Sounds ) )	PORT_DIPLOCATION("SW(C):6")
	PORT_DIPSETTING(    0x20, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x40, 0x00, DEF_STR( Allow_Continue ) )	PORT_DIPLOCATION("SW(C):7")
	PORT_DIPSETTING(    0x40, DEF_STR( No ) )
	PORT_DIPSETTING(    0x00, DEF_STR( Yes ) )
	PORT_DIPNAME( 0x80, 0x80, "Game Mode")		PORT_DIPLOCATION("SW(C):8")
	PORT_DIPSETTING(    0x80, "Game" )
	PORT_DIPSETTING(    0x00, DEF_STR( Test ) )
INPUT_PORTS_END


/* --- MACHINE DRIVER --- */
static MACHINE_START( fcrash )
{
	cps_state *state = (cps_state *)machine->driver_data;

	state->maincpu = machine->device("maincpu");
	state->audiocpu = machine->device("audiocpu");
	state->msm_1 = machine->device("msm1");
	state->msm_2 = machine->device("msm2");

	UINT8 *src = memory_region(machine, "audiocpu");

	memory_configure_bank(machine, "bank1", 0, 8, &src[0x10000], 0x4000);

	state->layer_enable_reg = 0x20;
	state->layer_mask_reg[0] = 0x26;
	state->layer_mask_reg[1] = 0x30;
	state->layer_mask_reg[2] = 0x28;
	state->layer_mask_reg[3] = 0x32;
	state->layer_scroll1x_offset = 0x3e;
	state->layer_scroll2x_offset = 0x3c;
	state->layer_scroll3x_offset = 0x40;
	state->sprite_base = 0x50c8;
	state->sprite_list_end_marker = 0x8000;
	state->sprite_x_offset = 0x00;

	state_save_register_global(machine, state->sample_buffer1);
	state_save_register_global(machine, state->sample_buffer2);
	state_save_register_global(machine, state->sample_select1);
	state_save_register_global(machine, state->sample_select2);
}

static MACHINE_RESET( fcrash )
{
	cps_state *state = (cps_state *)machine->driver_data;

	state->sample_buffer1 = 0;
	state->sample_buffer2 = 0;
	state->sample_select1 = 0;
	state->sample_select2 = 0;
}

static MACHINE_START( sf2mdt )
{
	cps_state *state = (cps_state *)machine->driver_data;

	state->maincpu = machine->device("maincpu");
	state->audiocpu = machine->device("audiocpu");
	state->msm_1 = machine->device("msm1");
	state->msm_2 = machine->device("msm2");

	UINT8 *src = memory_region(machine, "audiocpu");

	memory_configure_bank(machine, "bank1", 0, 8, &src[0x10000], 0x4000);

	state->layer_enable_reg = 0x26;
	state->layer_mask_reg[0] = 0x28;
	state->layer_mask_reg[1] = 0x2a;
	state->layer_mask_reg[2] = 0x2c;
	state->layer_mask_reg[3] = 0x2e;
	state->layer_scroll1x_offset = 0x00;
	state->layer_scroll2x_offset = 0x00;
	state->layer_scroll3x_offset = 0x00;
	state->sprite_base = 0x1000;
	state->sprite_list_end_marker = 0x8000;
	state->sprite_x_offset = 0x02;

	state_save_register_global(machine, state->sample_buffer1);
	state_save_register_global(machine, state->sample_buffer2);
	state_save_register_global(machine, state->sample_select1);
	state_save_register_global(machine, state->sample_select2);
}

static MACHINE_START( knightsb )
{
	cps_state *state = (cps_state *)machine->driver_data;

	state->maincpu = machine->device("maincpu");
	state->audiocpu = machine->device("audiocpu");
	state->msm_1 = machine->device("msm1");
	state->msm_2 = machine->device("msm2");

	UINT8 *src = memory_region(machine, "audiocpu");

	memory_configure_bank(machine, "bank1", 0, 16, &src[0x10000], 0x4000);

	state->layer_enable_reg = 0x30;
	state->layer_mask_reg[0] = 0x28;
	state->layer_mask_reg[1] = 0x2a;
	state->layer_mask_reg[2] = 0x2c;
	state->layer_mask_reg[3] = 0x2e;
	state->layer_scroll1x_offset = 0x3e;
	state->layer_scroll2x_offset = 0x3c;
	state->layer_scroll3x_offset = 0x40;
	state->sprite_base = 0x1000;
	state->sprite_list_end_marker = 0x8000;
	state->sprite_x_offset = 0x00;
}

static MACHINE_START( kodb )
{
	cps_state *state = (cps_state *)machine->driver_data;

	state->maincpu = machine->device("maincpu");
	state->audiocpu = machine->device("audiocpu");

	state->layer_enable_reg = 0x20;
	state->layer_mask_reg[0] = 0x2e;
	state->layer_mask_reg[1] = 0x2c;
	state->layer_mask_reg[2] = 0x2a;
	state->layer_mask_reg[3] = 0x28;
	state->layer_scroll1x_offset = 0x00;
	state->layer_scroll2x_offset = 0x00;
	state->layer_scroll3x_offset = 0x00;
	state->sprite_base = 0x1000;
	state->sprite_list_end_marker = 0xffff;
	state->sprite_x_offset = 0x00;
}


static MACHINE_DRIVER_START( fcrash )
	/* driver data */
	MDRV_DRIVER_DATA(cps_state)

	/* basic machine hardware */
	MDRV_CPU_ADD("maincpu", M68000, 10000000)
	MDRV_CPU_PROGRAM_MAP(fcrash_map)
	MDRV_CPU_VBLANK_INT("screen", cps1_interrupt)
	MDRV_CPU_ADD("audiocpu", Z80, 24000000 / 6)	 /* ? */
	MDRV_CPU_PROGRAM_MAP(sound_map)
	MDRV_MACHINE_START(fcrash)
	MDRV_MACHINE_RESET(fcrash)

	/* video hardware */
	MDRV_SCREEN_ADD("screen", RASTER)
	MDRV_SCREEN_REFRESH_RATE(60)
	MDRV_SCREEN_VBLANK_TIME(ATTOSECONDS_IN_USEC(0))
	MDRV_SCREEN_FORMAT(BITMAP_FORMAT_INDEXED16)
	MDRV_SCREEN_SIZE(64*8, 32*8)
	MDRV_SCREEN_VISIBLE_AREA( 8*8, (64-8)*8-1, 2*8, 30*8-1 )
	MDRV_VIDEO_UPDATE(bootleg_updatescreen)
	MDRV_VIDEO_EOF(cps1)
	MDRV_GFXDECODE(cps1)
	MDRV_PALETTE_LENGTH(0xc00)
	MDRV_VIDEO_START(cps1)

	// sound hardware
	MDRV_SPEAKER_STANDARD_MONO("mono")
	MDRV_SOUND_ADD("ym1", YM2203, 24000000 / 6)	/* ? */
	MDRV_SOUND_ROUTE(0, "mono", 0.10)
	MDRV_SOUND_ROUTE(1, "mono", 0.10)
	MDRV_SOUND_ROUTE(2, "mono", 0.10)
	MDRV_SOUND_ROUTE(3, "mono", 1.0)
	MDRV_SOUND_ADD("ym2", YM2203, 24000000 / 6)	/* ? */
	MDRV_SOUND_ROUTE(0, "mono", 0.10)
	MDRV_SOUND_ROUTE(1, "mono", 0.10)
	MDRV_SOUND_ROUTE(2, "mono", 0.10)
	MDRV_SOUND_ROUTE(3, "mono", 1.0)
	MDRV_SOUND_ADD("msm1", MSM5205, 24000000 / 64)	/* ? */
	MDRV_SOUND_CONFIG(msm5205_interface1)
	MDRV_SOUND_ROUTE(ALL_OUTPUTS, "mono", 0.25)
	MDRV_SOUND_ADD("msm2", MSM5205, 24000000 / 64)	/* ? */
	MDRV_SOUND_CONFIG(msm5205_interface2)
	MDRV_SOUND_ROUTE(ALL_OUTPUTS, "mono", 0.25)
MACHINE_DRIVER_END

static MACHINE_DRIVER_START( kodb )
	/* driver data */
	MDRV_DRIVER_DATA(cps_state)

	/* basic machine hardware */
	MDRV_CPU_ADD("maincpu", M68000, 10000000)
	MDRV_CPU_PROGRAM_MAP(fcrash_map)
	MDRV_CPU_VBLANK_INT("screen", cps1_interrupt)
	MDRV_CPU_ADD("audiocpu", Z80, 3579545)
	MDRV_CPU_PROGRAM_MAP(kodb_z80map)
	MDRV_MACHINE_START(kodb)

	/* video hardware */
	MDRV_SCREEN_ADD("screen", RASTER)
	MDRV_SCREEN_REFRESH_RATE(60)
	MDRV_SCREEN_VBLANK_TIME(ATTOSECONDS_IN_USEC(0))
	MDRV_SCREEN_FORMAT(BITMAP_FORMAT_INDEXED16)
	MDRV_SCREEN_SIZE(64*8, 32*8)
	MDRV_SCREEN_VISIBLE_AREA(8*8, (64-8)*8-1, 2*8, 30*8-1 )
	MDRV_VIDEO_UPDATE(bootleg_updatescreen)
	MDRV_VIDEO_EOF(cps1)
	MDRV_GFXDECODE(cps1)
	MDRV_PALETTE_LENGTH(0xc00)
	MDRV_VIDEO_START(cps1)

	/* sound hardware */
	MDRV_SPEAKER_STANDARD_MONO("mono")
	MDRV_SOUND_ADD("2151", YM2151, 3579545)
	MDRV_SOUND_CONFIG(ym2151_config)
	MDRV_SOUND_ROUTE(0, "mono", 0.35)
	MDRV_SOUND_ROUTE(1, "mono", 0.35)
	MDRV_OKIM6295_ADD("oki", XTAL_16MHz / 4 / 4, OKIM6295_PIN7_HIGH)
	MDRV_SOUND_ROUTE(ALL_OUTPUTS, "mono", 0.30)
MACHINE_DRIVER_END

static MACHINE_DRIVER_START( sf2mdt )
	MDRV_DRIVER_DATA(cps_state)

	/* basic machine hardware */
	MDRV_CPU_ADD("maincpu", M68000, 12000000)
	MDRV_CPU_PROGRAM_MAP(sf2mdt_map)
	MDRV_CPU_VBLANK_INT("screen", irq4_line_hold)
	MDRV_CPU_ADD("audiocpu", Z80, 3579545)
	MDRV_CPU_PROGRAM_MAP(sf2mdt_z80map)
	MDRV_MACHINE_START(sf2mdt)
	MDRV_MACHINE_RESET(fcrash)

	/* video hardware */
	MDRV_SCREEN_ADD("screen", RASTER)
	MDRV_SCREEN_REFRESH_RATE(60)
	MDRV_SCREEN_VBLANK_TIME(ATTOSECONDS_IN_USEC(0))
	MDRV_SCREEN_FORMAT(BITMAP_FORMAT_INDEXED16)
	MDRV_SCREEN_SIZE(64*8, 32*8)
	MDRV_SCREEN_VISIBLE_AREA(8*8, (64-8)*8-1, 2*8, 30*8-1 )
	MDRV_VIDEO_UPDATE(bootleg_updatescreen)
	MDRV_VIDEO_EOF(cps1)
	MDRV_GFXDECODE(cps1)
	MDRV_PALETTE_LENGTH(4096)
	MDRV_VIDEO_START(cps1)

	/* sound hardware */
	MDRV_SPEAKER_STANDARD_MONO("mono")
	MDRV_SOUND_ADD("2151", YM2151, 3579545)
	MDRV_SOUND_ROUTE(0, "mono", 0.35)
	MDRV_SOUND_ROUTE(1, "mono", 0.35)
	MDRV_SOUND_ADD("msm1", MSM5205, 24000000/64)
	MDRV_SOUND_CONFIG(msm5205_interface1)
	MDRV_SOUND_ROUTE(ALL_OUTPUTS, "mono", 0.25)
	MDRV_SOUND_ADD("msm2", MSM5205, 24000000/64)
	MDRV_SOUND_CONFIG(msm5205_interface2)
	MDRV_SOUND_ROUTE(ALL_OUTPUTS, "mono", 0.25)
MACHINE_DRIVER_END

static MACHINE_DRIVER_START( knightsb )
	/* driver data */
	MDRV_DRIVER_DATA(cps_state)

	/* basic machine hardware */
	MDRV_CPU_ADD("maincpu", M68000, 12000000)
	MDRV_CPU_PROGRAM_MAP(knightsb_map)
	MDRV_CPU_VBLANK_INT("screen", cps1_interrupt)
	MDRV_CPU_ADD("audiocpu", Z80, 29821000 / 8)
	MDRV_CPU_PROGRAM_MAP(knightsb_z80map)
	MDRV_MACHINE_START(knightsb)

	/* video hardware */
	MDRV_SCREEN_ADD("screen", RASTER)
	MDRV_SCREEN_REFRESH_RATE(60)
	MDRV_SCREEN_VBLANK_TIME(ATTOSECONDS_IN_USEC(0))
	MDRV_SCREEN_FORMAT(BITMAP_FORMAT_INDEXED16)
	MDRV_SCREEN_SIZE(64*8, 32*8)
	MDRV_SCREEN_VISIBLE_AREA( 8*8, (64-8)*8-1, 2*8, 30*8-1 )
	MDRV_VIDEO_UPDATE(bootleg_updatescreen)
	MDRV_VIDEO_EOF(cps1)
	MDRV_GFXDECODE(cps1)
	MDRV_PALETTE_LENGTH(0xc00)
	MDRV_VIDEO_START(cps1)

	/* sound hardware */
	MDRV_SPEAKER_STANDARD_MONO("mono")
	MDRV_SOUND_ADD("2151", YM2151, 29821000 / 8)
	MDRV_SOUND_CONFIG(ym2151_config)
	MDRV_SOUND_ROUTE(0, "mono", 0.35)
	MDRV_SOUND_ROUTE(1, "mono", 0.35)
	MDRV_SOUND_ADD("msm1", MSM5205, 24000000 / 64)	/* ? */
	MDRV_SOUND_CONFIG(msm5205_interface1)
	MDRV_SOUND_ROUTE(ALL_OUTPUTS, "mono", 0.25)
	MDRV_SOUND_ADD("msm2", MSM5205, 24000000 / 64)	/* ? */
	MDRV_SOUND_CONFIG(msm5205_interface2)
	MDRV_SOUND_ROUTE(ALL_OUTPUTS, "mono", 0.25)
MACHINE_DRIVER_END


/* --- DRIVER INIT --- */
static DRIVER_INIT( sf2mdta )
{
	cps_state *state = (cps_state *)machine->driver_data;

	state->bootleg_sprite_ram = (UINT16 *)memory_install_ram(cputag_get_address_space(machine, "maincpu", ADDRESS_SPACE_PROGRAM), 0x700000, 0x703fff, 0, 0, NULL);
	memory_install_ram(cputag_get_address_space(machine, "maincpu", ADDRESS_SPACE_PROGRAM), 0x704000, 0x707fff, 0, 0, state->bootleg_sprite_ram);
	state->bootleg_work_ram = (UINT16 *)memory_install_ram(cputag_get_address_space(machine, "maincpu", ADDRESS_SPACE_PROGRAM), 0xfc0000, 0xfcffff, 0, 0, NULL);

	DRIVER_INIT_CALL(cps1);
}

static DRIVER_INIT( sf2mdt )
{
	memory_install_write16_handler(cputag_get_address_space(machine, "maincpu", ADDRESS_SPACE_PROGRAM), 0x708100, 0x7081ff, 0, 0, sf2mdt_layer_w);

	DRIVER_INIT_CALL(sf2mdta);
}

static DRIVER_INIT( kodb )
{
	cps_state *state = (cps_state *)machine->driver_data;

	memory_install_read_port(cputag_get_address_space(machine, "maincpu", ADDRESS_SPACE_PROGRAM), 0x800000, 0x800007, 0, 0, "IN1");
	memory_install_read16_handler(cputag_get_address_space(machine, "maincpu", ADDRESS_SPACE_PROGRAM), 0x800018, 0x80001f, 0, 0, cps1_dsw_r);
	memory_install_write16_handler(cputag_get_address_space(machine, "maincpu", ADDRESS_SPACE_PROGRAM), 0x800180, 0x800187, 0, 0, cps1_soundlatch_w);
	memory_install_write16_handler(cputag_get_address_space(machine, "maincpu", ADDRESS_SPACE_PROGRAM), 0x980000, 0x98002f, 0, 0, kodb_layer_w);
	state->bootleg_sprite_ram = (UINT16 *)memory_install_ram(cputag_get_address_space(machine, "maincpu", ADDRESS_SPACE_PROGRAM), 0x900000, 0x903fff, 0, 0, NULL);
	memory_install_ram(cputag_get_address_space(machine, "maincpu", ADDRESS_SPACE_PROGRAM), 0x904000, 0x907fff, 0, 0, state->bootleg_sprite_ram);

	DRIVER_INIT_CALL(cps1);
}

static DRIVER_INIT( knightsb )
{
	cps_state *state = (cps_state *)machine->driver_data;

	state->bootleg_sprite_ram = (UINT16 *)memory_install_ram(cputag_get_address_space(machine, "maincpu", ADDRESS_SPACE_PROGRAM), 0x990000, 0x993fff, 0, 0, NULL);

	DRIVER_INIT_CALL(cps1);
}


/* --- LOAD ROM --- */
ROM_START( fcrash )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )		/* 68000 code */
	ROM_LOAD16_BYTE( "9.bin",  0x00000, 0x20000, CRC(c6854c91) SHA1(29f01cc65be5eaa3f86e99eebdd284104623abb0) )
	ROM_LOAD16_BYTE( "5.bin",  0x00001, 0x20000, CRC(77f7c2b3) SHA1(feea48d9555824a2e5bf5e99ce159edc015f0792) )
	ROM_LOAD16_BYTE( "8.bin",  0x40000, 0x20000, CRC(1895b3df) SHA1(415a26050c50ed79a7ee5ddd1b8d61593b1ce876) )
	ROM_LOAD16_BYTE( "4.bin",  0x40001, 0x20000, CRC(bbd411ee) SHA1(85d50ca72ec46d627f9c88ff0809aa30e164821a) )
	ROM_LOAD16_BYTE( "7.bin",  0x80000, 0x20000, CRC(5b23ebf2) SHA1(8c28c21a72a28ad249170026891c6bb865943f84) )
	ROM_LOAD16_BYTE( "3.bin",  0x80001, 0x20000, CRC(aba2aebe) SHA1(294109b5929ed63859a55bef16643e3ade7da16f) )
	ROM_LOAD16_BYTE( "6.bin",  0xc0000, 0x20000, CRC(d4bf37f6) SHA1(f47e1cc9aa3b3019ee57f59715e3a611acf9fe3e) )
	ROM_LOAD16_BYTE( "2.bin",  0xc0001, 0x20000, CRC(07ac8f43) SHA1(7a41b003c76adaabd3f94929cc163461b70e0ed9) )

	ROM_REGION( 0x30000, "audiocpu", 0 )		/* Audio CPU + Sample Data */
	ROM_LOAD( "1.bin",   0x00000, 0x20000, CRC(5b276c14) SHA1(73e53c077d4e3c1b919eee28b29e34176ee204f8) )
	ROM_RELOAD(          0x10000, 0x20000 )

	ROM_REGION( 0x200000, "gfx", 0 )
	ROMX_LOAD( "18.bin",     0x000000, 0x20000, CRC(f1eee6d9) SHA1(bee95efbff49c582cff1cc6d9bb5ef4ea5c4a074) , ROM_SKIP(3) )
	ROMX_LOAD( "20.bin",     0x000001, 0x20000, CRC(675f4537) SHA1(acc68822da3aafbb62f76cbffa5f3389fcc91447) , ROM_SKIP(3) )
	ROMX_LOAD( "22.bin",     0x000002, 0x20000, CRC(db8a32ac) SHA1(b95f73dff291acee239e22e5fd7efe15d0de23be) , ROM_SKIP(3) )
	ROMX_LOAD( "24.bin",     0x000003, 0x20000, CRC(f4113e57) SHA1(ff1f443c13494a169b9be24abc361d27a6d01c09) , ROM_SKIP(3) )
	ROMX_LOAD( "10.bin",     0x080000, 0x20000, CRC(d478853e) SHA1(91fcf8eb022ccea66d291bec84ace557181cf861) , ROM_SKIP(3) )
	ROMX_LOAD( "12.bin",     0x080001, 0x20000, CRC(25055642) SHA1(578cf6a436489cc1f2d1acdb0cba6c1cbee2e21f) , ROM_SKIP(3) )
	ROMX_LOAD( "14.bin",     0x080002, 0x20000, CRC(b77d0328) SHA1(42eb1ebfda301f2b09f3add5932e8331f4790706) , ROM_SKIP(3) )
	ROMX_LOAD( "16.bin",     0x080003, 0x20000, CRC(ea111a79) SHA1(1b86aa984d2d6c527e96b61274a82263f34d0d89) , ROM_SKIP(3) )
	ROMX_LOAD( "19.bin",     0x100000, 0x20000, CRC(b3aa1f48) SHA1(411f3855739992f5967e915f2a5255afcedeac2e) , ROM_SKIP(3) )
	ROMX_LOAD( "21.bin",     0x100001, 0x20000, CRC(04d175c9) SHA1(33e6e3fefae4e3977c8c954fbd7feff36e92d723) , ROM_SKIP(3) )
	ROMX_LOAD( "23.bin",     0x100002, 0x20000, CRC(e592ba4f) SHA1(62559481e0da3954a90da0ab0fb51f87f1b3dd9d) , ROM_SKIP(3) )
	ROMX_LOAD( "25.bin",     0x100003, 0x20000, CRC(b89a740f) SHA1(516d73c772e0a904dfb0bd84874919d78bbbd200) , ROM_SKIP(3) )
	ROMX_LOAD( "11.bin",     0x180000, 0x20000, CRC(d4457a60) SHA1(9e956efafa81a81aca92837df03968f5670ffc15) , ROM_SKIP(3) )
	ROMX_LOAD( "13.bin",     0x180001, 0x20000, CRC(3b26a37d) SHA1(58d8d0cdef81c938fb1a5595f2d02b228865893b) , ROM_SKIP(3) )
	ROMX_LOAD( "15.bin",     0x180002, 0x20000, CRC(6d837e09) SHA1(b4a133ab96c35b689ee692bfcc04981791099b6f) , ROM_SKIP(3) )
	ROMX_LOAD( "17.bin",     0x180003, 0x20000, CRC(c59a4d6c) SHA1(59e49c7d24dd333007de4bb621050011a5392bcc) , ROM_SKIP(3) )

	ROM_REGION( 0x08000, "gfx2", 0 )
	ROM_COPY( "gfx", 0x00000, 0x00000, 0x08000 )	/* stars */
ROM_END

ROM_START( kodb )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )		/* 68000 code */
	ROM_LOAD16_BYTE( "3.ic172",    0x00000, 0x080000, CRC(036dd74c) SHA1(489344e56863429e86b4c362b82d89819c1d6afb) )
	ROM_LOAD16_BYTE( "4.ic171",    0x00001, 0x080000, CRC(3e4b7295) SHA1(3245640bae7d141238051dfe5c7683d05c6d3848) )

	ROM_REGION( 0x18000, "audiocpu", 0 )		/* 64k for the audio CPU (+banks) */
	ROM_LOAD( "1.ic28",        0x00000, 0x08000, CRC(01cae60c) SHA1(b2cdd883fd859f0b701230831aca1f1a74ad6087) )
	ROM_CONTINUE(              0x10000, 0x08000 )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROMX_LOAD( "cp.ic90",   0x000000, 0x80000, CRC(e3b8589e) SHA1(775f97e43cb995b93da40063a1f1e4d73b34437c), ROM_SKIP(7) )
	ROMX_LOAD( "dp.ic89",   0x000001, 0x80000, CRC(3eec9580) SHA1(3d8d0cfbeae077544e514a5eb96cc83f716e494f), ROM_SKIP(7) )
	ROMX_LOAD( "ap.ic88",   0x000002, 0x80000, CRC(fdf5f163) SHA1(271ee96886c958accaca9a82484ab80fe32bd38e), ROM_SKIP(7) )
	ROMX_LOAD( "bp.ic87",   0x000003, 0x80000, CRC(4e1c52b7) SHA1(74570e7d577c999c62203c97b3d449e3b61a678a), ROM_SKIP(7) )
	ROMX_LOAD( "ci.ic91",   0x000004, 0x80000, CRC(22228bc5) SHA1(d48a09ee284d9e4b986f5c3c1c865930f76986e2), ROM_SKIP(7) )
	ROMX_LOAD( "di.ic92",   0x000005, 0x80000, CRC(ab031763) SHA1(5bcd89b1debf029b779aa1bb73b3a572d27154ec), ROM_SKIP(7) )
	ROMX_LOAD( "ai.ic93",   0x000006, 0x80000, CRC(cffbf4be) SHA1(f805bafc855d4a656c055a76eaeb26e36835541e), ROM_SKIP(7) )
	ROMX_LOAD( "bi.ic94",   0x000007, 0x80000, CRC(4a1b43fe) SHA1(7957f45b2862825c9509043c63c7da7108bd251b), ROM_SKIP(7) )

	ROM_REGION( 0x8000, "gfx2", 0 )
	ROM_COPY( "gfx", 0x000000, 0x000000, 0x8000 )	/* stars */

	ROM_REGION( 0x40000, "oki", 0 )			/* Samples */
	ROM_LOAD( "2.ic19",      0x00000, 0x40000, CRC(a2db1575) SHA1(1a4a29e4b045af50700adf1665697feab12cc234) )
ROM_END

ROM_START( sf2mdt )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )		/* 68000 code */
	ROM_LOAD16_BYTE( "3.ic172",   0x000000, 0x80000, CRC(5301b41f) SHA1(6855a57b21e8c5d74e5cb18f9ce6af650d7fb422) )
	ROM_LOAD16_BYTE( "1.ic171",   0x000001, 0x80000, CRC(c1c803f6) SHA1(9fe18ae2553a63d8e4dcc20bafd5a4634f8b93c4) )
	ROM_LOAD16_BYTE( "4.ic176",   0x100000, 0x20000, CRC(1073b7b6) SHA1(81ca1eab65ceac69520584bb23a684ccb9d92f89) )
	ROM_LOAD16_BYTE( "2.ic175",   0x100001, 0x20000, CRC(924c6ce2) SHA1(676a912652bd75da5087f0c7eae047b7681a993c) )

	ROM_REGION( 0x600000, "gfx", 0 )		/* rearranged in init */
	ROMX_LOAD( "7.ic90",    0x000000, 0x80000, CRC(896eaf48) SHA1(5a13ae8b554e05eed3d5749aaf5845d499bce45b) , ROM_SKIP(3) )
	ROMX_LOAD( "13.ic89",   0x000001, 0x80000, CRC(305dd72a) SHA1(c373b517c23f3b019abb06e21f6b9ab6e1e47909) , ROM_SKIP(3) )
	ROMX_LOAD( "10.ic88",   0x000002, 0x80000, CRC(ef3f5be8) SHA1(d4e1de7d7caf6977e48544d6701618ae70c717f9) , ROM_SKIP(3) )
	ROMX_LOAD( "16.ic87",   0x000003, 0x80000, CRC(e57f6db9) SHA1(b37f95737804002ec0e237472eaacf0bc1e868e8) , ROM_SKIP(3) )
	ROMX_LOAD( "6.ic91",    0x200000, 0x80000, CRC(054cd5c4) SHA1(07f275e118c141a84ca15a2e9edc81694af37cf2) , ROM_SKIP(3) )
	ROMX_LOAD( "12.ic92",   0x200001, 0x80000, CRC(87e069e8) SHA1(cddd3be84f8379134590bfbbb080518f28120e49) , ROM_SKIP(3) )
	ROMX_LOAD( "9.ic93",    0x200002, 0x80000, CRC(818ca33d) SHA1(dfb707e17c83216f8a62e905f8c7cd6d406b417b) , ROM_SKIP(3) )
	ROMX_LOAD( "15.ic94",   0x200003, 0x80000, CRC(5dfb44d1) SHA1(08e44b8efc84f9cfc829aabf704155ddc700de76) , ROM_SKIP(3) )
	ROMX_LOAD( "8.ic86",    0x400000, 0x80000, CRC(34bbb3fa) SHA1(7794e89258f12b17d38c3d302dc15c502a8c8eb6) , ROM_SKIP(3) )
	ROMX_LOAD( "14.ic85",   0x400001, 0x80000, CRC(7d9f1a67) SHA1(6deb7fff867c42b13a32bb11eda798cfdb4cbaa8) , ROM_SKIP(3) )
	ROMX_LOAD( "11.ic84",   0x400002, 0x80000, CRC(cea6d1d6) SHA1(9c953db42f0d877e43c0c239f69a00df39a18295) , ROM_SKIP(3) )
	ROMX_LOAD( "17.ic83",   0x400003, 0x80000, CRC(91a9a05d) SHA1(5266ceddd2df925e79b4200843dec2f7aa9297b3) , ROM_SKIP(3) )

	ROM_REGION( 0x30000, "audiocpu", 0 )		/* Sound program + samples  */
	ROM_LOAD( "5.ic26",    0x00000, 0x20000, CRC(17d5ba8a) SHA1(6ff3b8860d7e1fdee3561846f645eb4d3a8965ec) )
	ROM_RELOAD(            0x10000, 0x20000 )
ROM_END

ROM_START( knightsb )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )		/* 68000 code */
	ROM_LOAD16_BYTE( "3.ic173",    0x00001, 0x40000, CRC(c9c6e720) SHA1(e8a1cd73458b548e88fc49d8f659e0dc33a8e756) )
	ROM_LOAD16_BYTE( "5.ic172",    0x00000, 0x40000, CRC(7fd91118) SHA1(d2832b21309a467938891946d7af35d8095787a4) )
	ROM_LOAD16_BYTE( "2.ic175",    0x80001, 0x40000, CRC(1eb91343) SHA1(e02cfbbd7689346f14f2e3455ed17e7f0b51bad0) )
	ROM_LOAD16_BYTE( "4.ic176",    0x80000, 0x40000, CRC(af352703) SHA1(7855ac65752203f45af4ef41af8c291540a1c8a8) )

	ROM_REGION( 0x400000, "gfx", 0 )		/* bootleg had 4x 1meg MASKroms, these need dumping so that the format is known */
	ROMX_LOAD( "kr_gfx1.rom",  0x000000, 0x80000, BAD_DUMP CRC(9e36c1a4) SHA1(772daae74e119371dfb76fde9775bda78a8ba125) , ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "kr_gfx3.rom",  0x000002, 0x80000, BAD_DUMP CRC(c5832cae) SHA1(a188cf401cd3a2909b377d3059f14d22ec3b0643) , ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "kr_gfx2.rom",  0x000004, 0x80000, BAD_DUMP CRC(f095be2d) SHA1(0427d1574062f277a9d04440019d5638b05de561) , ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "kr_gfx4.rom",  0x000006, 0x80000, BAD_DUMP CRC(179dfd96) SHA1(b1844e69da7ab13474da569978d5b47deb8eb2be) , ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "kr_gfx5.rom",  0x200000, 0x80000, BAD_DUMP CRC(1f4298d2) SHA1(4b162a7f649b0bcd676f8ca0c5eee9a1250d6452) , ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "kr_gfx7.rom",  0x200002, 0x80000, BAD_DUMP CRC(37fa8751) SHA1(b88b39d1f08621f15a5620095aef998346fa9891) , ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "kr_gfx6.rom",  0x200004, 0x80000, BAD_DUMP CRC(0200bc3d) SHA1(c900b1be2b4e49b951e5c1e3fd1e19d21b82986e) , ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "kr_gfx8.rom",  0x200006, 0x80000, BAD_DUMP CRC(0bb2b4e7) SHA1(983b800925d58e4aeb4e5105f93ed5faf66d009c) , ROM_GROUPWORD | ROM_SKIP(6) )

	ROM_REGION( 0x50000, "audiocpu", 0 )		/* 64k for the audio CPU (+banks) */
	ROM_LOAD( "1.ic26",     0x00000, 0x40000, CRC(bd6f9cc1) SHA1(9f33cccef224d2204736a9eae761196866bd6e41) )
	ROM_RELOAD(             0x10000, 0x40000 )
ROM_END


/*
GAME( year,   archives name,   parent name,  MACHINE_DRIVER_START,  INPUT_PORTS,  DRIVER_INIT,	flip,	producer name,	title information,	status )
*/
GAME( 1990,	fcrash,		ffight,     fcrash,	fcrash,    cps1,     ROT0,   "bootleg (Playmark)",  "Final Crash (bootleg of Final Fight)", GAME_IMPERFECT_GRAPHICS | GAME_SUPPORTS_SAVE ) /* old sprites show on next screen. Patch used. */
GAME( 1991,	kodb,		kod,	    kodb,	kodb,	   kodb,     ROT0,   "bootleg (Playmark)",  "The King of Dragons (bootleg)", GAME_IMPERFECT_GRAPHICS | GAME_SUPPORTS_SAVE ) /* old sprites show on next screen. Patch used. */
GAME( 1991,	knightsb,	knights,    knightsb,   knights,   knightsb, ROT0,   "bootleg",		    "Knights of the Round (bootleg)", GAME_IMPERFECT_GRAPHICS | GAME_SUPPORTS_SAVE )
GAME( 1992,	sf2mdt,		sf2ce,	    sf2mdt,     sf2hack,   sf2mdt,   ROT0,   "bootleg",		    "Street Fighter II': Magic Delta Turbo (bootleg)", GAME_IMPERFECT_GRAPHICS | GAME_SUPPORTS_SAVE )
