Mame2010-mini from MAME2010-libretro (https://github.com/libretro/mame2010-libretro)
After the codes is streamlined , only CPS 1/2,NEOGEO,IREM M92 roms is supported.

Build :

     for now you must build in 2 pass: 
     one for the native buildtools and second for the target emulator build.
  
 build for android:

     make -f Makefile "NATIVE=1" buildtools
     make -f Makefile platform=android
     
 build for pc linux/win:

     make -f Makefile buildtools
     make -f Makefile platform= #platform name
  
     (NB: for 64 bits build export PTR64=1 at least on win64)


 controls are: 

	RETRO_DEVICE_ID_JOYPAD_START		[KEY_START]
	RETRO_DEVICE_ID_JOYPAD_SELECT		[KEY_COIN]
	RETRO_DEVICE_ID_JOYPAD_A		[KEY_BUTTON_1]
	RETRO_DEVICE_ID_JOYPAD_B		[KEY_BUTTON_2]
	RETRO_DEVICE_ID_JOYPAD_X		[KEY_BUTTON_3]
	RETRO_DEVICE_ID_JOYPAD_Y		[KEY_BUTTON_4]
	RETRO_DEVICE_ID_JOYPAD_L 		[KEY_BUTTON_5]
	RETRO_DEVICE_ID_JOYPAD_R		[KEY_BUTTON_6]
	RETRO_DEVICE_ID_JOYPAD_UP		[KEY_JOYSTICK_U]
	RETRO_DEVICE_ID_JOYPAD_DOWN		[KEY_JOYSTICK_D]
	RETRO_DEVICE_ID_JOYPAD_LEFT		[KEY_JOYSTICK_L]
	RETRO_DEVICE_ID_JOYPAD_RIGHT		[KEY_JOYSTICK_R]

        tips: P1 L2 to show MAMEUI
              P1 R3 to show framerate
              P1 L3 to access Service Mode
