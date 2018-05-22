
ROM_START( sfzch )		/* Street Fighter Zero (CPS Changer, Japan 951020) */
	ROM_REGION( CODE_SIZE, "maincpu", 0 )		/* 68000 code */
	ROM_LOAD16_WORD_SWAP( "sfzch23",  0x000000, 0x080000, VERIFY_OFF )
	ROM_LOAD16_WORD_SWAP( "sfza22",   0x080000, 0x080000, VERIFY_OFF )
	ROM_LOAD16_WORD_SWAP( "sfzch21",  0x100000, 0x080000, VERIFY_OFF )
	ROM_LOAD16_WORD_SWAP( "sfza20",   0x180000, 0x080000, VERIFY_OFF )

	ROM_REGION( 0x800000, "gfx", 0 )
	ROMX_LOAD( "sfz_01.3a",  0x000000, 0x080000, VERIFY_OFF, ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "sfz_02.4a",  0x000002, 0x080000, VERIFY_OFF, ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "sfz_03.5a",  0x000004, 0x080000, VERIFY_OFF, ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "sfz_04.6a",  0x000006, 0x080000, VERIFY_OFF, ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "sfz_05.7a",  0x200000, 0x080000, VERIFY_OFF, ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "sfz_06.8a",  0x200002, 0x080000, VERIFY_OFF, ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "sfz_07.9a",  0x200004, 0x080000, VERIFY_OFF, ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "sfz_08.10a", 0x200006, 0x080000, VERIFY_OFF, ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "sfz_10.3c",  0x400000, 0x080000, VERIFY_OFF, ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "sfz_11.4c",  0x400002, 0x080000, VERIFY_OFF, ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "sfz_12.5c",  0x400004, 0x080000, VERIFY_OFF, ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "sfz_13.6c",  0x400006, 0x080000, VERIFY_OFF, ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "sfz_14.7c",  0x600000, 0x080000, VERIFY_OFF, ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "sfz_15.8c",  0x600002, 0x080000, VERIFY_OFF, ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "sfz_16.9c",  0x600004, 0x080000, VERIFY_OFF, ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "sfz_17.10c", 0x600006, 0x080000, VERIFY_OFF, ROM_GROUPWORD | ROM_SKIP(6) )

	ROM_REGION( 0x08000, "stars", 0 )
	ROM_COPY( "gfx", 0x000000, 0x000000, 0x8000 )	/* stars */

	ROM_REGION( 0x028000, "audiocpu", 0 )		/* 64k for the audio CPU (+bank) */
	ROM_LOAD( "sfz_09.12a", 0x000000, 0x08000, VERIFY_OFF )
	ROM_CONTINUE(	   0x010000, 0x08000 )

	ROM_REGION( 0x040000, "oki", 0 )		/* Samples */
	ROM_LOAD( "sfz_18.11c", 0x000000, 0x020000, VERIFY_OFF )
	ROM_LOAD( "sfz_19.12c", 0x020000, 0x020000, VERIFY_OFF )
ROM_END

ROM_START( sfach )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )		/* 68000 code */
	ROM_LOAD16_WORD_SWAP( "sfach23",  0x000000, 0x80000, CRC(02a1a853) SHA1(d92b9e774844fdcc9d9946b3e892b021e672d876) )
	ROM_LOAD16_WORD_SWAP( "sfza22",   0x080000, 0x80000, CRC(8d9b2480) SHA1(405305c1572908d00eab735f28676fbbadb4fac6) )
	ROM_LOAD16_WORD_SWAP( "sfzch21",  0x100000, 0x80000, CRC(5435225d) SHA1(6b1156fd82d0710e244ede39faaae0847c598376) )
	ROM_LOAD16_WORD_SWAP( "sfza20",   0x180000, 0x80000, CRC(806e8f38) SHA1(b6d6912aa8f2f590335d7ff9a8214648e7131ebb) )

	ROM_REGION( 0x800000, "gfx", 0 )
	ROMX_LOAD( "sfz_01.3a",   0x000000, 0x80000, CRC(0dd53e62) SHA1(5f3bcf5ca0fd564d115fe5075a4163d3ee3226df) , ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "sfz_02.4a",   0x000002, 0x80000, CRC(94c31e3f) SHA1(2187b3d4977514f2ae486eb33ed76c86121d5745) , ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "sfz_03.5a",   0x000004, 0x80000, CRC(9584ac85) SHA1(bbd62d66b0f6909630e801ce5d6331d43f44d741) , ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "sfz_04.6a",   0x000006, 0x80000, CRC(b983624c) SHA1(841106bb9453e3dfb7869c4b0e9149cc610d515a) , ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "sfz_05.7a",   0x200000, 0x80000, CRC(2b47b645) SHA1(bc6426eff5df9417f32666586744626fa544f7b5) , ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "sfz_06.8a",   0x200002, 0x80000, CRC(74fd9fb1) SHA1(7945472591f3c06970e96611a0363ed8f3d52c36) , ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "sfz_07.9a",   0x200004, 0x80000, CRC(bb2c734d) SHA1(97a06935f86f31755d2ffdc5b56bef53944bdecd) , ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "sfz_08.10a",  0x200006, 0x80000, CRC(454f7868) SHA1(eecccba7542d893bc41676246a20aa4914b79bbc) , ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "sfz_10.3c",   0x400000, 0x80000, CRC(2a7d675e) SHA1(0144ba34a29fb08b41c780ce65bb06d25724e88f) , ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "sfz_11.4c",   0x400002, 0x80000, CRC(e35546c8) SHA1(7b08aa3413494d12c5c550263a5f00b64b98e6ab) , ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "sfz_12.5c",   0x400004, 0x80000, CRC(f122693a) SHA1(71ce901d8d30207e506b6a8d6a4e0fcf3a1b0eac) , ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "sfz_13.6c",   0x400006, 0x80000, CRC(7cf942c8) SHA1(a7109facb97a8a11ddf1b4e07de6ff3164d713a1) , ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "sfz_14.7c",   0x600000, 0x80000, CRC(09038c81) SHA1(3461d70902fbfb92ce40f804be6388276a01d153) , ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "sfz_15.8c",   0x600002, 0x80000, CRC(1aa17391) SHA1(b4d0f760a430b7fc4443b6c94da2659315c5b926) , ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "sfz_16.9c",   0x600004, 0x80000, CRC(19a5abd6) SHA1(73ba1de15c883fdc69fd7dccdb58d00ca512d4ea) , ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "sfz_17.10c",  0x600006, 0x80000, CRC(248b3b73) SHA1(95810a17b1caf6372b33ed3e4ee8a7e51482c70d) , ROM_GROUPWORD | ROM_SKIP(6) )

	ROM_REGION( 0x8000, "stars", 0 )
	ROM_COPY( "gfx", 0x000000, 0x000000, 0x8000 )	/* stars */

	ROM_REGION( 0x18000, "audiocpu", 0 )		/* 64k for the audio CPU (+banks) */
	ROM_LOAD( "sfz_09.12a",   0x00000, 0x08000, CRC(c772628b) SHA1(ebc5b7c173caf1e151f733f23c1b20abec24e16d) )
	ROM_CONTINUE(             0x10000, 0x08000 )

	ROM_REGION( 0x40000, "oki", 0 )			/* Samples */
	ROM_LOAD( "sfz_18.11c",   0x00000, 0x20000, CRC(61022b2d) SHA1(6369d0c1d08a30ee19b94e52ab1463a7784b9de5) )
	ROM_LOAD( "sfz_19.12c",   0x20000, 0x20000, CRC(3b5886d5) SHA1(7e1b7d40ef77b5df628dd663d45a9a13c742cf58) )
ROM_END

ROM_START( wofch )		/* Tenchi wo Kurau II: Sekiheki no Tatakai (CPS Changer, Japan 921031) */
	ROM_REGION( CODE_SIZE, "maincpu", 0 )		/* 68000 code */
	ROM_LOAD16_WORD_SWAP( "tk2(ch)_23.8f",  0x000000, 0x080000, VERIFY_OFF )
	ROM_LOAD16_WORD_SWAP( "tk2(ch)_22.7f",  0x080000, 0x080000, VERIFY_OFF )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROMX_LOAD( "tk2-1m.3a",     	0x000000, 0x080000, VERIFY_OFF, ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "tk2-3m.5a",     	0x000002, 0x080000, VERIFY_OFF, ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "tk2-2m.4a",     	0x000004, 0x080000, VERIFY_OFF, ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "tk2-4m.6a",     	0x000006, 0x080000, VERIFY_OFF, ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "tk2(ch)_05.7a",	0x200000, 0x080000, VERIFY_OFF, ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "tk2(ch)_06.8a",     0x200002, 0x080000, VERIFY_OFF, ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "tk2(ch)_07.9a",    	0x200004, 0x080000, VERIFY_OFF, ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "tk2(ch)_08.10a",   	0x200006, 0x080000, VERIFY_OFF, ROM_GROUPWORD | ROM_SKIP(6) )

	ROM_REGION( 0x028000, "audiocpu", 0 ) /* 64k for the audio CPU (+bank) */
	ROM_LOAD( "tk2_qa.5k",  0x000000, 0x08000, VERIFY_OFF )
	ROM_CONTINUE(	 	0x010000, 0x18000 )

	ROM_REGION( 0x200000, "qsound", 0 )	 /* QSound Samples */
	ROM_LOAD( "tk2-q1.1k", 0x000000, 0x080000, VERIFY_OFF )
	ROM_LOAD( "tk2-q2.2k", 0x080000, 0x080000, VERIFY_OFF )
	ROM_LOAD( "tk2-q3.3k", 0x100000, 0x080000, VERIFY_OFF )
	ROM_LOAD( "tk2-q4.4k", 0x180000, 0x080000, VERIFY_OFF )

	ROM_REGION( 0x0200, "aboardplds", 0 )
	ROM_LOAD( "buf1", 	0x0000, 0x0117, VERIFY_OFF )
	ROM_LOAD( "ioa1", 	0x0000, 0x0117, VERIFY_OFF )
	ROM_LOAD( "prg2", 	0x0000, 0x0117, VERIFY_OFF )
	ROM_LOAD( "rom1", 	0x0000, 0x0117, VERIFY_OFF )
	ROM_LOAD( "sou1", 	0x0000, 0x0117, VERIFY_OFF )

	ROM_REGION( 0x0200, "bboardplds", 0 )
	ROM_LOAD( "tk263b.1a", 	0x0000, 0x0117, VERIFY_OFF )
	ROM_LOAD( "iob1.12d",	0x0000, 0x0117, VERIFY_OFF )
	ROM_LOAD( "bprg1.11d",	0x0000, 0x0117, VERIFY_OFF )

	ROM_REGION( 0x0200, "cboardplds", 0 )
	ROM_LOAD( "ioc1.ic7",	0x0000, 0x0117, VERIFY_OFF )
	ROM_LOAD( "c632.ic1",	0x0000, 0x0117, VERIFY_OFF )

	ROM_REGION( 0x0200, "dboardplds", 0 )
	ROM_LOAD( "d7l1.7l",	0x0000, 0x0117, VERIFY_OFF )
	ROM_LOAD( "d8l1.8l",	0x0000, 0x0117, VERIFY_OFF )
	ROM_LOAD( "d9k1.9k",	0x0000, 0x0117, NO_DUMP )
	ROM_LOAD( "d10f1.10f",	0x0000, 0x0117, VERIFY_OFF )
ROM_END

ROM_START( wofches )		/* Tenchi wo Kurau II: Sekiheki no Tatakai (hackrom)(CPS Changer, Japan 921031) */
	ROM_REGION( CODE_SIZE, "maincpu", 0 )		/* 68000 code */
	ROM_LOAD16_WORD_SWAP( "tk2(ch)_23.bin",  0x000000, 0x080000, VERIFY_OFF )
	ROM_LOAD16_WORD_SWAP( "tk2(ch)_22.bin",  0x080000, 0x080000, VERIFY_OFF )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROMX_LOAD( "tk2-1m.3a",     	0x000000, 0x080000, VERIFY_OFF, ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "tk2-3m.5a",     	0x000002, 0x080000, VERIFY_OFF, ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "tk2-2m.4a",     	0x000004, 0x080000, VERIFY_OFF, ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "tk2-4m.6a",     	0x000006, 0x080000, VERIFY_OFF, ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "tk2(ch)_05.7a",	0x200000, 0x080000, VERIFY_OFF, ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "tk2(ch)_06.8a",     0x200002, 0x080000, VERIFY_OFF, ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "tk2(ch)_07.9a",    	0x200004, 0x080000, VERIFY_OFF, ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "tk2(ch)_08.10a",   	0x200006, 0x080000, VERIFY_OFF, ROM_GROUPWORD | ROM_SKIP(6) )

	ROM_REGION( 0x028000, "audiocpu", 0 )		/* 64k for the audio CPU (+bank) */
	ROM_LOAD( "tk2_qa.5k",  0x000000, 0x08000, VERIFY_OFF )
	ROM_CONTINUE(	 	0x010000, 0x18000 )

	ROM_REGION( 0x200000, "qsound", 0 )		/* QSound Samples */
	ROM_LOAD( "tk2-q1.1k", 0x000000, 0x080000, VERIFY_OFF )
	ROM_LOAD( "tk2-q2.2k", 0x080000, 0x080000, VERIFY_OFF )
	ROM_LOAD( "tk2-q3.3k", 0x100000, 0x080000, VERIFY_OFF )
	ROM_LOAD( "tk2-q4.4k", 0x180000, 0x080000, VERIFY_OFF )

	ROM_REGION( 0x0200, "aboardplds", 0 )
	ROM_LOAD( "buf1", 	0x0000, 0x0117, VERIFY_OFF )
	ROM_LOAD( "ioa1", 	0x0000, 0x0117, VERIFY_OFF )
	ROM_LOAD( "prg2", 	0x0000, 0x0117, VERIFY_OFF )
	ROM_LOAD( "rom1", 	0x0000, 0x0117, VERIFY_OFF )
	ROM_LOAD( "sou1", 	0x0000, 0x0117, VERIFY_OFF )

	ROM_REGION( 0x0200, "bboardplds", 0 )
	ROM_LOAD( "tk263b.1a", 	0x0000, 0x0117, VERIFY_OFF )
	ROM_LOAD( "iob1.12d",	0x0000, 0x0117, VERIFY_OFF )
	ROM_LOAD( "bprg1.11d",	0x0000, 0x0117, VERIFY_OFF )

	ROM_REGION( 0x0200, "cboardplds", 0 )
	ROM_LOAD( "ioc1.ic7",	0x0000, 0x0117, VERIFY_OFF )
	ROM_LOAD( "c632.ic1",	0x0000, 0x0117, VERIFY_OFF )

	ROM_REGION( 0x0200, "dboardplds", 0 )
	ROM_LOAD( "d7l1.7l",	0x0000, 0x0117, VERIFY_OFF )
	ROM_LOAD( "d8l1.8l",	0x0000, 0x0117, VERIFY_OFF )
	ROM_LOAD( "d9k1.9k",	0x0000, 0x0117, NO_DUMP )
	ROM_LOAD( "d10f1.10f",	0x0000, 0x0117, VERIFY_OFF )
ROM_END

ROM_START( wofchdx )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )      /* 68000 code */
	ROM_LOAD16_WORD_SWAP( "tk2(ch)dx_23.8f",  0x000000, 0x080000, VERIFY_OFF )
	ROM_LOAD16_WORD_SWAP( "tk2(ch)dx_22.7f",  0x080000, 0x080000, VERIFY_OFF )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROMX_LOAD( "tk2-1m.3a",     	0x000000, 0x080000, VERIFY_OFF, ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "tk2-3m.5a",     	0x000002, 0x080000, VERIFY_OFF, ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "tk2-2m.4a",     	0x000004, 0x080000, VERIFY_OFF, ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "tk2-4m.6a",     	0x000006, 0x080000, VERIFY_OFF, ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "tk2(ch)_05.7a",	0x200000, 0x080000, VERIFY_OFF, ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "tk2(ch)_06.8a",     0x200002, 0x080000, VERIFY_OFF, ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "tk2(ch)_07.9a",    	0x200004, 0x080000, VERIFY_OFF, ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "tk2(ch)_08.10a",   	0x200006, 0x080000, VERIFY_OFF, ROM_GROUPWORD | ROM_SKIP(6) )

	ROM_REGION( 0x028000, "audiocpu", 0 ) /* 64k for the audio CPU (+bank) */
	ROM_LOAD( "tk2_qa.5k",  0x000000, 0x08000, VERIFY_OFF )
	ROM_CONTINUE(	 	0x010000, 0x18000 )

	ROM_REGION( 0x200000, "qsound", 0 )	 /* QSound Samples */
	ROM_LOAD( "tk2-q1.1k", 0x000000, 0x080000, VERIFY_OFF )
	ROM_LOAD( "tk2-q2.2k", 0x080000, 0x080000, VERIFY_OFF )
	ROM_LOAD( "tk2-q3.3k", 0x100000, 0x080000, VERIFY_OFF )
	ROM_LOAD( "tk2-q4.4k", 0x180000, 0x080000, VERIFY_OFF )

	ROM_REGION( 0x0200, "aboardplds", 0 )
	ROM_LOAD( "buf1", 	0x0000, 0x0117, VERIFY_OFF )
	ROM_LOAD( "ioa1", 	0x0000, 0x0117, VERIFY_OFF )
	ROM_LOAD( "prg2", 	0x0000, 0x0117, VERIFY_OFF )
	ROM_LOAD( "rom1", 	0x0000, 0x0117, VERIFY_OFF )
	ROM_LOAD( "sou1", 	0x0000, 0x0117, VERIFY_OFF )

	ROM_REGION( 0x0200, "bboardplds", 0 )
	ROM_LOAD( "tk263b.1a", 	0x0000, 0x0117, VERIFY_OFF )
	ROM_LOAD( "iob1.12d",	0x0000, 0x0117, VERIFY_OFF )
	ROM_LOAD( "bprg1.11d",	0x0000, 0x0117, VERIFY_OFF )

	ROM_REGION( 0x0200, "cboardplds", 0 )
	ROM_LOAD( "ioc1.ic7",	0x0000, 0x0117, VERIFY_OFF )
	ROM_LOAD( "c632.ic1",	0x0000, 0x0117, VERIFY_OFF )

	ROM_REGION( 0x0200, "dboardplds", 0 )
	ROM_LOAD( "d7l1.7l",	0x0000, 0x0117, VERIFY_OFF )
	ROM_LOAD( "d8l1.8l",	0x0000, 0x0117, VERIFY_OFF )
	ROM_LOAD( "d9k1.9k",	0x0000, 0x0117, NO_DUMP )
	ROM_LOAD( "d10f1.10f",	0x0000, 0x0117, VERIFY_OFF )
ROM_END

ROM_START( wofhfh )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )      /* 68000 code */
	ROM_LOAD16_WORD_SWAP( "23",  0x000000, 0x080000, VERIFY_OFF )
	ROM_LOAD16_WORD_SWAP( "22",  0x080000, 0x080000, VERIFY_OFF )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROMX_LOAD( "1",     	0x000000, 0x080000, VERIFY_OFF, ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "2",     	0x000002, 0x080000, VERIFY_OFF, ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "3",     	0x000004, 0x080000, VERIFY_OFF, ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "4",     	0x000006, 0x080000, VERIFY_OFF, ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "5",		0x200000, 0x080000, VERIFY_OFF, ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "6",     	0x200002, 0x080000, VERIFY_OFF, ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "7",    	0x200004, 0x080000, VERIFY_OFF, ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "8",   	0x200006, 0x080000, VERIFY_OFF, ROM_GROUPWORD | ROM_SKIP(6) )

	ROM_REGION( 0x028000, "audiocpu", 0 )
	ROM_LOAD( "9",		0x000000, 0x08000, VERIFY_OFF )
	ROM_CONTINUE(	 	0x010000, 0x18000 )

	ROM_REGION( 0x040000, "oki", 0 )
	ROM_LOAD( "18",		0x000000, 0x020000, VERIFY_OFF )
	ROM_LOAD( "19",		0x020000, 0x020000, VERIFY_OFF )
ROM_END

ROM_START( sf2m3 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )		/* 68000 code */
	ROM_LOAD16_BYTE( "u222chp",           0x000000, 0x80000, CRC(db567b66) SHA1(315bfbf2786ef67a95afb87de836ab348523dbbe) )
	ROM_LOAD16_BYTE( "u196chp",           0x000001, 0x80000, CRC(95ea597e) SHA1(5eb82feaa1de5611a96888e4670744bbb7d90393) )
	ROM_LOAD16_WORD_SWAP( "s92_21a.bin",  0x100000, 0x80000, CRC(925a7877) SHA1(1960dca35f0ca6f2b399a9fccfbc0132ac6425d1) )

	ROM_REGION( 0x600000, "gfx", 0 )
	ROMX_LOAD( "s92_01.bin",   0x000000, 0x80000, CRC(03b0d852) SHA1(f370f25c96ad2b94f8c53d6b7139100285a25bef) , ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "s92_02.bin",   0x000002, 0x80000, CRC(840289ec) SHA1(2fb42a242f60ba7e74009b5a90eb26e035ba1e82) , ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "s92_03.bin",   0x000004, 0x80000, CRC(cdb5f027) SHA1(4c7d944fef200fdfcaf57758b901b5511188ed2e) , ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "s92_04.bin",   0x000006, 0x80000, CRC(e2799472) SHA1(27d3796429338d82a8de246a0ea06dd487a87768) , ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "s92_05.bin",   0x200000, 0x80000, CRC(ba8a2761) SHA1(4b696d66c51611e43522bed752654314e76d33b6) , ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "s92_06.bin",   0x200002, 0x80000, CRC(e584bfb5) SHA1(ebdf1f5e2638eed3a65dda82b1ed9151a355f4c9) , ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "s92_07.bin",   0x200004, 0x80000, CRC(21e3f87d) SHA1(4a4961bb68c3a1ce15f9d393d9c03ecb2466cc29) , ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "s92_08.bin",   0x200006, 0x80000, CRC(befc47df) SHA1(520390420da3a0271ba90b0a933e65143265e5cf) , ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "s92_10.bin",   0x400000, 0x80000, CRC(960687d5) SHA1(2868c31121b1c7564e9767b9a19cdbf655c7ed1d) , ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "s92_11.bin",   0x400002, 0x80000, CRC(978ecd18) SHA1(648a59706b93c84b4206a968ecbdc3e834c476f6) , ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "s92_12.bin",   0x400004, 0x80000, CRC(d6ec9a0a) SHA1(ed6143f8737013b6ef1684e37c05e037e7a80dae) , ROM_GROUPWORD | ROM_SKIP(6) )
	ROMX_LOAD( "s92_13.bin",   0x400006, 0x80000, CRC(ed2c67f6) SHA1(0083c0ffaf6fe7659ff0cf822be4346cd6e61329) , ROM_GROUPWORD | ROM_SKIP(6) )

	ROM_REGION( 0x18000, "audiocpu", 0 )		/* 64k for the audio CPU (+banks) */
	ROM_LOAD( "s92_09.bin",    0x00000, 0x08000, CRC(08f6b60e) SHA1(8258fcaca4ac419312531eec67079b97f471179c) )
	ROM_CONTINUE(              0x10000, 0x08000 )

	ROM_REGION( 0x40000, "oki", 0 )			/* Samples */
	ROM_LOAD( "s92_18.bin",    0x00000, 0x20000, CRC(7f162009) SHA1(346bf42992b4c36c593e21901e22c87ae4a7d86d) )
	ROM_LOAD( "s92_19.bin",    0x20000, 0x20000, CRC(beade53f) SHA1(277c397dc12752719ec6b47d2224750bd1c07f79) )
ROM_END

ROM_START( sf2ceuab3 )			/* bootleg of SF2CE, in FBA, it's name is sf2ceuab3; in MAME, it's name is sf2m8a */
	/* unconfirmed if working on real hardware */
	/* this board has unsupported pals */
	ROM_REGION( CODE_SIZE, "maincpu", 0 )		/* 68000 code */
	ROM_LOAD16_BYTE( "yyc-2.2", 0x000000, 0x80000, CRC(db567b66) SHA1(315bfbf2786ef67a95afb87de836ab348523dbbe) )
	ROM_LOAD16_BYTE( "yyc-3.4", 0x000001, 0x80000, CRC(95ea597e) SHA1(5eb82feaa1de5611a96888e4670744bbb7d90393) )
	ROM_LOAD16_BYTE( "yyc-4.1", 0x100000, 0x20000, CRC(1073b7b6) SHA1(81ca1eab65ceac69520584bb23a684ccb9d92f89) )
	ROM_LOAD16_BYTE( "yyc-5.3", 0x100001, 0x20000, CRC(924c6ce2) SHA1(676a912652bd75da5087f0c7eae047b7681a993c) )

	ROM_REGION( 0x600000, "gfx", 0 )
	ROMX_LOAD( "yyc-a",    0x000000, 0x40000, CRC(8242621f) SHA1(9d566176462bd25f9a377104b0c78a962708bc2b), ROM_SKIP(7) )
	ROM_CONTINUE(          0x000004, 0x40000)
	ROM_CONTINUE(          0x200000, 0x40000)
	ROM_CONTINUE(          0x200004, 0x40000)
	ROMX_LOAD( "yyc-c",    0x000001, 0x40000, CRC(0793a960) SHA1(f351163dd1090f8cd8d4c77e2a29764fee627b13), ROM_SKIP(7) )
	ROM_CONTINUE(          0x000005, 0x40000)
	ROM_CONTINUE(          0x200001, 0x40000)
	ROM_CONTINUE(          0x200005, 0x40000)
	ROMX_LOAD( "yyc-b",    0x000002, 0x40000, CRC(b0159973) SHA1(2a236b9d98fa0acddd844aa94bc5118012a6fb2f), ROM_SKIP(7) )
	ROM_CONTINUE(          0x000006, 0x40000)
	ROM_CONTINUE(          0x200002, 0x40000)
	ROM_CONTINUE(          0x200006, 0x40000)
	ROMX_LOAD( "yyc-d",    0x000003, 0x40000, CRC(92a8b572) SHA1(cbad24e519f0152989764c054da914f55e2b118c), ROM_SKIP(7) )
	ROM_CONTINUE(          0x000007, 0x40000)
	ROM_CONTINUE(          0x200003, 0x40000)
	ROM_CONTINUE(          0x200007, 0x40000)
	/* extra gfx layer roms loaded over the former ones to remove the capcom copyright logo */
	ROMX_LOAD( "yyc-6.1",  0x400000, 0x10000, CRC(94778332) SHA1(c0b9a05c710b89864ee5df1a53b39de30c994e2d), ROM_SKIP(7) )
	ROM_CONTINUE(          0x400004, 0x10000 )
	ROMX_LOAD( "yyc-8.9",  0x400002, 0x10000, CRC(f95bc505) SHA1(385beb2f9f8a473d928d729d722372ae49f410e7), ROM_SKIP(7) )
	ROM_CONTINUE(          0x400006, 0x10000 )
	ROMX_LOAD( "yyc-7.10", 0x400001, 0x10000, CRC(d1e452d3) SHA1(794f7ebf6c46c2938a5477451cf05f2e0c7b9049), ROM_SKIP(7) )
	ROM_CONTINUE(          0x400005, 0x10000 )
	ROMX_LOAD( "yyc-9.8",  0x400003, 0x10000, CRC(155824a9) SHA1(74d7e86be22c11234f5d9d0b25fa709b59ef471b), ROM_SKIP(7) )
	ROM_CONTINUE(          0x400007, 0x10000 )
	/* end of extra gfx layer roms */

	ROM_REGION( 0x18000, "audiocpu", 0 )		/* 64k for the audio CPU (+banks) */
	ROM_LOAD( "a-15.5", 0x00000, 0x08000, CRC(6f07d2cb) SHA1(8ef1338d04c1a0b43e24303085105cfdced0bd5e) )
	ROM_CONTINUE(       0x10000, 0x08000 )

	ROM_REGION( 0x010000, "user1", 0 )		/* unknown (bootleg priority?) */
	ROM_LOAD( "c-27.7", 0x00000, 0x10000, CRC(13ea1c44) SHA1(5b05fe4c3920e33d94fac5f59e09ff14b3e427fe) )

	ROM_REGION( 0x200000, "user2", 0 )
	ROM_LOAD( "yyc-e",    0x000000, 0x100000, CRC(61138469) SHA1(dec3b3af6e3f4fedf51600ddf0515f61b2122493) )
	ROM_LOAD( "yyc-f",    0x100000, 0x100000, CRC(b800dcdb) SHA1(2ec3251b78159b15032d55a5ee5138f159e67190) )

	ROM_REGION( 0x40000, "oki", 0 )			/* Samples */
	ROM_LOAD( "b-16.6", 0x00000, 0x40000, CRC(6cfffb11) SHA1(995526183ffd35f92e9096500a3fe6237faaa2dd) )
ROM_END

ROM_START( sf2amf )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )		/* 68000 code */
	ROM_LOAD16_BYTE( "5.amf",	0x000000, 0x80000, CRC(03991fba) SHA1(6c42bf15248640fdb3e98fb01b0a870649deb410) )
	ROM_LOAD16_BYTE( "4.amf",	0x000001, 0x80000, CRC(39f15a1e) SHA1(901c4fea76bf5bff7330ed07ffde54cdccdaa680) )
	/* missing last part(s) of program roms, some gfx loading instructions are missing */
	/* do not move this outside comments, this is only for testing purpose*/
	ROM_LOAD16_BYTE( "prg part 3.amf", 0x100000, 0x40000, NO_DUMP )
	ROM_LOAD16_BYTE( "prg part 4.amf", 0x100001, 0x40000, NO_DUMP )
	ROM_LOAD16_BYTE( "4.u221",   0x100000, 0x40000, VERIFY_OFF )
	ROM_LOAD16_BYTE( "3.u195",   0x100001, 0x40000, VERIFY_OFF )

	ROM_REGION( 0x600000, "gfx", 0 )
	ROMX_LOAD( "y.c.e.c m.k.r-001", 0x000000, 0x80000, CRC(a258de13) SHA1(2e477948c4c8a2fb7cfdc4a739766bc4a4e01c49), ROM_GROUPWORD | ROM_SKIP(6) )
	ROM_CONTINUE(                   0x000004, 0x80000)
	ROMX_LOAD( "y.c.e.c m.k.r-003", 0x000002, 0x80000, CRC(c781bf87) SHA1(034baa9807c2ce8dc800200963a38cd9262b21fb), ROM_GROUPWORD | ROM_SKIP(6) )
	ROM_CONTINUE(                   0x000006, 0x80000)
	ROMX_LOAD( "y.c.e.c m.k.r-002", 0x200000, 0x80000, CRC(5726cab8) SHA1(0b2243a9a7184d53d42ddab7a8c51b63001c2f56), ROM_GROUPWORD | ROM_SKIP(6) )
	ROM_CONTINUE(                   0x200004, 0x80000)
	ROMX_LOAD( "y.c.e.c d.w.c-011", 0x200002, 0x80000, CRC(bc90c12f) SHA1(ecdb776239b22bd56b7c3a87c9e561f650a4dfea), ROM_GROUPWORD | ROM_SKIP(6) )
	ROM_CONTINUE(                   0x200006, 0x80000)
	ROMX_LOAD( "y.c.e.c d.w.c-012", 0x400000, 0x80000, CRC(187667cc) SHA1(fae65bf23f49a32903fda8080659ccf8d42b911f), ROM_GROUPWORD | ROM_SKIP(6) )
	ROM_CONTINUE(                   0x400004, 0x80000)
	ROMX_LOAD( "y.c.e.c d.w.c-013", 0x400002, 0x80000, CRC(5b585071) SHA1(ad3371b1ba0441c67d9fcbb23b09464710e4e28a), ROM_GROUPWORD | ROM_SKIP(6) )
	ROM_CONTINUE(                   0x400006, 0x80000)
	ROMX_LOAD( "6.amf",             0x400000, 0x10000, CRC(3a85a275) SHA1(01907d69f912abffe3ad9745638ce3f282cfb2e8), ROM_SKIP(7) )
	ROM_CONTINUE(                   0x400004, 0x10000 )
	ROMX_LOAD( "9.amf",             0x400002, 0x10000, CRC(9156472f) SHA1(5db2acfc54308d4d26e0459f9486620a968c81d8), ROM_SKIP(7) )
	ROM_CONTINUE(                   0x400006, 0x10000 )
	ROMX_LOAD( "8.amf",             0x400001, 0x10000, CRC(ecdb083b) SHA1(899894c1db004e98f755ffbdf28d32296b9c0a86), ROM_SKIP(7) )
	ROM_CONTINUE(                   0x400005, 0x10000 )
	ROMX_LOAD( "10.amf",            0x400003, 0x10000, CRC(8fea8384) SHA1(8b31fd8d16cbafb5144f772653336b41db8f64fc), ROM_SKIP(7) )
	ROM_CONTINUE(                   0x400007, 0x10000 )

	ROM_REGION( 0x18000, "audiocpu", 0 )		/* 64k for the audio CPU (+banks) */
	ROM_LOAD( "3.amf", 0x00000, 0x08000, CRC(a4823a1b) SHA1(7b6bf59dfd578bfbbdb64c27988796783442d659) )
	ROM_CONTINUE(      0x10000, 0x08000 )

	ROM_REGION( 0x20000, "user1", 0 )		/* unknown (bootleg priority?) */
	ROM_LOAD( "7.amf", 0x00000, 0x10000, CRC(13ea1c44) SHA1(5b05fe4c3920e33d94fac5f59e09ff14b3e427fe) )

	ROM_REGION( 0x40000, "oki", 0 )			/* Samples */
	ROM_LOAD( "2.amf", 0x00000, 0x20000, CRC(7f162009) SHA1(346bf42992b4c36c593e21901e22c87ae4a7d86d) )
	ROM_LOAD( "1.amf", 0x20000, 0x20000, CRC(beade53f) SHA1(277c397dc12752719ec6b47d2224750bd1c07f79) )
ROM_END
