#include <unistd.h>
#include <stdint.h>
#include "osdepend.h"
#include "emu.h"
#include "clifront.h"
#include "render.h"
#include "ui.h"
#include "uiinput.h"
#include "libretro.h"
#include "log.h"

extern "C" int mmain(int argc, const char *argv);

#if !defined(HAVE_OPENGL) && !defined(HAVE_OPENGLES) && !defined(HAVE_RGB32)
   #define M16B
#endif
#ifdef M16B
   #define FUNC_PREFIX(x) rgb565_##x
   #define PIXEL_TYPE UINT16
   #define SRCSHIFT_R 3
   #define SRCSHIFT_G 2
   #define SRCSHIFT_B 3
   #define DSTSHIFT_R 11
   #define DSTSHIFT_G 5
   #define DSTSHIFT_B 0
#else
   #define FUNC_PREFIX(x) rgb888_##x
   #define PIXEL_TYPE UINT32
   #define SRCSHIFT_R 0
   #define SRCSHIFT_G 0
   #define SRCSHIFT_B 0
   #define DSTSHIFT_R 16
   #define DSTSHIFT_G 8
   #define DSTSHIFT_B 0
#endif
#include "rendersw.c"

//============================================================
//  CONSTANTS
//============================================================

// fake a keyboard mapped to retro joypad
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

#ifdef DEBUG_LOG
   #define LOG(msg) fprintf(stderr, "%s\n", msg)
#else
   #define LOG(msg)
#endif

//============================================================
//  GLOBALS
//============================================================

// rendering target
static render_target *our_target = NULL;

// input device
static input_device *P1_device;		// P1 JOYPAD
static input_device *P2_device; 	// P2 JOYPAD
static input_device *P3_device; 	// P3 JOYPAD
static input_device *P4_device; 	// P4 JOYPAD
static input_device *retrokbd_device;	// KEYBD

// state
static UINT8 pad_state[4][KEY_TOTAL];
static UINT16 retrokbd_state[RETROK_LAST];
static UINT16 retrokbd_state2[RETROK_LAST];

struct kt_table
{
	const char *mame_key_name;
   	int retro_key_name;
   	input_item_id mame_key;
};

//enables / disables tate mode
static unsigned int tate;
static unsigned int screenRot;
int vertical, orient;

static char MgamePath[1024];
static char MgameName[512];

char g_rom_dir[1024];
static bool draw_this_frame;
static bool set_par = false;
static bool retro_load_ok = false;
static bool keyboard_input = true;
static int ui_ipt_pushchar = -1;
static int set_frame_skip;
static unsigned int pauseg = 0;
static unsigned int mame_reset = 0;
static unsigned int FirstTimeUpdate = 1;
static unsigned int turbo_enable;
static unsigned int turbo_delay;
static unsigned int turbo_state;
static unsigned int sample_rate = 48000;
static double refresh_rate = 60.0;
static unsigned adjust_opt[6] = {0/*Enable/Disable*/, 0/*Limit*/, 0/*GetRefreshRate*/, 0/*Brightness*/, 0/*Contrast*/, 0/*Gamma*/};
static float arroffset[3] = {0/*For brightness*/, 0/*For contrast*/, 0/*For gamma*/};
static void update_geometry();

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

//============================================================
//  RETRO
//============================================================

#include "retromapper.c"
#include "retroinput.c"
#include "retroosd.c"

//============================================================
//  main
//============================================================

static const char *xargv[] = {
	"-joystick",
	"-noautoframeskip",
	"-sound",
	"-samplerate",
	"48000",
	"-contrast",
	"1.0",
	"-brightness",
	"1.0",
	"-gamma",
	"1.0",
	"-rompath",

	NULL,
	NULL,
	NULL,
	NULL,
};


#ifdef _WIN32
	char slash = '\\';
#else
	char slash = '/';
#endif

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
		write_log("gamePath=%s , gameName=%s\n", gamePath, gameName);
		return 1;
	}

	if (slashIndex < 0 || dotIndex < 0)
		return 0;

	strncpy(gamePath, path, slashIndex);
	gamePath[slashIndex] = 0;
	strncpy(gameName, path + (slashIndex + 1), dotIndex - (slashIndex + 1));
	gameName[dotIndex - (slashIndex + 1)] = 0;

	write_log("gamePath=%s , gameName=%s\n", gamePath, gameName);
	return 1;
}

static int getGameInfo(char *gameName, int *rotation, int *driverIndex)
{
	int gameFound = 0;
	int drvindex;
#if 1
	//check invalid game name
	if (gameName[0] == 0)
		return 0;

	for (drvindex = 0; drivers[drvindex]; drvindex++)
	{
		if ( (drivers[drvindex]->flags & GAME_NO_STANDALONE) == 0 && mame_strwildcmp(gameName, drivers[drvindex]->name) == 0 )
		{
			gameFound = 1;
			*driverIndex = drvindex;
			*rotation = drivers[drvindex]->flags & 0x7;
			write_log("%-18s\"%s\" rot=%i \n", drivers[drvindex]->name, drivers[drvindex]->description, *rotation);
		}
	}
#else
	gameFound = 1;
#endif
	return gameFound;
}

int executeGame(char *path)
{
	/* cli_frontend does the heavy lifting; if we have osd-specific options, we create a derivative of cli_options and add our own */
	int result = 0;
	int gameRot = 0;
	int paramCount;
	int driverIndex;

	screenRot = 0;
	FirstTimeUpdate = 1;

	//split the path to directory and the name without the zip extension
	result = parsePath(path, MgamePath, MgameName);

	if (result == 0)
	{
		write_log("parse path failed! path=%s\n", path);
		strcpy(MgameName, path);
//		return -1;
	}

	//Find the game info. Exit if game driver was not found.
	if (getGameInfo(MgameName, &gameRot, &driverIndex) == 0)
	{
		write_log("game not found: %s\n", MgameName);
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

	write_log("creating frontend... game=%s\n", MgameName);

	//find how many parameters we have
	for (paramCount = 0; xargv[paramCount] != NULL; paramCount++) ;

	xargv[paramCount++] = (char*)g_rom_dir;

	if (!tate)
	{
		if (screenRot == 2)
			xargv[paramCount++] = (char*)"-rol";
		else if (screenRot)
			xargv[paramCount++] = (char*)"-ror";
	}

	xargv[paramCount++] = MgameName;

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

//============================================================
//  main
//============================================================

#ifdef __cplusplus
	extern "C"
#endif
int mmain(int argc, const char *argv)
{
	static char gameName[1024];
	int result = 0;

	strcpy(gameName, argv);
	result = executeGame(gameName);

	if (result != 0)
		return -1;

	return 1;
}
