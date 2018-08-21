
	M.B.A-mini (M.B.A = MAME's skeleton + FBA's romsets)

	M.B.A-mini from MAME2010-libretro (https://github.com/libretro/mame2010-libretro)
	After the codes is streamlined, only CPS 1/2,NEOGEO,IREM M92 roms is supported.
	Browse Compatibility-list for a list of supported games.

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


	Compatibility-list - Parent roms with the "*" :
	#####################################################
	#                       EXAMPLE:                    #
	#  Parent name: Title name (year.month.day) - Note: #
	#  Clone name: Title name (year.month.day) - Note:  #
	#####################################################

	Support the following: 
	/************************************************************/
	/ * Capcom System 1 ( 269 parent+clone roms are supported) * /	/* Unfinished */
	/************************************************************/

	*1941: 1941 - Counter Attack (900227 World)
		1941j (clone of 1941): 1941 - Counter Attack (Japan)
		1941r1 (clone of 1941): 1941 - Counter Attack (World)
		1941u (clone of 1941): 1941 - Counter Attack (900227 USA)
	
	*3wonders: Three Wonders (wonder 3 910520 etc)
		3wondersb (clone of 3wonders): Three Wonders (bootleg set 2, wonder 3 910520 etc)
		3wondersh (clone of 3wonders): Three Wonders (bootleg set 1, wonder 3 910520 etc)
		3wondersr1 (clone of 3wonders): Three Wonders (wonder 3 910513 etc)
		3wondersu (clone of 3wonders): Three Wonders (wonder 3 910520 USA)
		wonder3 (clone of 3wonders): Wonder 3 (910520 Japan)
	
	*captcomm: Captain Commando (911202 other country)
		captcommb (clone of captcomm): Captain Commando (bootleg set 1, 911014 other country)
		captcommb2 (clone of captcomm): Captain Commando (bootleg set 2 (with YM2151 + 2xMSM5205), 911014 other country)
		captcommj (clone of captcomm): Captain Commando (911202 Japan)
		captcommjr1 (clone of captcomm): Captain Commando (910928 Japan)
		captcommr1 (clone of captcomm): Captain Commando (911014 other country)
		captcommu (clone of captcomm): Captain Commando (910928 USA)

	*cawing: Carrier Air Wing (U.S. navy 901012 etc)
		cawingb2 (clone of cawing): Carrier Air Wing (bootleg set 2 (with 2xYM2203 + 2xMSM5205), U.S. navy 901012 etc)
		cawingbl (clone of cawing): Carrier Air Wing (bootleg set 1 (with 2xYM2203 + 2xMSM5205), U.S. navy 901012 etc)
		cawingj (clone of cawing): U.S. Navy (901012 Japan)
		cawingr1 (clone of cawing): Carrier Air Wing (U.S. navy 901009 etc)
		cawingu (clone of cawing): Carrier Air Wing (U.S. navy 901012 USA)
	
	*cworld2j: Adventure Quiz Capcom World 2 (クイズ５ あどべんちゃーくいず かぷこんわーるど２ 920611 Japan)
		cworld2ja (clone of cworld2j): Adventure Quiz Capcom World 2 (クイズ５ あどべんちゃーくいず かぷこんわーるど２ Japan 920611, B-Board 90629B-3, no battery)
		cworld2jb (clone of cworld2j): Adventure Quiz Capcom World 2 (クイズ５ あどべんちゃーくいず かぷこんわーるど２ Japan 920611, B-Board 91634B-2)

	*dino: Cadillacs & Dinosaurs (930201 etc)
		dinoeh (clone of dino): Cadillacs and Dinosaurs (hack, 930201 etc)
		dinoh (clone of dino): Cadillacs and Dinosaurs (bootleg set 3, 930223 Asia TW)
		dinohc (clone of dino): Cadillacs and Dinosaurs (Chinese bootleg, 930223 Asia TW)
		dinohunt (clone of dino): Dinosaur Hunter 恐龍獵人 (Chinese bootleg, 930223 Asia TW)
		dinoj (clone of dino): Cadillacs 恐竜新世紀 ◦ Cadillacs Kyouryuu-Shinseiki (Cadillacs 930201 Japan)
		dinopic (clone of dino): Cadillacs and Dinosaurs (bootleg set 1 (with PIC16c57), 930201 etc)
		dinopic2 (clone of dino): Cadillacs and Dinosaurs (bootleg set 2 (with PIC16c57), 930201 etc)
		dinopic3 (clone of dino): Cadillacs and Dinosaurs (bootleg set 3 (with PIC16c57), 930201 etc)
		dinopic4 (clone of dino): Cadillacs and Dinosaurs (bootleg set 4 (with PIC16c57), 930223 Asia TW)
		dinot (clone of dino): Cadillacs and Dinosaurs Turbo (bootleg set 1, 930223 Asia TW)
		dinotpic (clone of dino): Cadillacs and Dinosaurs Turbo (bootleg set 2 (with PIC16c57), 930201 etc)
		dinou (clone of dino): Cadillacs & Dinosaurs (930201 USA)
	
	*dynwar: Dynasty Wars (US set 1)
		dynwara (clone of dynwar): Dynasty Wars (US set 2)
		dynwarj (clone of dynwar): 天地を喰らう ◦ Tenchi wo Kurau (Japan)
		dynwarjr (clone of dynwar): 天地を喰らう ◦ Tenchi wo Kurau (Japan Resale Ver.)
	
	*ffight: Final Fight (World, set 1)
		fcrash (clone of ffight): Final Crash (bootleg (with 2xYM2203 + 2xMSM5205))
		ffighta (clone of ffight): Final Fight (World, set 2)
		ffightbl (clone of ffight): Final Fight (bootleg set 1 (with 2xYM2203 + 2xMSM5205), World)
		ffightbla (clone of ffight): Final Fight (bootleg set 2 (with 2xYM2203 + 2xMSM5205), World))
		ffightj (clone of ffight): Final Fight (Japan)
		ffightj1 (clone of ffight): Final Fight (900112 Japan)
		ffightj2 (clone of ffight): Final Fight (900305 Japan)
		ffightj3 (clone of ffight): Final Fight (900613 Japan)
		ffightjh (clone of ffight): Street Smart / Final Fight (Japan, hack)
		ffightu (clone of ffight): Final Fight (USA, set 1)
		ffightu1 (clone of ffight): Final Fight (USA, set 2)
		ffightu2 (clone of ffight): Final Fight (USA, set 3)
		ffightua (clone of ffight): Final Fight (900112 USA)
		ffightub (clone of ffight): Final Fight (900424 USA)
		ffightuc (clone of ffight): Final Fight (900613 USA)

	*forgottn: Forgotten Worlds (World, newer)
		forgottna (clone of forgottn): Forgotten Worlds (World)
		forgottnu (clone of forgottn): Forgotten Worlds (US, B-Board 88621B-2, rev C)
		forgottnu1 (clone of forgottn): Forgotten Worlds (US, B-Board 88618B-2, Rev C)
		forgottnua (clone of forgottn): Forgotten Worlds (US, B-Board 88618B-2, rev A)
		forgottnuaa (clone of forgottn): Forgotten Worlds (US, B-Board 88618B-2, rev AA)
		lostwrld (clone of forgottn): Lost Worlds (Japan)
		lostwrldo (clone of forgottn): Lost Worlds (Japan Old ver.)
	
	*ganbare: Ganbare! Marine Kun (Marine 2K0411 JPN)

	*ghouls: Ghouls'n Ghosts (World)
		daimakai (clone of ghouls): 大魔界村 ◦ Dai Makai-Mura (Japan)
		daimakaib (clone of ghouls): 大魔界村 ◦ Dai Makai-Mura (bootleg, Japan)
		daimakair (clone of ghouls): 大魔界村 ◦ Dai Makai-Mura (Japan Resale Ver.)
		ghoulsu (clone of ghouls): Ghouls'n Ghosts (US)

	*knights: Knights of the Round (911127 etc)
		knightsb (clone of knights): Knights of the Round (bootleg set 1 (with YM2151 + 2xMSM5205), 911127 etc)
		knightsb2 (clone of knights): Knights of the Round (bootleg set 2, 911127 etc)
		knightsb3 (clone of knights): Knights of the Round (bootleg set 3, 911127 Japan)
		knightsb4 (clone of knights): Knights of the Round (bootleg set 4 (with YM2151 + 2xMSM5205), 911127 etc)
		knightsh (clone of knights): Knights of the Round (hack set 1)
		knightsh2 (clone of knights): Knights of the Round (hack set 2, 911127 etc)
		knightsj (clone of knights): Knights of the Round (911127 Japan, B-Board 91634B-2)
		knightsja (clone of knights): Knights of the Round (911127 Japan, B-Board 89625B-1)
		knightsu (clone of knights): Knights of the Round (911127 USA)

	*kod: The King of Dragons (910805 etc)
		kodb (clone of kod): The King of Dragons (bootleg, 910731 etc)
		kodda (clone of kod): The King of Dragons (Phoenix bootleg, 910731 etc)
		kodh (clone of kod): The King of Dragons (hack)
		kodj (clone of kod): The King of Dragons (Japan 910805, B-Board 90629B-3)
		kodja (clone of kod): The King of Dragons (Japan 910805, B-Board 89625B-1)
		kodr1 (clone of kod): The King of Dragons (910711 etc)
		kodu (clone of kod): The King of Dragons (US 910910)

	*mbombrd: Muscle Bomber Duo - ultimate team battle (931206 etc)
		mbombrdj (clone of mbombrd): Muscle Bomber Duo - heat up warriors (931206 Japan)
	
	*megaman: Mega Man - the power battle (951006 USA)
		megamana (clone of megaman): Mega Man - the power battle (951006 Asia)
		rockmanj (clone of megaman): Rockman - the power battle (950922 Japan)

	*mercs: Mercs (900302 etc)
		mercsj (clone of mercs): 戦場の狼 II ◦ Senjo no Ookami II (Ookami 2 900302 Japan)
		mercsu (clone of mercs): Mercs (900608 USA)
		mercsur1 (clone of mercs): Mercs (900302 USA)

	*msword: Magic Sword - heroic fantasy (25.07.1990 other country)
		mswordj (clone of msword): Magic Sword (23.06.1990 Japan) ◦ Magic Swordマジック･ソード
		mswordr1 (clone of msword): Magic Sword - heroic fantasy (23.06.1990 other country)
		mswordu (clone of msword): Magic Sword - heroic fantasy (25.07.1990 USA)

	*mtwins: Mega Twins (chiki chiki boys 900619 etc)
		chikij (clone of mtwins): Chiki Chiki Boys (900619 Japan)
	
	*nemo: Nemo (90 11 30 etc)
		nemoj (clone of nemo): Nemo (90 11 20 Japan)
		nemor1 (clone of nemo): Nemo (90 11 09 etc)

	*pang3: Pang! 3 (950601 Euro)
		pang3b (clone of pang3): Pang! 3 (bootleg set 1, 950511 Euro)
		pang3b2 (clone of pang3): Pang! 3 (bootleg set 2, 950511 Euro)
		pang3b3 (clone of pang3): Pang! 3 (bootleg set 3, 950601 Euro)
		pang3j (clone of pang3): Pang! 3: Kaitou Tachi no Karei na Gogo (950511 Japan)
		pang3r1 (clone of pang3): Pang! 3 (950511 Euro)
		pang3r1a (clone of pang3): Pang! 3 (950511 Euro, alt)

	*pnickj: Pnickies (940608 Japan)

	*punisher: The Punisher (930422 etc)
		punipic (clone of punisher): The Punisher (bootleg set 1 (with PIC16c57), 930422 etc)
		punipic2 (clone of punisher): The Punisher (bootleg set 2 (with PIC16c57), 930422 etc)
		punipic3 (clone of punisher): The Punisher (bootleg set 3 (with PIC16c57), 930422 etc)
		punisherb (clone of punisher): The Punisher (bootleg, 930422 etc)
		punisherbz (clone of punisher): ﻿飆風戰警 ◦ The Punisher: Biaofeng Zhanjing (Chinese bootleg)
		punisherh (clone of punisher): The Punisher (930422 Hispanic)
		punisherj (clone of punisher): The Punisher (930422 Japan)
		punisheru (clone of punisher): The Punisher (930422 USA)
	
	*qad: Quiz & Dragons (capcom quiz game 920701 USA)
		qadjr (clone of qad): Quiz & Dragons (940921 Japan Resale Ver.)
	
	*qtono2j: クイズ殿様の野望２ (とのさまのやぼう 2) ◦ Quiz Tonosama no Yabou 2 Zenkoku-ban (tonosama 2 950123 Japan)

	*sf2: Street Fighter II - The World Warrior (910522 etc)
		sf2b (clone of sf2): Street Fighter II - The World Warrior (bootleg, 910214 etc, set 1  (with YM2151 + 2xMSM5205))
		sf2b2 (clone of sf2): Street Fighter II - The World Warrior (bootleg, 910214 etc, set 2)
		sf2b3 (clone of sf2): Street Fighter II - The World Warrior (bootleg, 910214 etc, set 3)
		sf2eb (clone of sf2): Street Fighter II - The World Warrior (910214 etc)
		sf2ebbl (clone of sf2): Street Fighter II - The World Warrior (TAB Austria bootleg, 910214 etc)
		sf2ebbl2 (clone of sf2): Street Fighter II - The World Warrior (TAB Austria bootleg set 2, 910214 etc)
		sf2ebbl3 (clone of sf2): Street Fighter II - The World Warrior (TAB Austria bootleg set 3, 910214 etc)
		sf2ed (clone of sf2): Street Fighter II - The World Warrior (910318 etc)
		sf2ee (clone of sf2): Street Fighter II - The World Warrior (910228 etc)
		sf2j (clone of sf2): Street Fighter II - The World Warrior (911210 Japan)
		sf2ja (clone of sf2): Street Fighter II - The World Warrior (910214 Japan)
		sf2jc (clone of sf2): Street Fighter II - The World Warrior (910306 Japan)
		sf2jf (clone of sf2): Street Fighter II - The World Warrior (910411 Japan)
		sf2jh (clone of sf2): Street Fighter II - The World Warrior (910522 Japan)
		sf2jl (clone of sf2): Street Fighter II - The World Warrior (920312 Japan)
		sf2qp1 (clone of sf2): Street Fighter II - The World Warrior (Quicken Pt-I bootleg, 910214 USA)
		sf2stt (clone of sf2): Street Fighter II - The World Warrior (STT, TAB Austria bootleg, 910214 etc)
		sf2thndr (clone of sf2): Street Fighter II - The World Warrior (Thunder Edition bootleg, 910214 etc)
		sf2ua (clone of sf2): Street Fighter II - The World Warrior (910206 USA)
		sf2ub (clone of sf2): Street Fighter II - The World Warrior (910214 USA)
		sf2uc (clone of sf2): Street Fighter II - The World Warrior (910306 USA)
		sf2ud (clone of sf2): Street Fighter II - The World Warrior (910318 USA)
		sf2ue (clone of sf2): Street Fighter II - The World Warrior (910228 USA)
		sf2uf (clone of sf2): Street Fighter II - The World Warrior (910411 USA)
		sf2ug (clone of sf2): Street Fighter II - The World Warrior (910522 USA, rev G)
		sf2ui (clone of sf2): Street Fighter II - The World Warrior (910522 USA, rev I)
		sf2uk (clone of sf2): Street Fighter II - The World Warrior (911101 USA)

	*sf2ce: Street Fighter II' - Champion Edition (street fighter 2' 920513 etc)
		sf2acc (clone of sf2ce): Street Fighter II' - Champion Edition (Accelerator! bootleg set 1, 920313 USA)
		sf2acca (clone of sf2ce): Street Fighter II' - Champion Edition (Accelerator! bootleg set 2, 920310 Accelerator!)
		sf2accp2 (clone of sf2ce): Street Fighter II' - Champion Edition (Accelerator Pt.II bootleg, 920313 Testron)
		sf2amf (clone of sf2ce): Street Fighter II - Champion Edition (Alpha Magic-F bootleg set 1, 920313 etc)
		sf2amf2 (clone of sf2ce): Street Fighter II - Champion Edition (Alpha Magic-F bootleg set 2, 920313 etc)
		sf2amf3 (clone of sf2ce): Street Fighter II - Champion Edition (Alpha Magic-F bootleg set 3, 920313 etc)
		sf2amf4 (clone of sf2ce): Street Fighter II - Champion Edition (Alpha Magic-F bootleg set 4, 920313 etc)
		sf2amf5 (clone of sf2ce): Street Fighter II - Champion Edition (Alpha Magic-F bootleg set 5, 920313 etc)
		sf2amf6 (clone of sf2ce): Street Fighter II - Champion Edition (Alpha Magic-F bootleg set 6, 920313 etc)
		sf2amf7 (clone of sf2ce): Street Fighter II - Champion Edition (Alpha Magic-F bootleg set 7, 920313 etc)
		sf2amf8 (clone of sf2ce): Street Fighter II - Champion Edition (Alpha Magic-F bootleg set 8, 920313 etc)
		sf2bhh (clone of sf2ce): Street Fighter II' - Champion Edition (Hungh-Hsi bootleg, 920313 Taiwan)
		sf2cebltw (clone of sf2ce): Street Fighter II' - Champion Edition (920313 'Taiwan' bootleg with PAL)
		sf2ceea (clone of sf2ce): Street Fighter II' - Champion Edition (street fighter 2' 920313 etc)
		sf2ceeab2 (clone of sf2ce): Street Fighter II' - Champion Edition (920313 etc bootleg set 2)
		sf2ceeabl (clone of sf2ce): Street Fighter II' - Champion Edition (920313 etc bootleg set 1)
		sf2ceja (clone of sf2ce): Street Fighter II' - Champion Edition (street fighter 2' 920322 Japan)
		sf2cejab2 (clone of sf2ce): Street Fighter II' - Champion Edition (920322 Japan bootleg set 2)
		sf2cejabl (clone of sf2ce): Street Fighter II' - Champion Edition (920322 Japan bootleg set 1)
		sf2cejb (clone of sf2ce): Street Fighter II' - Champion Edition (street fighter 2' 920513 Japan)
		sf2cejc (clone of sf2ce): Street Fighter II' - Champion Edition (street fighter 2' 920803 Japan)
		sf2ceua (clone of sf2ce): Street Fighter II' - Champion Edition (street fighter 2' 920313 USA)
		sf2ceuab2 (clone of sf2ce): Street Fighter II' - Champion Edition (920313 USA bootleg set 2)
		sf2ceuab3 (clone of sf2ce): Street Fighter II' - Champion Edition (920313 USA bootleg set 3)
		sf2ceuab4 (clone of sf2ce): Street Fighter II' - Champion Edition (920313 USA bootleg set 4)
		sf2ceuab5 (clone of sf2ce): Street Fighter II' - Champion Edition (920313 USA bootleg set 5)
		sf2ceuab6 (clone of sf2ce): Street Fighter II' - Champion Edition (920313 USA bootleg set 6)
		sf2ceuab7 (clone of sf2ce): Street Fighter II' - Champion Edition (920313 USA bootleg set 7)
		sf2ceuabl (clone of sf2ce): Street Fighter II' - Champion Edition (920313 USA bootleg set 1)
		sf2ceub (clone of sf2ce): Street Fighter II' - Champion Edition (street fighter 2' 920513 USA)
		sf2ceuc (clone of sf2ce): Street Fighter II' - Champion Edition (street fighter 2' 920803 USA)
		sf2ceucbl (clone of sf2ce): Street Fighter II' - Champion Edition (920803 USA bootleg)
		sf2dkot2 (clone of sf2ce): Street Fighter II' - Champion Edition (Double K.O. Turbo II bootleg, 902140 USA)
		sf2dongb (clone of sf2ce): Street Fighter II': Champion Edition (Dongfang Bubai protection bootleg, etc 920313)
		sf2koryu (clone of sf2ce): Street Fighter II' - Xiang Long (bootleg set 1, 811102 001)
		sf2koryu2 (clone of sf2ce): Street Fighter II' - Xiang Long (bootleg set 2, 811102 001)
		sf2koryu3 (clone of sf2ce): Street Fighter II' - Xiang Long (bootleg set 3, 811102 001)
		sf2md (clone of sf2ce): Street Fighter II' - Champion Edition (Magic Delta, bootleg, 920313 etc)
		sf2mdt (clone of sf2ce): Street Fighter II' - Magic Delta Turbo (bootleg set 1 (with YM2151 + 2xMSM5205), 920313 etc)
		sf2mdta (clone of sf2ce): Street Fighter II' - Magic Delta Turbo (bootleg set 2 (with YM2151 + 2xMSM5205), 920313 etc)
		sf2mdtb (clone of sf2ce): Street Fighter II' - Magic Delta Turbo (bootleg set 3 (with YM2151 + 2xMSM5205), 920313 etc)
		sf2mega (clone of sf2ce): Street Fighter II' - Champion Edition (Mega Co bootleg set 1, 920313 etc)
		sf2mega2 (clone of sf2ce): Street Fighter II' - Champion Edition (Mega Co bootleg set 2, 920313 etc)
		sf2rb (clone of sf2ce): Street Fighter II' - Champion Edition (Rainbow bootleg set 1, 920322 etc)
		sf2rb2 (clone of sf2ce): Street Fighter II' - Champion Edition (Rainbow bootleg set 2, 920322 etc)
		sf2rb3 (clone of sf2ce): Street Fighter II' - Champion Edition (Rainbow bootleg set 3, 920322 etc)
		sf2rb4 (clone of sf2ce): Street Fighter II' - Champion Edition (Rainbow bootleg set 4, 920322 Japan)
		sf2rb5 (clone of sf2ce): Street Fighter II' - Champion Edition (Rainbow bootleg set 5, 920322 Japan)
		sf2rb6 (clone of sf2ce): Street Fighter II' - Champion Edition (Rainbow bootleg set 6, 920313 etc)
		sf2red (clone of sf2ce): Street Fighter II' - Champion Edition (Red Wave bootleg set 1, 920313 etc)
		sf2red2 (clone of sf2ce): Street Fighter II' - Champion Edition (Red Wave bootleg set 2, 920313 etc)
		sf2sl73a (clone of sf2ce): Street Fighter II' - Champion Edition (Sheng Long v 7.3a)
		sf2tlona (clone of sf2ce): Street Fighter II' - Champion Edition (Tu Long bootleg set 1, 811102 001)
		sf2tlonb (clone of sf2ce): Street Fighter II' - Champion Edition (Tu Long bootleg set 2, 811102 001)
		sf2tlonc (clone of sf2ce): Street Fighter II' - Champion Edition (Tu Long bootleg set 3, 811102 001)
		sf2v004 (clone of sf2ce): Street Fighter II' - Champion Edition (V004 bootleg set 1, 102092 USA)
		sf2v0042 (clone of sf2ce): Street Fighter II' - Champion Edition (V004 bootleg set 2, 920313 etc)
		sf2v0043 (clone of sf2ce): Street Fighter II' - Champion Edition (V004 bootleg set 3, 920313 etc)
		sf2yyc (clone of sf2ce): Street Fighter II' - Champion Edition (YYC bootleg set 1, 920313 etc)
		sf2yyc2 (clone of sf2ce): Street Fighter II' - Champion Edition (YYC bootleg set 2, 920313 etc)

	*sf2hf: Street Fighter II' - Hyper Fighting (street fighter 2' T 921209 ETC)
		sf2hfj (clone of sf2hf): Street Fighter II' Turbo - Hyper Fighting (street fighter 2' T 921209 Japan)
		sf2hfjb (clone of sf2hf): Street Fighter II' Turbo - Hyper Fighting (bootleg set 1, 921209 Japan)
		sf2hfjb2 (clone of sf2hf): Street Fighter II' Turbo - Hyper Fighting (bootleg set 2, 921209 Japan)
		sf2hfu (clone of sf2hf): Street Fighter II' - Hyper Fighting (street fighter 2' T 921209 USA)

	*sfzch: Street Fighter Zero (CPS Changer, 951020 Japan)

	*slammast: Saturday Night Slam Masters (Slam Masters 930713 etc)
		mbomberj (clone of slammast): Muscle Bomber - the body explosion (930713 Japan)
		slammastu (clone of slammast): Saturday Night Slam Masters (slam masters 930713 USA)
		slampic (clone of slammast): Saturday Night Slam Masters (bootleg (with PIC16c57), 930713 etc)
	
	*strider: Strider (US set 1)
		striderj (clone of strider): ストライダー飛竜 ◦ Strider Hiryu (Japan)
		striderjr (clone of strider): ストライダー飛竜 ◦ Strider Hiryu (Japan Resale Ver.)
		striderua (clone of strider): Strider (US set 2)
		strideruc (clone of strider): Strider (US, Street Fighter II conversion)
	
	*unsquad: U.N. Squadron (US)
		area88 (clone of unsquad): エリア８８ ◦ Area 88 (Japan)
		area88r (clone of unsquad): エリア８８ ◦ Area 88 (Japan Resale ver.)

	*varth: Varth - operation thunderstorm (920714 etc)
		varthb (clone of varth): Varth - operation thunderstorm (bootleg, 920612 etc)
		varthj (clone of varth): Varth - operation thunderstorm (920714 Japan)
		varthjr (clone of varth): Varth - operation thunderstorm (920714 Japan Resale Ver.)
		varthr1 (clone of varth): Varth - operation thunderstorm (920612 etc)
		varthu (clone of varth): Varth - operation thunderstorm (920612 USA)
	
	*willow: Willow (World)
		willowj (clone of willow): Willow (Japan, Japanese)
		willowu (clone of willow): Willow (USA)
		willowuo (clone of willow): Willow (USA Old Ver.)

	*wof: Warriors of Fate (921031 etc)
		sgyxz (clone of wof): 三国志 II: 三國英雄傳 ◦ Sangokushi II: Sanguo YingXiongZhuan (Chinese bootleg set 3, 921005 Asia)
		wof3js (clone of wof): 三国志 II: 三劍聖 ◦ Sangokushi II: San Jian Sheng (Chinese bootleg set 1, 921005 Asia)
		wof3jsa (clone of wof): 三国志 II: 三劍聖 ◦ Sangokushi II: San Jian Sheng (Chinese bootleg set 2, 921005 Asia)
		wof3sj (clone of wof): 三国志 II: 三圣剑 ◦ Sangokushi II: San Sheng Jian (Chinese bootleg set 1, 921005 Asia)
		wof3sja (clone of wof): 三国志 II: 三圣剑 ◦ Sangokushi II: San Sheng Jian (Chinese bootleg set 2, 921005 Asia)
		wofa (clone of wof): 三国志 II ◦ Sangokushi II (921005 Asia)
		wofah (clone of wof): 三国志 II ◦ Sangokushi II (hack set 1, 921005 Asia)
		wofaha (clone of wof): 三国志 II ◦ Sangokushi II (hack set 2, 921005 Asia)
		wofahb (clone of wof): 三国志 II ◦ Sangokushi II (hack set 3, 921005 Asia)
		wofb (clone of wof): Warriors of Fate (bootleg, 921002 etc)
		wofh (clone of wof): 三国志 II: 三國英雄傳 ◦ Sangokushi II: Sanguo YingXiongZhuan (Chinese bootleg set 1, 921005 Asia)
		wofha (clone of wof): 三国志 II: 三國英雄傳 ◦ Sangokushi II: Sanguo YingXiongZhuan (Chinese bootleg set 2, 921005 Asia)
		wofhfh (clone of wof): 三国志 II: 火鳳凰 ◦ Sangokushi II: Huo Feng Huang (Chinese bootleg, 921005 Asia)
		wofj (clone of wof): 天地を喰らう II - 赤壁の戦い ◦ Tenchi wo Kurau II - Sekiheki no Tatakai (921031 Japan)
		wofjh (clone of wof): 天地を喰らう II - 赤壁の戦い ◦ Tenchi wo Kurau II - Sekiheki no Tatakai (hack, 921031 Japan)
		wofr1 (clone of wof): Warriors of Fate (921002 etc)
		wofsj (clone of wof): 三国志 II: 圣剑三 ◦ Sangokushi II: Sheng Jian Sanguo (Chinese bootleg set 1, 921005 Asia)
		wofsja (clone of wof): 三国志 II: 圣剑三 ◦ Sangokushi II: Sheng Jian Sanguo (Chinese bootleg set 2, 921005 Asia)
		wofsjb (clone of wof): 三国志 II: 圣剑三 ◦ Sangokushi II: Sheng Jian Sanguo (Chinese bootleg set 3, 921005 Asia)
		wofu (clone of wof): Warriors of Fate (921031 USA)

	*wofch: 天地を喰らう II - 赤壁の戦い ◦ Tenchi wo Kurau II - Sekiheki no Tatakai (CPS Changer, 921031 Japan)
		wofchdx (clone of wofch): 三国志 III 外传: 夏侯恩的复仇 ◦ Sangokushi III Gaiden: Kakou-On's Revenge DX (hack)

	/ ********************************************************* /
	/ *  Capcom System 2 (304 parent+clone roms are supported) */
	/ ********************************************************* /
	*1944: 1944 - the loop master (000620 USA)
		1944ad (clone of 1944): 1944 - the loop master (000620 USA Phoenix Edition, alt)
		1944d (clone of 1944): 1944 - the loop master (000620 USA Phoenix Edition)
		1944j (clone of 1944): 1944 - the loop master (000620 Japan)
	
	*19xx: 19XX - the war against destiny (951207 USA)
		19xxa (clone of 19xx): 19XX - the war against destiny (960104 Asia)
		19xxar1 (clone of 19xx): 19XX - the war against destiny (951207 Asia)
		19xxb (clone of 19xx): 19XX - the war against destiny (951218 Brazil)
		19xxd (clone of 19xx): 19XX - the war against destiny (951207 USA Phoenix Edition)
		19xxh (clone of 19xx): 19XX - the war against destiny (951218 Hispanic)
		19xxj (clone of 19xx): 19XX - the war against destiny (960104 Japan, yellow case)
		19xxjr1 (clone of 19xx): 19XX - the war against destiny (951225 Japan)
		19xxjr2 (clone of 19xx): 19XX - the war against destiny (951207 Japan)

	*armwar: Armored Warriors (941024 Europe)
		armwar1d (clone of armwar): Armored Warriors (941011 Europe Phoenix Edition)
		armwara (clone of armwar): Armored Warriors (941024 Asia)
		armwarar1 (clone of armwar): Armored Warriors (940920 Asia)
		armwarr1 (clone of armwar): Armored Warriors (941011 Europe)
		armwaru (clone of armwar): Armored Warriors (941024 USA)
		armwaru1 (clone of armwar): Armored Warriors (940920 USA)
		pgear (clone of armwar): Powered Gear - strategic variant armor equipment (941024 Japan)
		pgearr1 (clone of armwar): Powered Gear - strategic variant armor equipment (940916 Japan)

	*avsp: Alien vs Predator (940520 Euro)
		avspa (clone of avsp): Alien vs Predator (940520 Asia)
		avspd (clone of avsp): Alien vs Predator (940520 Euro Phoenix Edition)
		avsph (clone of avsp): Alien vs Predator (940520 Hispanic)
		avspj (clone of avsp): Alien vs Predator (940520 Japan)
		avspu (clone of avsp): Alien vs Predator (940520 USA)

	*batcir: Battle Circuit (970319 Euro)
		batcira (clone of batcir): Battle Circuit (970319 Asia)
		batcird (clone of batcir): Battle Circuit (970319 Euro Phoenix Edition)
		batcirj (clone of batcir): Battle Circuit (970319 Japan)

	*choko: Choko (010820 Japan)

	*csclub: Capcom Sports Club (971017 Euro)
		csclub1 (clone of csclub): Capcom Sports Club (970722 Euro)
		csclub1d (clone of csclub): Capcom Sports Club (970722 Euro Phoenix Edition)
		cscluba (clone of csclub): Capcom Sports Club (970722 Asia)
		csclubh (clone of csclub): Capcom Sports Club (970722 Hispanic)
		csclubj (clone of csclub): Capcom Sports Club (970722 Japan)
		csclubjy (clone of csclub): Capcom Sports Club (970722 Japan, yellow case)

	*cybots: Cyberbots - fullmetal madness (950424 Euro)
		cybotsj (clone of cybots): Cyberbots - fullmetal madness (950420 Japan)
		cybotsjd (clone of cybots): Cyberbots - fullmetal madness (Japan 950424) (decrypted bootleg)
		cybotsu (clone of cybots): Cyberbots - fullmetal madness (950424 USA)
		cybotsud (clone of cybots): Cyberbots - fullmetal madness (950424 USA Phoenix Edition)

	*ddsom: Dungeons & Dragons - shadow over mystara (960619 Euro)
		ddsoma (clone of ddsom): Dungeons & Dragons - shadow over mystara (960619 Asia)
		ddsomar1 (clone of ddsom): Dungeons & Dragons - shadow over mystara (960208 Asia)
		ddsomb (clone of ddsom): Dungeons & Dragons - shadow over mystara (960223 Brazil)
		ddsomh (clone of ddsom): Dungeons & Dragons - shadow over mystara (960223 Hispanic)
		ddsomj (clone of ddsom): Dungeons & Dragons - shadow over mystara (960619 Japan)
		ddsomjr1 (clone of ddsom): Dungeons & Dragons - shadow over mystara (960206 Japan)
		ddsomr1 (clone of ddsom): Dungeons & Dragons - shadow over mystara (960223 Euro)
		ddsomr2 (clone of ddsom): Dungeons & Dragons - shadow over mystara (960209 Euro)
		ddsomr3 (clone of ddsom): Dungeons & Dragons - shadow over mystara (960208 Euro)
		ddsomu (clone of ddsom): Dungeons & Dragons - shadow over mystara (960619 USA)
		ddsomud (clone of ddsom): Dungeons & Dragons - shadow over mystara (960619 USA Phoenix Edition)
		ddsomur1 (clone of ddsom): Dungeons & Dragons - shadow over mystara (960209 USA)

	*ddtod: Dungeons & Dragons - tower of doom (940412 Euro)
		ddtoda (clone of ddtod): Dungeons & Dragons - tower of doom (940412 Asia)
		ddtodar1 (clone of ddtod): Dungeons & Dragons - tower of doom (940113 Asia)
		ddtodd (clone of ddtod): Dungeons & Dragons - tower of doom (940412 Euro Phoenix Edition)
		ddtodh (clone of ddtod): Dungeons & Dragons - tower of doom (940412 Hispanic)
		ddtodhr1 (clone of ddtod): Dungeons & Dragons - tower of doom (940125 Hispanic)
		ddtodhr2 (clone of ddtod): Dungeons & Dragons - tower of doom (940113 Hispanic)
		ddtodj (clone of ddtod): Dungeons & Dragons - tower of doom (940412 Japan)
		ddtodjr1 (clone of ddtod): Dungeons & Dragons - tower of doom (940125 Japan)
		ddtodjr2 (clone of ddtod): Dungeons & Dragons - tower of doom (940113 Japan)
		ddtodr1 (clone of ddtod): Dungeons & Dragons - tower of doom (940113 Euro)
		ddtodu (clone of ddtod): Dungeons & Dragons - tower of doom (940125 USA)
		ddtodur1 (clone of ddtod): Dungeons & Dragons - tower of doom (940113 USA)

	*dimahoo: Dimahoo (000121 Euro)
		dimahoou (clone of dimahoo): Dimahoo (000121 USA)
		dimahoud (clone of dimahoo): Dimahoo (000121 USA Phoenix Edition)
		gmahou (clone of dimahoo): グレート魔法大作戦 (Great Mahou Daisakusen 000121 Japan)

	*dstlk: Darkstalkers - the night warriors (940705 Euro)
		dstlka (clone of dstlk): Darkstalkers - the night warriors (940705 Asia)
		dstlkh (clone of dstlk): Darkstalkers - the night warriors (940818 Hispanic)
		dstlku (clone of dstlk): Darkstalkers - the night warriors (940818 USA)
		dstlku1d (clone of dstlk): Darkstalkers - the night warriors (940705 USA Phoenix Edition)
		dstlkur1 (clone of dstlk): Darkstalkers - the night warriors (940705 USA)
		vampj (clone of dstlk): Vampire - the night warriors (940705 Japan)
		vampja (clone of dstlk): Vampire - the night warriors (940705 Japan, alt)
		vampjr1 (clone of dstlk): Vampire - the night warriors (940630 Japan)

	*ecofghtr: Eco Fighters (931203 etc)
		ecofghtra (clone of ecofghtr): Eco Fighters (931203 Asia)
		ecofghtrd (clone of ecofghtr): Eco Fighters (931203 World Phoenix Edition)
		ecofghtrh (clone of ecofghtr): Eco Fighters (931203 Hispanic)
		ecofghtru (clone of ecofghtr): Eco Fighters (940215 USA)
		ecofghtru1 (clone of ecofghtr): Eco Fighters (931203 USA)
		uecology (clone of ecofghtr): Ultimate Ecology (931203 Japan)

	*gigawing: Giga Wing (990222 USA)
		gigawinga (clone of gigawing): Giga Wing (990222 Asia)
		gigawingb (clone of gigawing): Giga Wing (990222 Brazil)
		gigawingd (clone of gigawing): Giga Wing (990222 USA Phoenix Edition)
		gigawingh (clone of gigawing): Giga Wing (990222 Hispanic)
		gigawingj (clone of gigawing): Giga Wing (990223 Japan)
		gigawingjd (clone of gigawing): Giga Wing (990223 Japan Phoenix Edition)

	*hsf2: Hyper Street Fighter II: The Anniversary Edition (040202 USA)
		hsf2a (clone of hsf2): Hyper Street Fighter II: The Anniversary Edition (040202 Asia)
		hsf2d (clone of hsf2): Hyper Street Fighter II: The Anniversary Edition (040202 Asia Phoenix Edition)
		hsf2da (clone of hsf2): Hyper Street Fighter II: The Anniversary Edition (040202 Asia Phoenix Edition, alt)
		hsf2j (clone of hsf2): Hyper Street Fighter II: The Anniversary Edition (031222 Japan)

	*jyangoku: 雀國志 -覇王の采牌- (Jyangokushi 990527 Japan)

	*megaman: Mega Man - the power battle (951006 USA) (1995, CPS1 hardware) - Note: the rom is based CPS1 hardware.
		mmancp2ur1 (clone of megaman): Mega Man - The Power Battle (950926 USA, SAMPLE Version)
		mmancp2u (clone of megaman): Mega Man - The Power Battle (951006 USA, SAMPLE Version)
		rmancp2j (clone of megaman): Rockman: The Power Battle (950922 Japan)

	*megaman2: Mega Man 2 - the power fighters (960708 USA)
		gigaman2 (clone of megaman2): Gigaman 2: The Power Fighters (bootleg) - Note: No sound.
		megaman2a (clone of megaman2): Mega Man 2 - the power fighters (960708 Asia)
		megaman2h (clone of megaman2): Mega Man 2 - the power fighters (960712 Hispanic)
		megamn2d (clone of megaman2): Mega Man 2 - the power fighters (960708 USA Phoenix Edition)
		rockman2j (clone of megaman2): Rockman 2 - the power fighters (960708 Japan)

	*mmatrix: Mars Matrix (000412 USA)
		mmatrixd (clone of mmatrix): Mars Matrix (000412 USA Phoenix Edition)
		mmatrixj (clone of mmatrix): Mars Matrix (000412 Japan)

	*mpang: Mighty! Pang (001010 Euro)
		mpangj (clone of mpang): Mighty! Pang (001011 Japan)
		mpangjd (clone of mpang): Mighty! Pang (001011 Japan Phoenix Edition)
		mpangr1 (clone of mpang): Mighty! Pang (000925 Euro)
		mpangu (clone of mpang): Mighty! Pang (001010 USA)

	*msh: Marvel Super Heroes (951024 Euro)
		msha (clone of msh): Marvel Super Heroes (951024 Asia)
		mshb (clone of msh): Marvel Super Heroes (951117 Brazil)
		mshh (clone of msh): Marvel Super Heroes (951117 Hispanic)
		mshj (clone of msh): Marvel Super Heroes (951117 Japan)
		mshjr1 (clone of msh): Marvel Super Heroes (951024 Japan)
		mshu (clone of msh): Marvel Super Heroes (951024 USA)
		mshud (clone of msh): Marvel Super Heroes (951024 USA Phoenix Edition)

	*mshvsf: Marvel Super Heroes vs Street Fighter (970625 Euro)
		mshvsfa (clone of mshvsf): Marvel Super Heroes vs Street Fighter (970625 Asia)
		mshvsfa1 (clone of mshvsf): Marvel Super Heroes vs Street Fighter (970620 Asia)
		mshvsfb (clone of mshvsf): Marvel Super Heroes vs Street Fighter (970827 Brazil)
		mshvsfb1 (clone of mshvsf): Marvel Super Heroes vs Street Fighter (970625 Brazil)
		mshvsfh (clone of mshvsf): Marvel Super Heroes vs Street Fighter (970625 Hispanic)
		mshvsfj (clone of mshvsf): Marvel Super Heroes vs Street Fighter (970707 Japan)
		mshvsfj1 (clone of mshvsf): Marvel Super Heroes vs Street Fighter (970702 Japan)
		mshvsfj2 (clone of mshvsf): Marvel Super Heroes vs Street Fighter (970625 Japan)
		mshvsfu (clone of mshvsf): Marvel Super Heroes vs Street Fighter (970827 USA)
		mshvsfu1 (clone of mshvsf): Marvel Super Heroes vs Street Fighter (970625 USA)
		mshvsfu1d (clone of mshvsf): Marvel Super Heroes vs Street Fighter (970625 USA Phoenix Edition)

	*mvsc: Marvel vs Capcom - clash of super heroes (980123 Euro)
		mvsca (clone of mvsc): Marvel vs Capcom - clash of super heroes (980123 Asia)
		mvscar1 (clone of mvsc): Marvel vs Capcom - clash of super heroes (980112 Asia)
		mvscb (clone of mvsc): Marvel vs Capcom - clash of super heroes (980123 Brazil)
		mvsch (clone of mvsc): Marvel vs Capcom - clash of super heroes (980123 Hispanic)
		mvscj (clone of mvsc): Marvel vs Capcom - clash of super heroes (980123 Japan)
		mvscjr1 (clone of mvsc): Marvel vs Capcom - clash of super heroes (980112 Japan)
		mvscjsing (clone of mvsc): Marvel vs Capcom - clash of super heroes (980123 Japan, single PCB)
		mvscr1 (clone of mvsc): Marvel vs Capcom - clash of super heroes (980112 Euro)
		mvscu (clone of mvsc): Marvel vs Capcom - clash of super heroes (980123 USA)
		mvscud (clone of mvsc): Marvel vs Capcom - clash of super heroes (980123 USA Phoenix Edition)
		mvscur1 (clone of mvsc): Marvel vs Capcom - clash of super heroes (971222 USA)

	*nwarr: Night Warriors - darkstalkers' revenge (950316 Euro)
		nwarra (clone of nwarr): Night Warriors - darkstalkers' revenge (950302 Asia)
		nwarrb (clone of nwarr): Night Warriors - darkstalkers' revenge (950403 Brazil)
		nwarrh (clone of nwarr): Night Warriors - darkstalkers' revenge (950403 Hispanic)
		nwarru (clone of nwarr): Night Warriors - darkstalkers' revenge (950406 USA)
		nwarrud (clone of nwarr): Night Warriors - darkstalkers' revenge (950406 USA Phoenix Edition)
		vhuntj (clone of nwarr): Vampire Hunter - darkstalkers' revenge (950316 Japan)
		vhuntjr1 (clone of nwarr): Vampire Hunter - darkstalkers' revenge (950307 Japan)
		vhuntjr1s (clone of nwarr): Vampire Hunter - darkstalkers' revenge (950307 Japan stop version)
		vhuntjr2 (clone of nwarr): Vampire Hunter - darkstalkers' revenge (950302 Japan)

	*progear: Progear (010117 USA)
		halfway (clone of progear): Halfway To Hell: Progear Red Label (2016-1-17 Red label ver)
		progeara (clone of progear): Progear (010117 Asia)
		progearj (clone of progear): プロギアの嵐 (Progear No Arashi 010117 Japan)
		progearjbl (clone of progear): プロギアの嵐 (Progear No Arashi 010117 Japan, decrypted set)
		progearjd (clone of progear): プロギアの嵐 (Progear No Arashi 010117 Japan Phoenix Edition)
		progearud (clone of progear): Progear(010117 USA Phoenix Edition)

	*pzloop2: Puzz Loop 2 (010302 Euro)
		pzloop2j (clone of pzloop2): Puzz Loop 2 (010226 Japan)
		pzloop2jr1 (clone of pzloop2): Puzz Loop 2 (010205 Japan)

	*qndream: Quiz なないろ Dreams - 虹色町の奇跡 (Nanairo Dreams 960826 Japan) ? Quiz Nanairo Dreams - nijiirochou no kiseki

	*ringdest: Ring of Destruction - slammasters II (940902 Euro)
		ringdesta (clone of ringdest): Ring of Destruction - slammasters II (940831 Asia)
		ringdesth (clone of ringdest): Ring of Destruction - slammasters II (940902 Hispanic)
		ringdstd (clone of ringdest): Ring of Destruction - slammasters II (940902 Euro Phoenix Edition)
		smbomb (clone of ringdest): Super Muscle Bomber - the international blowout (940831 Japan)
		smbombr1 (clone of ringdest): Super Muscle Bomber - the international blowout (940808 Japan)

	*sfa: Street Fighter Alpha - warriors' dreams (950727 Euro)
		sfad (clone of sfa): Street Fighter Alpha - warriors' dreams (950727 Euro Phoenix Edition)
		sfar1 (clone of sfa): Street Fighter Alpha - warriors' dreams (950718 Euro)
		sfar2 (clone of sfa): Street Fighter Alpha - warriors' dreams (950627 Euro)
		sfar3 (clone of sfa): Street Fighter Alpha - warriors' dreams (950605 Euro)
		sfau (clone of sfa): Street Fighter Alpha - warriors' dreams (950627 USA)
		sfaud (clone of sfa): Street Fighter Alpha - warriors' dreams (950727 USA Phoenix Edition)
		sfza (clone of sfa): Street Fighter Zero (950627 Asia)
		sfzar1 (clone of sfa): Street Fighter Zero (950605 Asia)
		sfzb (clone of sfa): Street Fighter Zero (951109 Brazil)
		sfzbr1 (clone of sfa): Street Fighter Zero (950727 Brazil)
		sfzh (clone of sfa): Street Fighter Zero (950718 Hispanic)
		sfzhr1 (clone of sfa): Street Fighter Zero (950627 Hispanic)
		sfzj (clone of sfa): Street Fighter Zero (950727 Japan)
		sfzjr1 (clone of sfa): Street Fighter Zero (950627 Japan)
		sfzjr2 (clone of sfa): Street Fighter Zero (950605 Japan)

	*sfa2: Street Fighter Alpha 2 (960229 Euro)
		sfa2u (clone of sfa2): Street Fighter Alpha 2 (960430 USA)
		sfa2ur1 (clone of sfa2): Street Fighter Alpha 2 (960306 USA)
		sfz2a (clone of sfa2): Street Fighter Zero 2 (960227 Asia)
		sfz2ad (clone of sfa2): Street Fighter Zero 2 (960227 Asia Phoenix Edition)
		sfz2b (clone of sfa2): Street Fighter Zero 2 (960531 Brazil)
		sfz2br1 (clone of sfa2): Street Fighter Zero 2 (960304 Brazil)
		sfz2h (clone of sfa2): Street Fighter Zero 2 (960304 Hispanic)
		sfz2j (clone of sfa2): Street Fighter Zero 2 (960430 Japan)
		sfz2jd (clone of sfa2): Street Fighter Zero 2 (960227 Japan Phoenix Edition)
		sfz2jr1 (clone of sfa2): Street Fighter Zero 2 (960227 Japan)
		sfz2n (clone of sfa2): Street Fighter Zero 2 (960229 Oceania)

	*sfa3: Street Fighter Alpha 3 (980904 Euro)
		sfa3b (clone of sfa3): Street Fighter Alpha 3 (980629 Brazil)
		sfa3h (clone of sfa3): Street Fighter Alpha 3 (980904 Hispanic)
		sfa3hr1 (clone of sfa3): Street Fighter Alpha 3 (980629 Hispanic)
		sfa3u (clone of sfa3): Street Fighter Alpha 3 (980904 USA)
		sfa3ud (clone of sfa3): Street Fighter Alpha 3 (980904 USA Phoenix Edition)
		sfa3ur1 (clone of sfa3): Street Fighter Alpha 3 (980629 USA)
		sfa3us (clone of sfa3): Street Fighter Alpha 3 (980616 USA, SAMPLE Version)
		sfz3a (clone of sfa3): Street Fighter Zero 3 (980904 Asia)
		sfz3ar1 (clone of sfa3): Street Fighter Zero 3 (980701 Asia)
		sfz3j (clone of sfa3): Street Fighter Zero 3 (980904 Japan)
		sfz3jr1 (clone of sfa3): Street Fighter Zero 3 (980727 Japan)
		sfz3jr2 (clone of sfa3): Street Fighter Zero 3 (980629 Japan)
		sfz3jr2d (clone of sfa3): Street Fighter Zero 3 (980629 Japan Phoenix Edition)

	*sfz2al: Street Fighter Zero 2 Alpha (960826 Asia)
		sfz2alb (clone of sfz2al): Street Fighter Zero 2 Alpha (960813 Brazil)
		sfz2ald (clone of sfz2al): Street Fighter Zero 2 Alpha (960826 Asia Phoenix Edition)
		sfz2alh (clone of sfz2al): Street Fighter Zero 2 Alpha (960813 Hispanic)
		sfz2alj (clone of sfz2al): Street Fighter Zero 2 Alpha (960805 Japan)

	*sgemf: Super Gem Fighter Mini Mix (970904 USA)
		pfghtj (clone of sgemf): Pocket Fighter (970904 Japan)
		sgemfa (clone of sgemf): Super Gem Fighter Mini Mix (970904 Asia)
		sgemfd (clone of sgemf): Super Gem Fighter Mini Mix (970904 USA Phoenix Edition)
		sgemfh (clone of sgemf): Super Gem Fighter Mini Mix (970904 Hispanic)

	*spf2t: Super Puzzle Fighter II Turbo (Super Puzzle Fighter 2 Turbo 960529 Euro)
		spf2ta (clone of spf2t): Super Puzzle Fighter II Turbo (Super Puzzle Fighter 2 Turbo 960529 Asia)
		spf2td (clone of spf2t): Super Puzzle Fighter II Turbo (Super Puzzle Fighter 2 Turbo 960620 USA Phoenix Edition)
		spf2th (clone of spf2t): Super Puzzle Fighter II Turbo (Super Puzzle Fighter 2 Turbo 960531 Hispanic)
		spf2tu (clone of spf2t): Super Puzzle Fighter II Turbo (Super Puzzle Fighter 2 Turbo 960620 USA)
		spf2xj (clone of spf2t): Super Puzzle Fighter II X (Super Puzzle Fighter 2 X 960531 Japan)
		spf2xjd (clone of spf2t): Super Puzzle Fighter II X (Super Puzzle Fighter 2 X 960531 Japan Phoenix Edition)

	*ssf2: Super Street Fighter II - the new challengers (super street fighter 2 931005 etc)
		ssf2a (clone of ssf2): Super Street Fighter II - the new challengers (super street fighter 2 931005 Asia)
		ssf2ar1 (clone of ssf2): Super Street Fighter II - the new challengers (super street fighter 2 930914 Asia)
		ssf2d (clone of ssf2): Super Street Fighter II - the new challengers (super street fighter 2 930911 etc Phoenix Edition)
		ssf2h (clone of ssf2): Super Street Fighter II - the new challengers (super street fighter 2 930911 Hispanic)
		ssf2j (clone of ssf2): Super Street Fighter II - the new challengers (super street fighter 2 931005 Japan)
		ssf2jr1 (clone of ssf2): Super Street Fighter II - the new challengers (super street fighter 2 930911 Japan)
		ssf2jr2 (clone of ssf2): Super Street Fighter II - the new challengers (super street fighter 2 930910 Japan)
		ssf2r1 (clone of ssf2): Super Street Fighter II - the new challengers (super street fighter 2 930911 etc)
		ssf2tb (clone of ssf2): Super Street Fighter II - the tournament battle (931119 etc)
		ssf2tbd (clone of ssf2): Super Street Fighter II - the tournament battle (931119 etc Phoenix Edition)
		ssf2tbh (clone of ssf2): Super Street Fighter II - the tournament battle (931005 Hispanic)
		ssf2tbj (clone of ssf2): Super Street Fighter II - the tournament battle (930911 Japan)
		ssf2tbr1 (clone of ssf2): Super Street Fighter II - the tournament battle (930911 etc)
		ssf2u (clone of ssf2): Super Street Fighter II - the new challengers (super street fighter 2 930911 USA)
		ssf2ud (clone of ssf2): Super Street Fighter II - the new challengers (super street fighter 2 930911 USA Phoenix Edition)

	*ssf2t: Super Street Fighter II Turbo (super street fighter 2 X 940223 etc)
		ssf2ta (clone of ssf2t): Super Street Fighter II Turbo (super street fighter 2 X 940223 Asia)
		ssf2tad (clone of ssf2t): Super Street Fighter II Turbo (super street fighter 2 X 940223 Asia Phoenix Edition)
		ssf2td (clone of ssf2t): Super Street Fighter II Turbo (super street fighter 2 X 940223 etc Phoenix Edition)
		ssf2th (clone of ssf2t): Super Street Fighter II Turbo (super street fighter 2 X 940223 Hispanic)
		ssf2tu (clone of ssf2t): Super Street Fighter II Turbo (super street fighter 2 X 940323 USA)
		ssf2tur1 (clone of ssf2t): Super Street Fighter II Turbo (super street fighter 2 X 940223 USA)
		ssf2xj (clone of ssf2t): Super Street Fighter II X - grand master challenge (super street fighter 2 X 940223 Japan)
		ssf2xjd (clone of ssf2t): Super Street Fighter II X - grand master challenge (super street fighter 2 X 940223 Japan Phoenix Edition)
		ssf2xjr (clone of ssf2t): Super Street Fighter II X - grand master challenge (super street fighter 2 X 940223 Japan rent version)

	*vhunt2: Vampire Hunter 2 - darkstalkers revenge (970929 Japan)
		vhunt2d (clone of vhunt2): Vampire Hunter 2 - darkstalkers revenge (970913 Japan Phoenix Edition)
		vhunt2r1 (clone of vhunt2): Vampire Hunter 2 - darkstalkers revenge (970913 Japan)

	*vsav: Vampire Savior - the lord of vampire (970519 Euro)
		vsava (clone of vsav): Vampire Savior - the lord of vampire (970519 Asia)
		vsavd (clone of vsav): Vampire Savior - the lord of vampire (970519 Euro Phoenix Edition)
		vsavh (clone of vsav): Vampire Savior - the lord of vampire (970519 Hispanic)
		vsavj (clone of vsav): Vampire Savior - the lord of vampire (970519 Japan)
		vsavu (clone of vsav): Vampire Savior - the lord of vampire (970519 USA)

	*vsav2: Vampire Savior 2 - the lord of vampire (970913 Japan)
		vsav2d (clone of vsav2): Vampire Savior 2 - the lord of vampire (970913 Japan Phoenix Edition)

	*xmcota: X-Men - children of the atom (950331 Euro)
		xmcotaa (clone of xmcota): X-Men - children of the atom (950105 Asia)
		xmcotaar1 (clone of xmcota): X-Men - children of the atom (941217 Asia)
		xmcotah (clone of xmcota): X-Men - children of the atom (950331 Hispanic)
		xmcotahr1 (clone of xmcota): X-Men - children of the atom (950105 Hispanic)
		xmcotaj (clone of xmcota): X-Men - children of the atom (950105 Japan)
		xmcotaj1 (clone of xmcota): X-Men - children of the atom (941222 Japan)
		xmcotaj2 (clone of xmcota): X-Men - children of the atom (941219 Japan)
		xmcotaj3 (clone of xmcota): X-Men - children of the atom (941217 Japan)
		xmcotajr (clone of xmcota): X-Men - children of the atom (941208 Japan, rent version)
		xmcotar1 (clone of xmcota): X-Men - children of the atom (950105 Euro)
		xmcotar1d (clone of xmcota): X-Men - children of the atom (950105 Euro Phoenix Edition)
		xmcotau (clone of xmcota): X-Men - children of the atom (950105 USA)

	*xmvsf: X-Men vs Street Fighter (961004 Euro)
		xmvsfa (clone of xmvsf): X-Men vs Street Fighter (961023 Asia)
		xmvsfar1 (clone of xmvsf): X-Men vs Street Fighter (961004 Asia)
		xmvsfar2 (clone of xmvsf): X-Men vs Street Fighter (960919 Asia)
		xmvsfar3 (clone of xmvsf): X-Men vs Street Fighter (960910 Asia)
		xmvsfb (clone of xmvsf): X-Men vs Street Fighter (961023 Brazil)
		xmvsfh (clone of xmvsf): X-Men vs Street Fighter (961004 Hispanic)
		xmvsfj (clone of xmvsf): X-Men vs Street Fighter (961004 Japan)
		xmvsfjr1 (clone of xmvsf): X-Men vs Street Fighter (960910 Japan)
		xmvsfjr2 (clone of xmvsf): X-Men vs Street Fighter (960909 Japan)
		xmvsfr1 (clone of xmvsf): X-Men vs Street Fighter (960910 Euro)
		xmvsfu (clone of xmvsf): X-Men vs Street Fighter (961023 USA)
		xmvsfu1d (clone of xmvsf): X-Men vs Street Fighter (961004 USA Phoenix Edition)
		xmvsfur1 (clone of xmvsf): X-Men vs Street Fighter (961004 USA)
		xmvsfur2 (clone of xmvsf): X-Men vs Street Fighter (960910 USA)

	/ ****************************************************** /
	/ *    IREM M92 (37 parent+clone roms are supported)   * /
	/ ****************************************************** /
	*bmaster: Blade Master (World)
		crossbld (clone of bmaster): Cross Blades! (Japan)

	*dsoccr94j: Dream Soccer '94 (Japan, M92 hardware)

	*gunforc2: Gunforce 2 (US)
		geostorm (clone of gunforc2): Geostorm (Japan)

	*gunforce: Gunforce - Battle Fire Engulfed Terror Island (World)
		gunforcej (clone of gunforce): Gunforce - Battle Fire Engulfed Terror Island (Japan)
		gunforceu (clone of gunforce): Gunforce - Battle Fire Engulfed Terror Island (US)

	*hook: Hook (World)
		hookj (clone of hook): Hook (Japan)
		hooku (clone of hook): Hook (US)
		ppan (clone of hook): Peter Pan (bootleg of Hook) - Note: Imperfect graphics & sound.

	*inthunt: In The Hunt (World)
		inthuntu (clone of inthunt): In The Hunt (US)
		kaiteids (clone of inthunt): Kaitei Daisensou (Japan)

	*lethalth: Lethal Thunder (World)
		thndblst (clone of lethalth): Thunder Blaster (Japan)

	*majtitl2: Major Title 2 (World)
		majtitl2a (clone of majtitl2): Major Title 2 (World, set 1, alt sound CPU)
		majtitl2b (clone of majtitl2): Major Title 2 (World, set 2)
		majtitl2j (clone of majtitl2): Major Title 2 (Japan)
		skingame (clone of majtitl2): The Irem Skins Game (US set 1)
		skingame2 (clone of majtitl2): The Irem Skins Game (US set 2)

	*mysticri: Mystic Riders (World)
		gunhohki (clone of mysticri): Gun Hohki (Japan)
		mysticrib (clone of mysticri): Mystic Riders (bootleg?)

	*nbbatman: Ninja Baseball Batman (World) - Note: Imperfect graphics.
		leaguemn (clone of nbbatman): Yakyuu Kakutou League-Man (Japan) - Note: Imperfect graphics.
		nbbatmanu (clone of nbbatman): Ninja Baseball Batman (US) - Note: Imperfect graphics.

	*rtypeleo: R-Type Leo (World)
		rtypeleoj: R-Type Leo (Japan)

	*ssoldier: Superior Soldiers (US)
		psoldier (clone of ssoldier): Perfect Soldiers (Japan)

	*uccops: Undercover Cops (World)
		uccopsar (clone of uccops): Undercover Cops (Alpha Renewal Version)
		uccopsj (clone of uccops): Undercover Cops (Japan)
		uccopsu (clone of uccops): Undercover Cops (US)

	/****************************************************/
	/ *  NEO GEO (293 parent+clone roms are supported)  */
	/****************************************************/
	*2020bb: 2020 Super Baseball (set 1)
		2020bba (clone of 2020bb): 2020 Super Baseball (set 2)
		2020bbh (clone of 2020bb): 2020 Super Baseball (set 3)
	
	*3countb: 3 Count Bout / Fire Suplex (NGM-043)(NGH-043)

	*3countba (uses ROMs from 3countb): 3 Count Bout / Fire Suplex (NGM-043)

	*alpham2: Alpha Mission II / ASO II - Last Guardian (NGM-007)(NGH-007)
		alpham2p (clone of alpham2): Alpha Mission II / ASO II - Last Guardian (prototype)

	*androdun: Andro Dunos (NGM-049)(NGH-049)

	*aodk: Aggressors of Dark Kombat· 痛快Ｇａｎｇａｎ行進曲 (ADM-008)(ADH-008)

	*aof: Art of Fighting· 龍虎の拳 (NGM-044)(NGH-044)

	*aof2: Art of Fighting 2 ◦ 龍虎の拳２ (NGM-056)
		aof2a (clone of aof2): Art of Fighting 2 ◦ 龍虎の拳２ (NGH-056)

	*aof3: Art of Fighting 3 - The Path of the Warrior ◦ Art of Fighting - 龍虎の拳外伝
		aof3k (clone of aof3): Art of Fighting 3 - The Path of the Warrior (Korean release)
	
	*b2b: Bang Bang Busters (2010 NCI release)

	*bakatono: バカ殿様 麻雀漫遊記 ◦ Bakatonosama Mahjong Manyuki (MOM-002)(MOH-002)

	*bangbead: Bang Bead
		bangbedp (clone of bangbead): Bang Bead (Prototype?)
	
	*bjourney: Blue's Journey / Raguy (ALM-001)(ALH-001)

	*blazstar: Blazing Star

	*breakers: Breakers

	*breakrev: Breakers Revenge

	*bstars: Baseball Stars Professional (NGM-002)
		bstarsh (clone of bstars): Baseball Stars Professional (NGH-002)
	
	*bstars2: Baseball Stars 2

	*burningf: Burning Fight (NGM-018)(NGH-018)
		burningfh (clone of burningf): Burning Fight (NGH-018)(US)
		burningfp (clone of burningf): Burning Fight (prototype)
	
	*crsword: Crossed Swords (ALM-002)(ALH-002)

	*ctomaday: Captain Tomaday

	*cyberlip: Cyber-Lip (NGM-010)

	*doubledr: Double Dragon (Neo-Geo)

	*eightman: Eight Man (NGM-025)(NGH-025)

	*fatfursp: Fatal Fury Special ◦ 餓狼伝説 Special (set 1)(NGM-058)(NGH-058)
		fatfurspa (clone of fatfursp): Fatal Fury Special ◦ 餓狼伝説 Special (set 2)(NGM-058)(NGH-058)
	
	*fatfury1: Fatal Fury - King of Fighters ◦ 餓狼伝説 - 宿命の闘い (NGM-033)(NGH-033)

	*fatfury2: Fatal Fury 2 ◦ 餓狼伝説２ - 新たなる闘 (NGM-047)(NGH-047)
		fatfury2a (clone of fatfury2): Fatal Fury 2 ◦ 餓狼伝説２ - 新たなる闘 (NGM-047)

	*fatfury3: Fatal Fury 3 - Road to the Final Victory ◦ 餓狼伝説３ (NGM-069)(NGH-069)
		fatfury3a (clone of fatfury3): Fatal Fury 3 - Road to the Final Victory ◦ 餓狼伝説３ (NGM-069)(NGH-069) (alternate set)
	
	*fbfrenzy: Football Frenzy (NGM-034)(NGH-034)

	*fightfev: Fight Fever ◦ 왕중왕 (set 1)
		fightfeva (clone of fightfev): Fight Fever ◦ 왕중왕 (set 2)

	*flipshot: Battle Flip Shot

	*fr2ch: Idol Mahjong - final romance 2 (Neo CD Conversion)

	*galaxyfg: Galaxy Fight - Universal Warriors

	*ganryu: Ganryu ◦ 武蔵巌流記

	*garou: Garou ◦ 餓狼 - mark of the wolves (NGM-2530)
		garoubl (clone of garou): Garou ◦ 餓狼 - mark of the wolves (bootleg)
		garouh (clone of garou): Garou ◦ 餓狼 - mark of the wolves (NGM-2530) (NGH-2530)
		garoup (clone of garou): Garou ◦ 餓狼 - mark of the wolves (prototype)
	
	*ghostlop: Ghostlop ゴーストロップ (prototype)

	*goalx3: Goal! Goal! Goal!

	*gowcaizr: Voltage Fighter - Gowcaizer ◦ 超人学園ゴウカイザー

	*gpilots: Ghost Pilots (NGM-020)(NGH-020)
		gpilotsh (clone of gpilots): Ghost Pilots (NGH-020)(US)

	*gururin: ぐるりん ◦ Gururin

	*ironclad: Choutetsu Brikin'ger - Iron clad (Prototype)
		ironclado (clone of ironclad): Choutetsu Brikin'ger - Iron clad (Prototype, older)
	
	*irrmaze: The Irritating Maze ◦ ウルトラ電流イライラ棒

	*janshin: 雀神伝説 - quest of jongmaster ◦ Jyanshin Densetsu

	*jockeygp: Jockey Grand Prix (set 1)
		jockeygpa (clone of jockeygp): Jockey Grand Prix (set 2)
	
	*joyjoy: Puzzled / Joy Joy Kid (NGM-021)(NGH-021)

	*kabukikl: Kabuki Klash - far east of eden ◦ 天外魔境 - 真伝

	*karnovr: Karnov's Revenge / Fighter's History Dynamite

	*kf2k3pcb: The King of Fighters 2003 (Japan, JAMMA PCB)

	*kizuna: Kizuna Encounter - Super Tag Battle ◦ 風雲 Super Tag Battle

	*kof2000: The King of Fighters 2000 (NGM-2570) (NGH-2570)
		kof2000n (clone of kof2000): The King of Fighters 2000 (not encrypted)
		kof2000ps2 (clone of kof2000): The King of Fighters 2000 (Playstation 2 ver. , EGHT hack)
	
	*kof2001: The King of Fighters 2001 (NGM-262?)
		ct2k3sa (clone of kof2001): Crouching Tiger Hidden Dragon 2003 Super Plus (alternate set) ◦ 臥虎藏龍
		ct2k3sp (clone of kof2001): Crouching Tiger Hidden Dragon 2003 Super Plus ◦ 臥虎藏龍
		cthd2003 (clone of kof2001): Crouching Tiger Hidden Dragon 2003 (set 1) ◦ 臥虎藏龍
		cthd2k3a (clone of kof2001): Crouching Tiger Hidden Dragon 2003 (set 2) ◦ 臥虎藏龍
		kf2k1pa (clone of kof2001): The King of Fighters 2001 Plus (set 2, bootleg / hack)
		kf2k1pls (clone of kof2001): The King of Fighters 2001 Plus (set 1, bootleg / hack)
		kof2001h (clone of kof2001): The King of Fighters 2001 (NGH-2621)

	*kof2002: The King of Fighters 2002 (NGM-2650)(NGH-2650)
		kf10thep (clone of kof2002): The King of Fighters 10th Anniversary Extra Plus (bootleg)
		kf2k2mp (clone of kof2002): The King of Fighters 2002 Magic Plus (bootleg)
		kf2k2mp2 (clone of kof2002): The King of Fighters 2002 Magic Plus II (bootleg)
		kf2k2pla (clone of kof2002): The King of Fighters 2002 Plus (bootleg set 2)
		kf2k2plb (clone of kof2002): The King of Fighters 2002 Plus (bootleg set 3)
		kf2k2plc (clone of kof2002): The King of Fighters 2002 Super (bootleg)
		kf2k2pls (clone of kof2002): The King of Fighters 2002 Plus (bootleg set 1)
		kf2k2ps2 (clone of kof2002): The King of Fighters 2002 (PlayStation 2 ver 0.4, EGHT hack)
		kf2k4pls (clone of kof2002): The King of Fighters Special Edition 2004 Plus (bootleg)
		kf2k5uni (clone of kof2002): The King of Fighters 10th Anniversary 2005 Unique (bootleg)
		kof10th (clone of kof2002): The King of Fighters 10th Anniversary (bootleg)
		kof2002b (clone of kof2002): The King of Fighters 2002 (bootleg)
		kof2k4se (clone of kof2002): The King of Fighters Special Edition 2004 (bootleg)
	
	*kof2003: The King of Fighters 2003 (NGM-2710)
		kf2k3bl (clone of kof2003): The King of Fighters 2003 (bootleg set 1)
		kf2k3bla (clone of kof2003): The King of Fighters 2003 (bootleg set 2)
		kf2k3pl (clone of kof2003): The King of Fighters 2004 Plus / Hero (bootleg)
		kf2k3upl (clone of kof2003): The King of Fighters 2004 Ultra Plus (bootleg)
		kof2003h (clone of kof2003): The King of Fighters 2003 (NGH-2710)

	*kof94: The King of Fighters '94 (NGM-055)(NGH-055)

	*kof95: The King of Fighters '95 (NGM-084)
		kof95a (clone of kof95): The King of Fighters '95 (NGH-084, alternate board)
		kof95h (clone of kof95): The King of Fighters '95 (NGH-084)
	
	*kof96: The King of Fighters '96 (NGM-214)
		kof96a (clone of kof96): The King of Fighters '96 (NGM-214, alternate board)
		kof96ae (clone of kof96): The King of Fighters '96 (Anniversary Edition, EGHT hack)
		kof96ae20 (clone of kof96): The King of Fighters '96 (Anniversary Edition 2.0.0430, EGHT hack)
		kof96cn (clone of kof96): The King of Fighters '96 (Chinese Edition ver 1.0, hack)
		kof96ep (clone of kof96): The King of Fighters '96 (bootleg / hack)
		kof96h (clone of kof96): The King of Fighters '96 (NGH-214)
	
	*kof97: The King of Fighters '97 (NGM-2320)
		kof97cn (clone of kof97): The King of Fighters '97 (10th Anniversary Chinese Edition, EGHT hack)
		kof97h (clone of kof97): The King of Fighters '97 (NGH-2320)
		kof97k (clone of kof97): The King of Fighters '97 (Korean release)
		kof97oro (clone of kof97): The King of Fighters '97 oroshi plus 2003
		kof97pla (clone of kof97): The King of Fighters '97 Plus 2003 (bootleg / hack)
		kof97pls (clone of kof97): The King of Fighters '97 Plus (bootleg)
		kof97xt (clone of kof97): The King of Fighters '97 - Final Battle (hack)
		kog (clone of kof97): King of Gladiator (The King of Fighters '97 bootleg)
		kogplus (clone of kof97): King of Gladiator Plus (The King of Fighters '97 bootleg)
	
	*kof98: The King of Fighters '98 - The Slugfest / King of Fighters '98 - dream match never ends (NGM-2420)
		kof98a (clone of kof98): The King of Fighters '98 - The Slugfest / King of Fighters '98 - dream match never ends (NGM-2420, alternate board)
		kof98ae (clone of kof98): The King of Fighters '98 (Anniversary Edition, EGHT hack)
		kof98ae2016 (clone of kof98): The King of Fighters '98 (Anniversary Edition 2016, EGHT hack)
		kof98cb (clone of kof98): The King of Fighters '98 (Combo)
		kof98h (clone of kof98): The King of Fighters '98 - The Slugfest / King of Fighters '98 - dream match never ends (NGH-2420)
		kof98k (clone of kof98): The King of Fighters '98 - The Slugfest / King of Fighters '98 - dream match never ends (Korean board)
		kof98ka (clone of kof98): The King of Fighters '98 - The Slugfest / King of Fighters '98 - dream match never ends (Korean board 2)

	*kof99: The King of Fighters '99 - Millennium Battle (NGM-2510)
		kof99ae (clone of kof99): The King of Fighters '99 (15th Anniversary Edition)
		kof99e (clone of kof99): The King of Fighters '99 - Millennium Battle (earlier)
		kof99h (clone of kof99): The King of Fighters '99 - Millennium Battle (NGH-2510)
		kof99k (clone of kof99): The King of Fighters '99 - Millennium Battle (Korean release)
		kof99p (clone of kof99): The King of Fighters '99 - Millennium Battle (prototype)

	*kotm: King of the Monsters (set 1)
		kotmh (clone of kotm): King of the Monsters (set 2)
	
	*kotm2: King of the Monsters 2 - The Next Thing (NGM-039)(NGH-039)
		kotm2p (clone of kotm2): King of the Monsters 2 - The Next Thing (prototype)
	
	*lastblad: The Last Blade ◦ 幕末浪漫 月華の剣士 (NGM-2340)
		lastblada (clone of lastblad): The Last Blade ◦ 幕末浪漫 月華の剣士 (NGM-2340, alternate board)
		lastbladh (clone of lastblad): The Last Blade ◦ 幕末浪漫 月華の剣士 (NGH-2340)
		lastsold (clone of lastblad): The Last Soldier (Korean release of The Last Blade)
	
	*lastbld2: The Last Blade 2 ◦ 幕末浪漫第二幕 - 月華の剣士 - 月に咲く華、散りゆく花 (NGM-2430)(NGH-2430)

	*lbowling: League Bowling (NGM-019)(NGH-019)

	*legendos: Legend of Success Joe / Ashitano Joe Densetsu ◦ あしたのジョー伝説

	*lhcdb: Last Hope CD Beta (Neo CD conversion)

	*lresort: Last Resort
		lresortp (clone of lresort): Last Resort (prototype)

	*magdrop2: Magical Drop II ◦ マジカルドロップ２

	*magdrop3: Magical Drop III ◦ マジカルドロップIII

	*maglord: Magician Lord (NGM-005)
		maglordh (clone of maglord): Magician Lord (NGH-005)
	
	*mahretsu: 麻雀狂列伝 ◦ Mahjong Kyo Retsuden (NGM-004)(NGH-004)

	*marukodq: ちびまる子ちゃん まる子デラックスクイズ ◦ Chibi Marukochan Deluxe Quiz

	*matrim: 新豪血寺一族 - 闘婚 ◦ Matrimelee (NGM-2660) (NGH-2660)
		matrimbl (clone of matrim): 新豪血寺一族 - 闘婚 ◦ Matrimelee (bootleg)
	
	*miexchng: Money Puzzle Exchanger ◦ マネーアイドルエクスチェンジャー

	*minasan: みなさんのおかげさまです！ ◦ Minnasanno Okagesamadesu (MOM-001)(MOH-001)

	*mosyougi: 将棋の達人 ◦ Master of Syougi

	*ms5pcb: Metal Slug 5 (JAMMA PCB)

	*mslug: Metal Slug - Super Vehicle-001

	*mslug2: Metal Slug 2 - Super Vehicle-001/II (NGM-2410) (NGH-2410)
		mslug2t (clone of mslug2): Metal Slug 2 - Super Vehicle-001/II (NGM-9410) (slowdown/fix hack by Trap15)
	
	*mslug3: Metal Slug 3 (NGM-2560)
		mslug3b6 (clone of mslug3): Metal Slug 6 (Metal Slug 3 bootleg)
		mslug3h (clone of mslug3): Metal Slug 3 (NGH-2560)

	*mslug4: Metal Slug 4 (NGM-2630)
		ms4plus (clone of mslug4): Metal Slug 4 Plus (bootleg)
		mslug4h (clone of mslug4): Metal Slug 4 (NGH-2630)
	
	*mslug5: Metal Slug 5 (NGM-2680)
		ms5plus (clone of mslug5): Metal Slug 5 Plus (bootleg)
		mslug5b (clone of mslug5): Metal Slug 5 (bootleg)
		mslug5h (clone of mslug5): Metal Slug 5 (NGH-2680)

	*mslugx: Metal Slug X - Super Vehicle-001 (NGM-2500)(NGH-2500)

	*mutnat: Mutation Nation (NGM-014)(NGH-014)

	*nam1975: NAM-1975 (NGM-001)(NGH-001)

	*ncombat: Ninja Combat (NGM-009)
		ncombath (clone of ncombat): Ninja Combat (NGH-009)

	*ncommand: Ninja Commando

	*neobombe: Neo Bomberman

	*neocup98: Neo-Geo Cup '98 - The Road to the Victory

	*neodrift: Neo Drift Out - New Technology

	*neomrdo: Neo Mr. Do!

	*ninjamas: Ninja master's 覇王忍法帖 ◦ Ninja Master's haoh ninpo cho

	*nitd: Nightmare in the Dark
		nitdbl (clone of nitd): Nightmare in the Dark (bootleg)

	*overtop: Over Top

	*panicbom: Panic Bomber ◦ ぱにっくボンバー ボンバーマン

	*pbobbl2n: Puzzle Bobble 2 / Bust-A-Move Again (Neo-Geo)

	*pbobblen: Puzzle Bobble / Bust-A-Move (Neo-Geo) (NGM-083)
		pbobblenb (clone of pbobblen): Puzzle Bobble / Bust-A-Move (Neo-Geo) (bootleg)
	
	*pgoal: Pleasure Goal / Futsal - 5 on 5 Mini Soccer (NGM-219)

	*pnyaa: Pochi and Nyaa ◦ ポチッとにゃ～

	*popbounc: Pop 'n Bounce ◦ ガッポリン

	*preisle2: Prehistoric Isle 2

	*pspikes2: Power Spikes II (NGM-068)

	*pulstar: Pulstar

	*puzzledp: Puzzle De Pon!
		puzzldpr (clone of puzzledp): Puzzle De Pon! R!
	
	*quizdai2: クイズ迷探偵ネオ＆ジオ - Quiz Daisousa Sen part 2 (NGM-042)(NGH-042)

	*quizdais: クイズ大捜査線 - the last count down ◦ Quiz Daisousa Sen - The Last Count Down (NGM-023)(NGH-023)
		quizdaisk (clone of quizdais): クイズ大捜査線 - The Last Count Down ◦ Quiz Daisousa Sen - The Last Count Down (Korean release)
	
	*quizkof: クイズキングオブファイターズ ◦ Quiz King of Fighters (SAM-080)(SAH-080)
		quizkofk (clone of quizkof): クイズキングオブファイターズ ◦ Quiz King of Fighters (Korean release)
	
	*ragnagrd: Operation Ragnagard ◦ 神凰拳

	*rbff1: Real Bout Fatal Fury ◦ Real Bout 餓狼伝説 (NGM-095)(NGH-095)
		rbff1a (clone of rbff1): Real Bout Fatal Fury ◦ Real Bout 餓狼伝説 (bug fix revision)

	*rbff2: Real Bout Fatal Fury 2 - The Newcomers ◦ Real Bout 餓狼伝説２ (NGM-2400)
		rbff2h (clone of rbff2): Real Bout Fatal Fury 2 - The Newcomers ◦ Real Bout 餓狼伝説２ (NGH-2400)
		rbff2k (clone of rbff2): Real Bout Fatal Fury 2 - The Newcomers (Korean release)
	
	*rbffspec: Real Bout Fatal Fury Special / Real Bout Garou Densetsu Special
		rbffspeck (clone of rbffspec): Real Bout Fatal Fury Special / Real Bout Garou Densetsu Special (Korean release)
	
	*ridhero: Riding Hero (NGM-006)(NGH-006)
		ridheroh (clone of ridhero): Riding Hero (set 2)
	
	*roboarmy: Robo Army (set 1)
		roboarma (clone of roboarmy): Robo Army (set 2)
	
	*rotd: Rage of the Dragons (NGM-264?)

	*s1945p: Strikers 1945 Plus

	*samsh5sp: Samurai Shodown V Special ◦ サムライスピリッツ零 Special  (NGM-2720)
		samsh5sph (clone of samsh5sp): Samurai Shodown V Special ◦ サムライスピリッツ零 Special (NGH-2720) (2nd release, less censored)
		samsh5spho (clone of samsh5sp): Samurai Shodown V Special ◦ サムライスピリッツ零 Special (NGH-2720) (1st release, censored)

	*samsho: Samurai Shodown / Samurai Spirits (NGM-045)
		samshoa (clone of samsho): Samurai Shodown / Samurai Spirits (NGM-045, alternate board)
		samshoh (clone of samsho): Samurai Shodown / Samurai Spirits (NGH-045)

	*samsho2: Samurai Shodown II ◦ 真 Samurai Spirits - 覇王丸地獄変 (NGM-063)(NGH-063)
		samsho2k (clone of samsho2): Saulabi Spirits / Jin Saulabi Tu Hon (Korean release of Samurai Shodown II)
		samsho2k2 (clone of samsho2): Saulabi Spirits / Jin Saulabi Tu Hon (Korean release of Samurai Shodown II, set 2)
	
	*samsho3: Samurai Shodown III ◦ サムライスピリッツ - 斬紅郎無双剣 (NGM-087)
		fswords (clone of samsho3): Fighters Swords (Korean release of Samurai Shodown III)
		samsho3h (clone of samsho3): Samurai Shodown III ◦ サムライスピリッツ - 斬紅郎無双剣 (NGH-087)
	
	*samsho4: Samurai Shodown IV - Amakusa's Revenge ◦ サムライスピリッツ - 天草降臨 (NGM-222)(NGH-222)
		samsho4k (clone of samsho4): Pae Wang Jeon Seol / Legend of a Warrior (Korean censored Samurai Shodown IV)
	
	*samsho5: Samurai Shodown V ◦ サムライスピリッツ零 (NGM-2700)
		samsho5b (clone of samsho5): Samurai Shodown V ◦ サムライスピリッツ零 (bootleg)
		samsho5h (clone of samsho5): Samurai Shodown V ◦ サムライスピリッツ零 (NGH-2700)
		samsho5x (clone of samsho5): Samurai Shodown V ◦ サムライスピリッツ零 (hack of XBOX version)
	
	*savagere: Savage Reign ◦ 風雲黙示録 - 格闘創世

	*sdodgeb: Super Dodge Ball ◦ くにおの熱血闘球伝説

	*sengoku: Sengoku / Sengoku Denshou (NGM-017)(NGH-017) ◦ 戦国伝承
		sengokuh (clone of sengoku): Sengoku / Sengoku Denshou (NGH-017)(US) ◦ 戦国伝承
	
	*sengoku2: Sengoku 2 ◦ 戦国伝承２

	*sengoku3: Sengoku 3 ◦ 戦国伝承２００１
		sengoku3s (clone of sengoku3): Sengoku 3 ◦ 戦国伝承２００１ (Evolution 1.0, FCHT hack)
	
	*shocktr2: Shock Troopers - 2nd Squad
		lans2004 (clone of shocktr2): Lansquenet 2004 (Shock Troopers - 2nd Squad bootleg)
	
	*shocktro: Shock Troopers (set 1)
		shocktroa (clone of shocktro): Shock Troopers (set 2)
	
	*socbrawl: Soccer Brawl (NGM-031)
		socbrawlh (clone of socbrawl): Soccer Brawl (NGH-031)
	
	*sonicwi2: Aero Fighters 2 / Sonic Wings 2

	*sonicwi3: Aero Fighters 3 / Sonic Wings 3

	*spinmast: Spin Master / Miracle Adventure

	*ssideki: Super Sidekicks ◦ 得点王

	*ssideki2: Super Sidekicks 2 - the world championship ◦ 得点王２ - リアルファイトフットボール (NGM-061)(NGH-061)

	*ssideki3: Super Sidekicks 3 - The Next Glory ◦ 得点王３ - 栄光への挑戦

	*ssideki4: The Ultimate 11 - SNK Football Championship ◦ 得点王 - 炎のリベロ

	*stakwin: Stakes Winner / Stakes Winner - GI kinzen seiha e no michi

	*stakwin2: Stakes Winner 2

	*strhoop: Street Hoop / Street Slam / Dunk Dream (DEM-004) (DEH-004)

	*superspy: The Super Spy (NGM-011)(NGH-011)

	*svc: SNK vs. Capcom - SVC Chaos (NGM-2690)(NGH-2690)
		svcboot (clone of svc): SNK vs. Capcom - SVC Chaos (bootleg)
		svcplus (clone of svc): SNK vs. Capcom - SVC Chaos Plus (bootleg set 1)
		svcplusa (clone of svc): SNK vs. Capcom - SVC Chaos Plus (bootleg set 2)
		svcsplus (clone of svc): SNK vs. Capcom - SVC Chaos Super Plus (bootleg)

	*svcpcb: SNK vs. Capcom - SVC Chaos (JAMMA PCB, set 1)
		svcpcba (clone of svcpcb): SNK vs. Capcom - SVC Chaos (JAMMA PCB, set 2)
	
	*tophuntr: Top Hunter - Roddy & Cathy (NGM-046)
		tophuntrh (clone of tophuntr): Top Hunter - Roddy & Cathy (NGH-046)
	
	*totcarib: Treasure of the Carribean

	*tpgolf: Top Player's Golf (NGM-003)(NGH-003)

	*trally: Thrash Rally (ALM-003)(ALH-003)

	*turfmast: Neo Turf Masters / Big Tournament Golf

	*twinspri: TwinkleStar Sprites ◦ ティンクルスタースプライツ

	*tws96: Tecmo World Soccer '96

	*unkneo: Unknown Neo-Geo Vs. Fighter (prototype)

	*viewpoin: Viewpoint

	*vliner: V-Liner (set 1)
		vlinero (clone of vliner): V-Liner (set 2)
	
	*wakuwak7: Waku Waku 7 ◦ わくわく７

	*wh1: World Heroes (ALM-005)
		wh1h (clone of wh1): World Heroes (ALH-005)
		wh1ha (clone of wh1): World Heroes (set 3)
	
	*wh2: World Heroes 2 (ALM-006)(ALH-006)

	*wh2j: World Heroes 2 Jet (ADM-007)(ADH-007)
		wh2ja (clone of wh2j): World Heroes 2 Jet (ADM-007)
	
	*whp: World Heroes Perfect

	*wjammers: Windjammers / Flying Power Disc

	*zedblade: Zed Blade ◦ Operation Ragnarok ◦ 作戦名＂ラグナロク＂

	*zintrckb: Zintrick ◦ 押し出しジントリック (hack / bootleg)
		zintrkcd (clone of zintrckb): Zintrick ◦ 押し出しジントリック (Neo CD conversion)
	
	*zupapa: Zupapa!

	*crswd2bl: Crossed Swords 2 (bootleg CD to cartridge conversion)- Note: From fbalpha 02.97.43 romsets.
