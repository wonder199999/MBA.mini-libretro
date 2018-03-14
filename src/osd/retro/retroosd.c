#include "retroosd.h"

void osd_exit(running_machine &machine)
{
	write_log("osd_exit called \n");

   	if (our_target != NULL)
      		render_target_free(our_target);

   	our_target = NULL;

   	global_free(P1_device);
  	global_free(P2_device);
  	global_free(P3_device);
  	global_free(P4_device);
   	global_free(KB_device);
}

void osd_init(running_machine *machine)
{
	int gamRot = 0;

   	machine->add_notifier(MACHINE_NOTIFY_EXIT, osd_exit);

   	our_target = render_target_alloc(machine, NULL, 0);

   	initInput(machine);

   	write_log("machine screen orientation: %s \n", (machine->gamedrv->flags & ORIENTATION_SWAP_XY) ? "VERTICAL" : "HORIZONTAL");

   	orient = (machine->gamedrv->flags & ORIENTATION_MASK);
   	vertical = (machine->gamedrv->flags & ORIENTATION_SWAP_XY);

   	gamRot = (ROT270 == orient) ? 1 : gamRot;
   	gamRot = (ROT180 == orient) ? 2 : gamRot;
   	gamRot = (ROT90 == orient) ? 3 : gamRot;

   	prep_retro_rotation(gamRot);
   	machine->sample_rate = sample_rate;	/* Override original value */

   	write_log("osd init done\n");
}

void osd_update(running_machine *machine, int skip_redraw)
{
   	const render_primitive_list  *primlist;
   	UINT8 			     *surfptr;

   	if (mame_reset)
   	{
      		mame_reset = 0;
      		machine->schedule_soft_reset();
   	}

   	if (pauseg)
	{
      		machine->schedule_exit();
      		return;
   	}

   	if (FirstTimeUpdate)
      		skip_redraw = 0;		/* force redraw to make sure the video texture is created */

   	if (!skip_redraw)
   	{
      		draw_this_frame = true;
		int minwidth, minheight;

      		/* get the minimum width/height for the current layout */
		render_target_get_minimum_size(our_target, &minwidth, &minheight);

		if (FirstTimeUpdate)
		{
         		FirstTimeUpdate = 0;
         		write_log("Game screen width=%i height=%i rowPixels=%i\n", minwidth, minheight, minwidth);

         		rtwi = topw = minwidth;
         		rthe = minheight;
      		}

      		if (adjust_opt[0])
      		{
			adjust_opt[0] = 0;
			if (adjust_opt[1])
			{
				if (adjust_opt[2])
				{
					adjust_opt[2] = 0;
					refresh_rate = (machine->primary_screen == NULL) ? screen_device::k_default_frame_rate :
							ATTOSECONDS_TO_HZ(machine->primary_screen->frame_period().attoseconds);
					update_geometry();
				}

				if ((adjust_opt[3] || adjust_opt[4] || adjust_opt[5]))
				{
					screen_device *screen = screen_first(*machine);
					render_container *container = render_container_get_screen(screen);
					render_container_user_settings settings;
					render_container_get_user_settings(container, &settings);

					if (adjust_opt[3])
					{
						adjust_opt[3] = 0;
						settings.brightness = arroffset[0] + 1.0f;
						render_container_set_user_settings(container, &settings);
					}
					if (adjust_opt[4])
					{
						adjust_opt[4] = 0;
						settings.contrast = arroffset[1] + 1.0f;
						render_container_set_user_settings(container, &settings);
					}
					if (adjust_opt[5])
					{
						adjust_opt[5] = 0;
						settings.gamma = arroffset[2] + 1.0f;
						render_container_set_user_settings(container, &settings);
					}
				}

				if (adjust_opt[6])
				{
					adjust_opt[6] = 0;
					machine->device("maincpu")->set_clock_scale(arroffset[3]);
					/*
					machine->device("slave")->set_clock_scale(1.0f);
					machine->device("mcu")->set_clock_scale(1.0f);
					*/
				}
      			}
		}
      		/* make that the size of our target */
      		render_target_set_bounds(our_target, rtwi, rthe, 0);
      		/* get the list of primitives for the target at the current size */
      		primlist = render_target_get_primitives(our_target);
      		/* lock them, and then render them */
      		osd_lock_acquire(primlist->lock);

      		surfptr = (UINT8 *) videoBuffer;
#ifdef M16B
      		rgb565_draw_primitives(primlist->head, surfptr, rtwi, rthe, rtwi);
#else
      		rgb888_draw_primitives(primlist->head, surfptr, rtwi, rthe, rtwi);
#endif
      		osd_lock_release(primlist->lock);
	}
   	else
      		draw_this_frame = false;

	RLOOP = 0;

   	if (keyboard_input)
		if (ui_ipt_pushchar != -1)
   		{
      			ui_ipt_pushchar = -1;
      			ui_input_push_char_event(machine, our_target, (unicode_char)ui_ipt_pushchar);
   		}
}


 //============================================================
// osd_wait_for_debugger
//============================================================

void osd_wait_for_debugger(running_device *device, int firststop)
{
   // we don't have a debugger, so we just return here
}


//============================================================
//  update_audio_stream
//============================================================
void osd_update_audio_stream(running_machine *machine, short *buffer, int samples_this_frame)
{
	if (!pauseg)
		audio_batch_cb(buffer, samples_this_frame);
}


//============================================================
//  set_mastervolume
//============================================================
void osd_set_mastervolume(int attenuation)
{
   // if we had actual sound output, we would adjust the global
   // volume in response to this function
}


//============================================================
//  customize_input_type_list
//============================================================
void osd_customize_input_type_list(input_type_desc *typelist)
{
	// This function is called on startup, before reading the
	// configuration from disk. Scan the list, and change the
	// default control mappings you want. It is quite possible
	// you won't need to change a thing.
}
