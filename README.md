
	M.B.A-mini (M.B.A = MAME's skeleton + FBA's romsets)

	M.B.A-mini from MAME2010-libretro (https://github.com/libretro/mame2010-libretro)
	After the codes is streamlined, only CPS 1/2,NEOGEO,IREM M92 roms is supported.

	Build for android:
		make -f makefile "NATIVE=1" buildtools
		make -f makefile platform=android
     
	Build for pc linux/win:
		make -f makefile platform= #platform name
		(NB: for 64 bits build export PTR64=1 at least on win64)

	JOYPAD controls are: 
		RETRO_JOYPAD_START    Start
		RETRO_JOYPAD_SELECT   Coin
		RETRO_JOYPAD_A        Button 1
		RETRO_JOYPAD_B        Button 2
		RETRO_JOYPAD_X        Button 3
		RETRO_JOYPAD_Y        Button 4
		RETRO_JOYPAD_L        Button 5
		RETRO_JOYPAD_R        Button 6
		RETRO_JOYPAD_UP       Joystick UP
		RETRO_JOYPAD_DOWN     Joystick DOWN
		RETRO_JOYPAD_LEFT     Joystick LEFT
		RETRO_JOYPAD_RIGHT    Joystick RIGHT

	tips:   P1 L2 to show MAMEUI
		P1 R3 to show framerate
		P1 L3 to access Service Mode


	Browse Compatibility-list.md to learn about supported games.
