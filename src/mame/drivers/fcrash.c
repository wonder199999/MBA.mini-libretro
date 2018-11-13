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

#include "machine/eeprom.h"
#include "includes/cps1.h"

#include "sound/2203intf.h"
#include "sound/msm5205.h"
#include "sound/2151intf.h"
#include "sound/okim6295.h"


/* -------------- Functions ---------------- */
static WRITE16_HANDLER( fcrash_soundlatch_w )
{
	if (ACCESSING_BITS_0_7)
	{
		cps_state *state = space->machine->driver_data<cps_state>();
		soundlatch_w(space, 0, data & 0xff);
		cpu_set_input_line(state->audiocpu, 0, HOLD_LINE);
	}
}

static WRITE16_HANDLER( sf2mdt_soundlatch_w )	/* The function will also be used for 'cawingbl' */
{
	if (ACCESSING_BITS_8_15)
	{
		cps_state *state = space->machine->driver_data<cps_state>();
		soundlatch_w(space, 0, data >> 8);
		cpu_set_input_line(state->audiocpu, 0, HOLD_LINE);
		cpuexec_boost_interleave( space->machine, attotime_zero, ATTOTIME_IN_USEC(50) );
	}
}

static WRITE8_HANDLER( fcrash_snd_bankswitch_w )
{
	cps_state *state = space->machine->driver_data<cps_state>();
	sound_set_output_gain(state->msm_1, 0, (data & 0x08) ? 0.0 : 1.0);
	sound_set_output_gain(state->msm_2, 0, (data & 0x10) ? 0.0 : 1.0);
	memory_set_bank(space->machine, "bank1", data & 0x07);
}

static WRITE8_HANDLER( sf2mdt_snd_bankswitch_w )
{
	cps_state *state = space->machine->driver_data<cps_state>();
	sound_set_output_gain(state->msm_1, 0, (data & 0x20) ? 0.0 : 1.0);
	sound_set_output_gain(state->msm_2, 0, (data & 0x10) ? 0.0 : 1.0);
	memory_set_bank(space->machine, "bank1", data & 0x07);
}

static WRITE8_HANDLER( knightsb_snd_bankswitch_w )
{
	cps_state *state = space->machine->driver_data<cps_state>();
	sound_set_output_gain(state->msm_1, 0, (data & 0x20) ? 0.0 : 1.0);
	sound_set_output_gain(state->msm_2, 0, (data & 0x10) ? 0.0 : 1.0);
	memory_set_bank(space->machine, "bank1", data & 0x0f);
}

static void m5205_int1( running_device *device )
{
	cps_state *state = device->machine->driver_data<cps_state>();
	msm5205_data_w(device, state->sample_buffer1 & 0x0f);
	state->sample_buffer1 >>= 4;
	state->sample_select1 ^= 1;
	if (state->sample_select1 == 0)
		cpu_set_input_line(state->audiocpu, INPUT_LINE_NMI, PULSE_LINE);
}

static void m5205_int2( running_device *device )
{
	cps_state *state = device->machine->driver_data<cps_state>();
	msm5205_data_w(device, state->sample_buffer2 & 0x0f);
	state->sample_buffer2 >>= 4;
	state->sample_select2 ^= 1;
}

static WRITE8_HANDLER( fcrash_msm5205_0_data_w )
{
	cps_state *state = space->machine->driver_data<cps_state>();
	state->sample_buffer1 = data;
}

static WRITE8_HANDLER( fcrash_msm5205_1_data_w )
{
	cps_state *state = space->machine->driver_data<cps_state>();
	state->sample_buffer2 = data;
}

static const eeprom_interface bootleg_eeprom_interface = { 7, 8, "0110", "0101", "0111" };
static const msm5205_interface msm5205_interface1 = { m5205_int1, MSM5205_S96_4B };
static const msm5205_interface msm5205_interface2 = { m5205_int2, MSM5205_S96_4B };
static const ym2151_interface ym2151_config = { cps1_irq_handler_mus };

static WRITE16_HANDLER( kodb_layer_w )		/* The function will also be used for 'cawingbl' */
{
	cps_state *state = space->machine->driver_data<cps_state>();
	switch (offset)
	{
		case 0x06: state->cps_b_regs[state->layer_enable_reg / 2] = data; break;
		case 0x10: state->cps_b_regs[state->layer_mask_reg[1] / 2] = data; break;
		case 0x11: state->cps_b_regs[state->layer_mask_reg[2] / 2] = data;
	}
}

static WRITE16_HANDLER( dinopic_layer_w )
{
	cps_state *state = space->machine->driver_data<cps_state>();
	switch (offset)
	{
		case 0x00: state->cps_a_regs[0x0e / 2] = data; break;
		case 0x01: state->cps_a_regs[0x0c / 2] = data; break;
		case 0x02: state->cps_a_regs[0x12 / 2] = state->cps_a_regs[CPS1_ROWSCROLL_OFFS] = data; break;
		case 0x03: state->cps_a_regs[0x10 / 2] = data; break;
		case 0x04: state->cps_a_regs[0x16 / 2] = data; break;
		case 0x05: state->cps_a_regs[0x14 / 2] = data; break;
		default: logerror("Unknown layer cmd %X %X\n", offset << 1, data);
	}
}

static WRITE16_HANDLER( dinopic_layer2_w )
{
	cps_state *state = space->machine->driver_data<cps_state>();
	state->cps_a_regs[0x06 / 2] = data;
}

static WRITE16_HANDLER( slampic_layer_w )
{
	cps_state *state = space->machine->driver_data<cps_state>();
	switch (offset)
	{
		case 0x00:
		case 0x01:
		case 0x02:
		case 0x03:
		case 0x04:
		case 0x05: dinopic_layer_w(space, offset, data, 0xffff); break;
		case 0x06: state->cps_a_regs[0x04 / 2] = data << 4; break;
	}
}

static WRITE16_HANDLER( punipic_layer_w )
{
	cps_state *state = space->machine->driver_data<cps_state>();
	switch (offset)
	{
		case 0x00: state->cps_a_regs[0x0e / 2] = data; break;
		case 0x01: state->cps_a_regs[0x0c / 2] = data; break;
		case 0x02: state->cps_a_regs[0x12 / 2] = state->cps_a_regs[CPS1_ROWSCROLL_OFFS] = data; break;
		case 0x03: state->cps_a_regs[0x10 / 2] = data + 0xffc0; break;
		case 0x04: state->cps_a_regs[0x16 / 2] = data; break;
		case 0x05: state->cps_a_regs[0x14 / 2] = data; break;
		case 0x06:
		{
			switch (data)
			{
				case 0x14:
				case 0x54: state->cps_a_regs[0x04 / 2] = 0x9100; break;
				case 0x24:
				case 0x64: state->cps_a_regs[0x04 / 2] = 0x90c0; break;
				case 0x3c:
				case 0x7c: state->cps_a_regs[0x04 / 2] = 0x9180; break;
			}
			state->cps_a_regs[0x08 / 2] = state->mainram[0xdb90 / 2]; break;
		}
		case 0x07: break;
		default: logerror ("Unknown layer cmd %X %X\n", offset << 1, data);
	}
}

static WRITE16_HANDLER( knightsb_layer_w )
{
	cps_state *state = space->machine->driver_data<cps_state>();
	switch (offset)
	{
		case 0x00: state->cps_a_regs[0x0e / 2] = data; break;
		case 0x01: state->cps_a_regs[0x0c / 2] = data; break;
		case 0x02: state->cps_a_regs[0x12 / 2] = state->cps_a_regs[CPS1_ROWSCROLL_OFFS] = data; break;
		case 0x03: state->cps_a_regs[0x10 / 2] = data; break;
		case 0x04: state->cps_a_regs[0x16 / 2] = data; break;
		case 0x05: state->cps_a_regs[0x14 / 2] = data; break;
		case 0x06:
		{
			UINT16 draw_mask[4] = { 0, 0, 0, 0 };

			switch (data)
			{
				case 0x0000: data = 0x12c0; break;
				case 0x001f: data = 0x12c0; draw_mask[1] = 0x03ff; draw_mask[2] = 0x003f; draw_mask[3] = 0x01ff; break;
				case 0x00ff: data = 0x12c0; draw_mask[1] = 0x7fff; draw_mask[2] = 0x7ff8; draw_mask[3] = 0x00ff; break;
				case 0x07ff: data = 0x12c0; draw_mask[1] = 0x001f; draw_mask[2] = 0x00ff; draw_mask[3] = 0x07ff; break;
				case 0x2000: data = 0x06c0; break;
				case 0x5800: data = 0x12c0; draw_mask[1] = 0xffee; draw_mask[2] = 0x01ff; draw_mask[3] = 0x7800; break;
				case 0x5f00: data = 0x12c0; draw_mask[1] = 0x03ff; draw_mask[2] = 0x7e00; draw_mask[3] = 0x7f00; break;
				case 0x80ff: data = 0x1380; draw_mask[1] = 0x7fff; draw_mask[2] = 0x7ff8; draw_mask[3] = 0x00ff; break;
				case 0x87ff: data = 0x1380; draw_mask[1] = 0x001f; draw_mask[2] = 0x00ff; draw_mask[3] = 0x07ff; break;
				case 0xa000: data = 0x24c0; break;
				case 0xd800: data = 0x1380; draw_mask[1] = 0xffee; draw_mask[2] = 0x01ff; draw_mask[3] = 0x7800; break;
				default: logerror ("Unknown control word = %X\n", data); data = 0x12c0; break;
			}
			state->cps_b_regs[state->layer_mask_reg[0] / 2] = draw_mask[0];
			state->cps_b_regs[state->layer_mask_reg[1] / 2] = draw_mask[1];
			state->cps_b_regs[state->layer_mask_reg[2] / 2] = draw_mask[2];
			state->cps_b_regs[state->layer_mask_reg[3] / 2] = draw_mask[3];

			state->cps_b_regs[state->layer_enable_reg / 2] = data; break;
		}
	}
}

static WRITE16_HANDLER( sf2m1_layer_w )
{
	cps_state *state = space->machine->driver_data<cps_state>();
	switch (offset)
	{
		case 0x00: state->cps_a_regs[0x0e / 2] = data; break;
		case 0x01: state->cps_a_regs[0x0c / 2] = data; break;
		case 0x02: state->cps_a_regs[0x12 / 2] = state->cps_a_regs[CPS1_ROWSCROLL_OFFS] = data; break;
		case 0x03: state->cps_a_regs[0x10 / 2] = data; break;
		case 0x04: state->cps_a_regs[0x16 / 2] = data; break;
		case 0x05: state->cps_a_regs[0x14 / 2] = data; break;
		case 0x06:
		{
			switch (data)
			{
				case 0x00: data = 0x078e; break;
				case 0x01: data = 0x12c0; break;
				case 0x02: data = 0x06ce; break;
				case 0x03: data = 0x09ce; break;
				case 0x04: data = 0x12ce; break;
				case 0x05: data = 0x0b4e; break;
			}
			state->cps_b_regs[state->layer_enable_reg / 2] = data; break;
		}
		case 0x0b:
		case 0x1b: state->cps_a_regs[0x06 / 2] = data; break;
		case 0xb3: state->cps_b_regs[state->layer_enable_reg / 2] = data; break;
		default: logerror(" Unknown layer cmd\n");
	}
}

static WRITE16_HANDLER( sf2mdta_layer_w )
{
	cps_state *state = space->machine->driver_data<cps_state>();
	switch (offset)
	{
		case 0x06: state->cps_a_regs[0x0c / 2] = data + 0xffbe;	break;
		case 0x07: state->cps_a_regs[0x0e / 2] = data; break;
		case 0x08: state->cps_a_regs[0x14 / 2] = data + 0xffce;	break;
		case 0x09: state->cps_a_regs[0x12 / 2] = state->cps_a_regs[CPS1_ROWSCROLL_OFFS] = data; break;
		case 0x0a: state->cps_a_regs[0x10 / 2] = data + 0xffce;	break;
		case 0x0b: state->cps_a_regs[0x16 / 2] = data;	break;
		case 0x26: state->cps_b_regs[state->layer_enable_reg / 2] = data;
	}
}

static WRITE16_HANDLER( sf2mdt_layer_w )
{
	cps_state *state = space->machine->driver_data<cps_state>();
	switch (offset)
	{
		case 0x06: state->cps_a_regs[0x14 / 2] = data + 0xffce; break;
		case 0x07: state->cps_a_regs[0x16 / 2] = data; break;
		case 0x08: state->cps_a_regs[0x10 / 2] = data + 0xffce;	break;
		case 0x09: state->cps_a_regs[0x0c / 2] = data + 0xffca;	break;
		case 0x0a: state->cps_a_regs[0x12 / 2] = state->cps_a_regs[CPS1_ROWSCROLL_OFFS] = data; break;
		case 0x0b: state->cps_a_regs[0x0e / 2] = data; break;
		case 0x26: state->cps_b_regs[state->layer_enable_reg / 2] = data;
	}
}

static WRITE16_HANDLER( varthb_layer_w )
{
	cps_state *state = space->machine->driver_data<cps_state>();

	if (data > 0x9000)
		state->cps_a_regs[0x06 / 2] = data;
}


/* ---  RENDER HANDLER  --- */
static void bootleg_update_transmasks( running_machine *machine )
{
	cps_state *state = machine->driver_data<cps_state>();

	for (UINT32 i = 0; i < 4; i++)
	{
		INT32 mask = state->layer_mask_reg[i] ? (state->cps_b_regs[state->layer_mask_reg[i] / 2] ^ 0xffff) : 0xffff;

		tilemap_set_transmask(state->bg_tilemap[0], i, mask, 0x8000);
		tilemap_set_transmask(state->bg_tilemap[1], i, mask, 0x8000);
		tilemap_set_transmask(state->bg_tilemap[2], i, mask, 0x8000);
	}
}

static void bootleg_render_sprites( running_machine *machine, bitmap_t *bitmap, const rectangle *cliprect )
{
	cps_state *state = machine->driver_data<cps_state>();

	INT32 pos, base = state->sprite_base / 2;
	INT32 last_sprite_offset = 0x1ffc;
	INT32 num_sprites = machine->gfx[2]->total_elements;
	UINT16 *sprite_ram = state->gfxram;
	UINT16 tileno, color, xpos, ypos, flipx, flipy;

	if (state->bootleg_sprite_ram)							/* if we have separate sprite ram, use it */
		sprite_ram = state->bootleg_sprite_ram;

	for ( pos = 0x1ffc - base; pos >= 0x0000; pos -= 4)				/* get end of sprite list marker */
		if (sprite_ram[base + pos - 1] == state->sprite_list_end_marker)
			last_sprite_offset = pos;

	if ( ((base + last_sprite_offset) < 0x2000) || (!state->bootleg_sprite_ram) )	/* if we are using bootleg sprite ram, the index must be less than 0x2000 */
	{
		for (pos = last_sprite_offset; pos >= 0x0000; pos -= 4)
		{
			INT32 temp = base + pos + 1;
			tileno = sprite_ram[temp - 1];
			if (tileno >= num_sprites) continue;
			xpos   = sprite_ram[temp + 1] & 0x01ff;
			ypos   = sprite_ram[temp - 2] & 0x01ff;
			flipx  = sprite_ram[temp] & 0x20;
			flipy  = sprite_ram[temp] & 0x40;
			color  = sprite_ram[temp] & 0x1f;
			ypos   = 256 - 16 - ypos;
			xpos   += state->sprite_x_offset + 49;

			pdrawgfx_transpen(bitmap, cliprect, machine->gfx[2], tileno, color, flipx, flipy, xpos, ypos, machine->priority_bitmap, 0x02, 0x0f);
		}
	}
}

static void bootleg_render_layer( running_machine *machine, bitmap_t *bitmap, const rectangle *cliprect, int layer, int primask )
{
	cps_state *state = machine->driver_data<cps_state>();
	switch (layer)
	{
		case 0: bootleg_render_sprites(machine, bitmap, cliprect); break;
		case 1:
		case 2:
		case 3: tilemap_draw(bitmap, cliprect, state->bg_tilemap[layer - 1], TILEMAP_DRAW_LAYER1, primask); break;
	}
}

static void bootleg_render_high_layer( running_machine *machine, bitmap_t *bitmap, const rectangle *cliprect, int layer )
{
	cps_state *state = machine->driver_data<cps_state>();
	bitmap_t *dummy_bitmap = NULL;
	switch (layer)
	{
		case 0: break;
		case 1:
		case 2:
		case 3: tilemap_draw(dummy_bitmap, cliprect, state->bg_tilemap[layer - 1], TILEMAP_DRAW_LAYER0, 1); break;
	}
}

static void bootleg_build_palette( running_machine *machine )
{
	cps_state *state = machine->driver_data<cps_state>();

	UINT32 r, g, b, bright, palette, value;
	UINT32 palettebase = (state->cps_a_regs[0x0a / 2] << 8) & 0x01ffff;	/* all the bootlegs seem to write the palette offset as usual */

	for (UINT32 offset = 0; offset < 32 * 6 * 16; offset++)
	{
		palette = state->gfxram[palettebase / 2 + offset];
		/* from my understanding of the schematics, when the 'brightness' component is set to 0 it should reduce brightness to 1/3 */
		bright = ((palette >> 12) << 1) + 0x0f;
		value = bright * 0x11 / 0x2d;

		r = ((palette >> 8) & 0x0f) * value;
		g = ((palette >> 4) & 0x0f) * value;
		b = ((palette >> 0) & 0x0f) * value;

		palette_set_color( machine, offset, MAKE_RGB(r, g, b) );
	}
}

static VIDEO_UPDATE( bootleg_updatescreen )
{
	cps_state *state = screen->machine->driver_data<cps_state>();

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
		INT32 scrly = -state->scroll2y;
		tilemap_set_scroll_rows(state->bg_tilemap[1], 1024);
		INT32 otheroffs = state->cps_a_regs[CPS1_ROWSCROLL_OFFS];

		for (UINT32 i = 0; i < 256; )
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

	INT32 layercontrol = state->cps_b_regs[state->layer_enable_reg / 2];
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

/* --------------------- CPU PROGRAM_MAP --------------------- */
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

static ADDRESS_MAP_START( sgyxz_map, ADDRESS_SPACE_PROGRAM, 16 )
	AM_RANGE(0x000000, 0x3fffff) AM_ROM
	AM_RANGE(0x800030, 0x800031) AM_WRITE(cps1_coinctrl_w)
	AM_RANGE(0x800100, 0x80013f) AM_RAM AM_BASE_MEMBER(cps_state, cps_a_regs)
	AM_RANGE(0x800140, 0x80017f) AM_RAM AM_BASE_MEMBER(cps_state, cps_b_regs)
	AM_RANGE(0x880000, 0x880001) AM_READ_PORT("IN1")
	AM_RANGE(0x880006, 0x88000d) AM_READ(cps1_dsw_r)
	AM_RANGE(0x88000e, 0x88000f) AM_WRITE(cps1_soundlatch_w)
	AM_RANGE(0x880e78, 0x880e79) AM_READ(cps1_in2_r)
	AM_RANGE(0x890000, 0x890001) AM_WRITE(cps1_soundlatch2_w)
	AM_RANGE(0x900000, 0x92ffff) AM_RAM_WRITE(cps1_gfxram_w) AM_BASE_SIZE_MEMBER(cps_state, gfxram, gfxram_size)
	AM_RANGE(0xf1c004, 0xf1c005) AM_WRITE(cpsq_coinctrl2_w)
	AM_RANGE(0xf1c006, 0xf1c007) AM_READ_PORT("EEPROMIN") AM_WRITE_PORT("EEPROMOUT")
	AM_RANGE(0xff0000, 0xffffff) AM_RAM
ADDRESS_MAP_END

static ADDRESS_MAP_START( sf2m1_map, ADDRESS_SPACE_PROGRAM, 16 )
	AM_RANGE(0x000000, 0x3fffff) AM_ROM
	AM_RANGE(0x800000, 0x800007) AM_READ_PORT("IN1")
	AM_RANGE(0x800006, 0x800007) AM_WRITE(cps1_soundlatch_w)
	AM_RANGE(0x800012, 0x800013) AM_READ(cps1_in2_r)
	AM_RANGE(0x800018, 0x80001f) AM_READ(cps1_dsw_r)
	AM_RANGE(0x800100, 0x80013f) AM_WRITE(cps1_cps_a_w) AM_BASE_MEMBER(cps_state, cps_a_regs)
	AM_RANGE(0x800140, 0x80017f) AM_READWRITE(cps1_cps_b_r, cps1_cps_b_w) AM_BASE_MEMBER(cps_state, cps_b_regs)
	AM_RANGE(0x800180, 0x800181) AM_WRITENOP
	AM_RANGE(0x800188, 0x80018f) AM_WRITE(cps1_soundlatch2_w)
	AM_RANGE(0x880000, 0x880001) AM_WRITENOP
	AM_RANGE(0x900000, 0x93ffff) AM_RAM_WRITE(cps1_gfxram_w) AM_BASE_SIZE_MEMBER(cps_state, gfxram, gfxram_size)
	AM_RANGE(0x980000, 0x9801ff) AM_WRITE(sf2m1_layer_w)
	AM_RANGE(0x990000, 0x990001) AM_WRITENOP
	AM_RANGE(0xff0000, 0xffffff) AM_RAM
ADDRESS_MAP_END

static ADDRESS_MAP_START( punipic_map, ADDRESS_SPACE_PROGRAM, 16 )
	AM_RANGE(0x000000, 0x3fffff) AM_ROM
	AM_RANGE(0x800000, 0x800007) AM_READ_PORT("IN1")
	AM_RANGE(0x800006, 0x800007) AM_WRITE(cps1_soundlatch_w)
	AM_RANGE(0x800018, 0x80001f) AM_READ(cps1_dsw_r)
	AM_RANGE(0x800030, 0x800037) AM_WRITE(cps1_coinctrl_w)
	AM_RANGE(0x800100, 0x80013f) AM_WRITE(cps1_cps_a_w) AM_BASE_MEMBER(cps_state, cps_a_regs)
	AM_RANGE(0x800140, 0x80017f) AM_READWRITE(cps1_cps_b_r, cps1_cps_b_w) AM_BASE_MEMBER(cps_state, cps_b_regs)
	AM_RANGE(0x880000, 0x880001) AM_WRITENOP
	AM_RANGE(0x900000, 0x92ffff) AM_RAM_WRITE(cps1_gfxram_w) AM_BASE_SIZE_MEMBER(cps_state, gfxram, gfxram_size)
	AM_RANGE(0x980000, 0x98000f) AM_WRITE(punipic_layer_w)
	AM_RANGE(0x990000, 0x990001) AM_WRITENOP
	AM_RANGE(0x991000, 0x991017) AM_WRITENOP
	AM_RANGE(0xf18000, 0xf19fff) AM_RAM
	AM_RANGE(0xf1c006, 0xf1c007) AM_READ_PORT("EEPROMIN") AM_WRITE_PORT("EEPROMOUT")
	AM_RANGE(0xff0000, 0xffffff) AM_RAM AM_BASE_MEMBER(cps_state, mainram)
ADDRESS_MAP_END

static ADDRESS_MAP_START( dinopic_map, ADDRESS_SPACE_PROGRAM, 16 )
	AM_RANGE(0x000000, 0x3fffff) AM_ROM
	AM_RANGE(0x800000, 0x800007) AM_READ_PORT("IN1")
	AM_RANGE(0x800006, 0x800007) AM_WRITE(cps1_soundlatch_w)
	AM_RANGE(0x800018, 0x80001f) AM_READ(cps1_dsw_r)
	AM_RANGE(0x800030, 0x800037) AM_WRITE(cps1_coinctrl_w)
	AM_RANGE(0x800100, 0x80013f) AM_WRITE(cps1_cps_a_w) AM_BASE_MEMBER(cps_state, cps_a_regs)
	AM_RANGE(0x800140, 0x80017f) AM_READWRITE(cps1_cps_b_r, cps1_cps_b_w) AM_BASE_MEMBER(cps_state, cps_b_regs)
	AM_RANGE(0x800222, 0x800223) AM_WRITE(dinopic_layer2_w)
	AM_RANGE(0x880000, 0x880001) AM_WRITENOP
	AM_RANGE(0x900000, 0x92ffff) AM_RAM_WRITE(cps1_gfxram_w) AM_BASE_SIZE_MEMBER(cps_state, gfxram, gfxram_size)
	AM_RANGE(0x980000, 0x98000b) AM_WRITE(dinopic_layer_w)
	AM_RANGE(0xf18000, 0xf19fff) AM_RAM
	AM_RANGE(0xf1c000, 0xf1c001) AM_READ_PORT("IN2")
	AM_RANGE(0xf1c004, 0xf1c005) AM_WRITE(cpsq_coinctrl2_w)
	AM_RANGE(0xf1c006, 0xf1c007) AM_READ_PORT("EEPROMIN") AM_WRITE_PORT("EEPROMOUT")
	AM_RANGE(0xff0000, 0xffffff) AM_RAM
ADDRESS_MAP_END

static ADDRESS_MAP_START( slampic_map, ADDRESS_SPACE_PROGRAM, 16 )
	AM_RANGE(0x000000, 0x3fffff) AM_ROM
	AM_RANGE(0x800006, 0x800007) AM_WRITENOP
	AM_RANGE(0x800000, 0x800007) AM_READ_PORT("IN1")
	AM_RANGE(0x800018, 0x80001f) AM_READ(cps1_dsw_r)
	AM_RANGE(0x800030, 0x800037) AM_WRITE(cps1_coinctrl_w)
	AM_RANGE(0x800100, 0x80013f) AM_WRITE(cps1_cps_a_w) AM_BASE_MEMBER(cps_state, cps_a_regs)
	AM_RANGE(0x800140, 0x80017f) AM_READWRITE(cps1_cps_b_r, cps1_cps_b_w) AM_BASE_MEMBER(cps_state, cps_b_regs)
	AM_RANGE(0x880000, 0x880001) AM_WRITENOP
	AM_RANGE(0x900000, 0x92ffff) AM_RAM_WRITE(cps1_gfxram_w) AM_BASE_SIZE_MEMBER(cps_state, gfxram, gfxram_size)
	AM_RANGE(0x980000, 0x98000d) AM_WRITE(slampic_layer_w)
	AM_RANGE(0xf00000, 0xf0ffff) AM_READ(qsound_rom_r)
	AM_RANGE(0xf18000, 0xf19fff) AM_RAM
	AM_RANGE(0xf1c000, 0xf1c001) AM_READ_PORT("IN2")
	AM_RANGE(0xf1c004, 0xf1c005) AM_WRITE(cpsq_coinctrl2_w)
	AM_RANGE(0xf1c006, 0xf1c007) AM_READ_PORT("EEPROMIN") AM_WRITE_PORT("EEPROMOUT")
	AM_RANGE(0xf1f000, 0xf1ffff) AM_NOP
	AM_RANGE(0xff0000, 0xffffff) AM_RAM
ADDRESS_MAP_END

static ADDRESS_MAP_START( captcommb2_map, ADDRESS_SPACE_PROGRAM, 16 )
	AM_RANGE(0x000000, 0x1fffff) AM_ROM
	AM_RANGE(0x800000, 0x800001) AM_READ_PORT("IN1")
	AM_RANGE(0x800018, 0x80001f) AM_READ(cps1_dsw_r)
	AM_RANGE(0x800030, 0x800037) AM_WRITE(cps1_coinctrl_w)
	AM_RANGE(0x800100, 0x80013f) AM_WRITE(cps1_cps_a_w) AM_BASE_MEMBER(cps_state, cps_a_regs)
	AM_RANGE(0x800140, 0x80017f) AM_READWRITE(cps1_cps_b_r, cps1_cps_b_w) AM_BASE_MEMBER(cps_state, cps_b_regs)
	AM_RANGE(0x800180, 0x800181) AM_WRITE(fcrash_soundlatch_w)
	AM_RANGE(0x900000, 0x92ffff) AM_RAM_WRITE(cps1_gfxram_w) AM_BASE_SIZE_MEMBER(cps_state, gfxram, gfxram_size)
	AM_RANGE(0xff0000, 0xffffff) AM_RAM
ADDRESS_MAP_END

static ADDRESS_MAP_START( varthb_map, ADDRESS_SPACE_PROGRAM, 16 )
	AM_RANGE(0x000000, 0x1fffff) AM_ROM
	AM_RANGE(0x800000, 0x800001) AM_READ_PORT("IN1")
	AM_RANGE(0x800006, 0x800007) AM_WRITE(cps1_soundlatch_w)
	AM_RANGE(0x800018, 0x80001f) AM_READ(cps1_dsw_r)
	AM_RANGE(0x800030, 0x800037) AM_WRITE(cps1_coinctrl_w)
	AM_RANGE(0x800100, 0x80013f) AM_WRITE(cps1_cps_a_w) AM_BASE_MEMBER(cps_state, cps_a_regs)
	AM_RANGE(0x800140, 0x80017f) AM_READWRITE(cps1_cps_b_r, cps1_cps_b_w) AM_BASE_MEMBER(cps_state, cps_b_regs)
	AM_RANGE(0x800188, 0x800189) AM_WRITE(varthb_layer_w)
	AM_RANGE(0x980000, 0x98000b) AM_WRITE(dinopic_layer_w)
	AM_RANGE(0x900000, 0x92ffff) AM_RAM_WRITE(cps1_gfxram_w) AM_BASE_SIZE_MEMBER(cps_state, gfxram, gfxram_size)
	AM_RANGE(0xff0000, 0xffffff) AM_RAM
ADDRESS_MAP_END


/* --------------------- SUB PROGRAM_MAP --------------------- */
static ADDRESS_MAP_START( fcrash_soundmap, ADDRESS_SPACE_PROGRAM, 8 )
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

static ADDRESS_MAP_START( sf2mdt_soundmap, ADDRESS_SPACE_PROGRAM, 8 )
	AM_RANGE(0x0000, 0x7fff) AM_ROM
	AM_RANGE(0x8000, 0xbfff) AM_ROMBANK("bank1")
	AM_RANGE(0xd000, 0xd7ff) AM_RAM
	AM_RANGE(0xd800, 0xd801) AM_DEVREADWRITE("2151", ym2151_r, ym2151_w)
	AM_RANGE(0xdc00, 0xdc00) AM_READ(soundlatch_r)
	AM_RANGE(0xe000, 0xe000) AM_WRITE(sf2mdt_snd_bankswitch_w)
	AM_RANGE(0xe400, 0xe400) AM_WRITE(fcrash_msm5205_0_data_w)
	AM_RANGE(0xe800, 0xe800) AM_WRITE(fcrash_msm5205_1_data_w)
ADDRESS_MAP_END

static ADDRESS_MAP_START( knightsb_soundmap, ADDRESS_SPACE_PROGRAM, 8 )
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

static ADDRESS_MAP_START( kodb_soundmap, ADDRESS_SPACE_PROGRAM, 8 )
	AM_RANGE(0x0000, 0x7fff) AM_ROM
	AM_RANGE(0x8000, 0xbfff) AM_ROMBANK("bank1")
	AM_RANGE(0xd000, 0xd7ff) AM_RAM
	AM_RANGE(0xe000, 0xe001) AM_DEVREADWRITE("2151", ym2151_r, ym2151_w)
	AM_RANGE(0xe400, 0xe400) AM_DEVREADWRITE("oki", okim6295_r, okim6295_w)
	AM_RANGE(0xe800, 0xe800) AM_READ(soundlatch_r)
ADDRESS_MAP_END

static ADDRESS_MAP_START( sf2m1_soundmap, ADDRESS_SPACE_PROGRAM, 8 )	/* The define will also be used for 'sgyxz' */
	AM_RANGE(0x0000, 0x7fff) AM_ROM
	AM_RANGE(0x8000, 0xbfff) AM_ROMBANK("bank1")
	AM_RANGE(0xd000, 0xd7ff) AM_RAM
	AM_RANGE(0xf000, 0xf001) AM_DEVREADWRITE("2151", ym2151_r, ym2151_w)
	AM_RANGE(0xf002, 0xf002) AM_DEVREADWRITE("oki", okim6295_r, okim6295_w)
	AM_RANGE(0xf004, 0xf004) AM_WRITE(cps1_snd_bankswitch_w)
	AM_RANGE(0xf006, 0xf006) AM_DEVWRITE("oki", cps1_oki_pin7_w)
	AM_RANGE(0xf008, 0xf008) AM_READ(soundlatch_r)
	AM_RANGE(0xf00a, 0xf00a) AM_READ(soundlatch2_r)
ADDRESS_MAP_END


/* --- INPUT PORTS --- */

static INPUT_PORTS_START( sf2mdt )		/* for sf2/sf2ce hackrom */
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
	CPS1_COINAGE_1("SW(A)")
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

static INPUT_PORTS_START( sgyxz )
	PORT_INCLUDE( cps1_3players )

	PORT_MODIFY("IN0")
	PORT_SERVICE_NO_TOGGLE( 0x40, IP_ACTIVE_LOW )

	PORT_START ("DSWA")
	PORT_DIPNAME( 0x03, 0x00, "Play Mode" )
	PORT_DIPSETTING(    0x00, DEF_STR( Normal ) )
	PORT_DIPSETTING(    0x03, "Tournament" )
	PORT_BIT( 0xfc, IP_ACTIVE_LOW, IPT_UNKNOWN )

	PORT_START ("DSWB")
	PORT_DIPNAME( 0x03, 0x03, DEF_STR( Coinage ) )
	PORT_DIPSETTING(    0x03, DEF_STR( 1C_1C ) )
	PORT_DIPSETTING(    0x02, DEF_STR( 1C_2C ) )
	PORT_DIPSETTING(    0x01, DEF_STR( 1C_3C ) )
	PORT_DIPSETTING(    0x00, DEF_STR( 1C_4C ) )
	PORT_BIT( 0xfc, IP_ACTIVE_LOW, IPT_UNKNOWN )

	PORT_START ("DSWC")
	PORT_BIT( 0xff, IP_ACTIVE_HIGH, IPT_UNKNOWN )

	PORT_START( "EEPROMIN" )
	PORT_BIT( 0x01, IP_ACTIVE_HIGH, IPT_SPECIAL ) PORT_READ_LINE_DEVICE("eeprom", eeprom_read_bit)

	PORT_START( "EEPROMOUT" )
	PORT_BIT( 0x01, IP_ACTIVE_HIGH, IPT_OUTPUT ) PORT_WRITE_LINE_DEVICE("eeprom", eeprom_write_bit)
	PORT_BIT( 0x40, IP_ACTIVE_HIGH, IPT_OUTPUT ) PORT_WRITE_LINE_DEVICE("eeprom", eeprom_set_clock_line)
	PORT_BIT( 0x80, IP_ACTIVE_HIGH, IPT_OUTPUT ) PORT_WRITE_LINE_DEVICE("eeprom", eeprom_set_cs_line)
INPUT_PORTS_END

static INPUT_PORTS_START( cawingbl )
	PORT_INCLUDE( cawing )

	PORT_MODIFY("DSWA")
	CPS1_COINAGE_1("SW(A)")
	PORT_DIPUNUSED( 0x80, IP_ACTIVE_LOW )		/* This switch is not documented */
INPUT_PORTS_END


/* --- MACHINE DRIVER --- */
static MACHINE_RESET( fcrash )
{
	cps_state *state = machine->driver_data<cps_state>();

	state->sample_buffer1 = 0;
	state->sample_buffer2 = 0;
	state->sample_select1 = 0;
	state->sample_select2 = 0;
}

static MACHINE_START( common )
{
	cps_state *state = machine->driver_data<cps_state>();

	state->maincpu = machine->device("maincpu");
	state->audiocpu = machine->device("audiocpu");
}

static MACHINE_START( msm5205snd )
{
	MACHINE_START_CALL( common );
	cps_state *state = machine->driver_data<cps_state>();

	state->msm_1 = machine->device("msm1");
	state->msm_2 = machine->device("msm2");
}

static MACHINE_START( fcrash )
{
	MACHINE_START_CALL( msm5205snd );
	cps_state *state = machine->driver_data<cps_state>();

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

static MACHINE_START( captcommb2 )
{
	MACHINE_START_CALL( msm5205snd );
	cps_state *state = machine->driver_data<cps_state>();

	UINT8 *src = memory_region(machine, "audiocpu");
	memory_configure_bank(machine, "bank1", 0, 16, &src[0x10000], 0x4000);

	state_save_register_global(machine, state->sample_buffer1);
	state_save_register_global(machine, state->sample_buffer2);
	state_save_register_global(machine, state->sample_select1);
	state_save_register_global(machine, state->sample_select2);
}

static MACHINE_START( sf2mdt )
{
	MACHINE_START_CALL( msm5205snd );
	cps_state *state = machine->driver_data<cps_state>();

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
	MACHINE_START_CALL( captcommb2 );
	cps_state *state = machine->driver_data<cps_state>();

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

static MACHINE_START( sf2m1 )
{
	MACHINE_START_CALL( common );
	cps_state *state = machine->driver_data<cps_state>();

	UINT8 *src = memory_region(machine, "audiocpu");
	memory_configure_bank(machine, "bank1", 0, 8, &src[0x10000], 0x4000);

	state->layer_enable_reg = 0x26;
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
	MACHINE_START_CALL( common );
	cps_state *state = machine->driver_data<cps_state>();

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

static MACHINE_START( sgyxz )
{
	MACHINE_START_CALL(kodb);
	cps_state *state = machine->driver_data<cps_state>();

	state->layer_scroll1x_offset = 0x40;
	state->layer_scroll2x_offset = 0x40;
	state->layer_scroll3x_offset = 0x40;

	memory_configure_bank(machine, "bank1", 0, 2, memory_region(machine, "audiocpu") + 0x10000, 0x4000);
}

static MACHINE_START( punipic )
{
	cps_state *state = machine->driver_data<cps_state>();

	state->layer_enable_reg = 0x12;
	state->layer_mask_reg[0] = 0x14;
	state->layer_mask_reg[1] = 0x16;
	state->layer_mask_reg[2] = 0x08;
	state->layer_mask_reg[3] = 0x0a;
	state->layer_scroll1x_offset = 0x46;
	state->layer_scroll3x_offset = 0x46;
	state->sprite_base = 0x00;
	state->sprite_list_end_marker = 0x8000;
	state->sprite_x_offset = 0x00;
}

static MACHINE_START( cawingbl )
{
	MACHINE_START_CALL(fcrash);
	cps_state *state = machine->driver_data<cps_state>();

	state->layer_enable_reg = 0x0c;
	state->layer_mask_reg[0] = 0x0a;
	state->layer_mask_reg[1] = 0x08;
	state->layer_mask_reg[2] = 0x06;
	state->layer_mask_reg[3] = 0x04;
	state->layer_scroll1x_offset = 0x3f;
	state->layer_scroll2x_offset = 0x3e;
	state->layer_scroll3x_offset = 0x41;
	state->sprite_base = 0x1000;
}

static MACHINE_START( dinopic )
{
	cps_state *state = machine->driver_data<cps_state>();

	state->layer_enable_reg = 0x0a;
	state->layer_mask_reg[0] = 0x0c;
	state->layer_mask_reg[1] = 0x0e;
	state->layer_mask_reg[2] = 0x00;
	state->layer_mask_reg[3] = 0x02;
	state->layer_scroll1x_offset = 0x40;
	state->layer_scroll2x_offset = 0x40;
	state->layer_scroll3x_offset = 0x40;
	state->sprite_base = 0x1000;
	state->sprite_list_end_marker = 0x8000;
	state->sprite_x_offset = 0x00;
}

static MACHINE_START( slampic )
{
	cps_state *state = machine->driver_data<cps_state>();

	state->layer_enable_reg = 0x16;
	state->layer_mask_reg[0] = 0x00;
	state->layer_mask_reg[1] = 0x02;
	state->layer_mask_reg[2] = 0x28;
	state->layer_mask_reg[3] = 0x2a;
	state->layer_scroll1x_offset = 0x40;
	state->layer_scroll2x_offset = 0x40;
	state->layer_scroll3x_offset = 0x40;
	state->sprite_base = 0x1000;
	state->sprite_list_end_marker = 0x8000;
	state->sprite_x_offset = 0x00;
}

static MACHINE_START( varthb )
{
	MACHINE_START_CALL( common );
	cps_state *state = machine->driver_data<cps_state>();

	UINT8 *src = memory_region(machine, "audiocpu");
	memory_configure_bank(machine, "bank1", 0, 8, &src[0x10000], 0x4000);

	state->layer_enable_reg = 0x2e;
	state->layer_mask_reg[0] = 0x26;
	state->layer_mask_reg[1] = 0x30;
	state->layer_mask_reg[2] = 0x28;
	state->layer_mask_reg[3] = 0x32;
	state->layer_scroll1x_offset = 0x40;
	state->layer_scroll2x_offset = 0x40;
	state->layer_scroll3x_offset = 0x40;
	state->sprite_base = 0x1000;
	state->sprite_list_end_marker = 0x8000;
	state->sprite_x_offset = 0x00;
}

/* *********************************************** FCRASH */
static MACHINE_DRIVER_START( fcrash )
	MDRV_DRIVER_DATA(cps_state)

	/* basic machine hardware */
	MDRV_CPU_ADD("maincpu", M68000, XTAL_10MHz )
	MDRV_CPU_PROGRAM_MAP(fcrash_map)
	MDRV_CPU_VBLANK_INT("screen", cps1_interrupt)
	MDRV_CPU_ADD("audiocpu", Z80, XTAL_4MHz )	/* ? */
	MDRV_CPU_PROGRAM_MAP(fcrash_soundmap)
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
	MDRV_SOUND_ADD("ym1", YM2203, XTAL_4MHz)	/* ? */
	MDRV_SOUND_ROUTE(0, "mono", 0.10)
	MDRV_SOUND_ROUTE(1, "mono", 0.10)
	MDRV_SOUND_ROUTE(2, "mono", 0.10)
	MDRV_SOUND_ROUTE(3, "mono", 1.0)
	MDRV_SOUND_ADD("ym2", YM2203, XTAL_4MHz)	/* ? */
	MDRV_SOUND_ROUTE(0, "mono", 0.10)
	MDRV_SOUND_ROUTE(1, "mono", 0.10)
	MDRV_SOUND_ROUTE(2, "mono", 0.10)
	MDRV_SOUND_ROUTE(3, "mono", 1.0)
	MDRV_SOUND_ADD("msm1", MSM5205, XTAL_375KHz)	/* ? */
	MDRV_SOUND_CONFIG(msm5205_interface1)
	MDRV_SOUND_ROUTE(ALL_OUTPUTS, "mono", 0.25)
	MDRV_SOUND_ADD("msm2", MSM5205, XTAL_375KHz)	/* ? */
	MDRV_SOUND_CONFIG(msm5205_interface2)
	MDRV_SOUND_ROUTE(ALL_OUTPUTS, "mono", 0.25)
MACHINE_DRIVER_END

/* ********************************************* KODB */
static MACHINE_DRIVER_START( kodb )
	MDRV_DRIVER_DATA(cps_state)

	/* basic machine hardware */
	MDRV_CPU_ADD("maincpu", M68000, XTAL_10MHz )
	MDRV_CPU_PROGRAM_MAP(fcrash_map)
	MDRV_CPU_VBLANK_INT("screen", cps1_interrupt)
	MDRV_CPU_ADD("audiocpu", Z80, XTAL_3_579545MHz)
	MDRV_CPU_PROGRAM_MAP(kodb_soundmap)
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
	MDRV_SOUND_ADD("2151", YM2151, XTAL_3_579545MHz)
	MDRV_SOUND_CONFIG(ym2151_config)
	MDRV_SOUND_ROUTE(0, "mono", 0.35)
	MDRV_SOUND_ROUTE(1, "mono", 0.35)
	MDRV_OKIM6295_ADD("oki", XTAL_1MHz, OKIM6295_PIN7_HIGH)
	MDRV_SOUND_ROUTE(ALL_OUTPUTS, "mono", 0.30)
MACHINE_DRIVER_END

/* ******************************************** SF2MDT */
static MACHINE_DRIVER_START( sf2mdt )
	MDRV_DRIVER_DATA(cps_state)

	/* basic machine hardware */
	MDRV_CPU_ADD("maincpu", M68000, XTAL_12MHz )
	MDRV_CPU_PROGRAM_MAP(sf2mdt_map)
	MDRV_CPU_VBLANK_INT("screen", irq4_line_hold)
	MDRV_CPU_ADD("audiocpu", Z80, XTAL_3_579545MHz)
	MDRV_CPU_PROGRAM_MAP(sf2mdt_soundmap)
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
	MDRV_PALETTE_LENGTH(0xc00)
	MDRV_VIDEO_START(cps1)

	/* sound hardware */
	MDRV_SPEAKER_STANDARD_MONO("mono")
	MDRV_SOUND_ADD("2151", YM2151, XTAL_3_579545MHz)
	MDRV_SOUND_ROUTE(0, "mono", 0.35)
	MDRV_SOUND_ROUTE(1, "mono", 0.35)
	MDRV_SOUND_ADD("msm1", MSM5205, XTAL_375KHz)
	MDRV_SOUND_CONFIG(msm5205_interface1)
	MDRV_SOUND_ROUTE(ALL_OUTPUTS, "mono", 0.25)
	MDRV_SOUND_ADD("msm2", MSM5205, XTAL_375KHz)
	MDRV_SOUND_CONFIG(msm5205_interface2)
	MDRV_SOUND_ROUTE(ALL_OUTPUTS, "mono", 0.25)
MACHINE_DRIVER_END

/* ******************************************* KNIGHTSB */
static MACHINE_DRIVER_START( knightsb )
	MDRV_DRIVER_DATA(cps_state)

	/* basic machine hardware */
	MDRV_CPU_ADD("maincpu", M68000, XTAL_10MHz )
	MDRV_CPU_PROGRAM_MAP(knightsb_map)
	MDRV_CPU_VBLANK_INT("screen", cps1_interrupt)
	MDRV_CPU_ADD("audiocpu", Z80, XTAL_3_727625MHz)
	MDRV_CPU_PROGRAM_MAP(knightsb_soundmap)
	MDRV_MACHINE_START(knightsb)
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

	/* sound hardware */
	MDRV_SPEAKER_STANDARD_MONO("mono")
	MDRV_SOUND_ADD("2151", YM2151, XTAL_3_727625MHz)
	MDRV_SOUND_CONFIG(ym2151_config)
	MDRV_SOUND_ROUTE(0, "mono", 0.35)
	MDRV_SOUND_ROUTE(1, "mono", 0.35)
	MDRV_SOUND_ADD("msm1", MSM5205, XTAL_375KHz)	/* ? */
	MDRV_SOUND_CONFIG(msm5205_interface1)
	MDRV_SOUND_ROUTE(ALL_OUTPUTS, "mono", 0.25)
	MDRV_SOUND_ADD("msm2", MSM5205, XTAL_375KHz)	/* ? */
	MDRV_SOUND_CONFIG(msm5205_interface2)
	MDRV_SOUND_ROUTE(ALL_OUTPUTS, "mono", 0.25)
MACHINE_DRIVER_END

/* *********************************************** SF2M1 */
static MACHINE_DRIVER_START( sf2m1 )
	MDRV_DRIVER_DATA(cps_state)

	/* basic machine hardware */
	MDRV_CPU_ADD("maincpu", M68000, XTAL_12MHz )
	MDRV_CPU_PROGRAM_MAP(sf2m1_map)
	MDRV_CPU_VBLANK_INT("screen", cps1_interrupt)
	MDRV_CPU_ADD("audiocpu", Z80, XTAL_3_579545MHz)
	MDRV_CPU_PROGRAM_MAP(sf2m1_soundmap)
	MDRV_MACHINE_START(sf2m1)

	/* video hardware */
	MDRV_SCREEN_ADD("screen", RASTER)
	MDRV_SCREEN_FORMAT(BITMAP_FORMAT_INDEXED16)
	MDRV_SCREEN_RAW_PARAMS(CPS_PIXEL_CLOCK, CPS1_HTOTAL, CPS_HBEND, CPS_HBSTART, CPS1_VTOTAL, CPS_VBEND, CPS_VBSTART)
	MDRV_VIDEO_UPDATE(bootleg_updatescreen)
	MDRV_VIDEO_EOF(cps1)
	MDRV_GFXDECODE(cps1)
	MDRV_PALETTE_LENGTH(0xc00)
	MDRV_VIDEO_START(cps1)

	/* sound hardware */
	MDRV_SPEAKER_STANDARD_MONO("mono")
	MDRV_SOUND_ADD("2151", YM2151, XTAL_3_579545MHz)
	MDRV_SOUND_CONFIG(ym2151_config)
	MDRV_SOUND_ROUTE(0, "mono", 0.35)
	MDRV_SOUND_ROUTE(1, "mono", 0.35)
	MDRV_OKIM6295_ADD("oki", XTAL_1MHz, OKIM6295_PIN7_HIGH)
	MDRV_SOUND_ROUTE(ALL_OUTPUTS, "mono", 0.30)
MACHINE_DRIVER_END

/* *********************************************** SGYXZ */
static MACHINE_DRIVER_START( sgyxz )
	MDRV_DRIVER_DATA(cps_state)

	/* basic machine hardware */
	MDRV_CPU_ADD("maincpu", M68000, XTAL_12MHz )
	MDRV_CPU_PROGRAM_MAP(sgyxz_map)
	MDRV_CPU_VBLANK_INT("screen", cps1_interrupt)
	MDRV_CPU_ADD("audiocpu", Z80, XTAL_3_579545MHz)
	MDRV_CPU_PROGRAM_MAP(sf2m1_soundmap)
	MDRV_MACHINE_START(sgyxz)

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

	MDRV_EEPROM_ADD("eeprom", bootleg_eeprom_interface)
	/* sound hardware */
	MDRV_SPEAKER_STANDARD_MONO("mono")
	MDRV_SOUND_ADD("2151", YM2151, XTAL_3_579545MHz)
	MDRV_SOUND_CONFIG(ym2151_config)
	MDRV_SOUND_ROUTE(0, "mono", 0.35)
	MDRV_SOUND_ROUTE(1, "mono", 0.35)
	MDRV_OKIM6295_ADD("oki", XTAL_1MHz, OKIM6295_PIN7_HIGH)
	MDRV_SOUND_ROUTE(ALL_OUTPUTS, "mono", 0.30)
MACHINE_DRIVER_END

/* *********************************************** PUNIPIC, PUNIPIC2, PUNIPIC3 */
static MACHINE_DRIVER_START( punipic )
	MDRV_DRIVER_DATA(cps_state)

	/* basic machine hardware */
	MDRV_CPU_ADD("maincpu", M68000, XTAL_12MHz )
	MDRV_CPU_PROGRAM_MAP(punipic_map)
	MDRV_CPU_VBLANK_INT("screen", cps1_interrupt)
	MDRV_MACHINE_START(punipic)

	MDRV_EEPROM_ADD("eeprom", bootleg_eeprom_interface)

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
	MDRV_OKIM6295_ADD("oki", XTAL_1MHz, OKIM6295_PIN7_HIGH)
	MDRV_SOUND_ROUTE(ALL_OUTPUTS, "mono", 0.30)
MACHINE_DRIVER_END

/* *********************************************** CAWINGBL, CAWINGB2 */
static MACHINE_DRIVER_START( cawingbl )
	MDRV_IMPORT_FROM(fcrash)

	MDRV_CPU_MODIFY("maincpu")
	MDRV_CPU_VBLANK_INT("screen", irq6_line_hold)	 /* needed to write to scroll values */

	MDRV_MACHINE_START(cawingbl)
MACHINE_DRIVER_END

/* *********************************************** DINOPIC, DINOPIC2 */
static MACHINE_DRIVER_START( dinopic )
	MDRV_DRIVER_DATA(cps_state)

	/* basic machine hardware */
	MDRV_CPU_ADD("maincpu", M68000, XTAL_12MHz )
	MDRV_CPU_PROGRAM_MAP(dinopic_map)
	MDRV_CPU_VBLANK_INT("screen", cps1_interrupt)
	MDRV_MACHINE_START(dinopic)

	MDRV_EEPROM_ADD("eeprom", bootleg_eeprom_interface)

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
	MDRV_OKIM6295_ADD("oki", XTAL_1MHz, OKIM6295_PIN7_HIGH)
	MDRV_SOUND_ROUTE(ALL_OUTPUTS, "mono", 0.30)
MACHINE_DRIVER_END

/* *********************************************** SLAMPIC */
static MACHINE_DRIVER_START( slampic )
	MDRV_DRIVER_DATA(cps_state)

	/* basic machine hardware */
	MDRV_CPU_ADD("maincpu", M68000, XTAL_12MHz )
	MDRV_CPU_PROGRAM_MAP(slampic_map)
	MDRV_CPU_VBLANK_INT("screen", cps1_interrupt)
	MDRV_MACHINE_START(slampic)

	MDRV_EEPROM_ADD("eeprom", bootleg_eeprom_interface)

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
	MDRV_OKIM6295_ADD("oki", XTAL_1MHz, OKIM6295_PIN7_HIGH)
	MDRV_SOUND_ROUTE(ALL_OUTPUTS, "mono", 0.30)
MACHINE_DRIVER_END


/* *********************************************** CAPTCOMMB2 */
static MACHINE_DRIVER_START( captcommb2 )
	MDRV_DRIVER_DATA(cps_state)

	/* basic machine hardware */
	MDRV_CPU_ADD("maincpu", M68000, XTAL_10MHz )
	MDRV_CPU_PROGRAM_MAP(captcommb2_map)
	MDRV_CPU_VBLANK_INT("screen", cps1_interrupt)
	MDRV_CPU_ADD("audiocpu", Z80, XTAL_3_579545MHz)
	MDRV_CPU_PROGRAM_MAP(sf2mdt_soundmap)
	MDRV_MACHINE_START(captcommb2)
	MDRV_MACHINE_RESET(fcrash)

	/* video hardware */
	MDRV_SCREEN_ADD("screen", RASTER)
	MDRV_SCREEN_REFRESH_RATE(60)
	MDRV_SCREEN_VBLANK_TIME(ATTOSECONDS_IN_USEC(0))
	MDRV_SCREEN_FORMAT(BITMAP_FORMAT_INDEXED16)
	MDRV_SCREEN_SIZE(64*8, 32*8)
	MDRV_SCREEN_VISIBLE_AREA(8*8, (64-8)*8-1, 2*8, 30*8-1 )
	MDRV_VIDEO_UPDATE(cps1)
	MDRV_VIDEO_EOF(cps1)
	MDRV_GFXDECODE(cps1)
	MDRV_PALETTE_LENGTH(0xc00)
	MDRV_VIDEO_START(cps1)

	/* sound hardware */
	MDRV_SPEAKER_STANDARD_MONO("mono")
	MDRV_SOUND_ADD("2151", YM2151, XTAL_3_579545MHz)
	MDRV_SOUND_ROUTE(0, "mono", 0.35)
	MDRV_SOUND_ROUTE(1, "mono", 0.35)
	MDRV_SOUND_ADD("msm1", MSM5205, XTAL_375KHz)
	MDRV_SOUND_CONFIG(msm5205_interface1)
	MDRV_SOUND_ROUTE(ALL_OUTPUTS, "mono", 0.25)
	MDRV_SOUND_ADD("msm2", MSM5205, XTAL_375KHz)
	MDRV_SOUND_CONFIG(msm5205_interface2)
	MDRV_SOUND_ROUTE(ALL_OUTPUTS, "mono", 0.25)
MACHINE_DRIVER_END


/* *********************************************** VARTHB */
static MACHINE_DRIVER_START( varthb )
	MDRV_DRIVER_DATA(cps_state)

	/* basic machine hardware */
	MDRV_CPU_ADD("maincpu", M68000, XTAL_12MHz )
	MDRV_CPU_PROGRAM_MAP(varthb_map)
	MDRV_CPU_VBLANK_INT("screen", cps1_interrupt)
	MDRV_CPU_ADD("audiocpu", Z80, XTAL_3_579545MHz)
	MDRV_CPU_PROGRAM_MAP(sf2m1_soundmap)
	MDRV_MACHINE_START(varthb)

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
	MDRV_SOUND_ADD("2151", YM2151, XTAL_3_579545MHz)
	MDRV_SOUND_CONFIG(ym2151_config)
	MDRV_SOUND_ROUTE(0, "mono", 0.35)
	MDRV_SOUND_ROUTE(1, "mono", 0.35)
	MDRV_OKIM6295_ADD("oki", XTAL_1MHz, OKIM6295_PIN7_HIGH)
	MDRV_SOUND_ROUTE(ALL_OUTPUTS, "mono", 0.30)
MACHINE_DRIVER_END


/* --- DRIVER INIT --- */
static DRIVER_INIT( sf2mdta )
{
	cps_state *state = machine->driver_data<cps_state>();

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
	cps_state *state = machine->driver_data<cps_state>();
	UINT8 *src = (UINT8 *)memory_region( machine, "maincpu" );

	memory_install_read_port(cputag_get_address_space(machine, "maincpu", ADDRESS_SPACE_PROGRAM), 0x800000, 0x800007, 0, 0, "IN1");
	memory_install_read16_handler(cputag_get_address_space(machine, "maincpu", ADDRESS_SPACE_PROGRAM), 0x800018, 0x80001f, 0, 0, cps1_dsw_r);
	memory_install_write16_handler(cputag_get_address_space(machine, "maincpu", ADDRESS_SPACE_PROGRAM), 0x800180, 0x800187, 0, 0, cps1_soundlatch_w);
	memory_install_write16_handler(cputag_get_address_space(machine, "maincpu", ADDRESS_SPACE_PROGRAM), 0x980000, 0x98002f, 0, 0, kodb_layer_w);
	/* the original game alternates between 2 sprite ram areas to achieve flashing sprites - the bootleg doesn't do the write to the register to achieve this
	mapping both sprite ram areas to the same bootleg sprite ram - similar to how sf2mdt works */
	state->bootleg_sprite_ram = (UINT16 *)memory_install_ram(cputag_get_address_space(machine, "maincpu", ADDRESS_SPACE_PROGRAM), 0x900000, 0x903fff, 0, 0, NULL);
	memory_install_ram(cputag_get_address_space(machine, "maincpu", ADDRESS_SPACE_PROGRAM), 0x904000, 0x907fff, 0, 0, state->bootleg_sprite_ram);	/* both of these need to be mapped */

	src[0x0953] = 0x07;	/* fixes sprite ram clearing issue. */

	DRIVER_INIT_CALL(cps1);
}

static DRIVER_INIT( dinopic )
{
	cps_state *state = machine->driver_data<cps_state>();
	state->bootleg_sprite_ram = (UINT16 *)memory_install_ram(cputag_get_address_space(machine, "maincpu", ADDRESS_SPACE_PROGRAM), 0x990000, 0x993fff, 0, 0, NULL);

	DRIVER_INIT_CALL(cps1);
}

static DRIVER_INIT( sf2m1 )
{
	UINT16 *src = (UINT16 *)memory_region( machine, "maincpu" );
	src[0x064e / 2] = 0x6046;

	DRIVER_INIT_CALL(dinopic);
}

static DRIVER_INIT( punipic )
{
	UINT16 *src = (UINT16 *)memory_region( machine, "maincpu" );
	src[0x05a8 / 2] = 0x4e71;
	src[0x4df0 / 2] = 0x33ed;
	src[0x4df2 / 2] = 0xdb2e;
	src[0x4df4 / 2] = 0x0080;
	src[0x4df6 / 2] = 0x0152;
	src[0x4df8 / 2] = 0x4e75;

	DRIVER_INIT_CALL(dinopic);
}

static DRIVER_INIT( punipic3 )
{
	UINT16 *src = (UINT16 *)memory_region( machine, "maincpu" );
	src[0x05a6 / 2] = 0x4e71;
	src[0x05a8 / 2] = 0x4e71;

	DRIVER_INIT_CALL(dinopic);
}

static DRIVER_INIT( cawingbl )
{
	memory_install_read_port(cputag_get_address_space(machine, "maincpu", ADDRESS_SPACE_PROGRAM), 0x882000, 0x882001, 0, 0, "IN1");
	memory_install_write16_handler(cputag_get_address_space(machine, "maincpu", ADDRESS_SPACE_PROGRAM), 0x882006, 0x882007, 0, 0, sf2mdt_soundlatch_w);
	memory_install_read16_handler(cputag_get_address_space(machine, "maincpu", ADDRESS_SPACE_PROGRAM), 0x882008, 0x88200f, 0, 0, cps1_dsw_r);

	DRIVER_INIT_CALL(cps1);
}

static DRIVER_INIT( fcrash )
{
	UINT8 *src = (UINT8 *)memory_region( machine, "maincpu" );
	src[0x2611] = 0x07;	/* This fixes sprite ram clearing */

	DRIVER_INIT_CALL(cps1);
}


/* --- LOAD ROM --- */

ROM_START( captcommb2 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )      /* 68000 code */
	ROM_LOAD16_BYTE( "5.bin",   0x000000, 0x80000, CRC(c3a6ed28) SHA1(f79fed35f7b0dc383837a2ead846acc686dd3487) )
	ROM_LOAD16_BYTE( "4.bin",   0x000001, 0x80000, CRC(28729335) SHA1(6dd23c2d41e4e182434fe80c03d5c90785e6c0ce) )
	ROM_LOAD16_BYTE( "3.bin",   0x100000, 0x40000, CRC(1b526d73) SHA1(3dd8dec61db4f4f5546937602a8fb01c639d72f8) )
	ROM_LOAD16_BYTE( "2.bin",   0x100001, 0x40000, CRC(73c99709) SHA1(e122e3771b698c44fb998589af0542b1f2a3876a) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROMX_LOAD( "cap.bin",     0x000000, 0x80000, CRC(7261d8ba) SHA1(4b66292e42d20d0b79a756f0e445492ddb9c6bbc) , ROM_GROUPWORD | ROM_SKIP(6) )	// == cc-5m.3a
	ROMX_LOAD( "cbp.bin",     0x000002, 0x80000, CRC(6a60f949) SHA1(87391ff92abaf3e451f70d789a938cffbd1fd222) , ROM_GROUPWORD | ROM_SKIP(6) )	// == cc-7m.5a
	ROMX_LOAD( "ccp.bin",     0x000004, 0x80000, CRC(00637302) SHA1(2c554b59cceec2de67a9a4bc6281fe846d3c8cd2) , ROM_GROUPWORD | ROM_SKIP(6) )	// == cc-1m.4a
	ROMX_LOAD( "cdp.bin",     0x000006, 0x80000, CRC(cc87cf61) SHA1(7fb1f49494cc1a08aded20754bb0cefb1c323198) , ROM_GROUPWORD | ROM_SKIP(6) )	// == cc-3m.6a
	ROMX_LOAD( "cai.bin",     0x200000, 0x80000, CRC(28718bed) SHA1(dfdc4dd14dc609783bad94d608a9e9b137dea944) , ROM_GROUPWORD | ROM_SKIP(6) )	// == cc-6m.7a
	ROMX_LOAD( "cbi.bin",     0x200002, 0x80000, CRC(d4acc53a) SHA1(d03282ebbde362e679cc97f772aa9baf163d7606) , ROM_GROUPWORD | ROM_SKIP(6) )	// == cc-8m.9a
	ROMX_LOAD( "cci.bin",     0x200004, 0x80000, CRC(0c69f151) SHA1(a170b8e568439e4a26d84376d53560e4248e4e2f) , ROM_GROUPWORD | ROM_SKIP(6) )	// == cc-2m.8a
	ROMX_LOAD( "cdi.bin",     0x200006, 0x80000, CRC(1f9ebb97) SHA1(023d00cb7b6a52d1b29e2052abe08ef34cb0c55c) , ROM_GROUPWORD | ROM_SKIP(6) )	// == cc-4m.10a

	ROM_REGION( 0x50000, "audiocpu", 0 ) /* Sound program + samples  */
	ROM_LOAD( "1.bin",        0x00000, 0x40000, CRC(aed2f4bd) SHA1(3bd567dc350bf6ac3a349548790ad49eb5bd8307) )
	ROM_RELOAD(               0x10000, 0x40000 )
ROM_END

ROM_START( cawingbl )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )      /* 68000 code */
	ROM_LOAD16_BYTE( "caw2.bin",    0x00000, 0x80000, CRC(8125d3f0) SHA1(a0e48c326c6164ca189c9372f5c38a7c103772c1) )
	ROM_LOAD16_BYTE( "caw1.bin",    0x00001, 0x80000, CRC(b19b10ce) SHA1(3c71f1dc830d1e8b8ba26d8a71e12f477659480c) )

	ROM_REGION( 0x200000, "gfx", 0 )
	ROMX_LOAD( "caw7.bin", 0x000000, 0x80000, CRC(a045c689) SHA1(8946c55635121282ea03586a278e50de20d92633) , ROM_SKIP(3) )
	ROMX_LOAD( "caw6.bin", 0x000001, 0x80000, CRC(61192f7c) SHA1(86643c62653a62a5c7541d50cfdecae9b607440d) , ROM_SKIP(3) )
	ROMX_LOAD( "caw5.bin", 0x000002, 0x80000, CRC(30dd78db) SHA1(e0295001d6f5fb4a9276c432f971e88f73c5e39a) , ROM_SKIP(3) )
	ROMX_LOAD( "caw4.bin", 0x000003, 0x80000, CRC(4937fc41) SHA1(dac179715be483a521df8e515afc1fb7a2cd8f13) , ROM_SKIP(3) )

	ROM_REGION( 0x30000, "audiocpu", 0 ) /* 64k for the audio CPU (+banks) */
	ROM_LOAD( "caw3.bin",  0x00000, 0x20000, CRC(ffe16cdc) SHA1(8069ea69f0b89d61c35995c8040a4989d7be9c1f) )
	ROM_RELOAD(            0x10000, 0x20000 )
ROM_END

ROM_START( cawingb2 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )      /* 68000 code */
	ROM_LOAD16_BYTE( "8.8",    0x00000, 0x20000, CRC(f655708c) SHA1(9962a1c96ea08bc71b25d4f58e5d1fb1beebf0dc) )
	ROM_LOAD16_BYTE( "4.4",    0x00001, 0x20000, CRC(a02fb5aa) SHA1(c9c064a83899c48f681ac803cfc5886503b9d992) )
	ROM_LOAD16_BYTE( "7.7",    0x40000, 0x20000, CRC(8c6c7430) SHA1(3ed5713caf774b050b41a6adea026e1307b570df) )
	ROM_LOAD16_BYTE( "3.3",    0x40001, 0x20000, CRC(f585bf2c) SHA1(3a3169791f8deace8d9bee1adb08f19fbcd309c6) )
	ROM_LOAD16_BYTE( "6.6",    0x80000, 0x20000, CRC(5fda906e) SHA1(7b3ef17d494a2f92e58ab7e34a3beaad8c149fca) )
	ROM_LOAD16_BYTE( "2.2",    0x80001, 0x20000, CRC(736c1835) SHA1(a91f479fab30603a111304adc0478d430faa80fc) )
	ROM_LOAD16_BYTE( "5.5",    0xc0000, 0x20000, CRC(76458083) SHA1(cbb4ef5f7615c834b2ee1ad3c86e7262f2f62c01) )
	ROM_LOAD16_BYTE( "1.1",    0xc0001, 0x20000, CRC(d3523f34) SHA1(005ea378c2b78782f85ecc591946c027ca2ca023) )

	ROM_REGION( 0x200000, "gfx", 0 )
	ROMX_LOAD( "17.17",     0x000000, 0x20000, CRC(0b538062) SHA1(ac6e5dc82efdca311adfe6e6cdda160ad4a0d04d) , ROM_SKIP(3) )
	ROMX_LOAD( "19.19",     0x000001, 0x20000, CRC(3ad62311) SHA1(1c132696b55191d16af30ebd36d2320d979eab36) , ROM_SKIP(3) )
	ROMX_LOAD( "21.21",     0x000002, 0x20000, CRC(1b872a98) SHA1(7a3f72c6d384dfa8e224f93604997a7b6e5c8926) , ROM_SKIP(3) )
	ROMX_LOAD( "23.23",     0x000003, 0x20000, CRC(ad49eecd) SHA1(39909996765391ed734a02c74f683e1bd9ce1561) , ROM_SKIP(3) )
	ROMX_LOAD( "9.9",       0x080000, 0x20000, CRC(8cd4df5b) SHA1(771b6d6a6baa95a669335fe64e2219fe7226e140) , ROM_SKIP(3) )
	ROMX_LOAD( "11.11",     0x080001, 0x20000, CRC(bf14418a) SHA1(7a0e1c65b8825a252338d6c1db59a88966ec6cfb) , ROM_SKIP(3) )
	ROMX_LOAD( "13.13",     0x080002, 0x20000, CRC(cef1aab8) SHA1(677a889b939ff00e95737a4a53053744bb6744c0) , ROM_SKIP(3) )
	ROMX_LOAD( "15.15",     0x080003, 0x20000, CRC(397725dc) SHA1(9450362bbf2f91b4225a088d6e283d7b16407b74) , ROM_SKIP(3) )
	ROMX_LOAD( "18.18",     0x100000, 0x20000, CRC(9b14f7ed) SHA1(72b6e1174d4faab487261aa6739de842d2423e1a) , ROM_SKIP(3) )
	ROMX_LOAD( "20.20",     0x100001, 0x20000, CRC(59bcc1bb) SHA1(c725060e068294dea1d962c54a9018050fa70297) , ROM_SKIP(3) )
	ROMX_LOAD( "22.22",     0x100002, 0x20000, CRC(23dc647a) SHA1(2d8d4c4c7b2d0616430360d1639b07216dd731d6) , ROM_SKIP(3) )
	ROMX_LOAD( "24.24",     0x100003, 0x20000, CRC(eda9fa6b) SHA1(4a3510ce71b015a1ea568fd0bbe61c5c093a2fbf) , ROM_SKIP(3) )
	ROMX_LOAD( "10.10",     0x180000, 0x20000, CRC(17174249) SHA1(71c6424ab4629065dd6af8bb47b18f5b5d0fbe49) , ROM_SKIP(3) )
	ROMX_LOAD( "12.12",     0x180001, 0x20000, CRC(490440b2) SHA1(2597bf16340308f84b32cfa048c426db571b4a35) , ROM_SKIP(3) )
	ROMX_LOAD( "14.14",     0x180002, 0x20000, CRC(344a8270) SHA1(fdb588a7ba60783225e3b5c72446f79625de4f9c) , ROM_SKIP(3) )
	ROMX_LOAD( "16.16",     0x180003, 0x20000, CRC(b991ad91) SHA1(5c59131ddf068cb54d23f8836293360fbc967d58) , ROM_SKIP(3) )

	ROM_REGION( 0x30000, "audiocpu", 0 ) /* 64k for the audio CPU (+banks) */
	ROM_LOAD( "5.a",       0x00000, 0x20000, CRC(ffe16cdc) SHA1(8069ea69f0b89d61c35995c8040a4989d7be9c1f) )
	ROM_RELOAD(            0x10000, 0x20000 )
ROM_END

/*
Cadillac Bootleg Hardware:

1x 68000p10
1x PIC16c57
1x AD-65
1x OSC 30mhz
1x OSC 24mhz
13x 27c4000 ROMS
*/
ROM_START( dinopic )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )		/* 68000 code */
	ROM_LOAD16_BYTE( "3.bin",      0x000001, 0x80000, CRC(13dfeb08) SHA1(cd2f9dd64f4fabe93901247e36dff3763169716d) )
	ROM_LOAD16_BYTE( "5.bin",      0x000000, 0x80000, CRC(96dfcbf1) SHA1(a8bda6edae2c1b79db7ae8a8976fd2457f874373) )
	ROM_LOAD16_BYTE( "2.bin",      0x100001, 0x80000, CRC(0e4058ba) SHA1(346f9e34ea53dd1bf5cdafa1e38bf2edb09b9a7f) )
	ROM_LOAD16_BYTE( "7.bin",      0x100000, 0x80000, CRC(6133f349) SHA1(d13af99910623f62c090d25372a2253dbc2f8cbe) )

	ROM_REGION( 0x400000, "gfx", 0 )		// same data, different format, except for 8 which is a 99% match (bad rom?)
	ROMX_LOAD( "4.bin",   0x000000, 0x40000, CRC(f3c2c98d) SHA1(98ae51a67fa4159456a4a205eebdd8d1775888d1), ROM_SKIP(7) )
	ROM_CONTINUE(         0x000004, 0x40000)
	ROMX_LOAD( "8.bin",   0x000001, 0x40000, CRC(d574befc) SHA1(56482e7a9aa8439f30e3cf72311495ce677a083d), ROM_SKIP(7) )
	ROM_CONTINUE(         0x000005, 0x40000)
	ROMX_LOAD( "9.bin",   0x000002, 0x40000, CRC(55ef0adc) SHA1(3b5551ae76ae80882d37fc70a1031a57885d6840), ROM_SKIP(7) )
	ROM_CONTINUE(         0x000006, 0x40000)
	ROMX_LOAD( "6.bin",   0x000003, 0x40000, CRC(cc0805fc) SHA1(c512734c28b878a30a0de249929f69784d5d77a1), ROM_SKIP(7) )
	ROM_CONTINUE(         0x000007, 0x40000)
	ROMX_LOAD( "13.bin",  0x200000, 0x40000, CRC(1371f714) SHA1(d2c98096fab08e3d4fd2482e6ebfc970ead656ee), ROM_SKIP(7) )
	ROM_CONTINUE(         0x200004, 0x40000)
	ROMX_LOAD( "12.bin",  0x200001, 0x40000, CRC(b284c4a7) SHA1(166f571e0afa115f8e38ba427b40e30abcfd70ee), ROM_SKIP(7) )
	ROM_CONTINUE(         0x200005, 0x40000)
	ROMX_LOAD( "11.bin",  0x200002, 0x40000, CRC(b7ad3394) SHA1(58dec34d9d991ff2817c8a7847749716abae6c77), ROM_SKIP(7) )
	ROM_CONTINUE(         0x200006, 0x40000)
	ROMX_LOAD( "10.bin",  0x200003, 0x40000, CRC(88847705) SHA1(05dc90067921960e417b7436056a5e1f86abaa1a), ROM_SKIP(7) )
	ROM_CONTINUE(         0x200007, 0x40000)

	ROM_REGION( 0x28000, "audiocpu", 0 )		/* PIC16c57 - protected, dump isn't valid */
	ROM_LOAD( "pic16c57-rp", 0x00000, 0x2d4c, BAD_DUMP CRC(5a6d393c) SHA1(1391a1590aff5f75bb6fae1c83eddb796b53135d) )

	ROM_REGION( 0x80000, "oki", 0 )			/* OKI6295 samples */
	ROM_LOAD( "1.bin",      0x000000, 0x80000,  CRC(7d921309) SHA1(d51e60e904d302c2516b734189e141aa171b2b82) )
ROM_END

/* this is basically the same set as above, from a different bootleg pcb, with a few extra pal dumps etc.
   the first dump will probably be removed eventually

  CPU
  1x TS68000CP10 (main)
  1x AD-65 (sound)(equivalent to M6295)
  1x PIC16C57-XT/P
  1x A1020B-PL84C
  1x oscillator 24.000MHz (close to main)
  1x oscillator 30.000MHz (close to sound)

  ROMs
  13x 27C4000
  3x GAL20V8A
  3x PALCE16V8H (1 broken not dumped)
  1x CAT93C46P

  Note
  1x JAMMA edge connector
  1x 10 legs connector
  1x trimmer (volume)
*/
ROM_START( dinopic2 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )		/* 68000 code */
	ROM_LOAD16_BYTE( "27c4000-m12374r-2.bin",      0x000001, 0x80000, CRC(13dfeb08) SHA1(cd2f9dd64f4fabe93901247e36dff3763169716d) )
	ROM_LOAD16_BYTE( "27c4000-m12481.bin",         0x000000, 0x80000, CRC(96dfcbf1) SHA1(a8bda6edae2c1b79db7ae8a8976fd2457f874373) )
	ROM_LOAD16_BYTE( "27c4000-m12374r-1.bin",      0x100001, 0x80000, CRC(0e4058ba) SHA1(346f9e34ea53dd1bf5cdafa1e38bf2edb09b9a7f) )
	ROM_LOAD16_BYTE( "27c4000-m12374r-3.bin",      0x100000, 0x80000, CRC(6133f349) SHA1(d13af99910623f62c090d25372a2253dbc2f8cbe) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROMX_LOAD( "27c4000-m12481-4.bin",   0x000000, 0x40000, CRC(f3c2c98d) SHA1(98ae51a67fa4159456a4a205eebdd8d1775888d1), ROM_SKIP(7) )
	ROM_CONTINUE(                        0x000004, 0x40000)
	ROMX_LOAD( "27c4000-m12481-3.bin",   0x000001, 0x40000, BAD_DUMP CRC(d574befc) SHA1(56482e7a9aa8439f30e3cf72311495ce677a083d), ROM_SKIP(7) ) // this one is a perfect match, unlike dinopic set
	ROM_CONTINUE(                        0x000005, 0x40000)
	ROMX_LOAD( "27c4000-m12481-2.bin",   0x000002, 0x40000, CRC(55ef0adc) SHA1(3b5551ae76ae80882d37fc70a1031a57885d6840), ROM_SKIP(7) )
	ROM_CONTINUE(                        0x000006, 0x40000)
	ROMX_LOAD( "27c4000-m12481-1.bin",   0x000003, 0x40000, CRC(cc0805fc) SHA1(c512734c28b878a30a0de249929f69784d5d77a1), ROM_SKIP(7) )
	ROM_CONTINUE(                        0x000007, 0x40000)
	ROMX_LOAD( "27c4000-m12481-8.bin",   0x200000, 0x40000, CRC(1371f714) SHA1(d2c98096fab08e3d4fd2482e6ebfc970ead656ee), ROM_SKIP(7) )
	ROM_CONTINUE(                        0x200004, 0x40000)
	ROMX_LOAD( "27c4000-m12481-7.bin",   0x200001, 0x40000, CRC(b284c4a7) SHA1(166f571e0afa115f8e38ba427b40e30abcfd70ee), ROM_SKIP(7) )
	ROM_CONTINUE(                        0x200005, 0x40000)
	ROMX_LOAD( "27c4000-m12481-6.bin",   0x200002, 0x40000, CRC(b7ad3394) SHA1(58dec34d9d991ff2817c8a7847749716abae6c77), ROM_SKIP(7) )
	ROM_CONTINUE(                        0x200006, 0x40000)
	ROMX_LOAD( "27c4000-m12481-5.bin",   0x200003, 0x40000, CRC(88847705) SHA1(05dc90067921960e417b7436056a5e1f86abaa1a), ROM_SKIP(7) )
	ROM_CONTINUE(                        0x200007, 0x40000)

	ROM_REGION( 0x28000, "audiocpu", 0 )		/* PIC16c57 - protected, dump isn't valid */
	ROM_LOAD( "pic16c57-xt.hex", 0x00000, 0x26cc, BAD_DUMP CRC(a6a5eac4) SHA1(2039789084836769180f0bfd230c2553a37e2aaf) )

	ROM_REGION( 0x80000, "oki", 0 )			/* OKI6295 samples */
	ROM_LOAD( "27c4000-m12623.bin",      0x000000, 0x80000,  CRC(7d921309) SHA1(d51e60e904d302c2516b734189e141aa171b2b82) )

	ROM_REGION( 0xc00, "user1", 0 )			/* extra bits on this set */
	ROM_LOAD( "cat93c46p.bin",       0x000, 0x080,  CRC(d49fa351) SHA1(e6dfaff1c6aa962d34ae8e82b71e6f394d82e19c) )
	ROM_LOAD( "gal20v8a-1.bin",      0x200, 0x157,  CRC(cd99ca47) SHA1(ee1d990fd294aa46f56f31264134251569f6792e) )
	ROM_LOAD( "gal20v8a-2.bin",      0x400, 0x157,  CRC(60d016b9) SHA1(add42c763c819f3fe6d7cf3adc7123a52c2a3be9) )
	ROM_LOAD( "gal20v8a-3.bin",      0x600, 0x157,  CRC(049b7f4f) SHA1(6c6ea03d9a293db69a8bd10e042ee75e3c01313c) )
	ROM_LOAD( "palce16v8h-1.bin",    0x800, 0x117,  CRC(48253c66) SHA1(8c94e655b768c45c3edf6ef39e62e3b7a4e57530) )
	ROM_LOAD( "palce16v8h-2.bin",    0xa00, 0x117,  CRC(9ae375ba) SHA1(6f227c2a5b1170a41e6419f12d1e1f98edc6f8e5) )
ROM_END

ROM_START( dinopic3 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )		/* 68000 code */
	ROM_LOAD16_BYTE( "27c4000-m15262-a-3.bin",      0x000001, 0x80000, CRC(13dfeb08) SHA1(cd2f9dd64f4fabe93901247e36dff3763169716d) )
	ROM_LOAD16_BYTE( "27c4000-m15377-a-5.bin",      0x000000, 0x80000, CRC(96dfcbf1) SHA1(a8bda6edae2c1b79db7ae8a8976fd2457f874373) )
	ROM_LOAD16_BYTE( "27c4000-m15263-a-1-2.bin",    0x100001, 0x80000, CRC(0e4058ba) SHA1(346f9e34ea53dd1bf5cdafa1e38bf2edb09b9a7f) )
	ROM_LOAD16_BYTE( "27c4000-m15295-a-4.bin",      0x100000, 0x80000, CRC(6133f349) SHA1(d13af99910623f62c090d25372a2253dbc2f8cbe) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROMX_LOAD( "27c4000-m15263-a-1-9.bin",	0x000000, 0x40000, CRC(f3c2c98d) SHA1(98ae51a67fa4159456a4a205eebdd8d1775888d1), ROM_SKIP(7) )
	ROM_CONTINUE(				0x000004, 0x40000)
	ROMX_LOAD( "27c4000-m15262-a-8.bin",	0x000001, 0x40000, BAD_DUMP CRC(a0e1f6e0) SHA1(119af72fb6e75933b6d39bc4a8030823ce9b7611), ROM_SKIP(7) )
	ROM_CONTINUE(				0x000005, 0x40000)
	ROMX_LOAD( "27c4000-m15279-a-7.bin",	0x000002, 0x40000, CRC(55ef0adc) SHA1(3b5551ae76ae80882d37fc70a1031a57885d6840), ROM_SKIP(7) )
	ROM_CONTINUE(				0x000006, 0x40000)
	ROMX_LOAD( "27c4000-m15279-a-6.bin",	0x000003, 0x40000, CRC(cc0805fc) SHA1(c512734c28b878a30a0de249929f69784d5d77a1), ROM_SKIP(7) )
	ROM_CONTINUE(				0x000007, 0x40000)
	ROMX_LOAD( "27c4000-m15013-b-1-13.bin",	0x200000, 0x40000, CRC(1371f714) SHA1(d2c98096fab08e3d4fd2482e6ebfc970ead656ee), ROM_SKIP(7) )
	ROM_CONTINUE(				0x200004, 0x40000)
	ROMX_LOAD( "27c4000-m15263-a-1-12.bin",	0x200001, 0x40000, CRC(b284c4a7) SHA1(166f571e0afa115f8e38ba427b40e30abcfd70ee), ROM_SKIP(7) )
	ROM_CONTINUE(				0x200005, 0x40000)
	ROMX_LOAD( "27c4000-m15377-a-11.bin",	0x200002, 0x40000, CRC(b7ad3394) SHA1(58dec34d9d991ff2817c8a7847749716abae6c77), ROM_SKIP(7) )
	ROM_CONTINUE(				0x200006, 0x40000)
	ROMX_LOAD( "27c4000-m15279-a-10.bin",	0x200003, 0x40000, CRC(88847705) SHA1(05dc90067921960e417b7436056a5e1f86abaa1a), ROM_SKIP(7) )
	ROM_CONTINUE(				0x200007, 0x40000)

	ROM_REGION( 0x28000, "audiocpu", 0 )		/* PIC16c57 - protected, dump isn't valid */
	ROM_LOAD( "pic16c57-rc.bin",		0x00000, 0x1030, BAD_DUMP CRC(4d262eaa) SHA1(d659cfa9c9909eec9013b3dc164ba92dd2146479) )

	ROM_REGION( 0x80000, "oki", 0 )			/* OKI6295 samples */
	ROM_LOAD( "27c4000-m15388-a-1.bin",	0x000000, 0x80000,  CRC(7d921309) SHA1(d51e60e904d302c2516b734189e141aa171b2b82) )

	ROM_REGION( 0xc00, "user1", 0 )			/* extra bits on this set */
	ROM_LOAD( "cat93c46p.bin",       0x000, 0x080,  CRC(d49fa351) SHA1(e6dfaff1c6aa962d34ae8e82b71e6f394d82e19c) )
	ROM_LOAD( "gal20v8a-1.bin",      0x200, 0x157,  CRC(cd99ca47) SHA1(ee1d990fd294aa46f56f31264134251569f6792e) )
	ROM_LOAD( "gal20v8a-2.bin",      0x400, 0x157,  CRC(60d016b9) SHA1(add42c763c819f3fe6d7cf3adc7123a52c2a3be9) )
	ROM_LOAD( "gal20v8a-3.bin",      0x600, 0x157,  CRC(049b7f4f) SHA1(6c6ea03d9a293db69a8bd10e042ee75e3c01313c) )
	ROM_LOAD( "palce16v8h-1.bin",    0x800, 0x117,  CRC(48253c66) SHA1(8c94e655b768c45c3edf6ef39e62e3b7a4e57530) )
	ROM_LOAD( "palce16v8h-2.bin",    0xa00, 0x117,  CRC(9ae375ba) SHA1(6f227c2a5b1170a41e6419f12d1e1f98edc6f8e5) )
ROM_END

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

ROM_START( ffightbl )
	ROM_REGION( 0x400000, "maincpu", 0 )		/* 68000 code */
	ROM_LOAD16_BYTE( "fg-e.bin",  0x00000, 0x80000, CRC(f8ccf27e) SHA1(08ff445d946da81e7dc0cc021f686b5968fa34ab) )
	ROM_LOAD16_BYTE( "fg-f.bin",  0x00001, 0x80000, CRC(d96c76b2) SHA1(3f9ca4625491cab07cf4a1bf001f1325dc3652a3) )

	ROM_REGION( 0x30000, "audiocpu", 0 )		/* Audio CPU + Sample Data */
	ROM_LOAD( "ff1.bin",    0x00000, 0x20000, CRC(5b276c14) SHA1(73e53c077d4e3c1b919eee28b29e34176ee204f8) )
	ROM_RELOAD(		0x10000, 0x20000 )

	ROM_REGION( 0x200000, "gfx", 0 )
	ROMX_LOAD( "fg-d.bin",     0x000000, 0x80000, CRC(4303f863) SHA1(72a3246e14f9c4d1fb4712bd67d087db42d722d9) , ROM_SKIP(3) )
	ROMX_LOAD( "fg-c.bin",     0x000001, 0x80000, CRC(d1dfcd2d) SHA1(8796db70459e1e6232a75f5c3f4bf8b227b16f46) , ROM_SKIP(3) )
	ROMX_LOAD( "fg-b.bin",     0x000002, 0x80000, CRC(22f2c097) SHA1(bbf2d30d31c5a7802b7f7f164dd51a4584511936) , ROM_SKIP(3) )
	ROMX_LOAD( "fg-a.bin",     0x000003, 0x80000, CRC(16a89b2c) SHA1(4d0e1ec6ae9a2bd31fa77140532bbce64d3874e9) , ROM_SKIP(3) )

	ROM_REGION( 0x8000, "gfx2", 0 )
	ROM_COPY( "gfx", 0x000000, 0x000000, 0x8000 )	/* stars */
ROM_END

ROM_START( ffightbla )
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
	ROMX_LOAD( "ff-19.bin",  0x100000, 0x20000, CRC(7bc03747) SHA1(6964e5c562d6af5b4327ff828f3d0522c34911bc) , ROM_SKIP(3) )
	ROMX_LOAD( "ff-21.bin",  0x100001, 0x20000, CRC(0c248e2b) SHA1(28731fe25a8eb39c1e0822cf9074a7a32c6b2978) , ROM_SKIP(3) )
	ROMX_LOAD( "ff-23.bin",  0x100002, 0x20000, CRC(53949d0e) SHA1(1b11134005a47c323917b9892fe44819c36c6ee2) , ROM_SKIP(3) )
	ROMX_LOAD( "ff-25.bin",  0x100003, 0x20000, CRC(8d34a67d) SHA1(69e9f52efb73952313848a6d54dbdc17a2275c59) , ROM_SKIP(3) )
	ROMX_LOAD( "11.bin",     0x180000, 0x20000, CRC(d4457a60) SHA1(9e956efafa81a81aca92837df03968f5670ffc15) , ROM_SKIP(3) )
	ROMX_LOAD( "13.bin",     0x180001, 0x20000, CRC(3b26a37d) SHA1(58d8d0cdef81c938fb1a5595f2d02b228865893b) , ROM_SKIP(3) )
	ROMX_LOAD( "15.bin",     0x180002, 0x20000, CRC(6d837e09) SHA1(b4a133ab96c35b689ee692bfcc04981791099b6f) , ROM_SKIP(3) )
	ROMX_LOAD( "17.bin",     0x180003, 0x20000, CRC(c59a4d6c) SHA1(59e49c7d24dd333007de4bb621050011a5392bcc) , ROM_SKIP(3) )

	ROM_REGION( 0x8000, "gfx2", 0 )
	ROM_COPY( "gfx", 0x000000, 0x000000, 0x8000 )   /* stars */
ROM_END

/*

CPU:

1x MC68000P12 ic65 main
1x Z0840006PSC ic1 sound
1x YM2151 ic29 sound
1x YM3012 ic30 sound
2x LM324 ic15,ic31 sound
2x M5205 ic184,ic185 sound
1x TDA2003 ic14 sound
1x oscillator 24.000000MHz (close to main)
1x oscillator 29.821000MHz (close to sound)

ROMs

5x M27C2001 1,2,3,4,5 dumped
4x maskrom KA,KB,KC,KD not dumped

RAMs:

4x KM62256ALP ic112,ic113,ic168,ic170
1x SYC6116L ic24
1x MCM2018AN ic7,ic8,ic51,ic56,ic70,ic71,ic77,ic78

PLDs:

1x TPC1020AFN ic116 read protected
3x GAL20V8A ic120,ic121,ic169 read protected
3x GAL16V8A ic7,ic72,ic80 read protected

Note:

1x JAMMA edge connector
2x 10 legs connector
1x trimmer (volume)
3x 8x2 switches DIP

*/
/* bootleg */
/* FIXME - GFX ROMs are wrong, copied from the other version */
/* ROMs missing are KA.IC91 KB.IC92 KC.IC93 KD.IC94 */

ROM_START( knightsb )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )		/* 68000 code */
	ROM_LOAD16_BYTE( "5.ic172",    0x00000, 0x40000, CRC(7fd91118) SHA1(d2832b21309a467938891946d7af35d8095787a4) )
	ROM_LOAD16_BYTE( "3.ic173",    0x00001, 0x40000, CRC(c9c6e720) SHA1(e8a1cd73458b548e88fc49d8f659e0dc33a8e756) )
	ROM_LOAD16_BYTE( "4.ic176",    0x80000, 0x40000, CRC(af352703) SHA1(7855ac65752203f45af4ef41af8c291540a1c8a8) )
	ROM_LOAD16_BYTE( "2.ic175",    0x80001, 0x40000, CRC(1eb91343) SHA1(e02cfbbd7689346f14f2e3455ed17e7f0b51bad0) )

	ROM_REGION( 0x400000, "gfx", 0 )		/* bootleg had 4x 1meg MASKroms, these need dumping so that the format is known */
	ROMX_LOAD( "kr_gfx1.rom",  0x000000, 0x80000, CRC(9e36c1a4) SHA1(772daae74e119371dfb76fde9775bda78a8ba125) , ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "kr_gfx3.rom",  0x000002, 0x80000, CRC(c5832cae) SHA1(a188cf401cd3a2909b377d3059f14d22ec3b0643) , ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "kr_gfx2.rom",  0x000004, 0x80000, CRC(f095be2d) SHA1(0427d1574062f277a9d04440019d5638b05de561) , ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "kr_gfx4.rom",  0x000006, 0x80000, CRC(179dfd96) SHA1(b1844e69da7ab13474da569978d5b47deb8eb2be) , ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "kr_gfx5.rom",  0x200000, 0x80000, CRC(1f4298d2) SHA1(4b162a7f649b0bcd676f8ca0c5eee9a1250d6452) , ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "kr_gfx7.rom",  0x200002, 0x80000, CRC(37fa8751) SHA1(b88b39d1f08621f15a5620095aef998346fa9891) , ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "kr_gfx6.rom",  0x200004, 0x80000, CRC(0200bc3d) SHA1(c900b1be2b4e49b951e5c1e3fd1e19d21b82986e) , ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "kr_gfx8.rom",  0x200006, 0x80000, CRC(0bb2b4e7) SHA1(983b800925d58e4aeb4e5105f93ed5faf66d009c) , ROM_GROUPWORD | ROM_SKIP(6) )

	ROM_REGION( 0x50000, "audiocpu", 0 )		/* 64k for the audio CPU (+banks) */
	ROM_LOAD( "1.ic26",     0x00000, 0x40000, CRC(bd6f9cc1) SHA1(9f33cccef224d2204736a9eae761196866bd6e41) )
	ROM_RELOAD(             0x10000, 0x40000 )
ROM_END

ROM_START( knightsb4 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )		/* 68000 code */
	ROM_LOAD16_BYTE( "3.bin",   0x00000, 0x80000, CRC(b818272c) SHA1(680b1539bbeebf26706c9367decce2a8de0144e4) )
	ROM_LOAD16_BYTE( "2.bin",   0x00001, 0x80000, CRC(b0b9a4c2) SHA1(7d49b260224756303f9c6cdb67e8c531b0f5689f) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROMX_LOAD( "kr_gfx1.rom",  0x000000, 0x80000, CRC(9e36c1a4) SHA1(772daae74e119371dfb76fde9775bda78a8ba125) , ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "kr_gfx3.rom",  0x000002, 0x80000, CRC(c5832cae) SHA1(a188cf401cd3a2909b377d3059f14d22ec3b0643) , ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "kr_gfx2.rom",  0x000004, 0x80000, CRC(f095be2d) SHA1(0427d1574062f277a9d04440019d5638b05de561) , ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "kr_gfx4.rom",  0x000006, 0x80000, CRC(179dfd96) SHA1(b1844e69da7ab13474da569978d5b47deb8eb2be) , ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "kr_gfx5.rom",  0x200000, 0x80000, CRC(1f4298d2) SHA1(4b162a7f649b0bcd676f8ca0c5eee9a1250d6452) , ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "kr_gfx7.rom",  0x200002, 0x80000, CRC(37fa8751) SHA1(b88b39d1f08621f15a5620095aef998346fa9891) , ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "kr_gfx6.rom",  0x200004, 0x80000, CRC(0200bc3d) SHA1(c900b1be2b4e49b951e5c1e3fd1e19d21b82986e) , ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "kr_gfx8.rom",  0x200006, 0x80000, CRC(0bb2b4e7) SHA1(983b800925d58e4aeb4e5105f93ed5faf66d009c) , ROM_GROUPWORD | ROM_SKIP(6) )

	ROM_REGION( 0x50000, "audiocpu", 0 )
	ROM_LOAD( "1.bin",	0x00000, 0x40000, CRC(bd6f9cc1) SHA1(9f33cccef224d2204736a9eae761196866bd6e41) )
	ROM_RELOAD(		0x10000, 0x40000 )
ROM_END

/*
CPU

1x TS68000CP12 (main)
1x TPC1020AFN-084C
1x Z8400BB1-Z80CPU (sound)
1x YM2151 (sound)
1x YM3012A (sound)
1x OKI-M6295 (sound)
2x LM324N (sound)
1x TDA2003 (sound)
1x oscillator 10.0 MHz
1x oscillator 22.1184 MHz

ROMs

1x AM27C512 (1)(sound)
1x AM27C020 (2)(sound)
2x AM27C040 (3,4)(main)
1x Am27C040 (bp)(gfx)
7x maskrom (ai,bi,ci,di,ap,cp,dp)(gfx)
1x GAL20V8A (not dumped)
3x GAL16V8A (not dumped)
1x PALCE20V8H (not dumped)
1x GAL20V8S (not dumped)

Note

1x JAMMA edge connector
1x trimmer (volume)
3x 8 switches dip

*/
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

ROM_START( slampic )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )		/* 68000 code */
	ROM_LOAD16_BYTE( "5.bin",      0x000000, 0x80000,  CRC(7dba63cd) SHA1(222e781ffc40c5c23f5789c0682f549f00beeb8d) )
	ROM_LOAD16_BYTE( "3.bin",      0x000001, 0x80000,  CRC(d86671f3) SHA1(d95fae27b0f4d3688f1c2229c9d3780724a870a8) )
	ROM_LOAD16_BYTE( "4.bin",      0x100000, 0x80000,  CRC(d14d0e42) SHA1(b60c44193b247dc4856bd36d69cbbe9dcb2d21a7) )
	ROM_LOAD16_BYTE( "2.bin",      0x100001, 0x80000,  CRC(38063cd8) SHA1(e647433414ff4fdc0b2c4c7036b8995a95289efa) )

	ROM_REGION( 0x600000, "gfx", 0 )
	ROMX_LOAD( "9.bin",    0x000000, 0x40000, CRC(dc140351) SHA1(0e69e1c8ded85ba26eb8236449d38ead0243ae78), ROM_SKIP(7) )
	ROM_CONTINUE(          0x000004, 0x40000)
	ROMX_LOAD( "8.bin",    0x000001, 0x40000, CRC(9ae88035) SHA1(3329e9582ca052940e115e759bb3d96f4a9c87fa), ROM_SKIP(7) )
	ROM_CONTINUE(          0x000005, 0x40000)
	ROMX_LOAD( "7.bin",    0x000002, 0x40000, CRC(5321f759) SHA1(7538a6587cf1077921b938070185e0a0ce5ca922), ROM_SKIP(7) )
	ROM_CONTINUE(          0x000006, 0x40000)
	ROMX_LOAD( "6.bin",    0x000003, 0x40000, CRC(c8eb5f76) SHA1(a361d2d2dfe71789736666b744ae5f1e4bf7e1b2), ROM_SKIP(7) )
	ROM_CONTINUE(          0x000007, 0x40000)
	ROMX_LOAD( "17.bin",   0x200000, 0x40000, CRC(21652214) SHA1(039335251f6553c4f36e2d33e8b43fb5726e833e), ROM_SKIP(7) )
	ROM_CONTINUE(          0x200004, 0x40000)
	ROMX_LOAD( "16.bin",   0x200001, 0x40000, CRC(d49d2eb0) SHA1(1af01575340730166975be93bae438e2b0492f98), ROM_SKIP(7) )
	ROM_CONTINUE(          0x200005, 0x40000)
	ROMX_LOAD( "15.bin",   0x200002, 0x40000, CRC(0d98bfd6) SHA1(c11fbf555880a933a4cbf6faa517f59f8443304f), ROM_SKIP(7) )
	ROM_CONTINUE(          0x200006, 0x40000)
	ROMX_LOAD( "14.bin",   0x200003, 0x40000, CRC(807284f1) SHA1(c747c3eaade31c2633fb0a0682dbea900bf2b092), ROM_SKIP(7) )
	ROM_CONTINUE(          0x200007, 0x40000)
	ROMX_LOAD( "13.bin",   0x400000, 0x40000, CRC(293579c5) SHA1(9adafe29664b20834365b339f7ae379cdb9ee138), ROM_SKIP(7) )
	ROM_CONTINUE(          0x400004, 0x40000)
	ROMX_LOAD( "12.bin",   0x400001, 0x40000, CRC(c3727ce7) SHA1(c4abc2c59152c59a45f85393e9525505bc2c9e6e), ROM_SKIP(7) )
	ROM_CONTINUE(          0x400005, 0x40000)
	ROMX_LOAD( "11.bin",   0x400002, 0x40000, CRC(2919883b) SHA1(44ad979daae673c77b3157d2b352797d4ad0ec24), ROM_SKIP(7) )
	ROM_CONTINUE(          0x400006, 0x40000)
	ROMX_LOAD( "10.bin",   0x400003, 0x40000, CRC(f538e620) SHA1(354cd0548b067dfc8782bbe13b0a9c2083dbd290), ROM_SKIP(7) )
	ROM_CONTINUE(          0x400007, 0x40000)

	ROM_REGION( 0x2000, "audiocpu", 0 )		/* PIC16c57 - protected, dump isn't valid */
	ROM_LOAD( "pic16c57-xt-p.bin", 0x00000, 0x2000, BAD_DUMP CRC(aeae5ccc) SHA1(553afb68f7bf130cdf34e24512f72b4ecef1576f) )

	ROM_REGION( 0x80000, "oki", 0 )			/* OKI6295 samples */
	ROM_LOAD( "18.bin",   0x00000, 0x80000, CRC(73a0c11c) SHA1(a66e1a964313e21c4436200d36c598dcb277cd34) )

	ROM_REGION( 0x20000, "user1", 0 )		/* not in the dump, but needed for protection */
	ROM_LOAD( "mb_qa.5k",   0x00000, 0x20000, CRC(e21a03c4) SHA1(98c03fd2c9b6bf8a4fc25a4edca87fff7c3c3819) )
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

ROM_START( sf2mdta )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )		/* 68000 code */
	ROM_LOAD16_BYTE( "3.mdta", 0x000000, 0x80000, CRC(9f544ef4) SHA1(f784809e59a5fcabd6d15d3f1c36250a5528c9f8) )
	ROM_LOAD16_BYTE( "5.mdta", 0x000001, 0x80000, CRC(d76d6621) SHA1(aa9cea9ddace212a7b3c535b8f6e3fbc50da1f94) )
	ROM_LOAD16_BYTE( "2.mdta", 0x100000, 0x20000, CRC(74844192) SHA1(99cd546c78cce7f632007af454d8a55eddb6b19b) )
	ROM_LOAD16_BYTE( "4.mdta", 0x100001, 0x20000, CRC(bd98ff15) SHA1(ed902d949b0b5c5beaaea78a4b418ffa6db9e1df) )

	ROM_REGION( 0x600000, "gfx", 0 )
	/* unconfirmed if working on real hardware, pf4 is a bad dump (bad pin) */
	ROMX_LOAD( "pf4 sh058.ic89", 0x000000, 0x100000, VERIFY_OFF, ROM_GROUPWORD | ROM_SKIP(2) )
	ROMX_LOAD( "pf7 sh072.ic92", 0x000002, 0x100000, CRC(fb78022e) SHA1(b8974387056dd52db96b01cc4648edc814398c7e), ROM_GROUPWORD | ROM_SKIP(2) )
	ROMX_LOAD( "pf5 sh036.ic90", 0x200000, 0x100000, CRC(0a6be48b) SHA1(b7e72c94d4e3eb4a6bba6608d9b9a093c8901ad9), ROM_GROUPWORD | ROM_SKIP(2) )
	ROMX_LOAD( "pf8 sh074.ic93", 0x200002, 0x100000, CRC(6258c7cf) SHA1(4cd7519245c0aa816934a43e6743160f715d7dc2), ROM_GROUPWORD | ROM_SKIP(2) )
	ROMX_LOAD( "pf6 sh070.ic88", 0x400000, 0x100000, CRC(9b5b09d7) SHA1(698a6aab41e495bd0c37a19aee16a84f04d15797), ROM_GROUPWORD | ROM_SKIP(2) )
	ROMX_LOAD( "pf9 sh001.ic91", 0x400002, 0x100000, CRC(9f25090e) SHA1(12ff0431ef6550db446985c8914ac7d78eec6b6d), ROM_GROUPWORD | ROM_SKIP(2) )

	ROM_REGION( 0x30000, "audiocpu", 0 )		/* Sound program + samples  */
	ROM_LOAD( "1.ic28",    0x00000, 0x20000, CRC(d5bee9cc) SHA1(e638cb5ce7a22c18b60296a7defe8b03418da56c) )
	ROM_RELOAD(            0x10000, 0x20000 )
ROM_END

ROM_START( sf2m1 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )		/* 68000 code */
	ROM_LOAD16_BYTE( "222e",              0x000000, 0x80000, CRC(1e20d0a3) SHA1(5e05b52fd938aff5190bca7e178705d7236aef66) )
	ROM_LOAD16_BYTE( "196e",              0x000001, 0x80000, CRC(88cc38a3) SHA1(6049962f943bd37748a9531cc3254e8b59326eac) )
	ROM_LOAD16_WORD_SWAP( "s92_21a.bin",  0x100000, 0x80000, CRC(925a7877) SHA1(1960dca35f0ca6f2b399a9fccfbc0132ac6425d1) )

	ROM_REGION( 0x600000, "gfx", 0 )
	ROMX_LOAD( "s92_01.bin",   0x000000, 0x80000, CRC(03b0d852) SHA1(f370f25c96ad2b94f8c53d6b7139100285a25bef) , ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "s92_02.bin",   0x000002, 0x80000, CRC(840289ec) SHA1(2fb42a242f60ba7e74009b5a90eb26e035ba1e82) , ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "s92_03.bin",   0x000004, 0x80000, CRC(cdb5f027) SHA1(4c7d944fef200fdfcaf57758b901b5511188ed2e) , ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "s92_04.bin",   0x000006, 0x80000, CRC(e2799472) SHA1(27d3796429338d82a8de246a0ea06dd487a87768) , ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "s92_05.bin",   0x200000, 0x80000, CRC(ba8a2761) SHA1(4b696d66c51611e43522bed752654314e76d33b6) , ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "s92_06.bin",   0x200002, 0x80000, CRC(e584bfb5) SHA1(ebdf1f5e2638eed3a65dda82b1ed9151a355f4c9) , ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "s92_07.bin",   0x200004, 0x80000, CRC(21e3f87d) SHA1(4a4961bb68c3a1ce15f9d393d9c03ecb2466cc29) , ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "s92_08.bin",   0x200006, 0x80000, CRC(befc47df) SHA1(520390420da3a0271ba90b0a933e65143265e5cf) , ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "s92_10.bin",   0x400000, 0x80000, CRC(960687d5) SHA1(2868c31121b1c7564e9767b9a19cdbf655c7ed1d) , ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "s92_11.bin",   0x400002, 0x80000, CRC(978ecd18) SHA1(648a59706b93c84b4206a968ecbdc3e834c476f6) , ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "s92_12.bin",   0x400004, 0x80000, CRC(d6ec9a0a) SHA1(ed6143f8737013b6ef1684e37c05e037e7a80dae) , ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "s92_13.bin",   0x400006, 0x80000, CRC(ed2c67f6) SHA1(0083c0ffaf6fe7659ff0cf822be4346cd6e61329) , ROM_GROUPWORD | ROM_SKIP(6) )

	ROM_REGION( 0x18000, "audiocpu", 0 )		/* 64k for the audio CPU (+banks) */
	ROM_LOAD( "s92_09.bin",    0x00000, 0x08000, CRC(08f6b60e) SHA1(8258fcaca4ac419312531eec67079b97f471179c) )
	ROM_CONTINUE(              0x10000, 0x08000 )

	ROM_REGION( 0x40000, "oki", 0 )			/* Samples */
	ROM_LOAD( "s92_18.bin",    0x00000, 0x20000, CRC(7f162009) SHA1(346bf42992b4c36c593e21901e22c87ae4a7d86d) )
	ROM_LOAD( "s92_19.bin",    0x20000, 0x20000, CRC(beade53f) SHA1(277c397dc12752719ec6b47d2224750bd1c07f79) )
ROM_END

ROM_START( sgyxz )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )		/* 68000 Code */
	ROM_LOAD16_BYTE( "sgyxz_prg1.bin", 0x000001, 0x20000, CRC(d8511929) SHA1(4de9263778f327693f4d1e21b48e43806f673487) )
	ROM_CONTINUE( 0x80001, 0x20000 )
	ROM_CONTINUE( 0x40001, 0x20000 )
	ROM_CONTINUE( 0xc0001, 0x20000 )
	ROM_LOAD16_BYTE( "sgyxz_prg2.bin", 0x000000, 0x20000, CRC(95429c83) SHA1(e981624d018132e5625a66113b6ac4fc44e55cf7) )
	ROM_CONTINUE( 0x80000, 0x20000 )
	ROM_CONTINUE( 0x40000, 0x20000 )
	ROM_CONTINUE( 0xc0000, 0x20000 )
	ROM_FILL(0x708da, 4, 0xff)			/* patch out protections */
	ROM_FILL(0xf11ea, 1, 0x60)
	ROM_FILL(0x00007, 1, 0xa2)			/* start address */
	ROM_FILL(0x02448, 1, 0x07)			/* transitions */

	ROM_REGION( 0x400000, "gfx", 0 )
	ROMX_LOAD("sgyxz_gfx1.bin", 0x000000, 0x80000, CRC(a60be9f6) SHA1(2298a4b6a2c83b76dc106a1efa19606b298d378a), ROM_GROUPWORD | ROM_SKIP(6) )
	ROM_CONTINUE(		    0x000004, 0x80000 )
	ROM_CONTINUE(		    0x200000, 0x80000 )
	ROM_CONTINUE(		    0x200004, 0x80000 )
	ROMX_LOAD("sgyxz_gfx2.bin", 0x000002, 0x80000, CRC(6ad9d048) SHA1(d47212d28d0a1ce349e4c59e5d0d99c541b3458e), ROM_GROUPWORD | ROM_SKIP(6) )
	ROM_CONTINUE(		    0x000006, 0x80000 )
	ROM_CONTINUE(		    0x200002, 0x80000 )
	ROM_CONTINUE(		    0x200006, 0x80000 )

	ROM_REGION( 0x20000, "audiocpu", 0 )		/* Z80 code */
	ROM_LOAD( "sgyxz_snd2.bin", 0x00000, 0x10000,  CRC(210c376f) SHA1(0d937c86078d0a106f5636b7daf5fc0266c2c2ec) )
	ROM_RELOAD(		    0x08000, 0x10000 )
	ROM_REGION( 0x040000, "oki", 0 )		/* Samples */
	ROM_LOAD( "sgyxz_snd1.bin", 0x00000, 0x40000,  CRC(c15ac0f2) SHA1(8d9e5519d9820e4ac4f70555088c80e64d052c9d) )
ROM_END

ROM_START( punipic )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )		/* 68000 code */
	ROM_LOAD16_BYTE( "cpu5.bin",       0x000000, 0x80000, CRC(c3151563) SHA1(61d3a20c25fea8a94ae6e473a87c21968867cba0) )
	ROM_LOAD16_BYTE( "cpu3.bin",       0x000001, 0x80000, CRC(8c2593ac) SHA1(4261bc72b96c3a5690df35c5d8b71524765693d9) )
	ROM_LOAD16_BYTE( "cpu4.bin",       0x100000, 0x80000, CRC(665a5485) SHA1(c07920d110ca9c35f6cbff94a6a889c17300f994) )
	ROM_LOAD16_BYTE( "cpu2.bin",       0x100001, 0x80000, CRC(d7b13f39) SHA1(eb7cd92b44fdef3b72672b0be6786c526421b627) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROMX_LOAD( "gfx9.bin",    0x000000, 0x40000, CRC(9b9a887a) SHA1(8805b36fc18837bd7c64c751b435d72b763b2235), ROM_SKIP(7) )
	ROM_CONTINUE(             0x000004, 0x40000)
	ROMX_LOAD( "gfx8.bin",    0x000001, 0x40000, CRC(2b94287a) SHA1(815d88e66f537e17550fc0483616f02f7126bfb1), ROM_SKIP(7) )
	ROM_CONTINUE(             0x000005, 0x40000)
	ROMX_LOAD( "gfx7.bin",    0x000002, 0x40000, CRC(e9bd74f5) SHA1(8ed7098c69d1c70093c99956bf82e532bd6fc7ac), ROM_SKIP(7) )
	ROM_CONTINUE(             0x000006, 0x40000)
	ROMX_LOAD( "gfx6.bin",    0x000003, 0x40000, CRC(a5e1c8a4) SHA1(3596265a45cf6bbf16c623f0fce7cdc65f9338ad), ROM_SKIP(7) )
	ROM_CONTINUE(             0x000007, 0x40000)
	ROMX_LOAD( "gfx13.bin",   0x200000, 0x40000, CRC(6d75a193) SHA1(6c5a89517926d7ba4a925a3df800d4bdb8a6938d), ROM_SKIP(7) )
	ROM_CONTINUE(             0x200004, 0x40000)
	ROMX_LOAD( "gfx12.bin",   0x200001, 0x40000, CRC(a3c205c1) SHA1(6317cc49434dbbb9a249ddd4b50bd791803b3ebe), ROM_SKIP(7) )
	ROM_CONTINUE(             0x200005, 0x40000)
	ROMX_LOAD( "gfx11.bin",   0x200002, 0x40000, CRC(22f2ec92) SHA1(9186bfc5db71dc5b099c9a985e8fdd5710772d1c), ROM_SKIP(7) )
	ROM_CONTINUE(             0x200006, 0x40000)
	ROMX_LOAD( "gfx10.bin",   0x200003, 0x40000, CRC(763974c9) SHA1(f9b93c7cf0cb8c212fc21c57c85459b7d2e4e2fd), ROM_SKIP(7) )
	ROM_CONTINUE(             0x200007, 0x40000)

	ROM_REGION( 0x28000, "audiocpu", 0 )		/* PIC16c57 - protected */
	ROM_LOAD( "pic16c57", 0x00000, 0x04000, NO_DUMP )
	ROM_REGION( 0x200000, "oki", 0 )		/* OKI6295 */
	ROM_LOAD( "sound.bin",      0x000000, 0x80000, CRC(aeec9dc6) SHA1(56fd62e8db8aa96cdd242d8c705849a413567780) )
ROM_END

ROM_START( punipic2 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )		/* 68000 code */
	ROM_LOAD16_BYTE( "prg4.bin",       0x000000, 0x80000, CRC(c3151563) SHA1(61d3a20c25fea8a94ae6e473a87c21968867cba0) )
	ROM_LOAD16_BYTE( "prg3.bin",       0x000001, 0x80000, CRC(8c2593ac) SHA1(4261bc72b96c3a5690df35c5d8b71524765693d9) )
	ROM_LOAD16_BYTE( "prg2.bin",       0x100000, 0x80000, CRC(665a5485) SHA1(c07920d110ca9c35f6cbff94a6a889c17300f994) )
	ROM_LOAD16_BYTE( "prg1.bin",       0x100001, 0x80000, CRC(d7b13f39) SHA1(eb7cd92b44fdef3b72672b0be6786c526421b627) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROMX_LOAD( "pu11256.bin",   0x000000, 0x80000, CRC(6581faea) SHA1(2b0e96998002a1df96c7869ec965257d2ecfb531), ROM_GROUPWORD | ROM_SKIP(6) )
	ROM_CONTINUE(               0x200000, 0x80000 )
	ROM_CONTINUE(               0x000004, 0x80000 )
	ROM_CONTINUE(               0x200004, 0x80000 )
	ROMX_LOAD( "pu13478.bin",   0x000002, 0x80000, CRC(61613de4) SHA1(8f8c46ce907be2b4c4715ad88bfd1456818bdd2c), ROM_GROUPWORD | ROM_SKIP(6) )
	ROM_CONTINUE(               0x200002, 0x80000 )
	ROM_CONTINUE(               0x000006, 0x80000 )
	ROM_CONTINUE(               0x200006, 0x80000 )

	ROM_REGION( 0x28000, "audiocpu", 0 )		/* PIC16c57 - protected */
	ROM_LOAD( "pic16c57", 0x00000, 0x04000, NO_DUMP )
	ROM_REGION( 0x200000, "oki", 0 )		/* OKI6295 */
	ROM_LOAD( "sound.bin",      0x000000, 0x80000, CRC(aeec9dc6) SHA1(56fd62e8db8aa96cdd242d8c705849a413567780) )

	ROM_REGION( 0x200000, "user1", 0 )		/* other */
	ROM_LOAD( "93c46.bin",      0x0000, 0x0080, CRC(36ab4e7d) SHA1(60bea43051d86d9aefcbb7a390cf0c7d8b905a4b) )
ROM_END

ROM_START( punipic3 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )		/* 68000 code */
	ROM_LOAD16_BYTE( "psb5b.rom",       0x000000, 0x80000, CRC(58f42c05) SHA1(e243928f0bbecdf2a8d07cf4a6fdea4440e46c01) )
	ROM_LOAD16_BYTE( "psb3b.rom",       0x000001, 0x80000, CRC(90113db4) SHA1(4decc203ae3ee4abcb2e017f11cd20eae2abf3f3) )
	ROM_LOAD16_BYTE( "psb4a.rom",       0x100000, 0x80000, CRC(665a5485) SHA1(c07920d110ca9c35f6cbff94a6a889c17300f994) )
	ROM_LOAD16_BYTE( "psb2a.rom",       0x100001, 0x80000, CRC(d7b13f39) SHA1(eb7cd92b44fdef3b72672b0be6786c526421b627) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROMX_LOAD( "psb-a.rom",     0x000000, 0x80000, CRC(57f0f5e3) SHA1(130b6e92181994bbe874261e0895db65d4f3d5d1), ROM_GROUPWORD | ROM_SKIP(6) )
	ROM_CONTINUE(               0x000004, 0x80000 )
	ROM_CONTINUE(               0x200000, 0x80000 )
	ROM_CONTINUE(               0x200004, 0x80000 )
	ROMX_LOAD( "psb-b.rom",     0x000002, 0x80000, CRC(d9eb867e) SHA1(9b6eaa4a780da5c9cf09658fcab3a1a6f632c2f4), ROM_GROUPWORD | ROM_SKIP(6) )
	ROM_CONTINUE(               0x000006, 0x80000 )
	ROM_CONTINUE(               0x200002, 0x80000 )
	ROM_CONTINUE(               0x200006, 0x80000 )

	ROM_REGION( 0x28000, "audiocpu", ROMREGION_ERASE00 )
	ROM_REGION( 0x200000, "oki", ROMREGION_ERASE00 )
ROM_END

ROM_START( varthb )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )		/* 68000 code */
	ROM_LOAD16_BYTE( "2",   0x000000, 0x80000, CRC(2f010023) SHA1(bf4b6c0cd82cf1b86e17d6ea2670110c06e6eabe) )
	ROM_LOAD16_BYTE( "1",   0x000001, 0x80000, CRC(0861dff3) SHA1(bf6dfe18ecaeaa1bbea09267014891c4a4a07943) )
	ROM_LOAD16_BYTE( "4",   0x100000, 0x10000, CRC(aa51e43b) SHA1(46b9dab844c55b50a47d048e5bb114911773699c) )
	ROM_LOAD16_BYTE( "3",   0x100001, 0x10000, CRC(f7e4f2f0) SHA1(2ce4eadb2d6a0e0d5745323eff2c899950ad4d3b) )

	ROM_REGION( 0x200000, "gfx", 0 )
	ROMX_LOAD( "14",  0x000000, 0x40000, CRC(7ca73780) SHA1(26909db32f84157cd05719e5d1e715e76636d292) , ROM_SKIP(7) )
	ROMX_LOAD( "13",  0x000001, 0x40000, CRC(9fb11869) SHA1(a434fb0b588f934aaa68139495e1212a63ccf162) , ROM_SKIP(7) )
	ROMX_LOAD( "12",  0x000002, 0x40000, CRC(afeba416) SHA1(e722c65ea2e2bac3251c32208899484aa5ef6ad2) , ROM_SKIP(7) )
	ROMX_LOAD( "11",  0x000003, 0x40000, CRC(9eef3507) SHA1(ae03064ca5681fbdc43a34c54aaac11c8467428b) , ROM_SKIP(7) )
	ROMX_LOAD( "10",  0x000004, 0x40000, CRC(eeec6183) SHA1(40dc9c86e90d7c1a2ad600c195fe387180d95fd4) , ROM_SKIP(7) )
	ROMX_LOAD( "9",   0x000005, 0x40000, CRC(0e94f718) SHA1(249534f2323abcdb24099d0abc24c229c699ba94) , ROM_SKIP(7) )
	ROMX_LOAD( "8",   0x000006, 0x40000, CRC(c4ddc5b4) SHA1(79c2a42a664e387932b7804e7a80f5753338c3b0) , ROM_SKIP(7) )
	ROMX_LOAD( "7",   0x000007, 0x40000, CRC(8941ca12) SHA1(5ad5d47b8614c2899d05c65dc3b74947d4bac561) , ROM_SKIP(7) )

	ROM_REGION( 0x18000, "audiocpu", 0 )		/* 64k for the audio CPU (+banks) */
	ROM_LOAD( "6",    0x00000, 0x08000, CRC(7a99446e) SHA1(ca027f41e3e58be5abc33ad7380746658cb5380a) )
	ROM_CONTINUE(           0x10000, 0x08000 )

	ROM_REGION( 0x40000, "oki", 0 )			/* Samples */
	ROM_LOAD( "5",    0x00000, 0x40000, CRC(1547e595) SHA1(27f47b1afd9700afd9e8167d7e4e2888be34a9e5) )

	ROM_REGION( 0x1000, "pals", 0 )
	ROM_LOAD_OPTIONAL( "varth1.bin",    0x00000, 0x157, CRC(4c6a0d99) SHA1(081a307ef38675de178dd6221e6c4e55a5bfbd87) )
	ROM_LOAD_OPTIONAL( "varth2.bin",    0x00200, 0x157, NO_DUMP )		// Registered
	ROM_LOAD_OPTIONAL( "varth3.bin",    0x00400, 0x157, NO_DUMP )		// Registered
	ROM_LOAD_OPTIONAL( "varth4.bin",    0x00600, 0x117, CRC(53317bf6) SHA1(f7b8f8b2c40429a517e3be63e5aed9573972ddfb) )
	ROM_LOAD_OPTIONAL( "varth5.bin",    0x00800, 0x157, NO_DUMP )		// Registered
	ROM_LOAD_OPTIONAL( "varth6.bin",    0x00a00, 0x157, NO_DUMP )		// Registered
ROM_END



/*
GAME( year,  archives name,  parent name,  MACHINE_DRIVER_START, INPUT_PORTS,  DRIVER_INIT,	flip,  producer name,	title information,	status )
*/
/* captcommb2 - Okay */
GAME( 1991,	captcommb2,	captcomm,	captcommb2,	captcomm,	cps1,		ROT0,	"bootleg",	"Captain Commando (bootleg with YM2151 + 2xMSM5205)", GAME_SUPPORTS_SAVE )
/* cawingbl - that's ok */
GAME( 1990,	cawingbl,	cawing,		cawingbl,	cawingbl,	cawingbl,	ROT0,	"bootleg",	"Carrier Air Wing (bootleg with 2xYM2203 + 2xMSM205 set 1)", GAME_SUPPORTS_SAVE )
/* cawingb2 - ok */
GAME( 1990,	cawingb2,	cawing,		cawingbl,	cawingbl,	cawingbl,	ROT0,	"bootleg",	"Carrier Air Wing (bootleg with 2xYM2203 + 2xMSM205 set 2)", GAME_SUPPORTS_SAVE )
/* dinopic - no sound */
GAME( 1993,	dinopic,	dino,		dinopic,	dino,		dinopic,	ROT0,	"bootleg",	"Cadillacs and Dinosaurs (bootleg set 1 (with PIC16c57), 930201 etc)", GAME_NO_SOUND | GAME_SUPPORTS_SAVE )
/* dinopic2 - no sound, one bad gfx rom. Copying 8.bin from dinopic fixes it. */
GAME( 1993,	dinopic2,	dino,		dinopic,	dino,		dinopic,	ROT0,	"bootleg",	"Cadillacs and Dinosaurs (bootleg set 2 (with PIC16c57), 930201 etc)", GAME_NO_SOUND | GAME_IMPERFECT_GRAPHICS | GAME_SUPPORTS_SAVE )
/* dinopic3 - no sound, no different from dinopic2 */
GAME( 1993,	dinopic3,	dino,		dinopic,	dino,		dinopic,	ROT0,	"bootleg",	"Cadillacs and Dinosaurs (bootleg set 3 (with PIC16c57), 930201 etc)", GAME_NO_SOUND | GAME_IMPERFECT_GRAPHICS | GAME_SUPPORTS_SAVE )
/* fcrash - ok */
GAME( 1990,	fcrash,		ffight,		fcrash,		ffight,		fcrash,		ROT0,	"bootleg (Playmark)",	"Final Crash (bootleg with 2XYM2203 + 2XMSM5205)", GAME_SUPPORTS_SAVE )
/* ffightbl - ok */
GAME( 1990,	ffightbl,	ffight,		fcrash,		ffight,		fcrash,		ROT0,	"bootleg",	"Final Fight (bootleg set 1 with 2XYM2203 + 2XMSM5205, World)", GAME_SUPPORTS_SAVE )
/* ffightbl - ok */
GAME( 1990,	ffightbla,	ffight,		fcrash,		ffight,		fcrash,		ROT0,	"bootleg",	"Final Fight (bootleg set 2 with 2XYM2203 + 2XMSM5205, World)", GAME_SUPPORTS_SAVE )
/* knightsb - ok */
GAME( 1991,	knightsb,	knights,	knightsb,	knights,	dinopic,	ROT0,	"bootleg",	"Knights of the Round (bootleg set 1 with YM2151 + 2xMSM5205, 911127 etc)", GAME_SUPPORTS_SAVE )
/* knightsb4 - ok! */
GAME( 1991,	knightsb4,	knights,	knightsb,	knights,	dinopic,	ROT0,	"bootleg",	"Knights of the Round (bootleg set 4 with YM2151 + 2xMSM5205, 911127 etc)", GAME_SUPPORTS_SAVE )
/* kodb - ok */
GAME( 1991,	kodb,		kod,		kodb,		kod,		kodb,		ROT0,	"bootleg (Playmark)",	"The King of Dragons (bootleg, 910731 etc)", GAME_SUPPORTS_SAVE )
/* slampic - no sound. A priority problem between sprites and crowd. */
GAME( 1993,	slampic,	slammast,	slampic,	slammast,	dinopic,	ROT0,	"bootleg", "Saturday Night Slam Masters (bootleg with PIC16c57)", GAME_IMPERFECT_GRAPHICS | GAME_NO_SOUND | GAME_SUPPORTS_SAVE )






/* sf2mdt - problem with scrolls */
GAME( 1992,   sf2mdt,	  sf2ce,	sf2mdt,		sf2mdt,		sf2mdt,   ROT0,   "bootleg", "Street Fighter II': Magic Delta Turbo (bootleg)", GAME_IMPERFECT_GRAPHICS | GAME_IMPERFECT_SOUND | GAME_SUPPORTS_SAVE )
/* sf2mdta - problem with background */
GAME( 1992,   sf2mdta,	  sf2ce,	sf2mdt,		sf2mdt,		sf2mdta,  ROT0,   "bootleg", "Street Fighter II': Magic Delta Turbo (bootleg, set 2)", GAME_IMPERFECT_GRAPHICS | GAME_IMPERFECT_SOUND | GAME_SUPPORTS_SAVE )
/* sf2m1 - crowd is missing. Plane's tail comes off a bit. Patch used. */
GAME( 1992,   sf2m1,	  sf2ce,	sf2m1,		sf2,		sf2m1,    ROT0,   "bootleg", "Street Fighter II': Champion Edition (M1, bootleg)", GAME_IMPERFECT_GRAPHICS | GAME_SUPPORTS_SAVE )
/* sgyxz - garbage left behind. A priority problem can be seen in 3rd demo where the fighters walk through the crowd instead of behind. */
GAME( 1999,   sgyxz,	  wof,		sgyxz,		sgyxz,		cps1,     ROT0,   "bootleg (All-In Electronic)", "SanGuo YingXiongZhuan (Chinese bootleg of Sangokushi II, set 3)", GAME_IMPERFECT_GRAPHICS | GAME_SUPPORTS_SAVE )
/* punipic - no sound. Problems in Central Park. Patches used. */
GAME( 1993,   punipic,    punisher,	punipic,	punisher,	punipic,  ROT0,   "bootleg", "The Punisher (bootleg with PIC16c57, set 1)", GAME_NO_SOUND | GAME_SUPPORTS_SAVE )
/* punipic2 - no sound. Problems in Central Park. Patches used. */
GAME( 1993,   punipic2,   punisher,	punipic,	punisher,	punipic,  ROT0,   "bootleg", "The Punisher (bootleg with PIC16c57, set 2)", GAME_NO_SOUND | GAME_SUPPORTS_SAVE )
/* punipic3 - same as punipic, and doors are missing. */
GAME( 1993,   punipic3,   punisher,	punipic,	punisher,	punipic3, ROT0,   "bootleg", "The Punisher (bootleg with PIC16c57, set 3)", GAME_IMPERFECT_GRAPHICS | GAME_NO_SOUND | GAME_SUPPORTS_SAVE )
/* varthb - good */
GAME( 1992,   varthb,	  varth,	varthb,		varth,		dinopic,  ROT270, "bootleg", "Varth: Operation Thunderstorm (bootleg)", GAME_SUPPORTS_SAVE )


