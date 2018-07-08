
	M.B.A-mini (M.B.A = MAME's skeleton + FBA's romsets)

M.B.A-mini from MAME2010-libretro (https://github.com/libretro/mame2010-libretro)
After the codes is streamlined, only CPS 1/2,NEOGEO,IREM M92 roms is supported.

Build for android:

	make -f Makefile "NATIVE=1" buildtools
	make -f Makefile platform=android
     
Build for pc linux/win:

	make -f Makefile platform= #platform name
  
	(NB: for 64 bits build export PTR64=1 at least on win64)

JOYPAD Controls are: 
	RETRO_DEVICE_ID_JOYPAD_START		[KEY_START]
	RETRO_DEVICE_ID_JOYPAD_SELECT		[KEY_COIN]
	RETRO_DEVICE_ID_JOYPAD_A		[KEY_BUTTON_1]
	RETRO_DEVICE_ID_JOYPAD_B		[KEY_BUTTON_2]
	RETRO_DEVICE_ID_JOYPAD_X		[KEY_BUTTON_3]
	RETRO_DEVICE_ID_JOYPAD_Y		[KEY_BUTTON_4]
	RETRO_DEVICE_ID_JOYPAD_L		[KEY_BUTTON_5]
	RETRO_DEVICE_ID_JOYPAD_R		[KEY_BUTTON_6]
	RETRO_DEVICE_ID_JOYPAD_UP		[KEY_JOYSTICK_U]
	RETRO_DEVICE_ID_JOYPAD_DOWN		[KEY_JOYSTICK_D]
	RETRO_DEVICE_ID_JOYPAD_LEFT		[KEY_JOYSTICK_L]
	RETRO_DEVICE_ID_JOYPAD_RIGHT		[KEY_JOYSTICK_R]

        tips: P1 L2 to show MAMEUI
              P1 R3 to show framerate
              P1 L3 to access Service Mode


Compatibility-list:  - Parent roms with the "*"
/* ------------------------------------------------------ */
/*			EXAMPLE:			  */
/* Parent name: Title name (year.month.day) - Note: 	  */
/*	Clone name: Title name (year.month.day) - Note:	  */
/* ------------------------------------------------------ */


Support the following: 
/ ******************** /
/ *  Capcom System 1 * /
/ ******************** /
	TODO:

/ ********************************************************** /
/ *  Capcom System 2 (304 parent+clone roms are supported) * /
/ ********************************************************** /
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

*qndream: Quiz なないろ Dreams - 虹色町の奇跡 (Nanairo Dreams 960826 Japan) ◦ Quiz Nanairo Dreams - nijiirochou no kiseki

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

/ ***************************************************** /
/ *    IREM M92 (37 parent+clone roms are supported)  * /
/ ***************************************************** /
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

/ ******************** /
/ *     NEO GEO      * /
/ ******************** /
	TODO:

