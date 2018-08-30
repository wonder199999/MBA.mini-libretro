#include <unistd.h>

#include "osdepend.h"
#include "emu.h"
#include "clifront.h"
#include "render.h"
#include "ui.h"
#include "uiinput.h"
#include "libretro.h"
#include "options.h"


/*************************************************************************/
//	GLOBAL VARIABLES
/*************************************************************************/

/* fake a keyboard mapped to retro joypad */
enum
{
	KEY_F11,
	KEY_TAB,
	KEY_F3,
	KEY_F2,
	KEY_START,
	KEY_COIN,
	KEY_BUTTON_1,
	KEY_BUTTON_2,
	KEY_BUTTON_3,
	KEY_BUTTON_4,
	KEY_BUTTON_5,
	KEY_BUTTON_6,
	KEY_JOYSTICK_U,
	KEY_JOYSTICK_D,
	KEY_JOYSTICK_L,
	KEY_JOYSTICK_R,
	KEY_TOTAL
};

struct _keyboard_table
{
	const char	*mame_key_name;
	INT32		retro_key_name;
	input_item_id	mame_key;
};

static const _keyboard_table ktable[] = {
	{"A",        RETROK_a,           ITEM_ID_A},
/*   	{"B",        RETROK_b,           ITEM_ID_B}, */
   	{"C",        RETROK_c,           ITEM_ID_C},
   	{"D",        RETROK_d,           ITEM_ID_D},
/*   	{"E",        RETROK_e,           ITEM_ID_E}, */
   	{"F",        RETROK_f,           ITEM_ID_F},
   	{"G",        RETROK_g,           ITEM_ID_G},
/*   	{"H",        RETROK_h,           ITEM_ID_H}, */
   	{"I",        RETROK_i,           ITEM_ID_I},
   	{"J",        RETROK_j,           ITEM_ID_J},
   	{"K",        RETROK_k,           ITEM_ID_K},
   	{"L",        RETROK_l,           ITEM_ID_L},
/*   	{"M",        RETROK_m,           ITEM_ID_M},
   	{"N",        RETROK_n,           ITEM_ID_N},
   	{"O",        RETROK_o,           ITEM_ID_O},
   	{"P",        RETROK_p,           ITEM_ID_P}, */
   	{"Q",        RETROK_q,           ITEM_ID_Q},
   	{"R",        RETROK_r,           ITEM_ID_R},
   	{"S",        RETROK_s,           ITEM_ID_S},
/*   	{"T",        RETROK_t,           ITEM_ID_T},
   	{"U",        RETROK_u,           ITEM_ID_U}, */
   	{"V",        RETROK_v,           ITEM_ID_V},
   	{"W",        RETROK_w,           ITEM_ID_W},
   	{"X",        RETROK_x,           ITEM_ID_X},
/*   	{"Y",        RETROK_y,           ITEM_ID_Y}, */
   	{"Z",        RETROK_z,           ITEM_ID_Z},
/*   	{"0",        RETROK_0,           ITEM_ID_0}, */
   	{"1",        RETROK_1,           ITEM_ID_1},
   	{"2",        RETROK_2,           ITEM_ID_2},
   	{"3",        RETROK_3,           ITEM_ID_3},
   	{"4",        RETROK_4,           ITEM_ID_4},
   	{"5",        RETROK_5,           ITEM_ID_5},
   	{"6",        RETROK_6,           ITEM_ID_6},
   	{"7",        RETROK_7,           ITEM_ID_7},
   	{"8",        RETROK_8,           ITEM_ID_8},
/*   	{"9",        RETROK_9,           ITEM_ID_9},
   	{"F1",       RETROK_F1,          ITEM_ID_F1}, */
   	{"F2",       RETROK_F2,          ITEM_ID_F2},
   	{"F3",       RETROK_F3,          ITEM_ID_F3},
/*   	{"F4",       RETROK_F4,          ITEM_ID_F4},
   	{"F5",       RETROK_F5,          ITEM_ID_F5},
   	{"F6",       RETROK_F6,          ITEM_ID_F6},
   	{"F7",       RETROK_F7,          ITEM_ID_F7},
   	{"F8",       RETROK_F8,          ITEM_ID_F8},
   	{"F9",       RETROK_F9,          ITEM_ID_F9},
   	{"F10",      RETROK_F10,         ITEM_ID_F10}, */
   	{"F11",      RETROK_F11,         ITEM_ID_F11},
/*   	{"F12",      RETROK_F12,         ITEM_ID_F12},
   	{"F13",      RETROK_F13,         ITEM_ID_F13},
   	{"F14",      RETROK_F14,         ITEM_ID_F14},
   	{"F15",      RETROK_F15,         ITEM_ID_F15},
   	{"Esc",      RETROK_ESCAPE,      ITEM_ID_ESC},
   	{"TILDE",    RETROK_BACKQUOTE,   ITEM_ID_TILDE},
   	{"MINUS",    RETROK_MINUS,       ITEM_ID_MINUS},
   	{"EQUALS",   RETROK_EQUALS,      ITEM_ID_EQUALS},
   	{"BKCSPACE", RETROK_BACKSPACE,   ITEM_ID_BACKSPACE}, */
   	{"TAB",      RETROK_TAB,         ITEM_ID_TAB},
/*   	{"(",        RETROK_LEFTPAREN,   ITEM_ID_OPENBRACE},
   	{")",        RETROK_RIGHTPAREN,  ITEM_ID_CLOSEBRACE}, */
   	{"ENTER",    RETROK_RETURN,      ITEM_ID_ENTER},
/*   	{"·",        RETROK_COLON,       ITEM_ID_COLON},
   	{"\'",       RETROK_QUOTE,       ITEM_ID_QUOTE},
   	{"BCKSLASH", RETROK_BACKSLASH,   ITEM_ID_BACKSLASH},
   	{",",        RETROK_COMMA,       ITEM_ID_COMMA},
   	{"/",        RETROK_SLASH,       ITEM_ID_SLASH}, */
   	{"SPACE",    RETROK_SPACE,       ITEM_ID_SPACE},
/*   	{"INS",      RETROK_INSERT,      ITEM_ID_INSERT},
   	{"DEL",      RETROK_DELETE,      ITEM_ID_DEL},
   	{"HOME",     RETROK_HOME,        ITEM_ID_HOME},
   	{"END",      RETROK_END,         ITEM_ID_END},
   	{"PGUP",     RETROK_PAGEUP,      ITEM_ID_PGUP},
   	{"PGDW",     RETROK_PAGEDOWN,    ITEM_ID_PGDN}, */
   	{"LEFT",     RETROK_LEFT,        ITEM_ID_LEFT},
   	{"RIGHT",    RETROK_RIGHT,       ITEM_ID_RIGHT},
   	{"UP",       RETROK_UP,          ITEM_ID_UP},
   	{"DOWN",     RETROK_DOWN,        ITEM_ID_DOWN},
   	{"KO",       RETROK_KP0,         ITEM_ID_0_PAD},
/*	{"K1",       RETROK_KP1,         ITEM_ID_1_PAD}, */
   	{"K2",       RETROK_KP2,         ITEM_ID_2_PAD},
   	{"K3",       RETROK_KP3,         ITEM_ID_3_PAD},
   	{"K4",       RETROK_KP4,         ITEM_ID_4_PAD},
/*   	{"K5",       RETROK_KP5,         ITEM_ID_5_PAD}, */
   	{"K6",       RETROK_KP6,         ITEM_ID_6_PAD},
/*   	{"K7",       RETROK_KP7,         ITEM_ID_7_PAD}, */
   	{"K8",       RETROK_KP8,         ITEM_ID_8_PAD},
/*   	{"K9",       RETROK_KP9,         ITEM_ID_9_PAD},
   	{"K/",       RETROK_KP_DIVIDE,   ITEM_ID_SLASH_PAD},
   	{"K*",       RETROK_KP_MULTIPLY, ITEM_ID_ASTERISK},
   	{"K-",       RETROK_KP_MINUS,    ITEM_ID_MINUS_PAD},
   	{"K+",       RETROK_KP_PLUS,     ITEM_ID_PLUS_PAD}, */
   	{"KDEL",     RETROK_KP_PERIOD,   ITEM_ID_DEL_PAD},
   	{"KRTRN",    RETROK_KP_ENTER,    ITEM_ID_ENTER_PAD},
/*   	{"PRINT",    RETROK_PRINT,       ITEM_ID_PRTSCR},
   	{"PAUSE",    RETROK_PAUSE,       ITEM_ID_PAUSE}, */
   	{"LSHFT",    RETROK_LSHIFT,      ITEM_ID_LSHIFT},
   	{"RSHFT",    RETROK_RSHIFT,      ITEM_ID_RSHIFT},
   	{"LCTRL",    RETROK_LCTRL,       ITEM_ID_LCONTROL},
   	{"RCTRL",    RETROK_RCTRL,       ITEM_ID_RCONTROL},
   	{"LALT",     RETROK_LALT,        ITEM_ID_LALT},
   	{"RALT",     RETROK_RALT,        ITEM_ID_RALT},
/*   	{"SCRLOCK",  RETROK_SCROLLOCK,   ITEM_ID_SCRLOCK},
   	{"NUMLOCK",  RETROK_NUMLOCK,     ITEM_ID_NUMLOCK},
   	{"CPSLOCK",  RETROK_CAPSLOCK,    ITEM_ID_CAPSLOCK},
   	{"LMETA",    RETROK_LMETA,       ITEM_ID_LWIN},
   	{"RMETA",    RETROK_RMETA,       ITEM_ID_RWIN},
   	{"MENU",     RETROK_MENU,        ITEM_ID_MENU},
   	{"BREAK",    RETROK_BREAK,       ITEM_ID_CANCEL}, */
   	{"-1",       -1,                 ITEM_ID_INVALID}
};

struct _neogeo_bioses
{
	const char name[16];
	const char desc[32];
	const char bios[24];
};

static const struct _neogeo_bioses	neogeo_bioses[] = {
	{ "euro",	  "Europe MVS(Ver. 2)",		"sp-s2.sp1" },
	{ "euro-s1",	  "Europe MVS(Ver. 1)",		"sp-s.sp1" },
	{ "us",		  "USA MVS(Ver. 2?)",		"sp-u2.sp1" },
	{ "us-e",	  "USA MVS(Ver. 1)",		"sp-e.sp1" },
	{ "asia",	  "Asia MVS(Ver. 3)",		"asia-s3.rom" },
	{ "mv1c",	  "Asia MVS(Latest)",		"sp-45.sp1" },
	{ "japan",	  "Japan MVS(Ver. 3)",		"vs-bios.rom" },
	{ "japan-s2",	  "Japan MVS(Ver. 2)",		"sp-j2.sp1" },
	{ "japan-s1",	  "Japan MVS(Ver. 1)",		"sp1.jipan.1024" },
	{ "japan-j3",	  "Japan MVS(J3)",		"japan-j3.bin" },
	{ "japan-hotel",  "Custom Japanese Hotel",	"sp-1v1_3db8c.bin" },
	{ "unibios32",	  "UniBIOS(Ver. 3.2)",		"uni-bios_3_2.rom" },
	{ "unibios31",	  "UniBIOS(Ver. 3.1)",		"uni-bios_3_1.rom" },
	{ "unibios30",	  "UniBIOS(Ver. 3.0)",		"uni-bios_3_0.rom" },
	{ "unibios23",	  "UniBIOS(Ver. 2.3)",		"uni-bios_2_3.rom" },
	{ "unibios23o",	  "UniBIOS(Ver. 2.3 older?)",	"uni-bios_2_3o.rom" },
	{ "unibios22",	  "UniBIOS(Ver. 2.2)",		"uni-bios_2_2.rom" },
	{ "unibios21",	  "UniBIOS(Ver. 2.1)",		"uni-bios_2_1.rom" },
	{ "unibios20",	  "UniBIOS(Ver. 2.0)",		"uni-bios_2_0.rom" },
	{ "unibios13",	  "UniBIOS(Ver. 1.3)",		"uni-bios_1_3.rom" },
	{ "unibios12",	  "UniBIOS(Ver. 1.2)",		"uni-bios_1_2.rom" },
	{ "unibios12o",	  "UniBIOS(Ver. 1.2 older)",	"uni-bios_1_2o.rom" },
	{ "unibios11",	  "UniBIOS(Ver. 1.1)",		"uni-bios_1_1.rom" },
	{ "unibios10",	  "UniBIOS(Ver. 1.0)",		"uni-bios_1_0.rom" },
	{ "debug",	  "Debug MVS",			"neodebug.rom" },
	{ "asia-aes",	  "Asia AES",			"neo-epo.sp1" },
	{ "japan-aes",	  "Japan AES",			"neo-po.sp1" }
};

// extern variables
bool verify_rom_hash = false;
bool allow_select_newgame = false;
bool RETRO_LOOP = true;

#ifdef _WIN32
	char slash = '\\';
#else
	char slash = '/';
#endif

// input device
static input_device *P1_device = NULL;		// P1 JOYPAD
static input_device *P2_device = NULL;		// P2 JOYPAD
static input_device *P3_device = NULL; 		// P3 JOYPAD
static input_device *P4_device = NULL; 		// P4 JOYPAD
static input_device *KB_device = NULL;		// KEYBD

// rendering target
static render_target *our_target = NULL;

// state
static UINT8  pad_state[4][KEY_TOTAL];
static UINT16 retrokbd_state[RETROK_LAST];
static UINT16 retrokbd_state2[RETROK_LAST];

static char RETRO_GAME_PATH[512];
static char MAME_GAME_PATH[1024];
static char MAME_GAME_NAME[512];
static char retro_content_dir[1024];

static bool draw_this_frame;
static bool set_par = false;
static bool retro_load_ok = false;
static bool keyboard_input = true;
static bool macro_enable = true;
static bool is_neogeo = false;

static INT32 rtwi = 320, rthe = 240, topw = 320;	/* DEFAULT TEXW/TEXH/PITCH */
static INT32 ui_ipt_pushchar = -1;
static INT32 set_frame_skip;
static INT32 vertical;
static INT32 orient;
static INT32 set_neogeo_bios;
static UINT32 tate;
static UINT32 screenRot;
static UINT32 pauseg = 0;
static UINT32 mame_reset = 0;
static UINT32 FirstTimeUpdate = 1;
static UINT32 turbo_enable;
static UINT32 turbo_delay;
static UINT32 turbo_state;
static UINT32 macro_state;
static UINT32 sample_rate = 48000;
static UINT32 adjust_opt[7] = { 0/*Enable/Disable*/, 0/*Limit*/, 0/*GetRefreshRate*/, 0/*Brightness*/, 0/*Contrast*/, 0/*Gamma*/, 0/*Overclock*/ };
static float arroffset[4] = { 0/*For brightness*/, 0/*For contrast*/, 0/*For gamma*/, 1.0/*For overclock*/ };
static double refresh_rate = 60.0;


/**************************************************************************/
//	FUNCTION PROTOTYPES
/**************************************************************************/

extern void retro_finish(void);
extern void retro_main_loop(void);
void osd_init( running_machine *machine );
void osd_update( running_machine *machine, int skip_redraw );
void osd_update_audio_stream( running_machine *machine, short *buffer, int samples_this_frame );
void osd_set_mastervolume( int attenuation );
void osd_customize_input_type_list( input_type_desc *typelist );
void osd_exit( running_machine &machine );

static void update_geometry(void);
static int mmain(int argc, const char *argv);
static int executeGame(char *path);
static int iptdev_get_state(void *device_internal, void *item_internal);
static void retro_poll_mame_input(void);

/**************************************************************************/
//	MACROS
/**************************************************************************/

#define PLAYER_PRESS(button)	input_state_cb(i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_##button)
#define MAX_JOYPADS	(4)

#ifdef ANDROID
	#include <android/log.h>
	#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "mba_mini", __VA_ARGS__))
	#undef write_log
	#define write_log LOGI
#else
	#define write_log	printf
	#define LOGI		printf
#endif

#ifdef DEBUG_LOG
	#define LOG(msg) fprintf(stderr, "%s\n", msg)
#else
	#define LOG(msg)
#endif

#if !defined(HAVE_OPENGL) && !defined(HAVE_OPENGLES) && !defined(HAVE_RGB32)
	#define M16B
#endif

#ifdef M16B
	#define FUNC_PREFIX(x)	rgb565_##x
	#define PIXEL_TYPE	UINT16
	#define SRCSHIFT_R	3
	#define SRCSHIFT_G	2
	#define SRCSHIFT_B	3
	#define DSTSHIFT_R	11
	#define DSTSHIFT_G	5
	#define DSTSHIFT_B	0
#else
	#define FUNC_PREFIX(x)	rgb888_##x
	#define PIXEL_TYPE	UINT32
	#define SRCSHIFT_R	0
	#define SRCSHIFT_G	0
	#define SRCSHIFT_B	0
	#define DSTSHIFT_R	16
	#define DSTSHIFT_G	8
	#define DSTSHIFT_B	0
#endif

#ifdef M16B
	UINT16 videoBuffer[512 * 512];
	#define PITCH 1
#else
	UINT32 videoBuffer[512 * 512];
	#define PITCH 1 * 2
#endif

#include "rendersw.c"

retro_log_printf_t log_cb = NULL;
retro_environment_t environ_cb = NULL;
retro_video_refresh_t video_cb = NULL;

static retro_input_state_t input_state_cb = NULL;
static retro_audio_sample_batch_t audio_batch_cb = NULL;
static retro_input_poll_t input_poll_cb = NULL;

unsigned retro_get_region(void)
{
	return RETRO_REGION_NTSC;
}

size_t retro_serialize_size(void)
{
	return 0;
}

size_t retro_get_memory_size(unsigned type)
{
	return 0;
}

bool retro_serialize(void *data, size_t size)
{
	return false;
}

bool retro_unserialize(const void *data, size_t size)
{
	return false;
}

bool retro_load_game_special(unsigned game_type, const struct retro_game_info *info, size_t num_info)
{
	return false;
}

void *retro_get_memory_data(unsigned type)
{
	return 0;
}

void retro_cheat_reset(void)
{
}

void retro_cheat_set(unsigned unused, bool unused1, const char *unused2)
{
}

void retro_set_controller_port_device(unsigned in_port, unsigned device)
{
}

void retro_set_audio_sample_batch(retro_audio_sample_batch_t cb)
{
	audio_batch_cb = cb;
}

void retro_set_input_state(retro_input_state_t cb)
{
	input_state_cb = cb;
}

void retro_set_input_poll(retro_input_poll_t cb)
{
	input_poll_cb = cb;
}

void retro_set_audio_sample(retro_audio_sample_t cb)
{
}

void retro_set_video_refresh(retro_video_refresh_t cb)
{
	video_cb = cb;
}

static void extract_basename(char *buf, const char *path, size_t size)
{
	const char *base = strrchr(path, '/');

   	if (!base)
      		base = strrchr(path, '\\');

   	if (!base)
      		base = path;

   	if (*base == '\\' || *base == '/')
      		base++;

   	strncpy(buf, base, size - 1);
   	buf[size - 1] = '\0';

   	char *ext = strrchr(buf, '.');
   	if (ext)
      		*ext = '\0';
}

static void extract_directory(char *buf, const char *path, size_t size)
{
	strncpy(buf, path, size - 1);
   	buf[size - 1] = '\0';

   	char *base = strrchr(buf, '/');
   	if (!base)
      		base = strrchr(buf, '\\');

   	if (base)
      		*base = '\0';
   	else
      		buf[0] = '\0';
}

#if defined(HAVE_OPENGL) || defined(HAVE_OPENGLES)
	#include "retroogl.c"
#endif

/**************************************************************************/

void retro_set_environment(retro_environment_t cb)
{
	static const struct retro_variable vars[] = {
      	{ "mba_mini_cpu_overclock",	"Adjust CPU speed; disabled|110%|120%|130%|140%|150%|160%|170%|180%|190%|200%|50%|60%|70%|80%|90%" },
      	{ "mba_mini_frame_skip", 	"Set frameskip; 0|1|2|3|4|automatic" },
	{ "mba_mini_aspect_ratio",	"Core provided aspect ratio; DAR|PAR" },
      	{ "mba_mini_turbo_button", 	"Enable autofire; disabled|button 1|button 2|R2 to button 1 mapping|R2 to button 2 mapping" },
      	{ "mba_mini_turbo_delay", 	"Set autofire pulse speed; medium|slow|fast" },
      	{ "mba_mini_kb_input",		"Keyboard input; enabled|disabled" },
      	{ "mba_mini_macro_button", 	"Use macro button; disabled|assign A+B to L|assign A+B to R|assign C+D to L|assign C+D to R|assign A+B to L & C+D to R|assign A+B to R & C+D to L" },
      	{ "mba_mini_tate_mode", 	"T.A.T.E mode(Restart); disabled|enabled" },
      	{ "mba_mini_sample_rate", 	"Set sample rate (Restart); 48000Hz|44100Hz|32000Hz|22050Hz" },
	{ "mba_mini_rom_hash",		"Forced off ROM CRC verfiy(Restart); No|Yes" },
      	{ "mba_mini_neogeo_bios",
#if defined(USE_FULLY)
	   "Set NEOGEO BIOS(Restart); Default|Europe MVS(Ver. 2)|Europe MVS(Ver. 1)|USA MVS(Ver. 2?)|USA MVS(Ver. 1)|Asia MVS(Ver. 3)|Asia MVS(Latest)|Japan MVS(Ver. 3)|Japan MVS(Ver. 2)|Japan MVS(Ver. 1)|Japan MVS(J3)|Custom Japanese Hotel|UniBIOS(Ver. 3.2)|UniBIOS(Ver. 3.1)|UniBIOS(Ver. 3.0)|UniBIOS(Ver. 2.3)|UniBIOS(Ver. 2.3 older?)|UniBIOS(Ver. 2.2)|UniBIOS(Ver. 2.1)|UniBIOS(Ver. 2.0)|UniBIOS(Ver. 1.3)|UniBIOS(Ver. 1.2)|UniBIOS(Ver. 1.2 older)|UniBIOS(Ver. 1.1)|UniBIOS(Ver. 1.0)|Debug MVS|Asia AES|Japan AES" },
#else
	   "Set NEOGEO BIOS(Restart); Default|Europe MVS(Ver. 2)|USA MVS(Ver. 2?)|Asia MVS(Ver. 3)|Japan MVS(Ver. 3)|UniBIOS(Ver. 3.2)" },
#endif
      	{ "mba_mini_adj_brightness",
	   "Set brightness; default|+1%|+2%|+3%|+4%|+5%|+6%|+7%|+8%|+9%|+10%|+11%|+12%|+13%|+14%|+15%|+16%|+17%|+18%|+19%|+20%|-20%|-19%|-18%|-17%|-16%|-15%|-14%|-13%|-12%|-11%|-10%|-9%|-8%|-7%|-6%|-5%|-4%|-3%|-2%|-1%" },
      	{ "mba_mini_adj_contrast",
	   "Set contrast; default|+1%|+2%|+3%|+4%|+5%|+6%|+7%|+8%|+9%|+10%|+11%|+12%|+13%|+14%|+15%|+16%|+17%|+18%|+19%|+20%|-20%|-19%|-18%|-17%|-16%|-15%|-14%|-13%|-12%|-11%|-10%|-9%|-8%|-7%|-6%|-5%|-4%|-3%|-2%|-1%" },
      	{ "mba_mini_adj_gamma",
	   "Set gamma; default|+1%|+2%|+3%|+4%|+5%|+6%|+7%|+8%|+9%|+10%|+11%|+12%|+13%|+14%|+15%|+16%|+17%|+18%|+19%|+20%|-20%|-19%|-18%|-17%|-16%|-15%|-14%|-13%|-12%|-11%|-10%|-9%|-8%|-7%|-6%|-5%|-4%|-3%|-2%|-1%" },

      	{ NULL, NULL },	};

   	environ_cb = cb;
   	cb(RETRO_ENVIRONMENT_SET_VARIABLES, (void*)vars);
}

static void check_variables(void)
{
	struct retro_variable var = { 0 };
	bool tmp_ar = set_par;

	var.key = "mba_mini_neogeo_bios";
	var.value = NULL;
	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
	{
		set_neogeo_bios = -1;
		for (int i = 0; i < 27; i++)
		{
			if (!strcmp(var.value, neogeo_bioses[i].desc))
			{
				set_neogeo_bios = i;
				break;
			}
		}
	}
	else set_neogeo_bios = -1;

	var.key = "mba_mini_aspect_ratio";
	var.value = NULL;
	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
	{
		if (!strcmp(var.value, "PAR"))
			set_par = true;
		else
			set_par = false;
	}

	var.key = "mba_mini_tate_mode";
	var.value = NULL;
	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
	{
		if (!strcmp(var.value, "enabled"))
			tate = 1;
		else
			tate = 0;
   	}
	else tate = 0;

	var.key = "mba_mini_kb_input";
	var.value = NULL;
	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
	{
		if (!strcmp(var.value, "enabled"))
			keyboard_input = true;
		else
			keyboard_input = false;
	}

	var.key = "mba_mini_frame_skip";
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

	var.key = "mba_mini_turbo_button";
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

	var.key = "mba_mini_turbo_delay";
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

	var.key = "mba_mini_sample_rate";
	var.value = NULL;
	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
		sample_rate = atoi(var.value);

	var.key = "mba_mini_adj_brightness";
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

	var.key = "mba_mini_adj_contrast";
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

	var.key = "mba_mini_adj_gamma";
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

	var.key = "mba_mini_cpu_overclock";
	var.value = NULL;
	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
	{
		float temp_value = arroffset[3];
		if (!strcmp(var.value, "disabled"))
			arroffset[3] = 1.0f;
		else
			arroffset[3] = (float)atoi(var.value) / 100.0f;

		if (temp_value != arroffset[3])
			adjust_opt[0] = adjust_opt[6] = 1;
	}

	var.key = "mba_mini_macro_button";
	var.value = NULL;
	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
	{
		if (!strcmp(var.value, "disabled"))
			macro_state = 0;
		else if (!strcmp(var.value, "assign A+B to L"))
			macro_state = 1;
		else if (!strcmp(var.value, "assign A+B to R"))
			macro_state = 2;
		else if (!strcmp(var.value, "assign C+D to L"))
			macro_state = 3;
		else if (!strcmp(var.value, "assign C+D to R"))
			macro_state = 4;
		else if (!strcmp(var.value, "assign A+B to L & C+D to R"))
			macro_state = 5;
		else if (!strcmp(var.value, "assign A+B to R & C+D to L"))
			macro_state = 6;

		if (!macro_enable)
			macro_state = 0;
	}

	var.key = "mba_mini_rom_hash";
	var.value = NULL;
	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
	{
		if (!strcmp(var.value, "No"))
			verify_rom_hash = true;
		else
			verify_rom_hash = false;
	}

	if (tmp_ar != set_par)
		update_geometry();
}

unsigned retro_api_version(void)
{
	return RETRO_API_VERSION;
}

static void update_geometry(void)
{
	struct retro_system_av_info av_info;
   	retro_get_system_av_info(&av_info);
   	environ_cb(RETRO_ENVIRONMENT_SET_GEOMETRY, &av_info);
}

static void init_input_descriptors(void)
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
   		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L2,     	"UI Menu" },
   		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L3,     	"Service" },
   		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R3,     	"Framerate" },
      		describe_buttons(0)
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
   	info->library_name = "M.B.A mini";
#ifndef GIT_VERSION
	#define GIT_VERSION ""
#endif
   	info->library_version = GIT_VERSION " (M.B.A = MAME's skeleton + FBA's romsets)";
   	info->valid_extensions = "zip|chd|7z";
   	info->need_fullpath = true;
   	info->block_extract = true;
}

void retro_get_system_av_info(struct retro_system_av_info *info)
{
	int width =  (tate && vertical) ? rthe : rtwi;
	int height = (tate && vertical) ? rtwi : rthe;

	info->geometry.base_width   = width;
   	info->geometry.base_height  = height;

   	info->geometry.max_width    = width * 2;
   	info->geometry.max_height   = height * 2;

   	float display_ratio = set_par ? (vertical && !tate) ? (float)height / (float)width : (float)width / (float)height : (vertical && !tate) ? 3.0f / 4.0f : 4.0f / 3.0f;
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
	write_log("Current aspect ratio = %d : %d , screen refresh rate = %f , sound sample rate = %.1f \n", set_par ? vertical ? height / common_factor : width / common_factor :
			vertical ? 3 : 4, set_par ? vertical ? width / common_factor : height / common_factor : vertical ? 4 : 3, info->timing.fps, info->timing.sample_rate);
#endif
}

void retro_init (void)
{
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

	RETRO_LOOP = true;

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
	struct retro_log_callback log;
	char basename[128];
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
#ifdef M16B
	memset(videoBuffer, 0, 512 * 512 * 2);
#else
   	memset(videoBuffer, 0, 512 * 512 * 2 * 2);
#endif
   	check_variables();

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
	init_input_descriptors();

	basename[0] = '\0';
   	extract_basename(basename, info->path, sizeof(basename));
   	extract_directory(retro_content_dir, info->path, sizeof(retro_content_dir));
   	strcpy(RETRO_GAME_PATH, info->path);

	int result = mmain(1, RETRO_GAME_PATH);

   	if (result != 1)
	{
        	printf("Error: mame return an error\n");
		return 0;
   	}

	retro_load_ok = true;

   	video_set_frameskip(set_frame_skip);

   	for (int i = 0; i < 7; i++)
		adjust_opt[i] = 1;

   	if (environ_cb(RETRO_ENVIRONMENT_GET_LOG_INTERFACE, &log))
		log_cb = log.log;
   	else
		log_cb = NULL;

   	if (log_cb)
      		log_cb(RETRO_LOG_INFO, "CONTENT_DIRECTORY: %s", retro_content_dir);

	return 1;
}

void retro_unload_game(void)
{
	if (!pauseg)
		pauseg = 1;

	LOGI("Retro unload_game \n");
}


/**************************************************************************/

static int iptdev_get_state(void *device_internal, void *item_internal)
{
	UINT8 *itemdata = (UINT8 *)item_internal;
   	return *itemdata;
}

static const char *specific_games_initinput_vs6b[] = {
	"sf2",		"sf2ce",
	"sf2hf",	"sfzch",
	"ssf2",		"ssf2t",
	"sfa",		"sfa2",
	"sfa3",		"sfz2al",
	"hsf2",		"dstlk",
	"vsav",		"vsav2",
	"vhunt2",	"msh",
	"mshvsf",	"mvsc",
	"xmcota",	"xmvsf",
	"ringdest",	"nwarr",
	"ssoldier",
	NULL
};

static void initInput( running_machine *machine )
{
	UINT32 i;

   	KB_device = input_device_add(machine, DEVICE_CLASS_KEYBOARD, "Retrokdb", NULL);
   	if (KB_device == NULL) fatalerror("KBD Error creating keyboard device! \n");
   	P1_device = input_device_add(machine, DEVICE_CLASS_KEYBOARD, "Pad1", NULL);
   	if (P1_device == NULL) fatalerror("P1 Error creating keyboard device! \n");
   	P2_device = input_device_add(machine, DEVICE_CLASS_KEYBOARD, "Pad2", NULL);
   	if (P2_device == NULL) fatalerror("P2 Error creating keyboard device! \n");
   	P3_device = input_device_add(machine, DEVICE_CLASS_KEYBOARD, "Pad3", NULL);
   	if (P3_device == NULL) fatalerror("P3 Error creating keyboard device! \n");
   	P4_device = input_device_add(machine, DEVICE_CLASS_KEYBOARD, "Pad4", NULL);
   	if (P4_device == NULL) fatalerror("P4 Error creating keyboard device! \n");

   	/* our faux keyboard only has a couple of keys (corresponding to the common defaults) */
   	for (i = 0; i < RETROK_LAST; i++)
      		retrokbd_state[i] = retrokbd_state2[i] = 0;

	for (i = 0; i < MAX_JOYPADS; i++)
	{
		pad_state[i][KEY_F11] = pad_state[i][KEY_TAB] = pad_state[i][KEY_F2] =
		pad_state[i][KEY_START] = pad_state[i][KEY_COIN] = pad_state[i][KEY_BUTTON_1] =
		pad_state[i][KEY_BUTTON_2] = pad_state[i][KEY_BUTTON_3] = pad_state[i][KEY_BUTTON_4] =
		pad_state[i][KEY_BUTTON_5] = pad_state[i][KEY_BUTTON_6] = pad_state[i][KEY_JOYSTICK_U] =
		pad_state[i][KEY_JOYSTICK_D] = pad_state[i][KEY_JOYSTICK_L] = pad_state[i][KEY_JOYSTICK_R] = 0;
	}

   	fprintf(stderr, "SOURCE FILE: %s\n", machine->gamedrv->source_file);
   	fprintf(stderr, "PARENT: %s\n", machine->gamedrv->parent);
   	fprintf(stderr, "NAME: %s\n", machine->gamedrv->name);
   	fprintf(stderr, "DESCRIPTION: %s\n", machine->gamedrv->description);
   	fprintf(stderr, "YEAR: %s\n", machine->gamedrv->year);
   	fprintf(stderr, "MANUFACTURER: %s\n", machine->gamedrv->manufacturer);

	for (i = 0; ktable[i].retro_key_name != -1; i++)
      		input_device_item_add(KB_device, ktable[i].mame_key_name,
			&retrokbd_state[ktable[i].retro_key_name], ktable[i].mame_key, iptdev_get_state);

	/* Only for P1 */
   	input_device_item_add(P1_device, "F11",	     &pad_state[0][KEY_F11], 	    ITEM_ID_F11,   iptdev_get_state);	/* Bind display fps */
   	input_device_item_add(P1_device, "Tab",      &pad_state[0][KEY_TAB], 	    ITEM_ID_TAB,   iptdev_get_state);	/* Bind UI menu */
   	input_device_item_add(P1_device, "F3", 	     &pad_state[0][KEY_F3], 	    ITEM_ID_F3,    iptdev_get_state);	/* Bind reset */
   	input_device_item_add(P1_device, "F2",	     &pad_state[0][KEY_F2], 	    ITEM_ID_F2,    iptdev_get_state);	/* Bind service */
	/* P1 init */
   	input_device_item_add(P1_device, "P1 Start", &pad_state[0][KEY_START],	    ITEM_ID_1, 	   iptdev_get_state);
   	input_device_item_add(P1_device, "COIN1",    &pad_state[0][KEY_COIN],	    ITEM_ID_5, 	   iptdev_get_state);
   	input_device_item_add(P1_device, "P1 JoyU",  &pad_state[0][KEY_JOYSTICK_U], ITEM_ID_UP,    iptdev_get_state);
   	input_device_item_add(P1_device, "P1 JoyD",  &pad_state[0][KEY_JOYSTICK_D], ITEM_ID_DOWN,  iptdev_get_state);
   	input_device_item_add(P1_device, "P1 JoyL",  &pad_state[0][KEY_JOYSTICK_L], ITEM_ID_LEFT,  iptdev_get_state);
   	input_device_item_add(P1_device, "P1 JoyR",  &pad_state[0][KEY_JOYSTICK_R], ITEM_ID_RIGHT, iptdev_get_state);
	/* P2 init */
   	input_device_item_add(P2_device, "P2 Start", &pad_state[1][KEY_START], 	    ITEM_ID_2, 	   iptdev_get_state);
   	input_device_item_add(P2_device, "COIN2",    &pad_state[1][KEY_COIN], 	    ITEM_ID_6, 	   iptdev_get_state);
   	input_device_item_add(P2_device, "P2 JoyU",  &pad_state[1][KEY_JOYSTICK_U], ITEM_ID_R, 	   iptdev_get_state);
   	input_device_item_add(P2_device, "P2 JoyD",  &pad_state[1][KEY_JOYSTICK_D], ITEM_ID_F, 	   iptdev_get_state);
   	input_device_item_add(P2_device, "P2 JoyL",  &pad_state[1][KEY_JOYSTICK_L], ITEM_ID_D, 	   iptdev_get_state);
   	input_device_item_add(P2_device, "P2 JoyR",  &pad_state[1][KEY_JOYSTICK_R], ITEM_ID_G, 	   iptdev_get_state);
	/* P3 init */
   	input_device_item_add(P3_device, "P3 Start", &pad_state[2][KEY_START],	    ITEM_ID_3, 	   iptdev_get_state);
   	input_device_item_add(P3_device, "COIN3",    &pad_state[2][KEY_COIN], 	    ITEM_ID_7, 	   iptdev_get_state);
   	input_device_item_add(P3_device, "P3 JoyU",  &pad_state[2][KEY_JOYSTICK_U], ITEM_ID_I, 	   iptdev_get_state);
   	input_device_item_add(P3_device, "P3 JoyD",  &pad_state[2][KEY_JOYSTICK_D], ITEM_ID_K, 	   iptdev_get_state);
   	input_device_item_add(P3_device, "P3 JoyL",  &pad_state[2][KEY_JOYSTICK_L], ITEM_ID_J, 	   iptdev_get_state);
   	input_device_item_add(P3_device, "P3 JoyR",  &pad_state[2][KEY_JOYSTICK_R], ITEM_ID_L, 	   iptdev_get_state);
	/* P4 init */
   	input_device_item_add(P4_device, "P4 Start", &pad_state[3][KEY_START],	    ITEM_ID_4, 	   iptdev_get_state);
   	input_device_item_add(P4_device, "COIN4",    &pad_state[3][KEY_COIN], 	    ITEM_ID_8, 	   iptdev_get_state);
   	input_device_item_add(P4_device, "P4 JoyU",  &pad_state[3][KEY_JOYSTICK_U], ITEM_ID_8_PAD, iptdev_get_state);
   	input_device_item_add(P4_device, "P4 JoyD",  &pad_state[3][KEY_JOYSTICK_D], ITEM_ID_2_PAD, iptdev_get_state);
   	input_device_item_add(P4_device, "P4 JoyL",  &pad_state[3][KEY_JOYSTICK_L], ITEM_ID_4_PAD, iptdev_get_state);
   	input_device_item_add(P4_device, "P4 JoyR",  &pad_state[3][KEY_JOYSTICK_R], ITEM_ID_6_PAD, iptdev_get_state);

	/* Neo Geo buttons layout */
	if (!core_stricmp(machine->gamedrv->source_file, "src/mame/drivers/neogeo.inc"))
	{
      		input_device_item_add(P1_device, "P1 B1", &pad_state[0][KEY_BUTTON_1], ITEM_ID_LALT, 	 iptdev_get_state);
      		input_device_item_add(P1_device, "P1 B2", &pad_state[0][KEY_BUTTON_2], ITEM_ID_LCONTROL, iptdev_get_state);
      		input_device_item_add(P1_device, "P1 B3", &pad_state[0][KEY_BUTTON_3], ITEM_ID_LSHIFT,   iptdev_get_state);
      		input_device_item_add(P1_device, "P1 B4", &pad_state[0][KEY_BUTTON_4], ITEM_ID_SPACE,    iptdev_get_state);

      		input_device_item_add(P2_device, "P2 B1", &pad_state[1][KEY_BUTTON_1], ITEM_ID_S, iptdev_get_state);
      		input_device_item_add(P2_device, "P2 B2", &pad_state[1][KEY_BUTTON_2], ITEM_ID_A, iptdev_get_state);
      		input_device_item_add(P2_device, "P2 B3", &pad_state[1][KEY_BUTTON_3], ITEM_ID_W, iptdev_get_state);
      		input_device_item_add(P2_device, "P2 B4", &pad_state[1][KEY_BUTTON_4], ITEM_ID_Q, iptdev_get_state);

		goto FINISHED;
	}
	/* Capcom Eco Fighter , use L & R button to turn the weapon */
   	if (!core_stricmp(machine->gamedrv->name, "ecofghtr") || !core_stricmp(machine->gamedrv->parent, "ecofghtr"))
   	{
      		input_device_item_add(P1_device, "P1 B1", &pad_state[0][KEY_BUTTON_5], ITEM_ID_LCONTROL, iptdev_get_state);
      		input_device_item_add(P1_device, "P1 B2", &pad_state[0][KEY_BUTTON_2], ITEM_ID_LALT, 	 iptdev_get_state);
      		input_device_item_add(P1_device, "P1 B3", &pad_state[0][KEY_BUTTON_6], ITEM_ID_SPACE, 	 iptdev_get_state);

      		input_device_item_add(P2_device, "P2 B1", &pad_state[1][KEY_BUTTON_5], ITEM_ID_A, iptdev_get_state);
      		input_device_item_add(P2_device, "P2 B2", &pad_state[1][KEY_BUTTON_2], ITEM_ID_S, iptdev_get_state);
      		input_device_item_add(P2_device, "P2 B3", &pad_state[1][KEY_BUTTON_6], ITEM_ID_Q, iptdev_get_state);

		macro_enable = false;
		goto FINISHED;
	}
	/* Capcom Dynasty Wars layout */
	if (!core_stricmp(machine->gamedrv->name, "dynwar") || !core_stricmp(machine->gamedrv->parent, "dynwar"))
	{
      		input_device_item_add(P1_device, "P1 B1", &pad_state[0][KEY_BUTTON_2], ITEM_ID_LCONTROL, iptdev_get_state);
      		input_device_item_add(P1_device, "P1 B2", &pad_state[0][KEY_BUTTON_1], ITEM_ID_LALT, 	 iptdev_get_state);
      		input_device_item_add(P1_device, "P1 B3", &pad_state[0][KEY_BUTTON_3], ITEM_ID_SPACE, 	 iptdev_get_state);

      		input_device_item_add(P2_device, "P2 B1", &pad_state[1][KEY_BUTTON_2], ITEM_ID_A, iptdev_get_state);
      		input_device_item_add(P2_device, "P2 B2", &pad_state[1][KEY_BUTTON_1], ITEM_ID_S, iptdev_get_state);
      		input_device_item_add(P2_device, "P2 B3", &pad_state[1][KEY_BUTTON_3], ITEM_ID_Q, iptdev_get_state);

		macro_enable = false;
		goto FINISHED;
	}
	/* 6-buttons fighting games input layout */
	for (i = 0; specific_games_initinput_vs6b[i] != NULL; i++)
	{
		if ( !core_stricmp(machine->gamedrv->name, specific_games_initinput_vs6b[i]) || !core_stricmp(machine->gamedrv->parent, specific_games_initinput_vs6b[i]) )
		{
			input_device_item_add(P1_device, "P1 B1", &pad_state[0][KEY_BUTTON_1], ITEM_ID_Z,	 iptdev_get_state);
			input_device_item_add(P1_device, "P1 B2", &pad_state[0][KEY_BUTTON_2], ITEM_ID_LSHIFT, 	 iptdev_get_state);
			input_device_item_add(P1_device, "P1 B3", &pad_state[0][KEY_BUTTON_3], ITEM_ID_LALT, 	 iptdev_get_state);
			input_device_item_add(P1_device, "P1 B4", &pad_state[0][KEY_BUTTON_4], ITEM_ID_LCONTROL, iptdev_get_state);
			input_device_item_add(P1_device, "P1 B5", &pad_state[0][KEY_BUTTON_5], ITEM_ID_SPACE, 	 iptdev_get_state);
			input_device_item_add(P1_device, "P1 B6", &pad_state[0][KEY_BUTTON_6], ITEM_ID_X, 	 iptdev_get_state);

			input_device_item_add(P2_device, "P2 B1", &pad_state[1][KEY_BUTTON_1], ITEM_ID_C, iptdev_get_state);
			input_device_item_add(P2_device, "P2 B2", &pad_state[1][KEY_BUTTON_2], ITEM_ID_W, iptdev_get_state);
			input_device_item_add(P2_device, "P2 B3", &pad_state[1][KEY_BUTTON_3], ITEM_ID_S, iptdev_get_state);
			input_device_item_add(P2_device, "P2 B4", &pad_state[1][KEY_BUTTON_4], ITEM_ID_A, iptdev_get_state);
			input_device_item_add(P2_device, "P2 B5", &pad_state[1][KEY_BUTTON_5], ITEM_ID_Q, iptdev_get_state);
			input_device_item_add(P2_device, "P2 B6", &pad_state[1][KEY_BUTTON_6], ITEM_ID_V, iptdev_get_state);

			macro_enable = false;
			goto FINISHED;
		}
	}
	/* Default layout */
	{
      		input_device_item_add(P1_device, "P1 B1", &pad_state[0][KEY_BUTTON_1], ITEM_ID_LCONTROL, iptdev_get_state);
      		input_device_item_add(P1_device, "P1 B2", &pad_state[0][KEY_BUTTON_2], ITEM_ID_LALT, 	 iptdev_get_state);
      		input_device_item_add(P1_device, "P1 B3", &pad_state[0][KEY_BUTTON_3], ITEM_ID_SPACE, 	 iptdev_get_state);
      		input_device_item_add(P1_device, "P1 B4", &pad_state[0][KEY_BUTTON_4], ITEM_ID_LSHIFT, 	 iptdev_get_state);

      		input_device_item_add(P2_device, "P2 B1", &pad_state[1][KEY_BUTTON_1], ITEM_ID_A, iptdev_get_state);
      		input_device_item_add(P2_device, "P2 B2", &pad_state[1][KEY_BUTTON_2], ITEM_ID_S, iptdev_get_state);
      		input_device_item_add(P2_device, "P2 B3", &pad_state[1][KEY_BUTTON_3], ITEM_ID_Q, iptdev_get_state);
      		input_device_item_add(P2_device, "P2 B4", &pad_state[1][KEY_BUTTON_4], ITEM_ID_W, iptdev_get_state);

      		input_device_item_add(P3_device, "P3 B1", &pad_state[2][KEY_BUTTON_1], ITEM_ID_RCONTROL, iptdev_get_state);
      		input_device_item_add(P3_device, "P3 B2", &pad_state[2][KEY_BUTTON_2], ITEM_ID_RSHIFT, 	 iptdev_get_state);
      		input_device_item_add(P3_device, "P3 B3", &pad_state[2][KEY_BUTTON_3], ITEM_ID_ENTER, 	 iptdev_get_state);
      		input_device_item_add(P3_device, "P3 B4", &pad_state[2][KEY_BUTTON_4], ITEM_ID_RALT, 	 iptdev_get_state);

      		input_device_item_add(P4_device, "P4 B1", &pad_state[3][KEY_BUTTON_1], ITEM_ID_0_PAD, 	  iptdev_get_state);
      		input_device_item_add(P4_device, "P4 B2", &pad_state[3][KEY_BUTTON_2], ITEM_ID_DEL_PAD,   iptdev_get_state);
      		input_device_item_add(P4_device, "P4 B3", &pad_state[3][KEY_BUTTON_3], ITEM_ID_ENTER_PAD, iptdev_get_state);
      		input_device_item_add(P4_device, "P4 B4", &pad_state[3][KEY_BUTTON_4], ITEM_ID_3_PAD, 	  iptdev_get_state);
   	}

FINISHED: ;
}

static inline void retro_poll_mame_input( void )
{
	input_poll_cb();

	UINT32 i;

	if (keyboard_input)
	{
		for (i = 0; ktable[i].retro_key_name != -1; i++)
		{
      			retrokbd_state[ktable[i].retro_key_name] = input_state_cb(0, RETRO_DEVICE_KEYBOARD, 0, ktable[i].retro_key_name) ? 0x80 : 0;

      			if (retrokbd_state[ktable[i].retro_key_name] && !retrokbd_state2[ktable[i].retro_key_name])
      			{
         			ui_ipt_pushchar = ktable[i].retro_key_name;
         			retrokbd_state2[ktable[i].retro_key_name] = 1;
      			}
      			else if (!retrokbd_state[ktable[i].retro_key_name] && retrokbd_state2[ktable[i].retro_key_name])
         			retrokbd_state2[ktable[i].retro_key_name] = 0;
   		}
	}

	pad_state[0][KEY_F11] = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R3);	/* Only */
	pad_state[0][KEY_TAB] = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L2);	/* For */
	pad_state[0][KEY_F2]  = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L3);	/* Player1 */

	for (i = 0; i < MAX_JOYPADS; i++)
	{
   		pad_state[i][KEY_JOYSTICK_U] = input_state_cb(i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP);
   		pad_state[i][KEY_JOYSTICK_D] = input_state_cb(i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN);
   		pad_state[i][KEY_JOYSTICK_L] = input_state_cb(i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT);
   		pad_state[i][KEY_JOYSTICK_R] = input_state_cb(i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT);
   		pad_state[i][KEY_BUTTON_1]   = input_state_cb(i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A);
   		pad_state[i][KEY_BUTTON_2]   = input_state_cb(i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B);
   		pad_state[i][KEY_BUTTON_3]   = input_state_cb(i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_X);
   		pad_state[i][KEY_BUTTON_4]   = input_state_cb(i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_Y);
   		pad_state[i][KEY_BUTTON_5]   = input_state_cb(i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L);
   		pad_state[i][KEY_BUTTON_6]   = input_state_cb(i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R);
   		pad_state[i][KEY_START]      = input_state_cb(i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START);
   		pad_state[i][KEY_COIN]       = input_state_cb(i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_SELECT);

		switch (turbo_enable)
   		{
      			case 0: break;
      			case 1: if (PLAYER_PRESS(A))  pad_state[i][KEY_BUTTON_1] = turbo_state < turbo_delay ? FALSE : TRUE; break;
      			case 2: if (PLAYER_PRESS(B))  pad_state[i][KEY_BUTTON_2] = turbo_state < turbo_delay ? FALSE : TRUE; break;
      			case 3: if (PLAYER_PRESS(R2)) pad_state[i][KEY_BUTTON_1] = turbo_state < turbo_delay ? FALSE : TRUE; break;
      			case 4: if (PLAYER_PRESS(R2)) pad_state[i][KEY_BUTTON_2] = turbo_state < turbo_delay ? FALSE : TRUE; break;
   		}

		switch (macro_state)
		{
			case 0: break;
			case 1: if (PLAYER_PRESS(L)) pad_state[i][KEY_BUTTON_1] = pad_state[i][KEY_BUTTON_2] = TRUE; break;
			case 2: if (PLAYER_PRESS(R)) pad_state[i][KEY_BUTTON_1] = pad_state[i][KEY_BUTTON_2] = TRUE; break;
			case 3: if (PLAYER_PRESS(L)) pad_state[i][KEY_BUTTON_3] = pad_state[i][KEY_BUTTON_4] = TRUE; break;
			case 4: if (PLAYER_PRESS(R)) pad_state[i][KEY_BUTTON_3] = pad_state[i][KEY_BUTTON_4] = TRUE; break;
			case 5: if (PLAYER_PRESS(L)) pad_state[i][KEY_BUTTON_1] = pad_state[i][KEY_BUTTON_2] = TRUE;
				if (PLAYER_PRESS(R)) pad_state[i][KEY_BUTTON_3] = pad_state[i][KEY_BUTTON_4] = TRUE; break;
			case 6: if (PLAYER_PRESS(L)) pad_state[i][KEY_BUTTON_3] = pad_state[i][KEY_BUTTON_4] = TRUE;
				if (PLAYER_PRESS(R)) pad_state[i][KEY_BUTTON_1] = pad_state[i][KEY_BUTTON_2] = TRUE; break;
		}
	}
}

/**************************************************************************/

void osd_exit(running_machine &machine)
{
	write_log("osd_exit called \n");

   	if (our_target != NULL)
      		render_target_free(our_target);

   	our_target = NULL;

   	global_free(KB_device);
   	global_free(P4_device);
  	global_free(P3_device);
  	global_free(P2_device);
  	global_free(P1_device);
}

void osd_init(running_machine *machine)
{
   	machine->add_notifier(MACHINE_NOTIFY_EXIT, osd_exit);

   	our_target = render_target_alloc(machine, NULL, 0);

   	initInput(machine);

   	write_log("machine screen orientation: %s \n", (machine->gamedrv->flags & ORIENTATION_SWAP_XY) ? "VERTICAL" : "HORIZONTAL");
   	orient = (machine->gamedrv->flags & ORIENTATION_MASK);
   	vertical = (machine->gamedrv->flags & ORIENTATION_SWAP_XY);

	INT32 gameRot = 0;
   	gameRot = (ROT270 == orient) ? 1 : gameRot;
   	gameRot = (ROT180 == orient) ? 2 : gameRot;
   	gameRot = (ROT90 == orient) ? 3 : gameRot;

   	prep_retro_rotation(gameRot);
   	machine->sample_rate = sample_rate;	/* Override original value */

	if (!macro_enable)
		macro_state = 0;

   	write_log("osd init done\n");
}

void osd_update(running_machine *machine, int skip_redraw)
{
   	const render_primitive_list	*primlist;
   	UINT8				*surfptr;

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
		INT32 minwidth, minheight;

      		/* get the minimum width/height for the current layout */
		render_target_get_minimum_size(our_target, &minwidth, &minheight);

		if (FirstTimeUpdate)
		{
         		FirstTimeUpdate = 0;
         		write_log("Game screen: width=%i, height=%i, rowPixels=%i\n", minwidth, minheight, minwidth);

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
					/* RAM access waitstates etc. aren't emulated - slow the CPU to compensate */
					if (strcmp(machine->gamedrv->source_file, "src/mame/drivers/cps2.c") == 0)
						arroffset[3] *= 0.7375f;

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

	RETRO_LOOP = false;

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


//============================================================
//	main
//============================================================

static const char *xargv[] = {
	"-joystick",
	"-noautoframeskip",
	"-sound",
	"-rompath",
	NULL, NULL,
	NULL, NULL,
	NULL, NULL,
	NULL, NULL,
	NULL, NULL,
	NULL, NULL,
	NULL, NULL
};

static int parsePath(char *path, char *gamePath, char *gameName)
{
	int i;
	int slashIndex = -1;
	int dotIndex = -1;
	int len = strlen(path);

	if (len < 1)
		return 0;

	for (i = len - 1; i >= 0; i--)
	{
		if (path[i] == slash)
		{
			slashIndex = i;
			break;
		}
		else if (path[i] == '.')
			dotIndex = i;
	}

	if (slashIndex < 0 && dotIndex > 0)
	{
		strcpy(gamePath, ".\0");
		strncpy(gameName, path, dotIndex);
		gameName[dotIndex] = 0;
		write_log("*gamePath=%s , *gameName=%s\n", gamePath, gameName);
		return 1;
	}

	if (slashIndex < 0 || dotIndex < 0)
		return 0;

	strncpy(gamePath, path, slashIndex);
	gamePath[slashIndex] = 0;
	strncpy(gameName, path + (slashIndex + 1), dotIndex - (slashIndex + 1));
	gameName[dotIndex - (slashIndex + 1)] = 0;

/*	write_log("gamePath=%s , gameName=%s\n", gamePath, gameName); */
	return 1;
}

static int getGameInfo(char *gameName, int *rotation, int *driverIndex)
{
	int gameFound = 0;

	/* check invalid game name */
	if (gameName[0] == 0)
		return gameFound;

	for (int drvindex = 0; drivers[drvindex]; drvindex++)
	{
		if ( (drivers[drvindex]->flags & GAME_NO_STANDALONE) == 0 && mame_strwildcmp(gameName, drivers[drvindex]->name) == 0 )
		{
			gameFound = 1;
			*driverIndex = drvindex;
			*rotation = drivers[drvindex]->flags & 0x07;
/*			write_log("%-18s\"%s\" rot=%i\n", drivers[drvindex]->name, drivers[drvindex]->description, *rotation); */

			if (strcmp(drivers[drvindex]->source_file, "src/mame/drivers/neogeo.inc") == 0)
				is_neogeo = true;
		}
	}
	return gameFound;
}

static int executeGame(char *path)
{
	/* cli_frontend does the heavy lifting; if we have osd-specific options, we create a derivative of cli_options and add our own */
	int result = 0;
	int gameRot = 0;
	int paramCount;
	int driverIndex;

	screenRot = 0;
	FirstTimeUpdate = 1;

	//split the path to directory and the name without the zip extension
	result = parsePath(path, MAME_GAME_PATH, MAME_GAME_NAME);

	if (result == 0)
	{
		write_log("parse path failed! path=%s\n", path);
		strcpy(MAME_GAME_NAME, path);
	}

	//Find the game info. Exit if game driver was not found.
	if (getGameInfo(MAME_GAME_NAME, &gameRot, &driverIndex) == 0)
	{
		write_log("game not found: %s\n", MAME_GAME_NAME);
		return -2;
	}

	if (gameRot != ROT0)
	{
		screenRot = 1;
		if (gameRot & ORIENTATION_FLIP_X)
		{
			write_log("*********** flip X \n");
			screenRot = 2;
		}
	}

	write_log("creating frontend...\n");

	//find how many parameters we have
	for (paramCount = 0; xargv[paramCount] != NULL; paramCount++) { };

	xargv[paramCount++] = (char*)retro_content_dir;

	xargv[paramCount++] = (char*)"-cfg_directory";
	xargv[paramCount++] = (char*)retro_content_dir;

	xargv[paramCount++] = (char*)"-nvram_directory";
	xargv[paramCount++] = (char*)retro_content_dir;

	xargv[paramCount++] = (char*)"-memcard_directory";
	xargv[paramCount++] = (char*)retro_content_dir;

	if (!tate)
	{
		if (screenRot == 2)
			xargv[paramCount++] = (char*)"-rol";
		else if (screenRot)
			xargv[paramCount++] = (char*)"-ror";
	}

	xargv[paramCount++] = MAME_GAME_NAME;

	if (is_neogeo && set_neogeo_bios >= 0)
	{
		xargv[paramCount++] = (char*)"-bios";
		xargv[paramCount++] = (char*)neogeo_bioses[set_neogeo_bios].name;
		write_log("Current loaded NEOGEO BIOS is < %s >\n", neogeo_bioses[set_neogeo_bios].bios);
	}

	write_log("executing frontend... params:%i\n", paramCount);

	for (int i = 0; xargv[i] != NULL; i++)
	{
		write_log("%s ", xargv[i]);
		write_log("\n");
	}

	result = cli_execute(paramCount, (char**)xargv, NULL);
	xargv[paramCount - 2] = NULL;

	return result;
}


static int mmain(int argc, const char *argv)
{
	static char gameName[1024];
	int result = 0;

	strcpy(gameName, argv);
	result = executeGame(gameName);

	if (result != 0)
		return -1;

	return 1;
}
