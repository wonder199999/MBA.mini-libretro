/*************************************************************************/
//	for libretro.c
/*************************************************************************/

#ifndef ___LIBRETRO_H_
#define ___LIBRETRO_H_

/*************************************************************************/
//	GLOBAL VARIABLES
/*************************************************************************/

bool verify_rom_hash = false;
char RPATH[512];
char retro_content_dir[1024];
INT32 vertical, orient;
INT32 SHIFTON = -1;
UINT32 allow_select_newgame = 0;
UINT32 RETRO_LOOP = 1;

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

struct kt_table
{
	const char *mame_key_name;
	int retro_key_name;
	input_item_id mame_key;
};

const kt_table ktable[] = {
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
   	{"-1",       -1,                 ITEM_ID_INVALID},
};


/**************************************************************************/
//	DECLARE FUNCTIONS
/**************************************************************************/

void osd_init( running_machine *machine );
void osd_update( running_machine *machine, int skip_redraw );
void osd_update_audio_stream( running_machine *machine, short *buffer, int samples_this_frame );
void osd_set_mastervolume( int attenuation );
void osd_customize_input_type_list( input_type_desc *typelist );
void osd_exit( running_machine &machine );
void CLIB_DECL mame_printf_verbose( const char *text, ... ) ATTR_PRINTF(1, 2);	/* use if you want to print something with the verbose flag */

extern int osd_num_processors;
extern void retro_finish();
extern void retro_main_loop();

/**************************************************************************/
//	MACROS
/**************************************************************************/

#ifdef _WIN32
	char slash = '\\';
#else
	char slash = '/';
#endif

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

/* Capcom Eco Fighter , use L & R button to turn the weapon */
#define ECOFGT_LAYOUT	(core_stricmp(machine->gamedrv->name, "ecofghtr") == 0) || (core_stricmp(machine->gamedrv->parent, "ecofghtr") == 0)

/* Capcom Dynasty Wars */
#define DYNWAR_LAYOUT	(core_stricmp(machine->gamedrv->name, "dynwar") == 0) || (core_stricmp(machine->gamedrv->parent, "dynwar") == 0)

/* 6-buttons fighting games */
#define VS6B_LAYOUT	(core_stricmp(machine->gamedrv->name, "sf2") == 0) || (core_stricmp(machine->gamedrv->parent, "sf2") == 0) ||		\
			(core_stricmp(machine->gamedrv->name, "sf2ce") == 0) || (core_stricmp(machine->gamedrv->parent, "sf2ce") == 0) || 	\
         		(core_stricmp(machine->gamedrv->name, "sf2hf") == 0) || (core_stricmp(machine->gamedrv->parent, "sf2hf") == 0) || 	\
			(core_stricmp(machine->gamedrv->name, "sfzch") == 0) || (core_stricmp(machine->gamedrv->parent, "sfzch") == 0) || 	\
         		(core_stricmp(machine->gamedrv->name, "ssf2") == 0) || (core_stricmp(machine->gamedrv->parent, "ssf2") == 0) ||		\
			(core_stricmp(machine->gamedrv->name, "ssf2t") == 0) || (core_stricmp(machine->gamedrv->parent, "ssf2t") == 0) || 	\
			(core_stricmp(machine->gamedrv->name, "sfa") == 0) || (core_stricmp(machine->gamedrv->parent, "sfa") == 0) || 		\
			(core_stricmp(machine->gamedrv->name, "sfa2") == 0) || (core_stricmp(machine->gamedrv->parent, "sfa2") == 0) || 	\
			(core_stricmp(machine->gamedrv->name, "sfz2al") == 0) || (core_stricmp(machine->gamedrv->parent, "sfz2al") == 0) || 	\
         		(core_stricmp(machine->gamedrv->name, "sfa3") == 0) || (core_stricmp(machine->gamedrv->parent, "sfa3") == 0) || 	\
			(core_stricmp(machine->gamedrv->name, "hsf2") == 0) || (core_stricmp(machine->gamedrv->parent, "hsf2") == 0) || 	\
			(core_stricmp(machine->gamedrv->name, "dstlk") == 0) || (core_stricmp(machine->gamedrv->parent, "dstlk") == 0) || 	\
         		(core_stricmp(machine->gamedrv->name, "vsav") == 0) || (core_stricmp(machine->gamedrv->parent, "vsav") == 0) || 	\
			(core_stricmp(machine->gamedrv->name, "vsav2") == 0) || (core_stricmp(machine->gamedrv->parent, "vsav2") == 0) || 	\
			(core_stricmp(machine->gamedrv->name, "vhunt2") == 0) || (core_stricmp(machine->gamedrv->parent, "vhunt2") == 0) || 	\
			(core_stricmp(machine->gamedrv->name, "msh") == 0) || (core_stricmp(machine->gamedrv->parent, "msh") == 0) || 		\
         		(core_stricmp(machine->gamedrv->name, "mshvsf") == 0) || (core_stricmp(machine->gamedrv->parent, "mshvsf") == 0) || 	\
			(core_stricmp(machine->gamedrv->name, "mvsc") == 0) || (core_stricmp(machine->gamedrv->parent, "mvsc") == 0) || 	\
			(core_stricmp(machine->gamedrv->name, "xmcota") == 0) || (core_stricmp(machine->gamedrv->parent, "xmcota") == 0) || 	\
			(core_stricmp(machine->gamedrv->name, "xmvsf") == 0) || (core_stricmp(machine->gamedrv->parent, "xmvsf") == 0) || 	\
			(core_stricmp(machine->gamedrv->name, "ringdest") == 0) || (core_stricmp(machine->gamedrv->parent, "ringdest") == 0) || \
			(core_stricmp(machine->gamedrv->name, "nwarr") == 0) || (core_stricmp(machine->gamedrv->parent, "nwarr") == 0) || /* Before is the Capcom games */ \
			(core_stricmp(machine->gamedrv->name, "ssoldier") == 0) || (core_stricmp(machine->gamedrv->parent, "ssoldier") == 0) 

/* Neo Geo */
#define NEOGEO_LAYOUT	(core_stricmp(machine->gamedrv->parent, "neogeo") == 0)	|| (core_stricmp(machine->gamedrv->parent, "bstars") == 0) ||	\
			(core_stricmp(machine->gamedrv->parent, "mslug2") == 0) || (core_stricmp(machine->gamedrv->parent, "mslug3") == 0) || 	\
			(core_stricmp(machine->gamedrv->parent, "mslug4") == 0) || (core_stricmp(machine->gamedrv->parent, "mslug5") == 0) || 	 \
			(core_stricmp(machine->gamedrv->parent, "kof94") == 0) || (core_stricmp(machine->gamedrv->parent, "kof95") == 0) || 	  \
			(core_stricmp(machine->gamedrv->parent, "kof96") == 0) || (core_stricmp(machine->gamedrv->parent, "kof97") == 0) || 	   \
			(core_stricmp(machine->gamedrv->parent, "kof98") == 0) || (core_stricmp(machine->gamedrv->parent, "kof99") == 0) || 	    \
			(core_stricmp(machine->gamedrv->parent, "kof2000") == 0) || (core_stricmp(machine->gamedrv->parent, "kof2001") == 0) ||    \
			(core_stricmp(machine->gamedrv->parent, "kof2002") == 0) || (core_stricmp(machine->gamedrv->parent, "kof2003") == 0) ||   \
			(core_stricmp(machine->gamedrv->parent, "samsho") == 0) || (core_stricmp(machine->gamedrv->parent, "samsho2") == 0) || 	 \
			(core_stricmp(machine->gamedrv->parent, "samsho3") == 0) || (core_stricmp(machine->gamedrv->parent, "samsho4") == 0) || \
			(core_stricmp(machine->gamedrv->parent, "samsho5") == 0) || (core_stricmp(machine->gamedrv->parent, "samsh5sp") == 0) || \
			(core_stricmp(machine->gamedrv->parent, "fatfursp") == 0) || (core_stricmp(machine->gamedrv->parent, "garou") == 0) || 	  \
			(core_stricmp(machine->gamedrv->parent, "fatfury2") == 0) || (core_stricmp(machine->gamedrv->parent, "fatfury3") == 0) ||  \
			(core_stricmp(machine->gamedrv->parent, "rbff1") == 0) || (core_stricmp(machine->gamedrv->parent, "rbff2") == 0) || 	    \
			(core_stricmp(machine->gamedrv->parent, "rbffspec") == 0) || (core_stricmp(machine->gamedrv->parent, "3countb") == 0) ||     \
			(core_stricmp(machine->gamedrv->parent, "aof2") == 0) || (core_stricmp(machine->gamedrv->parent, "aof3") == 0) || 	    \
			(core_stricmp(machine->gamedrv->parent, "kotm") == 0) || (core_stricmp(machine->gamedrv->parent, "kotm2") == 0) || 	   \
			(core_stricmp(machine->gamedrv->parent, "sengoku") == 0) || (core_stricmp(machine->gamedrv->parent, "sengoku3") == 0) ||  \
			(core_stricmp(machine->gamedrv->parent, "svc") == 0) || (core_stricmp(machine->gamedrv->parent, "svcpcb") == 0) || 	 \
			(core_stricmp(machine->gamedrv->parent, "wh1") == 0) || (core_stricmp(machine->gamedrv->parent, "wh2j") == 0) || 	\
			(core_stricmp(machine->gamedrv->parent, "shocktro") == 0) || (core_stricmp(machine->gamedrv->parent, "shocktr2") == 0) || \
			(core_stricmp(machine->gamedrv->parent, "ridhero") == 0) || (core_stricmp(machine->gamedrv->parent, "nitd") == 0) || 	    \
			(core_stricmp(machine->gamedrv->parent, "maglord") == 0) || (core_stricmp(machine->gamedrv->parent, "burningf") == 0) ||     \
			(core_stricmp(machine->gamedrv->parent, "gpilots") == 0) || (core_stricmp(machine->gamedrv->parent, "quizdais") == 0) ||      \
			(core_stricmp(machine->gamedrv->parent, "2020bb") == 0) || (core_stricmp(machine->gamedrv->parent, "socbrawl") == 0) ||      \
			(core_stricmp(machine->gamedrv->parent, "roboarmy") == 0) || (core_stricmp(machine->gamedrv->parent, "lastblad") == 0) ||   \
			(core_stricmp(machine->gamedrv->parent, "tophuntr") == 0) || (core_stricmp(machine->gamedrv->parent, "vliner") == 0) ||    \
			(core_stricmp(machine->gamedrv->parent, "ncombat") == 0) || (core_stricmp(machine->gamedrv->parent, "quizkof") == 0) ||   \
			(core_stricmp(machine->gamedrv->parent, "fightfev") == 0) || (core_stricmp(machine->gamedrv->parent, "pbobblen") == 0) || \
			(core_stricmp(machine->gamedrv->parent, "matrim") == 0) || (core_stricmp(machine->gamedrv->parent, "puzzledp") == 0) ||   \
			(core_stricmp(machine->gamedrv->parent, "alpham2") == 0) || (core_stricmp(machine->gamedrv->parent, "bangbead") == 0) ||  \
			(core_stricmp(machine->gamedrv->parent, "jockeygp") == 0) || (core_stricmp(machine->gamedrv->parent, "ironclad") == 0) || \
			(core_stricmp(machine->gamedrv->parent, "lresort") == 0) 

#define PLAYER_PRESS(button)	input_state_cb(i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_##button)

#define MAX_JOYPADS	4

#endif	/* ___LIBRETRO_H_ */
