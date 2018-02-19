
void retro_poll_mame_input();

static int rtwi = 320, rthe = 240, topw = 1024;		/* DEFAULT TEXW/TEXH/PITCH */
int SHIFTON = -1;
char RPATH[512];

#ifdef M16B
	uint16_t videoBuffer[1024 * 1024];
	#define PITCH 1
#else
	unsigned int videoBuffer[1024 * 1024];
	#define PITCH 1 * 2
#endif

retro_video_refresh_t video_cb = NULL;
retro_environment_t environ_cb = NULL;

extern void retro_finish();
extern void retro_main_loop();
int RLOOP = 1;

const char *retro_save_directory;
const char *retro_system_directory;
const char *retro_content_directory;

retro_log_printf_t log_cb;

static retro_input_state_t input_state_cb = NULL;
static retro_audio_sample_batch_t audio_batch_cb = NULL;

#if defined(HAVE_OPENGL) || defined(HAVE_OPENGLES)
	#include "retroogl.c"
#endif

static retro_input_poll_t input_poll_cb;
void retro_set_audio_sample_batch(retro_audio_sample_batch_t cb) { audio_batch_cb = cb; }
void retro_set_input_state(retro_input_state_t cb) { input_state_cb = cb; }
void retro_set_input_poll(retro_input_poll_t cb) { input_poll_cb = cb; }
void retro_set_audio_sample(retro_audio_sample_t cb) { }
void retro_set_video_refresh(retro_video_refresh_t cb) { video_cb = cb; }

void retro_set_environment(retro_environment_t cb)
{
	static const struct retro_variable vars[] = {
      	{ "mame_mini_frame_skip", 	"Set frameskip; 0|1|2|3|4|automatic" },
	{ "mame_mini_aspect_ratio",	"Core provided aspect ratio; DAR|PAR" },
      	{ "mame_mini_turbo_button", 	"Enable autofire; disabled|button 1|button 2|R2 to button 1 mapping|R2 to button 2 mapping" },
      	{ "mame_mini_turbo_delay", 	"Set autofire pulse speed; medium|slow|fast" },
      	{ "mame_mini_sample_rate", 	"Set sample rate (Restart); 48000Hz|44100Hz|32000Hz|22050Hz" },
      	{ "mame_mini_kb_input", 	"Keyboard input; enabled|disabled" },
      	{ "mame_mini_adj_brightness",
	   "Set brightness; default|+1%|+2%|+3%|+4%|+5%|+6%|+7%|+8%|+9%|+10%|+11%|+12%|+13%|+14%|+15%|+16%|+17%|+18%|+19%|+20%|-20%|-19%|-18%|-17%|-16%|-15%|-14%|-13%|-12%|-11%|-10%|-9%|-8%|-7%|-6%|-5%|-4%|-3%|-2%|-1%" },
      	{ "mame_mini_adj_contrast",
	   "Set contrast; default|+1%|+2%|+3%|+4%|+5%|+6%|+7%|+8%|+9%|+10%|+11%|+12%|+13%|+14%|+15%|+16%|+17%|+18%|+19%|+20%|-20%|-19%|-18%|-17%|-16%|-15%|-14%|-13%|-12%|-11%|-10%|-9%|-8%|-7%|-6%|-5%|-4%|-3%|-2%|-1%" },
      	{ "mame_mini_adj_gamma",
	   "Set gamma; default|+1%|+2%|+3%|+4%|+5%|+6%|+7%|+8%|+9%|+10%|+11%|+12%|+13%|+14%|+15%|+16%|+17%|+18%|+19%|+20%|-20%|-19%|-18%|-17%|-16%|-15%|-14%|-13%|-12%|-11%|-10%|-9%|-8%|-7%|-6%|-5%|-4%|-3%|-2%|-1%" },

      	{ NULL, NULL },
   	};

   	environ_cb = cb;
   	cb(RETRO_ENVIRONMENT_SET_VARIABLES, (void*)vars);
}

static void check_variables(void)
{
	struct retro_variable var = {0};
   	bool tmp_ar = set_par;

   	var.key = "mame_mini_aspect_ratio";
   	var.value = NULL;
   	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   	{
      		if (!strcmp(var.value, "PAR"))
			set_par = true;
		else
			set_par = false;
   	}

   	var.key = "mame_mini_kb_input";
   	var.value = NULL;
   	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   	{
      		if (!strcmp(var.value, "enabled"))
			keyboard_input = true;
		else
			keyboard_input = false;
   	}

	var.key = "mame_mini_frame_skip";
   	var.value = NULL;
   	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   	{
		int temp_fs = set_frame_skip;
		if (!strcmp(var.value, "automatic"))
			set_frame_skip = -1;
		else
			set_frame_skip = atoi(var.value);

		if (temp_fs != set_frame_skip)
			video_set_frameskip(set_frame_skip);
   	}

   	var.key = "mame_mini_turbo_button";
   	var.value = NULL;
   	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   	{
      		if (!strcmp(var.value, "button 1"))
			turbo_enable = 1;
      		else if (!strcmp(var.value, "button 2"))
			turbo_enable = 2;
      		else if (!strcmp(var.value, "R2 to button 1 mapping"))
			turbo_enable = 3;
      		else if (!strcmp(var.value, "R2 to button 2 mapping"))
			turbo_enable = 4;
      		else
			turbo_enable = 0;
   	}

   	var.key = "mame_mini_turbo_delay";
   	var.value = NULL;
   	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   	{
      		if (!strcmp(var.value, "medium"))
			turbo_delay = 5;
      		else if (!strcmp(var.value, "slow"))
			turbo_delay = 7;
		else
			turbo_delay = 3;
   	}

   	var.key = "mame_mini_sample_rate";
   	var.value = NULL;
   	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
		sample_rate = atoi(var.value);

   	var.key = "mame_mini_adj_brightness";
   	var.value = NULL;
   	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   	{
		float temp_value = arroffset[0];
		if (!strcmp(var.value, "default"))
			arroffset[0] = 0.0;
		else
			arroffset[0] = (float)atoi(var.value) / 100.0f;

		if (temp_value != arroffset[0])
			adjust_opt[0] = adjust_opt[3] = 1;
   	}

   	var.key = "mame_mini_adj_contrast";
   	var.value = NULL;
   	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   	{
		float temp_value = arroffset[1];
		if (!strcmp(var.value, "default"))
			arroffset[1] = 0.0;
		else
			arroffset[1] = (float)atoi(var.value) / 100.0f;

		if (temp_value != arroffset[1])
			adjust_opt[0] = adjust_opt[4] = 1;
   	}

   	var.key = "mame_mini_adj_gamma";
   	var.value = NULL;
   	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   	{
		float temp_value = arroffset[2];
		if (!strcmp(var.value, "default"))
			arroffset[2] = 0.0;
		else
			arroffset[2] = (float)atoi(var.value) / 100.0f;

		if (temp_value != arroffset[2])
			adjust_opt[0] = adjust_opt[5] = 1;
   	}

   	if (tmp_ar != set_par)
		update_geometry();
}

unsigned retro_api_version(void)
{
	return RETRO_API_VERSION;
}

static void update_geometry()
{
	struct retro_system_av_info av_info;
   	retro_get_system_av_info( &av_info);
   	environ_cb(RETRO_ENVIRONMENT_SET_GEOMETRY, &av_info);
}

void init_input_descriptors(void)
{
#define describe_buttons(INDEX) \
		{ INDEX, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP,    	"Joystick Up" }, \
   		{ INDEX, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN,  	"Joystick Down" }, \
		{ INDEX, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT,	"Joystick Left" }, \
   		{ INDEX, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT, 	"Joystick Right" }, \
   		{ INDEX, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A,     	"Button 1" }, \
   		{ INDEX, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B,     	"Button 2" }, \
   		{ INDEX, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_X,     	"Button 3" }, \
   		{ INDEX, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_Y,     	"Button 4" }, \
   		{ INDEX, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L,     	"Button 5" }, \
   		{ INDEX, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R,     	"Button 6" }, \
   		{ INDEX, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R2,     "Turbo Button" }, \
   		{ INDEX, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_SELECT,	"Insert Coin" }, \
   		{ INDEX, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START,	"Start" },

	struct retro_input_descriptor desc[] = {
		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT,	"Joystick Left" }, \
   		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT, 	"Joystick Right" }, \
   		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP,    	"Joystick Up" }, \
   		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN,  	"Joystick Down" }, \
   		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A,     	"Button 1" }, \
   		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B,     	"Button 2" }, \
   		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_X,     	"Button 3" }, \
   		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_Y,     	"Button 4" }, \
   		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L,     	"Button 5" }, \
   		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R,     	"Button 6" }, \
   		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L2,     "UI Menu" }, \
   		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R2,     "Turbo Button" }, \
   		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L3,     "Service" }, \
   		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R3,     "Framerate" }, \
   		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_SELECT,	"Insert Coin" }, \
   		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START,	"Start" },

      		describe_buttons(1)
      		describe_buttons(2)
      		describe_buttons(3)
      		{ 0, 0, 0, 0, NULL }
   	};
   	environ_cb(RETRO_ENVIRONMENT_SET_INPUT_DESCRIPTORS, desc);
}


void retro_get_system_info(struct retro_system_info *info)
{
	memset(info, 0, sizeof(*info));
   	info->library_name = "MAME2010_MINI";
#ifndef GIT_VERSION
	#define GIT_VERSION ""
#endif
   	info->library_version = GIT_VERSION  "(Based by MAME0.139)";
   	info->valid_extensions = "zip|chd|7z";
   	info->need_fullpath = true;
   	info->block_extract = true;
}

void retro_get_system_av_info(struct retro_system_av_info *info)
{
	info->geometry.base_width   = rtwi;
   	info->geometry.base_height  = rthe;

   	info->geometry.max_width    = rtwi * 2;
   	info->geometry.max_height   = rthe * 2;

   	float display_ratio = set_par ? vertical ? (float)rthe / (float)rtwi : (float)rtwi / (float)rthe : vertical ? 3.0f / 4.0f : 4.0f / 3.0f;
   	info->geometry.aspect_ratio = display_ratio;

   	info->timing.fps            = refresh_rate;
   	info->timing.sample_rate    = (double)sample_rate;

#if 1	/* Test */
	int common_factor = 1;
	if (set_par)
	{
		int temp_width = rtwi;
		int temp_height = rthe;
		while (temp_width != temp_height)
		{
			if (temp_width > temp_height)
				temp_width -= temp_height;
			else
				temp_height -= temp_width;
		}
		common_factor = temp_height;
	}
	write_log("Current aspect ratio = %d : %d , screen refresh rate = %f , sound sample rate = %.1f \n", set_par ? vertical ? rthe / common_factor : rtwi / common_factor :
			vertical ? 3 : 4, set_par ? vertical ? rtwi / common_factor : rthe / common_factor : vertical ? 4 : 3, info->timing.fps, info->timing.sample_rate);
#endif
}

void retro_init (void)
{
	struct retro_log_callback log;
   	const char *system_dir  = NULL;
   	const char *content_dir = NULL;
   	const char *save_dir    = NULL;

   	if (environ_cb(RETRO_ENVIRONMENT_GET_LOG_INTERFACE, &log))
      		log_cb = log.log;
   	else
      		log_cb = NULL;

   	if (environ_cb(RETRO_ENVIRONMENT_GET_SYSTEM_DIRECTORY, &system_dir) && system_dir)
   	{
      		/* if defined, use the system directory */
      		retro_system_directory = system_dir;
   	}

   	if (log_cb)
      		log_cb(RETRO_LOG_INFO, "SYSTEM_DIRECTORY: %s", retro_system_directory);

   	if (environ_cb(RETRO_ENVIRONMENT_GET_CONTENT_DIRECTORY, &content_dir) && content_dir)
   	{
      		// if defined, use the system directory
      		retro_content_directory=content_dir;
   	}

   	if (log_cb)
      		log_cb(RETRO_LOG_INFO, "CONTENT_DIRECTORY: %s", retro_content_directory);


   	if (environ_cb(RETRO_ENVIRONMENT_GET_SAVE_DIRECTORY, &save_dir) && save_dir)
   	{
      		/* If save directory is defined use it, otherwise use system directory. */
      		retro_save_directory = *save_dir ? save_dir : retro_system_directory;
   	}
   	else
   	{
      		/* make retro_save_directory the same, in case RETRO_ENVIRONMENT_GET_SAVE_DIRECTORY is not implemented by the frontend. */
      		retro_save_directory=retro_system_directory;
   	}

   	if (log_cb)
      		log_cb(RETRO_LOG_INFO, "SAVE_DIRECTORY: %s", retro_save_directory);
}

void retro_deinit(void)
{
	if (retro_load_ok)
		retro_finish();

	LOGI("Retro DeInit\n");
}

void retro_reset (void)
{
	mame_reset = 1;
}

void retro_run (void)
{
	bool updated = false;
   	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE_UPDATE, &updated) && updated)
      		check_variables();

	turbo_state > turbo_delay ? turbo_state = 0 : turbo_state++;

	retro_poll_mame_input();
	retro_main_loop();

	RLOOP = 1;

#if defined(HAVE_OPENGL) || defined(HAVE_OPENGLES)
	do_gl2d();
#else
	if (draw_this_frame)
      		video_cb(videoBuffer, rtwi, rthe, topw << PITCH);
   	else
      		video_cb(	NULL, rtwi, rthe, topw << PITCH);
#endif
}


void prep_retro_rotation(int rot)
{
	LOGI("Rotation:%d\n", rot);
   	environ_cb(RETRO_ENVIRONMENT_SET_ROTATION, &rot);
}


bool retro_load_game(const struct retro_game_info *info)
{
	char basename[128];
   	int result;

#ifdef M16B
	enum retro_pixel_format fmt = RETRO_PIXEL_FORMAT_RGB565;
#else
   	enum retro_pixel_format fmt = RETRO_PIXEL_FORMAT_XRGB8888;
#endif

   	if (!environ_cb(RETRO_ENVIRONMENT_SET_PIXEL_FORMAT, &fmt))
   	{
      		fprintf(stderr, "RGB pixel format is not supported.\n");
      		exit(0);
   	}

	init_input_descriptors();

   	check_variables();
#ifdef M16B
	memset(videoBuffer, 0, 1024 * 1024 * 2);
#else
   	memset(videoBuffer, 0, 1024 * 1024 * 2 * 2);
#endif

#if defined(HAVE_OPENGL) || defined(HAVE_OPENGLES)
	#ifdef HAVE_OPENGLES
   	   hw_render.context_type = RETRO_HW_CONTEXT_OPENGLES2;
	#else
   	   hw_render.context_type = RETRO_HW_CONTEXT_OPENGL;
	#endif
	hw_render.context_reset = context_reset;
   	hw_render.context_destroy = context_destroy;

   	if (!environ_cb(RETRO_ENVIRONMENT_SET_HW_RENDER, &hw_render))
      		return false;
#endif
	basename[0] = '\0';
   	extract_basename(basename, info->path, sizeof(basename));
   	extract_directory(g_rom_dir, info->path, sizeof(g_rom_dir));
   	strcpy(RPATH, info->path);

   	result = mmain(1, RPATH);
   	if (result != 1)
	{
        	printf("Error: mame return an error\n");
		return 0;
   	}

	retro_load_ok = true;

   	video_set_frameskip(set_frame_skip);

   	for (int i = 0; i < 6; i++)
		adjust_opt[i] = 1;

   	return 1;
}

void retro_unload_game(void)
{
	if (!pauseg)
		pauseg = 1;

	LOGI("Retro unload_game \n");
}


// Stubs
size_t retro_serialize_size(void) { return 0; }
bool retro_serialize(void *data, size_t size) { return false; }
bool retro_unserialize(const void *data, size_t size) { return false; }
unsigned retro_get_region(void) {return RETRO_REGION_NTSC; }
void *retro_get_memory_data(unsigned type) {return 0; }
size_t retro_get_memory_size(unsigned type) {return 0; }
bool retro_load_game_special(unsigned game_type, const struct retro_game_info *info, size_t num_info) { return false; }
void retro_cheat_reset(void) { }
void retro_cheat_set(unsigned unused, bool unused1, const char *unused2) { }
void retro_set_controller_port_device(unsigned in_port, unsigned device) { }
