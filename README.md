
# M.B.A-mini

M.B.A = MAME's skeleton + FBA's romsets

M.B.A-mini from MAME2010-libretro [(https://github.com/libretro/mame2010-libretro)](https://github.com/libretro/mame2010-libretro.git)
after the codes is streamlined, only CPS 1/2, NEOGEO, IREM M92 machines && roms is supported.

![CPS1](https://raw.githubusercontent.com/wonder199999/images-repository/master/02-15-2019/sf2mdtb-190215-160214.png)
![CPS2](https://raw.githubusercontent.com/wonder199999/images-repository/master/02-15-2019/mvscjsing-190215-215942.png)
![M92](https://raw.githubusercontent.com/wonder199999/images-repository/master/02-15-2019/inthuntu-1.png)
![NEOGEO](https://raw.githubusercontent.com/wonder199999/images-repository/master/02-15-2019/samsho5b-190215-220141.png)

M.B.A-mini gave up the original mame0.139 romsets, but supported FBAlpha 02.97.38 romsets
with more games (it's >= mame0.172 romsets). also in process, many roms graphics and
sound issues have been fixed (especially for CPS1).

In the current situation, M.B.A-mini supports 270 CPS1 games (including clones), 305 CPS2 games,
293 NEOGEO games, and 37 games for IREM M92. for a more specific list, please visit [Compatibility-list.md](https://github.com/wonder199999/MBA.mini-libretro/blob/master/Compatibility-list.md) .

***
## The M.B.A-mini core has the following options:

* Adjust CPU speed:	(disabled|110%|120%|130%|140%|150%|160%|170%|180%|190%|200%|50%|60%|70%|80%|90%)
* Set frameskip: (0|1|2|3|4|automatic)
* Core provided aspect ratio: (DAR|PAR)
* Enable autofire: (disabled|button 1|button 2|R2 to button 1 mapping|R2 to button 2 mapping)
* Set autofire pulse speed: (medium|slow|fast)
* Keyboard input: (enabled|disabled)
* Use macro button: (disabled|assign A+B to L|assign A+B to R|assign C+D to L|assign C+D to R|assign A+B to L & C+D to R|assign A+B to R & C+D to L )
* CHEAT: (disabled|enabled)
* T.A.T.E mode: (disabled|enabled)
* Set sample rate: (48000Hz|44100Hz|32000Hz|22050Hz)
* Forced off ROM CRC verfiy: (No|Yes)
* Set NEOGEO BIOS(All options):  (Default|Europe MVS(Ver. 2)|Europe MVS(Ver. 1)|USA MVS(Ver. 2?)|USA MVS(Ver. 1)|Asia MVS(Ver. 3)|Asia MVS(Latest)|Japan MVS(Ver. 3)|Japan MVS(Ver. 2)|Japan MVS(Ver. 1)|Japan MVS(J3)|Custom Japanese Hotel|UniBIOS(Ver. 3.2)|UniBIOS(Ver. 3.1)|UniBIOS(Ver. 3.0)|UniBIOS(Ver. 2.3)|UniBIOS(Ver. 2.3 older?)|UniBIOS(Ver. 2.2)|UniBIOS(Ver. 2.1)|UniBIOS(Ver. 2.0)|UniBIOS(Ver. 1.3)|UniBIOS(Ver. 1.2)|UniBIOS(Ver. 1.2 older)|UniBIOS(Ver. 1.1)|UniBIOS(Ver. 1.0)|Debug MVS|Asia AES|Japan AES)
* Set NEOGEO BIOS(Typical options): (Default|Europe MVS(Ver. 2)|USA MVS(Ver. 2?)|Asia MVS(Ver. 3)|Japan MVS(Ver. 3)|UniBIOS(Ver. 3.2) )
* Set brightness: (default|+1%|+2%|+3%|+4%|+5%|+6%|+7%|+8%|+9%|+10%|+11%|+12%|+13%|+14%|+15%|+16%|+17%|+18%|+19%|+20%|-20%|-19%|-18%|-17%|-16%|-15%|-14%|-13%|-12%|-11%|-10%|-9%|-8%|-7%|-6%|-5%|-4%|-3%|-2%|-1%)
* Set contrast: (default|+1%|+2%|+3%|+4%|+5%|+6%|+7%|+8%|+9%|+10%|+11%|+12%|+13%|+14%|+15%|+16%|+17%|+18%|+19%|+20%|-20%|-19%|-18%|-17%|-16%|-15%|-14%|-13%|-12%|-11%|-10%|-9%|-8%|-7%|-6%|-5%|-4%|-3%|-2%|-1%)
* Set gamma: (default|+1%|+2%|+3%|+4%|+5%|+6%|+7%|+8%|+9%|+10%|+11%|+12%|+13%|+14%|+15%|+16%|+17%|+18%|+19%|+20%|-20%|-19%|-18%|-17%|-16%|-15%|-14%|-13%|-12%|-11%|-10%|-9%|-8%|-7%|-6%|-5%|-4%|-3%|-2%|-1%)

![T.A.T.E](https://raw.githubusercontent.com/wonder199999/images-repository/master/02-15-2019/1941-2.png)![UNIBIOS3.2](https://raw.githubusercontent.com/wonder199999/images-repository/master/02-15-2019/samsho5b-190215-220049.png)
![CHEAT1](https://raw.githubusercontent.com/wonder199999/images-repository/master/05-14-2019/cheat1.png)
![CHEAT2](https://raw.githubusercontent.com/wonder199999/images-repository/master/05-14-2019/cheat2.png)

***
## CHEAT function
MBA-mini only supports cheat documents is xml format, and these xml files  need to be in a directory named 'mba-cheat' in the RetroArch or frontend's system directory.  
The Repository only comes with samples of several xml files. for complete cheat files, please visit http://cheat.retrogames.com/mame_downloads.htm and select 'XML Cheat Collection for MAME 0.139'.

***
## Building
* Build for x86/x64 linux/win:  
	use:  `make` or `make -j4`

  Optimize for local machines when 64-bit building,  
	use:  `make "OPTFLAG=1"`

* Build and enable hardware render,  
	use:  `make "VRENDER=opengl"` or `make "VRENDER=gles"`

* Build for arm linux:  
	`make platform=armv`

* Build for raspberry pi 2/3:  
	`make platform=rpi2` or `make platform=rpi3`

* Build for android:  
	`make "NATIVE=1" buildtools` then `make platform=android`


***
*	JOYPAD controls are:

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

*	tips:

		P1 L2 to show MAMEUI
		P1 R3 to show framerate
		P1 L3 to access Service Mode

