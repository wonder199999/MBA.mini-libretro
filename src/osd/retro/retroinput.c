
#define PLAYER_PRESS(button)	input_state_cb(i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_##button)

static INT32 iptdev_get_state(void *device_internal, void *item_internal)
{
	UINT8 *itemdata = (UINT8 *)item_internal;
   	return *itemdata;
}

kt_table ktable[] = {
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

/* Capcom Eco Fighter , use L & R button to turn the weapon */
#define ECOFGT_LAYOUT	(core_stricmp(machine->gamedrv->name, "ecofghtr") == 0) || (core_stricmp(machine->gamedrv->parent, "ecofghtr") == 0)

/* Capcom Dynasty Wars */
#define DYNWAR_LAYOUT	(core_stricmp(machine->gamedrv->name, "dynwar") == 0) || (core_stricmp(machine->gamedrv->parent, "dynwar") == 0)

/* 6-buttons fighting games */
#define VS6B_LAYOUT	(core_stricmp(machine->gamedrv->name, "sf2") == 0) || (core_stricmp(machine->gamedrv->parent, "sf2") == 0) || \
			(core_stricmp(machine->gamedrv->name, "sf2ce") == 0) || (core_stricmp(machine->gamedrv->parent, "sf2ce") == 0) || \
         		(core_stricmp(machine->gamedrv->name, "sf2hf") == 0) || (core_stricmp(machine->gamedrv->parent, "sf2hf") == 0) || \
			(core_stricmp(machine->gamedrv->name, "sfzch") == 0) || (core_stricmp(machine->gamedrv->parent, "sfzch") == 0) || \
         		(core_stricmp(machine->gamedrv->name, "ssf2") == 0) || (core_stricmp(machine->gamedrv->parent, "ssf2") == 0) || \
			(core_stricmp(machine->gamedrv->name, "ssf2t") == 0) || (core_stricmp(machine->gamedrv->parent, "ssf2t") == 0) || \
			(core_stricmp(machine->gamedrv->name, "sfa") == 0) || (core_stricmp(machine->gamedrv->parent, "sfa") == 0) || \
			(core_stricmp(machine->gamedrv->name, "sfa2") == 0) || (core_stricmp(machine->gamedrv->parent, "sfa2") == 0) || \
			(core_stricmp(machine->gamedrv->name, "sfz2al") == 0) || (core_stricmp(machine->gamedrv->parent, "sfz2al") == 0) || \
         		(core_stricmp(machine->gamedrv->name, "sfa3") == 0) || (core_stricmp(machine->gamedrv->parent, "sfa3") == 0) || \
			(core_stricmp(machine->gamedrv->name, "hsf2") == 0) ||(core_stricmp(machine->gamedrv->parent, "hsf2") == 0) || \
			(core_stricmp(machine->gamedrv->name, "dstlk") == 0) || (core_stricmp(machine->gamedrv->parent, "dstlk") == 0) || \
         		(core_stricmp(machine->gamedrv->name, "vsav") == 0) || (core_stricmp(machine->gamedrv->parent, "vsav") == 0) || \
			(core_stricmp(machine->gamedrv->name, "vsav2") == 0) || (core_stricmp(machine->gamedrv->parent, "vsav2") == 0) || \
			(core_stricmp(machine->gamedrv->name, "vhunt2") == 0) || (core_stricmp(machine->gamedrv->parent, "vhunt2") == 0) || \
			(core_stricmp(machine->gamedrv->name, "msh") == 0) || (core_stricmp(machine->gamedrv->parent, "msh") == 0) || \
         		(core_stricmp(machine->gamedrv->name, "mshvsf") == 0) || (core_stricmp(machine->gamedrv->parent, "mshvsf") == 0) || \
			(core_stricmp(machine->gamedrv->name, "mvsc") == 0) || (core_stricmp(machine->gamedrv->parent, "mvsc") == 0) || \
			(core_stricmp(machine->gamedrv->name, "xmcota") == 0) || (core_stricmp(machine->gamedrv->parent, "xmcota") == 0) || \
			(core_stricmp(machine->gamedrv->name, "xmvsf") == 0) || (core_stricmp(machine->gamedrv->parent, "xmvsf") == 0) || \
			(core_stricmp(machine->gamedrv->name, "ringdest") == 0) || (core_stricmp(machine->gamedrv->parent, "ringdest") == 0) || \
			(core_stricmp(machine->gamedrv->name, "nwarr") == 0) || (core_stricmp(machine->gamedrv->parent, "nwarr") == 0) || /* Before is the Capcom games */ \
			(core_stricmp(machine->gamedrv->name, "soldier") == 0) || (core_stricmp(machine->gamedrv->parent, "soldier") == 0) 

/* Neo Geo */
#define NEOGEO_LAYOUT	(core_stricmp(machine->gamedrv->parent, "neogeo") == 0) || (core_stricmp(machine->gamedrv->name, "3countba") == 0) || \
			(core_stricmp(machine->gamedrv->name, "ms5pcb") == 0) || (core_stricmp(machine->gamedrv->parent, "ms5pcb") == 0) || \
			(core_stricmp(machine->gamedrv->name, "svcpcb") == 0) || (core_stricmp(machine->gamedrv->parent, "svcpcb") == 0) || \
			(core_stricmp(machine->gamedrv->name, "kf2k3pcb") == 0) || (core_stricmp(machine->gamedrv->parent, "kf2k3pcb") == 0) || \
			(core_stricmp(machine->gamedrv->name, "fatfury2a") == 0) || (core_stricmp(machine->gamedrv->name, "fatfury3a") == 0) 

static void initInput(running_machine *machine)
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

   	for (i = 0; i < RETROK_LAST; i++)
      		retrokbd_state[i] = retrokbd_state2[i] = 0;

	for (i = 0; i < 4; i++)
	{
		pad_state[i][KEY_F11] = pad_state[i][KEY_TAB] = pad_state[i][KEY_F2] =
		pad_state[i][KEY_START] = pad_state[i][KEY_COIN] = pad_state[i][KEY_BUTTON_1] =
		pad_state[i][KEY_BUTTON_2] = pad_state[i][KEY_BUTTON_3] = pad_state[i][KEY_BUTTON_4] =
		pad_state[i][KEY_BUTTON_5] = pad_state[i][KEY_BUTTON_6] = pad_state[i][KEY_JOYSTICK_U] =
		pad_state[i][KEY_JOYSTICK_D] = pad_state[i][KEY_JOYSTICK_L] = pad_state[i][KEY_JOYSTICK_R] = 0;
	}

   	/* our faux keyboard only has a couple of keys (corresponding to the common defaults) */
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

   	if (VS6B_LAYOUT)      /* 6-buttons fighting games */
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
   	}
   	else      	/* Neo Geo */
   	if (NEOGEO_LAYOUT)
   	{
      		input_device_item_add(P1_device, "P1 B1", &pad_state[0][KEY_BUTTON_1], ITEM_ID_LALT, 	 iptdev_get_state);
      		input_device_item_add(P1_device, "P1 B2", &pad_state[0][KEY_BUTTON_2], ITEM_ID_LCONTROL, iptdev_get_state);
      		input_device_item_add(P1_device, "P1 B3", &pad_state[0][KEY_BUTTON_3], ITEM_ID_LSHIFT,   iptdev_get_state);
      		input_device_item_add(P1_device, "P1 B4", &pad_state[0][KEY_BUTTON_4], ITEM_ID_SPACE,    iptdev_get_state);

      		input_device_item_add(P2_device, "P2 B1", &pad_state[1][KEY_BUTTON_1], ITEM_ID_S, iptdev_get_state);
      		input_device_item_add(P2_device, "P2 B2", &pad_state[1][KEY_BUTTON_2], ITEM_ID_A, iptdev_get_state);
      		input_device_item_add(P2_device, "P2 B3", &pad_state[1][KEY_BUTTON_3], ITEM_ID_W, iptdev_get_state);
      		input_device_item_add(P2_device, "P2 B4", &pad_state[1][KEY_BUTTON_4], ITEM_ID_Q, iptdev_get_state);
	}
   	else	    	/* Capcom Eco Fighters */
   	if (ECOFGT_LAYOUT)
   	{
      		input_device_item_add(P1_device, "P1 B1", &pad_state[0][KEY_BUTTON_5], ITEM_ID_LCONTROL, iptdev_get_state);
      		input_device_item_add(P1_device, "P1 B2", &pad_state[0][KEY_BUTTON_2], ITEM_ID_LALT, 	 iptdev_get_state);
      		input_device_item_add(P1_device, "P1 B3", &pad_state[0][KEY_BUTTON_6], ITEM_ID_SPACE, 	 iptdev_get_state);

      		input_device_item_add(P2_device, "P2 B1", &pad_state[1][KEY_BUTTON_5], ITEM_ID_A, iptdev_get_state);
      		input_device_item_add(P2_device, "P2 B2", &pad_state[1][KEY_BUTTON_2], ITEM_ID_S, iptdev_get_state);
      		input_device_item_add(P2_device, "P2 B3", &pad_state[1][KEY_BUTTON_6], ITEM_ID_Q, iptdev_get_state);

		macro_enable = false;
   	}
   	else	    	/* Capcom Dynasty Wars */
   	if (DYNWAR_LAYOUT)
	{
      		input_device_item_add(P1_device, "P1 B1", &pad_state[0][KEY_BUTTON_2], ITEM_ID_LCONTROL, iptdev_get_state);
      		input_device_item_add(P1_device, "P1 B2", &pad_state[0][KEY_BUTTON_1], ITEM_ID_LALT, 	 iptdev_get_state);
      		input_device_item_add(P1_device, "P1 B3", &pad_state[0][KEY_BUTTON_3], ITEM_ID_SPACE, 	 iptdev_get_state);

      		input_device_item_add(P2_device, "P2 B1", &pad_state[1][KEY_BUTTON_2], ITEM_ID_A, iptdev_get_state);
      		input_device_item_add(P2_device, "P2 B2", &pad_state[1][KEY_BUTTON_1], ITEM_ID_S, iptdev_get_state);
      		input_device_item_add(P2_device, "P2 B3", &pad_state[1][KEY_BUTTON_3], ITEM_ID_Q, iptdev_get_state);

		macro_enable = false;
	}
   	else      /* Default config */
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
}
#undef VS6B_LAYOUT
#undef NEOGEO_LAYOUT
#undef ECOFGT_LAYOUT
#undef DYNWAR_LAYOUT

static inline void retro_poll_mame_input(void)
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

	pad_state[0][KEY_F11] = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R3);
	pad_state[0][KEY_TAB] = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L2);
	pad_state[0][KEY_F2]  = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L3);

	for (i = 0; i < 4; i++)
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
      			case 0:
         		break;
      			case 1:
         			if (PLAYER_PRESS(A))  pad_state[i][KEY_BUTTON_1] = turbo_state < turbo_delay ? FALSE : TRUE;
         		break;
      			case 2:
				if (PLAYER_PRESS(B))  pad_state[i][KEY_BUTTON_2] = turbo_state < turbo_delay ? FALSE : TRUE;
         		break;
      			case 3:
         			if (PLAYER_PRESS(R2)) pad_state[i][KEY_BUTTON_1] = turbo_state < turbo_delay ? FALSE : TRUE;
         		break;
      			case 4:
         			if (PLAYER_PRESS(R2)) pad_state[i][KEY_BUTTON_2] = turbo_state < turbo_delay ? FALSE : TRUE;
         		break;
   		}

		switch (macro_state)
		{
			case 0:
			break;
			case 1:
				if (PLAYER_PRESS(L)) pad_state[i][KEY_BUTTON_1] = pad_state[i][KEY_BUTTON_2] = TRUE;
			break;
			case 2:
				if (PLAYER_PRESS(R)) pad_state[i][KEY_BUTTON_1] = pad_state[i][KEY_BUTTON_2] = TRUE;
			break;
			case 3:
				if (PLAYER_PRESS(L)) pad_state[i][KEY_BUTTON_3] = pad_state[i][KEY_BUTTON_4] = TRUE;
			break;
			case 4:
				if (PLAYER_PRESS(R)) pad_state[i][KEY_BUTTON_3] = pad_state[i][KEY_BUTTON_4] = TRUE;
			break;
			case 5:
				if (PLAYER_PRESS(L)) pad_state[i][KEY_BUTTON_1] = pad_state[i][KEY_BUTTON_2] = TRUE;
				if (PLAYER_PRESS(R)) pad_state[i][KEY_BUTTON_3] = pad_state[i][KEY_BUTTON_4] = TRUE;
			break;
			case 6:
				if (PLAYER_PRESS(L)) pad_state[i][KEY_BUTTON_3] = pad_state[i][KEY_BUTTON_4] = TRUE;
				if (PLAYER_PRESS(R)) pad_state[i][KEY_BUTTON_1] = pad_state[i][KEY_BUTTON_2] = TRUE;
			break;
		}
	}
}
