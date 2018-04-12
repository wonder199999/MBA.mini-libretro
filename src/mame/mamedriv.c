/******************************************************************************

    mamedriv.c

    Copyright Nicola Salmoria and the MAME Team.
    Visit http://mamedev.org for licensing and usage restrictions.

    The list of all available drivers. Drivers have to be included here to be
    recognized by the executable.

    To save some typing, we use a hack here. This file is recursively #included
    twice, with different definitions of the DRIVER() macro. The first one
    declares external references to the drivers; the second one builds an array
    storing all the drivers.

******************************************************************************/

#include "emu.h"

#ifndef DRIVER_RECURSIVE
#define DRIVER_RECURSIVE

/* step 1: declare all external references */
#define DRIVER(NAME)	GAME_EXTERN(NAME);
#include "mamedriv.c"

/* step 2: define the drivers[] array */
#undef DRIVER

#define DRIVER(NAME)	&GAME_NAME(NAME),
const game_driver *const drivers[] =
{
#include "mamedriv.c"
	0	/* end of array */
};

#else	/* DRIVER_RECURSIVE */

	/* *************** */
	/*  Part 1 - M92   */
	/* *************** */

	DRIVER( gunforce )	/* (c) 1991 Irem (World) */
	DRIVER( gunforceu )	/* (c) 1991 Irem America (US) */
	DRIVER( gunforcej )	/* (c) 1991 Irem (Japan) */
	DRIVER( bmaster )	/* (c) 1991 Irem */
	DRIVER( crossbld )	/* (c) 1991 Irem (Japan) */
	DRIVER( lethalth )	/* (c) 1991 Irem (World) */
	DRIVER( thndblst )	/* (c) 1991 Irem (Japan) */
	DRIVER( uccops )	/* (c) 1992 Irem (World) */
	DRIVER( uccopsu )	/* (c) 1992 Irem (US) */
	DRIVER( uccopsar )	/* (c) 1992 Irem (World) */
	DRIVER( uccopsj )	/* (c) 1992 Irem (Japan) */
	DRIVER( mysticri )	/* (c) 1992 Irem (World) */
	DRIVER( mysticrib )	/* bootleg? */
	DRIVER( gunhohki )	/* (c) 1992 Irem (Japan) */
	DRIVER( majtitl2 )	/* (c) 1992 Irem (World, set 1) */
	DRIVER( majtitl2j )	/* (c) 1992 Irem (Japan) */
	DRIVER( skingame )	/* (c) 1992 Irem America (US) */
	DRIVER( skingame2 )	/* (c) 1992 Irem America (US) */
	DRIVER( hook )		/* (c) 1992 Irem (World) */
	DRIVER( hooku )		/* (c) 1992 Irem America (US) */
	DRIVER( hookj )		/* (c) 1992 Irem (Japan) */
	DRIVER( ppan )		/* bootleg */
	DRIVER( rtypeleo )	/* (c) 1992 Irem (World) */
	DRIVER( rtypeleoj )	/* (c) 1992 Irem (Japan) */
	DRIVER( inthunt )	/* (c) 1993 Irem (World) */
	DRIVER( inthuntu )	/* (c) 1993 Irem (US) */
	DRIVER( kaiteids )	/* (c) 1993 Irem (Japan) */
	DRIVER( nbbatman )	/* (c) 1993 Irem America (US) */
	DRIVER( leaguemn )	/* (c) 1993 Irem (Japan) */
	DRIVER( ssoldier )	/* (c) 1993 Irem America (US) */
	DRIVER( psoldier )	/* (c) 1993 Irem (Japan) */
	DRIVER( gunforc2 )	/* (c) 1994 Irem */
	DRIVER( geostorm )	/* (c) 1994 Irem (Japan) */

	/* new added */
	DRIVER( majtitl2a )	/* (c) 1992 Irem (World, set 1, alt sound CPU) */
	DRIVER( majtitl2b )	/* (c) 1992 Irem (World, set 2) */


	/* ************************* */
	/*  Part 2 - Capcom System 1 */
	/* ************************* */

	DRIVER( forgottn )	/* 07/1988 (c) 1988 (World) */
	DRIVER( forgottnu )	/* 07/1988 (c) 1988 (US) */
	DRIVER( forgottnua )	/* 07/1988 (c) 1988 (US) */
	DRIVER( lostwrld )	/* 07/1988 (c) 1988 (Japan) */
	DRIVER( lostwrldo )	/* 07/1988 (c) 1988 (Japan) */
	DRIVER( ghouls )	/* 12/1988 (c) 1988 (World) */
	DRIVER( ghoulsu )	/* 12/1988 (c) 1988 (US) */
	DRIVER( daimakai )	/* 12/1988 (c) 1988 (Japan) */
	DRIVER( daimakair )	/* 12/1988 (c) 1988 (Japan) */
	DRIVER( strider )	/* 03/1989 (c) 1989 (not explicitly stated but should be US) */
	DRIVER( striderua )	/* 03/1989 (c) 1989 (not explicitly stated but should be US) */
	DRIVER( striderj )	/* 03/1989 (c) 1989 */
	DRIVER( striderjr )	/* 03/1989 (c) 1989 */
	DRIVER( dynwar )	/* 04/1989 (c) 1989 (World) */
	DRIVER( dynwaru )	/* 04/1989 (c) 1989 (US) */
	DRIVER( dynwarj )	/* 04/1989 (c) 1989 (Japan) */
	DRIVER( willow )	/* 06/1989 (c) 1989 (US) */
	DRIVER( willowj )	/* 06/1989 (c) 1989 (Japan) */
	DRIVER( willowje )	/* 06/1989 (c) 1989 (Japan) */
	DRIVER( unsquad )	/* 08/1989 (c) 1989 */
	DRIVER( area88 )	/* 08/1989 (c) 1989 */
	DRIVER( ffight )	/* 12/1989 (c) (World) */
	DRIVER( ffightu )	/* 12/1989 (c) (US)    */
	DRIVER( ffightua )	/* 12/1989 (c) (US)    */
	DRIVER( ffightub )	/* 12/1989 (c) (US)    */
	DRIVER( ffightj )	/* 12/1989 (c) (Japan) */
	DRIVER( ffightj1 )	/* 12/1989 (c) (Japan) */
	DRIVER( ffightj2 )	/* 12/1989 (c) (Japan) */
	DRIVER( ffightjh )	/* 12/1989 (c) (Japan) */
	DRIVER( 1941 )		/* 02/1990 (c) 1990 (World) */
	DRIVER( 1941j )		/* 02/1990 (c) 1990 (Japan) */
	DRIVER( mercs )		/* 02/03/1990 (c) 1990 (World) */
	DRIVER( mercsu )	/* 02/03/1990 (c) 1990 (US)    */
	DRIVER( mercsua )	/* 08/06/1990 (c) 1990 (US)    */
	DRIVER( mercsj )	/* 02/03/1990 (c) 1990 (Japan) */
	DRIVER( mtwins )	/* 19/06/1990 (c) 1990 (World) */
	DRIVER( chikij )	/* 19/06/1990 (c) 1990 (Japan) */
	DRIVER( msword )	/* 25/07/1990 (c) 1990 (World) */
	DRIVER( mswordr1 )	/* 23/06/1990 (c) 1990 (World) */
	DRIVER( mswordu )	/* 25/07/1990 (c) 1990 (US)    */
	DRIVER( mswordj )	/* 23/06/1990 (c) 1990 (Japan) */
	DRIVER( cawing )	/* 12/10/1990 (c) 1990 (World) */
	DRIVER( cawingr1 )	/* 09/10/1990 (c) 1990 (World) */
	DRIVER( cawingu )	/* 12/10/1990 (c) 1990 (US) */
	DRIVER( cawingj )	/* 12/10/1990 (c) 1990 (Japan) */
	DRIVER( nemo )		/* 30/11/1990 (c) 1990 (World) */
	DRIVER( nemoj )		/* 20/11/1990 (c) 1990 (Japan) */
	DRIVER( sf2 )		/* 22/05/1991 (c) 1991 (World) */
	DRIVER( sf2eb )		/* 14/02/1991 (c) 1991 (World) */
	DRIVER( sf2ebbl )	/* 14/02/1991 (c) 1991 (World, bootleg) */
	DRIVER( sf2ua )		/* 06/02/1991 (c) 1991 (US)    */
	DRIVER( sf2ub )		/* 14/02/1991 (c) 1991 (US)    */
	DRIVER( sf2ud )		/* 18/03/1991 (c) 1991 (US)    */
	DRIVER( sf2ue )		/* 28/02/1991 (c) 1991 (US)    */
	DRIVER( sf2uf )		/* 11/04/1991 (c) 1991 (US)    */
	DRIVER( sf2ui )		/* 22/05/1991 (c) 1991 (US)    */
	DRIVER( sf2uk )		/* 01/11/1991 (c) 1991 (US)    */
	DRIVER( sf2j )		/* 10/12/1991 (c) 1991 (Japan) */
	DRIVER( sf2ja )		/* 14/02/1991 (c) 1991 (Japan) */
	DRIVER( sf2jc )		/* 06/03/1991 (c) 1991 (Japan) */
	DRIVER( sf2qp1 )	/* hack */
	DRIVER( sf2thndr )	/* hack */
	DRIVER( 3wonders )	/* 20/05/1991 (c) 1991 (World) */
	DRIVER( 3wondersu )	/* 20/05/1991 (c) 1991 (US)    */
	DRIVER( wonder3 )	/* 20/05/1991 (c) 1991 (Japan) */
	DRIVER( 3wondersh )	/* hack? */
	DRIVER( kod )		/* 11/07/1991 (c) 1991 (World) */
	DRIVER( kodu )		/* 10/09/1991 (c) 1991 (US)    */
	DRIVER( kodj )		/* 05/08/1991 (c) 1991 (Japan) */
	DRIVER( captcomm )	/* 02/12/1991 (c) 1991 (World) */
	DRIVER( captcommr1 )	/* 14/10/1991 (c) 1991 (World) */
	DRIVER( captcommu )	/* 28/ 9/1991 (c) 1991 (US)    */
	DRIVER( captcommj )	/* 02/12/1991 (c) 1991 (Japan) */
	DRIVER( captcommjr1 )	/* 28/ 9/1991 (c) 1991 (Japan) */
	DRIVER( captcommb )	/* bootleg */
	DRIVER( knights )	/* 27/11/1991 (c) 1991 (World) */
	DRIVER( knightsu )	/* 27/11/1991 (c) 1991 (US)    */
	DRIVER( knightsj )	/* 27/11/1991 (c) 1991 (Japan) */
	DRIVER( knightsb )	/* bootleg */
	DRIVER( sf2ce )		/* 13/03/1992 (c) 1992 (World) */
	DRIVER( sf2ceua )	/* 13/03/1992 (c) 1992 (US)    */
	DRIVER( sf2ceub )	/* 13/05/1992 (c) 1992 (US)    */
	DRIVER( sf2ceuc )	/* 03/08/1992 (c) 1992 (US)    */
	DRIVER( sf2cej )	/* 13/05/1992 (c) 1992 (Japan) */
	DRIVER( sf2rb )		/* hack */
	DRIVER( sf2rb2 )	/* hack */
	DRIVER( sf2rb3 )	/* hack */
	DRIVER( sf2red )	/* hack */
	DRIVER( sf2v004 )	/* hack */
	DRIVER( sf2acc )	/* hack */
	DRIVER( sf2accp2 )	/* hack */
	DRIVER( sf2dkot2 )	/* hack */
	DRIVER( sf2m1 )		/* hack */
	DRIVER( sf2m2 )		/* hack */
	DRIVER( sf2m3 )		/* hack */
	DRIVER( sf2m4 )		/* hack */
	DRIVER( sf2m5 )		/* hack */
	DRIVER( sf2m6 )		/* hack */
	DRIVER( sf2m7 )		/* hack */
	DRIVER( sf2yyc )	/* hack */
	DRIVER( sf2koryu )	/* hack */
	DRIVER( sf2mdt )	/* bootleg */
	DRIVER( varth )		/* 14/07/1992 (c) 1992 (World) */
	DRIVER( varthr1 )	/* 12/06/1992 (c) 1992 (World) */
	DRIVER( varthu )	/* 12/06/1992 (c) 1992 (US) */
	DRIVER( varthj )	/* 14/07/1992 (c) 1992 (Japan) */
	DRIVER( cworld2j )	/* 11/06/1992 (QUIZ 5) (c) 1992 (Japan) */
	DRIVER( wof )		/* 02/10/1992 (c) 1992 (World) (CPS1 + QSound) */
	DRIVER( wofa )		/* 05/10/1992 (c) 1992 (Asia)  (CPS1 + QSound) */
	DRIVER( wofu )		/* 31/10/1992 (c) 1992 (US) (CPS1 + QSound) */
	DRIVER( wofj )		/* 31/10/1992 (c) 1992 (Japan) (CPS1 + QSound) */
	DRIVER( wofhfb )	/* bootleg */
	DRIVER( sf2hf )		/* 09/12/1992 (c) 1992 (World) */
	DRIVER( sf2hfu )	/* 09/12/1992 (c) 1992 (US)    */
	DRIVER( sf2hfj )	/* 09/12/1992 (c) 1992 (Japan) */
	DRIVER( dino )		/* 01/02/1993 (c) 1993 (World) (CPS1 + QSound) */
	DRIVER( dinou )		/* 01/02/1993 (c) 1993 (US)    (CPS1 + QSound) */
	DRIVER( dinoj )		/* 01/02/1993 (c) 1993 (Japan) (CPS1 + QSound) */
	DRIVER( dinopic )	/* bootleg */
	DRIVER( dinopic2 )	/* bootleg */
	DRIVER( dinohunt )	/* bootleg */
	DRIVER( punisher )	/* 22/04/1993 (c) 1993 (World) (CPS1 + QSound) */
	DRIVER( punisheru )	/* 22/04/1993 (c) 1993 (US)    (CPS1 + QSound) */
	DRIVER( punisherj )	/* 22/04/1993 (c) 1993 (Japan) (CPS1 + QSound) */
	DRIVER( punipic )	/* bootleg */
	DRIVER( punipic2 )	/* bootleg */
	DRIVER( punipic3 )	/* bootleg */
	DRIVER( punisherbz )	/* bootleg */
	DRIVER( slammast )	/* 13/07/1993 (c) 1993 (World) (CPS1 + QSound) */
	DRIVER( slammastu )	/* 13/07/1993 (c) 1993 (US)    (CPS1 + QSound) */
	DRIVER( mbomberj )	/* 13/07/1993 (c) 1993 (Japan) (CPS1 + QSound) */
	DRIVER( mbombrd )	/* 06/12/1993 (c) 1993 (World) (CPS1 + QSound) */
	DRIVER( mbombrdj )	/* 06/12/1993 (c) 1993 (Japan) (CPS1 + QSound) */
	DRIVER( pnickj )	/* 08/06/1994 (c) 1994 Compile + Capcom license (Japan) not listed on Capcom's site */
	DRIVER( qad )		/* 01/07/1992 (c) 1992 (US)    */
	DRIVER( qadj )		/* 21/09/1994 (c) 1994 (Japan) */
	DRIVER( qtono2j )	/* 23/01/1995 (c) 1995 (Japan) */
	DRIVER( pang3 )		/* 01/06/1995 (c) 1995 Mitchell (Euro) not listed on Capcom's site */
	DRIVER( pang3n )	/* 11/05/1995 (c) 1995 Mitchell (Euro) not listed on Capcom's site */
	DRIVER( pang3j )	/* 11/05/1995 (c) 1995 Mitchell (Japan) not listed on Capcom's site */
	DRIVER( megaman )	/* 06/10/1995 (c) 1995 (Asia)  */
	DRIVER( rockmanj )	/* 22/09/1995 (c) 1995 (Japan) */

	DRIVER( fcrash )	/* bootleg of Final Fight */
	DRIVER( kodb )		/* bootleg */

	/* new added */
	DRIVER( sfzch )		/* 20/10/1995 (c) CPS Changer version - Street Fighter Zero */
	DRIVER( wofch )		/* 31/10/1992 (c) CPS Changer version - Tenchi Wo Kurau II */
	DRIVER( wofches )	/* hackrom, based by wofch */
	DRIVER( wofchdx )	/* hackrom, bootleg, CPS Changer version -Sangokushi III Gaiden: Kakou-On's Revenge DX, based by wofch */
	DRIVER( wofhfh )	/* hackrom, Huo Feng Huang, Chinese bootleg */



	/* ************************* */
	/*  Part 3 - Capcom System 2 */
	/* ************************* */

	DRIVER( ssf2 )		/* 11/09/1993 (c) 1993 (World) */
	DRIVER( ssf2u )		/* 11/09/1993 (c) 1993 (US) */
	DRIVER( ssf2a )		/* 05/10/1993 (c) 1993 (Asia) */
	DRIVER( ssf2ar1 )	/* 14/09/1993 (c) 1993 (Asia) */
	DRIVER( ssf2j )		/* 05/10/1993 (c) 1993 (Japan) */
	DRIVER( ssf2jr1 )	/* 11/09/1993 (c) 1993 (Japan) */
	DRIVER( ssf2jr2 )	/* 10/09/1993 (c) 1993 (Japan) */
	DRIVER( ssf2h )		/* 11/09/1993 (c) 1993 (Hispanic) */
	DRIVER( ssf2tb )	/* 11/19/1993 (c) 1993 (World) */
	DRIVER( ssf2tbr1 )	/* 11/09/1993 (c) 1993 (World) */
	DRIVER( ssf2tbj )	/* 11/09/1993 (c) 1993 (Japan) */
	DRIVER( ecofghtr )	/* 03/12/1993 (c) 1993 (World) */
	DRIVER( ecofghtru )	/* 15/02/1994 (c) 1993 (US) */
	DRIVER( ecofghtru1 )	/* 03/12/1993 (c) 1993 (US) */
	DRIVER( ecofghtra )	/* 03/12/1993 (c) 1993 (Asia) */
	DRIVER( ecofghtrh )	/* 03/12/1993 (c) 1993 (Hispanic) */
	DRIVER( uecology )	/* 03/12/1993 (c) 1993 (Japan) */
	DRIVER( ddtod )		/* 12/04/1994 (c) 1993 (Euro) */
	DRIVER( ddtodr1 )	/* 13/01/1994 (c) 1993 (Euro) */
	DRIVER( ddtodu )	/* 25/01/1994 (c) 1993 (US) */
	DRIVER( ddtodur1 )	/* 13/01/1994 (c) 1993 (US) */
	DRIVER( ddtodj )	/* 12/04/1994 (c) 1993 (Japan) */
	DRIVER( ddtodjr1 )	/* 25/01/1994 (c) 1993 (Japan) */
	DRIVER( ddtodjr2 )	/* 13/01/1994 (c) 1993 (Japan) */
	DRIVER( ddtoda )	/* 13/01/1994 (c) 1993 (Asia) */
	DRIVER( ddtodh )	/* 12/04/1994 (c) 1993 (Hispanic) */
	DRIVER( ddtodhr1 )	/* 25/01/1994 (c) 1993 (Hispanic) */
	DRIVER( ddtodhr2 )	/* 13/01/1994 (c) 1993 (Hispanic) */
	DRIVER( ssf2t )		/* 23/02/1994 (c) 1994 (World) */
	DRIVER( ssf2tu )	/* 23/03/1994 (c) 1994 (US) */
	DRIVER( ssf2tur1 )	/* 23/02/1994 (c) 1994 (US) */
	DRIVER( ssf2ta )	/* 23/02/1994 (c) 1994 (Asia) */
	DRIVER( ssf2xj )	/* 23/02/1994 (c) 1994 (Japan) */
	DRIVER( avsp )		/* 20/05/1994 (c) 1994 (Euro) */
	DRIVER( avspu )		/* 20/05/1994 (c) 1994 (US) */
	DRIVER( avspj )		/* 20/05/1994 (c) 1994 (Japan) */
	DRIVER( avspa )		/* 20/05/1994 (c) 1994 (Asia) */
	DRIVER( avsph )		/* 20/05/1994 (c) 1994 (Hispanic) */
	DRIVER( dstlk )		/* 05/07/1994 (c) 1994 (Euro) */
	DRIVER( dstlku )	/* 18/08/1994 (c) 1994 (US) */
	DRIVER( dstlkur1 )	/* 05/07/1994 (c) 1994 (US) */
	DRIVER( dstlka )	/* 05/07/1994 (c) 1994 (Asia) */
	DRIVER( dstlkh )	/* 18/08/1994 (c) 1994 (Hispanic) */
	DRIVER( vampj )		/* 05/07/1994 (c) 1994 (Japan) */
	DRIVER( vampja )	/* 05/07/1994 (c) 1994 (Japan) */
	DRIVER( vampjr1 )	/* 30/06/1994 (c) 1994 (Japan) */
	DRIVER( ringdest )	/* 02/09/1994 (c) 1994 (Euro) */
	DRIVER( smbomb )	/* 31/08/1994 (c) 1994 (Japan) */
	DRIVER( smbombr1 )	/* 08/08/1994 (c) 1994 (Japan) */
	DRIVER( armwar )	/* 24/10/1994 (c) 1994 (Euro) */
	DRIVER( armwarr1 )	/* 11/10/1994 (c) 1994 (Euro) */
	DRIVER( armwaru )	/* 24/10/1994 (c) 1994 (US) */
	DRIVER( armwaru1 )	/* 20/09/1994 (c) 1994 (US) */
	DRIVER( pgear )		/* 24/10/1994 (c) 1994 (Japan) */
	DRIVER( pgearr1 )	/* 16/09/1994 (c) 1994 (Japan) */
	DRIVER( armwara )	/* 20/09/1994 (c) 1994 (Asia) */
	DRIVER( xmcota )	/* 05/01/1995 (c) 1994 (Euro) */
	DRIVER( xmcotau )	/* 05/01/1995 (c) 1994 (US) */
	DRIVER( xmcotah )	/* 31/03/1995 (c) 1994 (Hispanic) */
	DRIVER( xmcotahr1 )	/* 05/01/1995 (c) 1994 (Hispanic) */
	DRIVER( xmcotaj )	/* 05/01/1995 (c) 1994 (Japan) */
	DRIVER( xmcotaj1 )	/* 22/12/1994 (c) 1994 (Japan) */
	DRIVER( xmcotaj2 )	/* 19/12/1994 (c) 1994 (Japan) */
	DRIVER( xmcotaj3 )	/* 17/12/1994 (c) 1994 (Japan) */
	DRIVER( xmcotaa )	/* 17/12/1994 (c) 1994 (Asia) */
	DRIVER( xmcotajr )	/* 08/12/1994 (c) 1994 (Japan Rent) */
	DRIVER( nwarr )		/* 16/03/1995 (c) 1995 (Euro) */
	DRIVER( nwarru )	/* 06/04/1995 (c) 1995 (US) */
	DRIVER( nwarrh )	/* 03/04/1995 (c) 1995 (Hispanic) */
	DRIVER( nwarrb )	/* 03/04/1995 (c) 1995 (Brazil) */
	DRIVER( nwarra )	/* 02/03/1995 (c) 1995 (Asia) */
	DRIVER( vhuntj )	/* 02/03/1995 (c) 1995 (Japan) */
	DRIVER( vhuntjr1 )	/* 07/03/1995 (c) 1995 (Japan) */
	DRIVER( vhuntjr2 )	/* 02/03/1995 (c) 1995 (Japan) */
	DRIVER( cybots )	/* 24/04/1995 (c) 1995 (Euro) */
	DRIVER( cybotsu )	/* 24/04/1995 (c) 1995 (US) */
	DRIVER( cybotsj )	/* 20/04/1995 (c) 1995 (Japan) */
	DRIVER( sfa )		/* 27/07/1995 (c) 1995 (Euro) */
	DRIVER( sfar1 )		/* 18/07/1995 (c) 1995 (Euro) */
	DRIVER( sfar2 )		/* 27/06/1995 (c) 1995 (Euro) */
	DRIVER( sfar3 )		/* 05/06/1995 (c) 1995 (Euro) */
	DRIVER( sfau )		/* 27/06/1995 (c) 1995 (US) */
	DRIVER( sfza )		/* 27/06/1995 (c) 1995 (Asia) */
	DRIVER( sfzj )		/* 27/07/1995 (c) 1995 (Japan) */
	DRIVER( sfzjr1 )	/* 27/06/1995 (c) 1995 (Japan) */
	DRIVER( sfzjr2 )	/* 05/06/1995 (c) 1995 (Japan) */
	DRIVER( sfzh )		/* 18/07/1995 (c) 1995 (Hispanic) */
	DRIVER( sfzhr1 )	/* 27/06/1995 (c) 1995 (Hispanic) */
	DRIVER( sfzb )		/* 09/11/1995 (c) 1995 (Brazil) */
	DRIVER( sfzbr1 )	/* 27/07/1995 (c) 1995 (Brazil) */
	DRIVER( mmancp2u )	/* 06/10/1995 (c) 1995 (Japan) */
	DRIVER( rmancp2j )	/* 22/09/1995 (c) 1995 (Japan) */
	DRIVER( msh )		/* 24/10/1995 (c) 1995 (Euro) */
	DRIVER( mshu )		/* 24/10/1995 (c) 1995 (US) */
	DRIVER( mshj )		/* 17/11/1995 (c) 1995 (Japan) */
	DRIVER( mshjr1 )	/* 24/10/1995 (c) 1995 (Japan) */
	DRIVER( msha )		/* 24/10/1995 (c) 1995 (Asia) */
	DRIVER( mshh )		/* 17/11/1995 (c) 1996 (Hispanic) */
	DRIVER( mshb )		/* 17/11/1995 (c) 1996 (Brazil) */
	DRIVER( 19xx )		/* 07/12/1995 (c) 1996 (US) */
	DRIVER( 19xxj )		/* 25/12/1995 (c) 1996 (Japan) */
	DRIVER( 19xxjr1 )	/* 07/12/1995 (c) 1996 (Japan) */
	DRIVER( 19xxa )		/* 07/12/1995 (c) 1996 (Asia) */
	DRIVER( 19xxh )		/* 18/12/1995 (c) 1996 (Hispanic) */
	DRIVER( 19xxb )		/* 18/12/1995 (c) 1996 (Brazil) */
	DRIVER( ddsom )		/* 19/06/1996 (c) 1996 (Euro) */
	DRIVER( ddsomr1 )	/* 23/02/1996 (c) 1996 (Euro) */
	DRIVER( ddsomr2 )	/* 09/02/1996 (c) 1996 (Euro) */
	DRIVER( ddsomr3 )	/* 08/02/1996 (c) 1996 (Euro) */
	DRIVER( ddsomu )	/* 19/06/1996 (c) 1996 (US) */
	DRIVER( ddsomur1 )	/* 09/02/1996 (c) 1996 (US) */
	DRIVER( ddsomjr1 )	/* 06/02/1996 (c) 1996 (Japan) */
	DRIVER( ddsomj )	/* 19/06/1996 (c) 1996 (Japan) */
	DRIVER( ddsoma )	/* 19/06/1996 (c) 1996 (Asia) */
	DRIVER( ddsomh )	/* 23/02/1996 (c) 1996 (Hispanic) */
	DRIVER( ddsomb )	/* 23/02/1996 (c) 1996 (Brazil) */
	DRIVER( sfa2 )		/* 06/03/1996 (c) 1996 (Euro) */
	DRIVER( sfa2u )		/* 06/03/1996 (c) 1996 (US) */
	DRIVER( sfz2j )		/* 27/02/1996 (c) 1996 (Japan) */
	DRIVER( sfz2a )		/* 27/02/1996 (c) 1996 (Asia) */
	DRIVER( sfz2b )		/* 31/05/1996 (c) 1996 (Brazil) */
	DRIVER( sfz2br1 )	/* 04/03/1996 (c) 1996 (Brazil) */
	DRIVER( sfz2h )		/* 04/03/1996 (c) 1996 (Hispanic) */
	DRIVER( sfz2n )		/* 29/02/1996 (c) 1996 (Oceania) */
	DRIVER( spf2t )		/* 20/06/1996 (c) 1996 (US) */
	DRIVER( spf2xj )	/* 31/05/1996 (c) 1996 (Japan) */
	DRIVER( spf2ta )	/* 29/05/1996 (c) 1996 (Asia) */
	DRIVER( spf2th )	/* 31/05/1996 (c) 1996 (Hispanic) */
	DRIVER( qndream )	/* 26/06/1996 (c) 1996 (Japan) */
	DRIVER( megaman2 )	/* 08/07/1996 (c) 1996 (US) */
	DRIVER( megaman2a )	/* 08/07/1996 (c) 1996 (Asia) */
	DRIVER( rockman2j )	/* 08/07/1996 (c) 1996 (Japan) */
	DRIVER( megaman2h )	/* 12/07/1996 (c) 1996 (Hispanic) */
	DRIVER( gigaman2 )	/* bootleg */
	DRIVER( sfz2al )	/* 26/08/1996 (c) 1996 (Asia) */
	DRIVER( sfz2alj )	/* 05/08/1996 (c) 1996 (Japan) */
	DRIVER( sfz2alh )	/* 13/08/1996 (c) 1996 (Hispanic) */
	DRIVER( sfz2alb )	/* 13/08/1996 (c) 1996 (Brazil) */
	DRIVER( xmvsf )		/* 04/10/1996 (c) 1996 (Euro) */
	DRIVER( xmvsfr1 )	/* 10/09/1996 (c) 1996 (Euro) */
	DRIVER( xmvsfu )	/* 23/10/1996 (c) 1996 (US) */
	DRIVER( xmvsfur1 )	/* 04/10/1996 (c) 1996 (US) */
	DRIVER( xmvsfj )	/* 04/10/1996 (c) 1996 (Japan) */
	DRIVER( xmvsfjr1 )	/* 10/09/1996 (c) 1996 (Japan) */
	DRIVER( xmvsfjr2 )	/* 09/09/1996 (c) 1996 (Japan) */
	DRIVER( xmvsfa )	/* 23/10/1996 (c) 1996 (Asia) */
	DRIVER( xmvsfar1 )	/* 19/09/1996 (c) 1996 (Asia) */
	DRIVER( xmvsfar2 )	/* 10/09/1996 (c) 1996 (Asia) */
	DRIVER( xmvsfh )	/* 04/10/1996 (c) 1996 (Hispanic) */
	DRIVER( xmvsfb )	/* 23/10/1996 (c) 1996 (Brazil) */
	DRIVER( batcir )	/* 19/03/1997 (c) 1997 (Euro) */
	DRIVER( batcirj )	/* 19/03/1997 (c) 1997 (Japan) */
	DRIVER( batcira )	/* 19/03/1997 (c) 1997 (Asia) */
	DRIVER( vsav )		/* 19/05/1997 (c) 1997 (Euro) */
	DRIVER( vsavu )		/* 19/05/1997 (c) 1997 (US) */
	DRIVER( vsavj )		/* 19/05/1997 (c) 1997 (Japan) */
	DRIVER( vsava )		/* 19/05/1997 (c) 1997 (Asia) */
	DRIVER( vsavh )		/* 19/05/1997 (c) 1997 (Hispanic) */
	DRIVER( mshvsf )	/* 25/06/1997 (c) 1997 (Euro) */
	DRIVER( mshvsfu )	/* 27/08/1997 (c) 1997 (US) */
	DRIVER( mshvsfu1 )	/* 25/06/1997 (c) 1997 (US) */
	DRIVER( mshvsfj )	/* 07/07/1997 (c) 1997 (Japan) */
	DRIVER( mshvsfj1 )	/* 02/07/1997 (c) 1997 (Japan) */
	DRIVER( mshvsfj2 )	/* 25/06/1997 (c) 1997 (Japan) */
	DRIVER( mshvsfh )	/* 25/06/1997 (c) 1997 (Hispanic) */
	DRIVER( mshvsfa )	/* 25/06/1997 (c) 1997 (Asia) */
	DRIVER( mshvsfa1 )	/* 20/06/1997 (c) 1997 (Asia) */
	DRIVER( mshvsfb )	/* 27/08/1997 (c) 1997 (Brazil) */
	DRIVER( mshvsfb1 )	/* 25/06/1997 (c) 1997 (Brazil) */
	DRIVER( csclub )	/* 17/10/1997 (c) 1997 (Euro) */
	DRIVER( csclub1 )	/* 22/07/1997 (c) 1997 (Euro) */
	DRIVER( csclubj )	/* 22/07/1997 (c) 1997 (Japan) */
	DRIVER( cscluba )	/* 22/07/1997 (c) 1997 (Asia) */
	DRIVER( csclubh )	/* 22/07/1997 (c) 1997 (Hispanic) */
	DRIVER( sgemf )		/* 04/09/1997 (c) 1997 (US) */
	DRIVER( pfghtj )	/* 04/09/1997 (c) 1997 (Japan) */
	DRIVER( sgemfa )	/* 04/09/1997 (c) 1997 (Asia) */
	DRIVER( sgemfh )	/* 04/09/1997 (c) 1997 (Hispanic) */
	DRIVER( vhunt2r1 )	/* 13/09/1997 (c) 1997 (Japan) */
	DRIVER( vhunt2 )	/* 29/09/1997 (c) 1997 (Japan) */
	DRIVER( vsav2 )		/* 13/09/1997 (c) 1997 (Japan) */
	DRIVER( mvsc )		/* 23/01/1998 (c) 1998 (Euro) */
	DRIVER( mvscr1 )	/* 12/01/1998 (c) 1998 (Euro) */
	DRIVER( mvscu )		/* 23/01/1998 (c) 1998 (US) */
	DRIVER( mvscur1 )	/* 23/01/1998 (c) 1998 (US) */
	DRIVER( mvscj )		/* 23/01/1998 (c) 1998 (Japan) */
	DRIVER( mvscjr1 )	/* 12/01/1998 (c) 1998 (Japan) */
	DRIVER( mvsca )		/* 23/01/1998 (c) 1998 (Asia) */
	DRIVER( mvscar1 )	/* 12/01/1998 (c) 1998 (Asia) */
	DRIVER( mvsch )		/* 23/01/1998 (c) 1998 (Hispanic) */
	DRIVER( mvscb )		/* 23/01/1998 (c) 1998 (Brazil) */
	DRIVER( sfa3 )		/* 04/09/1998 (c) 1998 (US) */
	DRIVER( sfa3u )		/* 04/09/1998 (c) 1998 (US) */
	DRIVER( sfa3ur1 )	/* 29/06/1998 (c) 1998 (US) */
	DRIVER( sfa3h )		/* 29/06/1998 (c) 1998 (Hispanic) */
	DRIVER( sfa3b )		/* 29/06/1998 (c) 1998 (Brazil) */
	DRIVER( sfz3j )		/* 04/09/1998 (c) 1998 (Japan) */
	DRIVER( sfz3jr1 )	/* 27/07/1998 (c) 1998 (Japan) */
	DRIVER( sfz3jr2 )	/* 29/06/1998 (c) 1998 (Japan) */
	DRIVER( sfz3a )		/* 04/09/1998 (c) 1998 (Asia) */
	DRIVER( sfz3ar1 )	/* 01/07/1998 (c) 1998 (Asia) */
	DRIVER( gigawing )	/* 22/02/1999 (c) 1999 Takumi (US) */
	DRIVER( gigawingj )	/* 23/02/1999 (c) 1999 Takumi (Japan) */
	DRIVER( gigawinga )	/* 22/02/1999 (c) 1999 Takumi (Asia) */
	DRIVER( gigawingh )	/* 22/02/1999 (c) 1999 Takumi (Hispanic) */
	DRIVER( gigawingb )	/* 22/02/1999 (c) 1999 Takumi (Brazil) */
	DRIVER( jyangoku )	/* 27/05/1999 (c) 1999 (Japan) */
	DRIVER( dimahoo )	/* 21/01/2000 (c) 2000 Eighting/Raizing (US) */
	DRIVER( dimahoou )	/* 21/01/2000 (c) 2000 Eighting/Raizing (US) */
	DRIVER( gmahou )	/* 21/01/2000 (c) 2000 Eighting/Raizing (Japan) */
	DRIVER( mmatrix )	/* 12/04/2000 (c) 2000 Takumi (US) */
	DRIVER( mmatrixj )	/* 12/04/2000 (c) 2000 Takumi (Japan) */
	DRIVER( 1944 )		/* 20/06/2000 (c) 2000 Eighting/Raizing (US) */
	DRIVER( 1944j )		/* 20/06/2000 (c) 2000 Eighting/Raizing (Japan) */
	DRIVER( mpang )		/* 10/10/2000 (c) 2000 Mitchell (Euro) */
	DRIVER( mpangr1 )	/* 25/09/2000 (c) 2000 Mitchell (Euro) */
	DRIVER( mpangu )	/* 10/10/2000 (c) 2000 Mitchell (US) */
	DRIVER( mpangj )	/* 11/10/2000 (c) 2000 Mitchell (Japan) */
	DRIVER( progear )	/* 17/01/2001 (c) 2001 Cave (US) */
	DRIVER( progearj )	/* 17/01/2001 (c) 2001 Cave (Japan) */
	DRIVER( progeara )	/* 17/01/2001 (c) 2001 Cave (Asia) */
	DRIVER( pzloop2 )	/* 02/03/2001 (c) 2001 Mitchell (Euro) */
	DRIVER( pzloop2j )	/* 05/02/2001 (c) 2001 Mitchell (Japan) */
	DRIVER( choko )		/* 20/08/2001 (c) 2001 Mitchell (Japan) */
	DRIVER( hsf2 )		/* 02/02/2004 (c) 2004 Hyper Street Fighter 2 (USA) */
	DRIVER( hsf2j )		/* 22/12/2003 (c) 2003 Hyper Street Fighter 2 (Japan) */
	DRIVER( hsf2a )		/* 02/02/2004 (c) 2004 Hyper Street Fighter 2 (Asia) */

	/* CPS2 'Phoenix' bootlegs */
	DRIVER( ssf2ud )
	DRIVER( ddtodd )
	DRIVER( avspd )
	DRIVER( dstlku1d )
	DRIVER( ringdstd )
	DRIVER( xmcotad )
	DRIVER( nwarrud )
	DRIVER( sfad )
	DRIVER( mshud )
	DRIVER( 19xxd )
	DRIVER( sfz2ad )
	DRIVER( spf2xjd )
	DRIVER( ddsomud )
	DRIVER( megamn2d )
	DRIVER( sfz2ald )
	DRIVER( xmvsfu1d )
	DRIVER( batcird )
	DRIVER( vsavd )
	DRIVER( mvscud )
	DRIVER( sfa3ud )
	DRIVER( gwingjd )
	DRIVER( 1944d )
	DRIVER( progerjd )
	DRIVER( hsf2d )
	DRIVER( ssf2tbd )
	DRIVER( ssf2xjd )
	DRIVER( sgemfd )
	DRIVER( armwar1d )
	DRIVER( dimahoud )
	DRIVER( mmatrixd )

	/* New added */
	DRIVER( ringdesta )	/* 31/08/1994 (c) Ring of Destruction: Slammasters II, Asia */
	DRIVER( ringdesth )	/* 02/09/1994 (c) Ring of Destruction: Slammasters II, Hispanic */
	DRIVER( sfa3us )	/* 16/06/1998 (c) Street Fighter Alpha 3, USA, Sample Version */
	DRIVER( mvscjsing )	/* 23/01/1998 (c) Marvel Vs. Capcom: Clash of Super Heroes, Japan, Single PCB version */
	DRIVER( ssf2th )	/* 23/02/1994 (c) Super Street Fighter II Turbo, Hispanic */
	DRIVER( ssf2tbh )	/* 05/10/1993 (c) Super Street Fighter II: The Tournament Battle, Hispanic */
	DRIVER( csclubjy )	/* 22/07/1997 (c) Capcom Sports Club, Japan, Yellow case */
	DRIVER( ddsomar1 )	/* 08/02/1996 (c) Dungeons & Dragons: Shadow over Mystara, Asia */
	DRIVER( mmancp2ur1 )	/* 26/09/1995 (c) Mega Man: The Power Battle, USA, SAMPLE Version */
	DRIVER( vhuntjr1s )	/* 07/03/1995 (c) Vampire Hunter: Darkstalkers's Revenge, Japan stop version */


	/* New added Phoenix bootlegs */
	DRIVER( sfaud )		/* Phoenix Edition, Street Fighter Alpha: Warrior's Dream, USA */
	DRIVER( sfz2jd )	/* Phoenix Edition, Street Fighter Zero 2, Japan */
	DRIVER( sfz3jr2d )	/* Phoenix Edition, Street Fighter Zero 3, Japan */
	DRIVER( progearjbl )	/* Phoenix Edition, Progear no Arashi, Japan */
	DRIVER( ssf2tad )	/* Phoenix Edition, Super Street Fighter II Turbo, Asia */
	DRIVER( ssf2td )	/* Phoenix Edition, Super Street Fighter II Turbo, Etc */
	DRIVER( ssf2d )		/* Phoenix Edition, Super Street Fighter II: the new challengers, Etc */
	DRIVER( cybotsud )	/* Phoenix Edition, Cyberbots: Fullmetal Madness, USA */
	DRIVER( ecofghtrd )	/* Phoenix Edition, Eco fighters, World */
	DRIVER( mpangjd )	/* Phoenix Edition, Mighty! Pang, Japan */
	DRIVER( progearud )	/* Phoenix Edition, Progear, USA */
	DRIVER( gigawingd )	/* Phoenix Edition, Giga Wing, USA */
	DRIVER( mshvsfu1d )	/* Phoenix Edition, Marvel Super Heroes Vs. Street Fighter, USA */
	DRIVER( vhunt2d )	/* Phoenix Edition, Vampire Hunter 2: Darkstalkers Revenge, Japan */
	DRIVER( vsav2d )	/* Phoenix Edition, Vampire Savior 2: The Lord of Vampire, Japan */
	DRIVER( csclub1d )	/* Phoenix Edition, Capcom Sports Club, Euro */
	DRIVER( cybotsjd )	/* Decrypted bootleg, Cyberbots: Fullmetal Madness, Japan */
	DRIVER( spf2td )	/* Phoenix Edition, Super Puzzle Fighter II Turbo (USA 960620) */
	DRIVER( 1944ad )	/* Phoenix Edition, 1944 - the loop master (USA 000620) */
	DRIVER( hsf2da )	/* Phoenix Edition, Hyper Street Fighter 2 (Asia 040202 alt) */


	/* ***************** */
	/*  Part 4 - Neo Geo */
	/* ***************** */

	DRIVER( neogeo )
	DRIVER( nam1975 )	/* 0001 (c) 1990 SNK */
	DRIVER( bstars )	/* 0002 (c) 1990 SNK */
	DRIVER( bstarsh )	/* 0002 (c) 1990 SNK */
	DRIVER( tpgolf )	/* 0003 (c) 1990 SNK */
	DRIVER( mahretsu )	/* 0004 (c) 1990 SNK */
	DRIVER( maglord )	/* 0005 (c) 1990 Alpha Denshi Co. */
	DRIVER( maglordh )	/* 0005 (c) 1990 Alpha Denshi Co. */
	DRIVER( ridhero )	/* 0006 (c) 1990 SNK */
	DRIVER( ridheroh )	/* 0006 (c) 1990 SNK */
	DRIVER( alpham2 )	/* 0007 (c) 1991 SNK */
				/* 0008 Sunshine (prototype) 1990 SNK */
	DRIVER( ncombat )	/* 0009 (c) 1990 Alpha Denshi Co. */
	DRIVER( ncombath )	/* 0009 (c) 1990 Alpha Denshi Co. */
	DRIVER( cyberlip )	/* 0010 (c) 1990 SNK */
	DRIVER( superspy )	/* 0011 (c) 1990 SNK */
				/* 0012 */
				/* 0013 */
	DRIVER( mutnat )	/* 0014 (c) 1992 SNK */
				/* 0015 */
	DRIVER( kotm )		/* 0016 (c) 1991 SNK */
	DRIVER( kotmh )		/* 0016 (c) 1991 SNK */
	DRIVER( sengoku )	/* 0017 (c) 1991 SNK */
	DRIVER( sengokh )	/* 0017 (c) 1991 SNK */
	DRIVER( burningf )	/* 0018 (c) 1991 SNK */
	DRIVER( burningfh )	/* 0018 (c) 1991 SNK */
	DRIVER( lbowling )	/* 0019 (c) 1990 SNK */
	DRIVER( gpilots )	/* 0020 (c) 1991 SNK */
	DRIVER( gpilotsh )	/* 0020 (c) 1991 SNK */
	DRIVER( joyjoy )	/* 0021 (c) 1990 SNK */
	DRIVER( bjourney )	/* 0022 (c) 1990 Alpha Denshi Co. */
	DRIVER( quizdais )	/* 0023 (c) 1991 SNK */
	DRIVER( quizdaisk )	/* 0123 (c) 1991 SNK */
	DRIVER( lresort )	/* 0024 (c) 1992 SNK */
	DRIVER( eightman )	/* 0025 (c) 1991 SNK / Pallas */
				/* 0026 Fun Fun Brothers (prototype) 1991 Alpha */
	DRIVER( minasan )	/* 0027 (c) 1990 Monolith Corp. */
				/* 0028 Dunk Star (prototype) Sammy */
	DRIVER( legendos )	/* 0029 (c) 1991 SNK */
	DRIVER( 2020bb )	/* 0030 (c) 1991 SNK / Pallas */
	DRIVER( 2020bba )	/* 0030 (c) 1991 SNK / Pallas */
	DRIVER( 2020bbh )	/* 0030 (c) 1991 SNK / Pallas */
	DRIVER( socbrawl )	/* 0031 (c) 1991 SNK */
	DRIVER( socbrawlh )	/* 0031 (c) 1991 SNK */
	DRIVER( roboarmy )	/* 0032 (c) 1991 SNK */
	DRIVER( fatfury1 )	/* 0033 (c) 1991 SNK */
	DRIVER( fbfrenzy )	/* 0034 (c) 1992 SNK */
				/* 0035 Mystic Wand (prototype) 1991 Alpha */
	DRIVER( bakatono )	/* 0036 (c) 1991 Monolith Corp. */
	DRIVER( crsword )	/* 0037 (c) 1991 Alpha Denshi Co. */
	DRIVER( trally )	/* 0038 (c) 1991 Alpha Denshi Co. */
	DRIVER( kotm2 )		/* 0039 (c) 1992 SNK */
	DRIVER( sengoku2 )	/* 0040 (c) 1993 SNK */
	DRIVER( bstars2 )	/* 0041 (c) 1992 SNK */
	DRIVER( quizdai2 )	/* 0042 (c) 1992 SNK */
	DRIVER( 3countb )	/* 0043 (c) 1993 SNK */
	DRIVER( aof )		/* 0044 (c) 1992 SNK */
	DRIVER( samsho )	/* 0045 (c) 1993 SNK */
	DRIVER( samshoh )	/* 0045 (c) 1993 SNK */
	DRIVER( tophuntr )	/* 0046 (c) 1994 SNK */
	DRIVER( tophuntrh )	/* 0046 (c) 1994 SNK */
	DRIVER( fatfury2 )	/* 0047 (c) 1992 SNK */
	DRIVER( janshin )	/* 0048 (c) 1994 Aicom */
	DRIVER( androdun )	/* 0049 (c) 1992 Visco */
	DRIVER( ncommand )	/* 0050 (c) 1992 Alpha Denshi Co. */
	DRIVER( viewpoin )	/* 0051 (c) 1992 Sammy */
	DRIVER( ssideki )	/* 0052 (c) 1992 SNK */
	DRIVER( wh1 )		/* 0053 (c) 1992 Alpha Denshi Co. */
	DRIVER( wh1h )		/* 0053 (c) 1992 Alpha Denshi Co. */
	DRIVER( wh1ha )		/* 0053 (c) 1992 Alpha Denshi Co. */
				/* 0054 Crossed Swords 2  (CD only? not confirmed, MVS might exist) */
	DRIVER( kof94 )		/* 0055 (c) 1994 SNK */
	DRIVER( aof2 )		/* 0056 (c) 1994 SNK */
	DRIVER( aof2a )		/* 0056 (c) 1994 SNK */
	DRIVER( wh2 )		/* 0057 (c) 1993 ADK */
	DRIVER( fatfursp )	/* 0058 (c) 1993 SNK */
	DRIVER( fatfursa )	/* 0058 (c) 1993 SNK */
	DRIVER( savagere )	/* 0059 (c) 1995 SNK */
	DRIVER( fightfev )	/* 0060 (c) 1994 Viccom */
	DRIVER( fightfeva )	/* 0060 (c) 1994 Viccom */
	DRIVER( ssideki2 )	/* 0061 (c) 1994 SNK */
	DRIVER( spinmast )	/* 0062 (c) 1993 Data East Corporation */
	DRIVER( samsho2 )	/* 0063 (c) 1994 SNK */
	DRIVER( samsho2k )	/* 0063 (c) 1994 SNK (Korean hack) */
	DRIVER( wh2j )		/* 0064 (c) 1994 ADK / SNK */
	DRIVER( wh2jh )		/* 0064 (c) 1994 ADK / SNK */
	DRIVER( wjammers )	/* 0065 (c) 1994 Data East Corporation */
	DRIVER( karnovr )	/* 0066 (c) 1994 Data East Corporation */
	DRIVER( gururin )	/* 0067 (c) 1994 Face */
	DRIVER( pspikes2 )	/* 0068 (c) 1994 Video System Co. */
				/* Super Volley '94 was once released in Mar.1994, and recalled. Then released as Power Spikes 2 (with some tweaks). */
	DRIVER( fatfury3 )	/* 0069 (c) 1995 SNK */
	DRIVER( zupapa )	/* 0070 Zupapa - released in 2001, 1994 prototype probably exists */
				/* 0071 Bang Bang Busters (prototype) 1994 Visco */
				/* 0072 Last Odyssey Pinball Fantasia (prototype) 1995 Monolith */
	DRIVER( panicbom )	/* 0073 (c) 1994 Eighting / Hudson */
	DRIVER( aodk )		/* 0074 (c) 1994 ADK / SNK */
	DRIVER( sonicwi2 )	/* 0075 (c) 1994 Video System Co. */
	DRIVER( zedblade )	/* 0076 (c) 1994 NMK */
				/* 0077 The Warlocks of the Fates (prototype) 1995 Astec */
	DRIVER( galaxyfg )	/* 0078 (c) 1995 Sunsoft */
	DRIVER( strhoop )	/* 0079 (c) 1994 Data East Corporation */
	DRIVER( quizkof )	/* 0080 (c) 1995 Saurus */
	DRIVER( quizkofk )	/* 0080 (c) 1995 Saurus */
	DRIVER( ssideki3 )	/* 0081 (c) 1995 SNK */
	DRIVER( doubledr )	/* 0082 (c) 1995 Technos */
	DRIVER( pbobblen )	/* 0083 (c) 1994 Taito */
	DRIVER( pbobblenb )	/* bootleg */
	DRIVER( kof95 )		/* 0084 (c) 1995 SNK */
	DRIVER( kof95h )	/* 0084 (c) 1995 SNK */
				/* 0085 Shinsetsu Samurai Spirits Bushidoretsuden / Samurai Shodown RPG (CD only) */
	DRIVER( tws96 )		/* 0086 (c) 1996 Tecmo */
	DRIVER( samsho3 )	/* 0087 (c) 1995 SNK */
	DRIVER( samsho3h )	/* 0087 (c) 1995 SNK */
	DRIVER( fswords )	/* 0187 Korean hack of samsho3 */
	DRIVER( stakwin )	/* 0088 (c) 1995 Saurus */
	DRIVER( pulstar )	/* 0089 (c) 1995 Aicom */
	DRIVER( whp )		/* 0090 (c) 1995 ADK / SNK */
				/* 0091 */
	DRIVER( kabukikl )	/* 0092 (c) 1995 Hudson */
	DRIVER( neobombe )	/* 0093 (c) 1997 Hudson */
	DRIVER( gowcaizr )	/* 0094 (c) 1995 Technos */
	DRIVER( rbff1 )		/* 0095 (c) 1995 SNK */
	DRIVER( rbff1a )	/* 0095 (c) 1995 SNK */
	DRIVER( aof3 )		/* 0096 (c) 1996 SNK */
	DRIVER( aof3k )		/* 0196 Censored Korean release of aof3 */
	DRIVER( sonicwi3 )	/* 0097 (c) 1995 Video System Co. */
				/* 0098 Idol Mahjong - final romance 2 (CD only? not confirmed, MVS might exist) */
				/* 0099 Neo Pool Masters */
	DRIVER( turfmast )	/* 0200 (c) 1996 Nazca */
	DRIVER( mslug )		/* 0201 (c) 1996 Nazca */
	DRIVER( puzzledp )	/* 0202 (c) 1995 Taito (Visco license) */
	DRIVER( mosyougi )	/* 0203 (c) 1995 ADK / SNK */
				/* 0204 QP (prototype) */
				/* 0205 Neo-Geo CD Special (CD only) */
	DRIVER( marukodq )	/* 0206 (c) 1995 Takara */
	DRIVER( neomrdo )	/* 0207 (c) 1996 Visco */
	DRIVER( sdodgeb )	/* 0208 (c) 1996 Technos */
	DRIVER( goalx3 )	/* 0209 (c) 1995 Visco */
				/* 0210 Karate Ninja Sho (prototype) 1995 Yumekobo */
				/* 0211 Oshidashi Zintrick (CD only? not confirmed, MVS might exist) 1996 SNK/ADK */
	DRIVER( zintrckb )	/* 0211 hack - this is not a genuine MVS proto, its a bootleg made from the CD version */
	DRIVER( overtop )	/* 0212 (c) 1996 ADK */
	DRIVER( neodrift )	/* 0213 (c) 1996 Visco */
	DRIVER( kof96 )		/* 0214 (c) 1996 SNK */
	DRIVER( kof96h )	/* 0214 (c) 1996 SNK */
	DRIVER( ssideki4 )	/* 0215 (c) 1996 SNK */
	DRIVER( kizuna )	/* 0216 (c) 1996 SNK */
				/*  Fu-un Super Tag Battle Special Version (4-player battle available) exists */
	DRIVER( ninjamas )	/* 0217 (c) 1996 ADK / SNK */
	DRIVER( ragnagrd )	/* 0218 (c) 1996 Saurus */
	DRIVER( pgoal )		/* 0219 (c) 1996 Saurus */
				/* 0220 (c) 1996 Saurus - Choutetsu Brikin'ger - Iron clad (undumped protoype) */
	DRIVER( magdrop2 )	/* 0221 (c) 1996 Data East Corporation */
	DRIVER( samsho4 )	/* 0222 (c) 1996 SNK */
	DRIVER( samsho4k )	/* Censored Korean release of samsho4 */
	DRIVER( rbffspec )	/* 0223 (c) 1996 SNK */
	DRIVER( rbffspeck )	/* 0124 (c) 1996 SNK */
	DRIVER( twinspri )	/* 0224 (c) 1996 ADK */
	DRIVER( wakuwak7 )	/* 0225 (c) 1996 Sunsoft */
				/* 0226 Pair Pair Wars (prototype) 1996 Sunsoft? */
	DRIVER( stakwin2 )	/* 0227 (c) 1996 Saurus */
	DRIVER( ghostlop )	/* 0228 GhostLop (prototype) 1996? Data East */
				/* 0229 King of Fighters '96 CD Collection (CD only) */
	DRIVER( breakers )	/* 0230 (c) 1996 Visco */
	DRIVER( miexchng )	/* 0231 (c) 1997 Face */
	DRIVER( kof97 )		/* 0232 (c) 1997 SNK */
	DRIVER( kof97h )	/* 0232 (c) 1997 SNK */
	DRIVER( kof97pls )	/* bootleg of kof97 */
	DRIVER( kog )		/* bootleg of kof97 */
	DRIVER( magdrop3 )	/* 0233 (c) 1997 Data East Corporation */
	DRIVER( lastblad )	/* 0234 (c) 1997 SNK */
	DRIVER( lastbladh )	/* 0234 (c) 1997 SNK */
	DRIVER( lastsold )	/* 0196 Censored Korean release of lastblad */
	DRIVER( puzzldpr )	/* 0235 (c) 1997 Taito (Visco license) */
	DRIVER( irrmaze )	/* 0236 (c) 1997 SNK / Saurus */
	DRIVER( popbounc )	/* 0237 (c) 1997 Video System Co. */
	DRIVER( shocktro )	/* 0238 (c) 1997 Saurus */
	DRIVER( shocktra )	/* 0238 (c) 1997 Saurus */
	DRIVER( blazstar )	/* 0239 (c) 1998 Yumekobo */
	DRIVER( rbff2 )		/* 0240 (c) 1998 SNK */
	DRIVER( rbff2h )	/* 0240 (c) 1998 SNK */
	DRIVER( rbff2k )	/* 0140 Censored Korean release of rbff2 */
	DRIVER( mslug2 )	/* 0241 (c) 1998 SNK */
	DRIVER( kof98 )		/* 0242 (c) 1998 SNK */
	DRIVER( kof98k )	/* 0242 (c) 1998 SNK */
	DRIVER( kof98ka )	/* 0242 (c) 1998 SNK */
	DRIVER( kof98h )	/* 0242 (c) 1998 SNK */
	DRIVER( lastbld2 )	/* 0243 (c) 1998 SNK */
	DRIVER( neocup98 )	/* 0244 (c) 1998 SNK */
	DRIVER( breakrev )	/* 0245 (c) 1998 Visco */
	DRIVER( shocktr2 )	/* 0246 (c) 1998 Saurus */
	DRIVER( lans2004 )	/* bootleg of shocktr2 */
	DRIVER( flipshot )	/* 0247 (c) 1998 Visco */
	DRIVER( pbobbl2n )	/* 0248 (c) 1999 Taito (SNK license) */
	DRIVER( ctomaday )	/* 0249 (c) 1999 Visco */
	DRIVER( mslugx )	/* 0250 (c) 1999 SNK */
	DRIVER( kof99 )		/* 0251 (c) 1999 SNK */
	DRIVER( kof99a )	/* 0251 (c) 1999 SNK */
	DRIVER( kof99e )	/* 0251 (c) 1999 SNK */
	DRIVER( kof99k )	/* 0152 (c) 1999 SNK */
	DRIVER( kof99p )	/* 0251 (c) 1999 SNK */
	DRIVER( ganryu )	/* 0252 (c) 1999 Visco */
	DRIVER( garou )		/* 0253 (c) 1999 SNK */
	DRIVER( garouo )	/* 0253 (c) 1999 SNK */
	DRIVER( garoup )	/* 0253 (c) 1999 SNK */
	DRIVER( garoubl )	/* bootleg */
	DRIVER( s1945p )	/* 0254 (c) 1999 Psikyo */
	DRIVER( preisle2 )	/* 0255 (c) 1999 Yumekobo */
	DRIVER( mslug3 )	/* 0256 (c) 2000 SNK */
	DRIVER( mslug3h )	/* 0256 (c) 2000 SNK */
	DRIVER( mslug3b6 )	/* bootleg */
	DRIVER( kof2000 )	/* 0257 (c) 2000 SNK */
	DRIVER( kof2000n )	/* 0257 (c) 2000 SNK */
				/* 0258 SNK vs. Capcom? */
	DRIVER( bangbead )	/* 0259 (c) 2000 Visco */
	DRIVER( nitd )		/* 0260 (c) 2000 Eleven / Gavaking */
	DRIVER( nitdbl )	/* bootleg */
	DRIVER( sengoku3 )	/* 0261 (c) 2001 Noise Factory / SNK */
	DRIVER( kof2001 )	/* 0262 (c) 2001 Eolith / SNK */
	DRIVER( kof2001h )	/* 0262 (c) 2001 Eolith / SNK */
	DRIVER( cthd2003 )	/* bootleg of kof2001 */
	DRIVER( ct2k3sp )	/* bootleg of kof2001 */
	DRIVER( ct2k3sa )	/* bootleg of kof2001 */
	DRIVER( mslug4 )	/* 0263 (c) 2002 Mega Enterprise */
	DRIVER( mslug4h )	/* 0263 (c) 2002 Mega Enterprise */
	DRIVER( ms4plus )	/* bootleg */
	DRIVER( rotd )		/* 0264 (c) 2002 Evoga */
	DRIVER( kof2002 )	/* 0265 (c) 2002 Eolith / Playmore */
	DRIVER( kof2002b )	/* bootleg */
	DRIVER( kf2k2pls )	/* bootleg */
	DRIVER( kf2k2pla )	/* bootleg */
	DRIVER( kf2k2mp )	/* bootleg */
	DRIVER( kf2k2mp2 )	/* bootleg */
	DRIVER( kof10th )	/* bootleg of kof2002 */
	DRIVER( kf2k5uni )	/* bootleg of kof2002 */
	DRIVER( kf10thep )	/* bootleg of kof2002 */
	DRIVER( kof2k4se )	/* bootleg of kof2002 */
	DRIVER( matrim )	/* 0266 (c) 2002 Atlus */
	DRIVER( matrimbl )	/* bootleg */
	DRIVER( pnyaa )		/* 0267 (c) 2003 Aiky / Taito */
	DRIVER( ms5pcb )	/* 0268 (c) 2003 Playmore */
	DRIVER( mslug5 )	/* 0268 (c) 2003 Playmore */
	DRIVER( mslug5h )	/* 0268 (c) 2003 Playmore */
	DRIVER( ms5plus )	/* bootleg */
	DRIVER( svcpcb )	/* 0269 (c) 2003 Playmore / Capcom - JAMMA PCB */
	DRIVER( svcpcba )	/* 0269 (c) 2003 Playmore / Capcom - JAMMA PCB */
	DRIVER( svc )		/* 0269 (c) 2003 Playmore / Capcom */
	DRIVER( svcboot )	/* bootleg */
	DRIVER( svcplus )	/* bootleg */
	DRIVER( svcplusa )	/* bootleg */
	DRIVER( svcsplus )	/* bootleg */
	DRIVER( samsho5 )	/* 0270 (c) 2003 Playmore */
	DRIVER( samsho5h )	/* 0270 (c) 2003 Playmore */
	DRIVER( samsho5b )	/* bootleg */
	DRIVER( kf2k3pcb )	/* 0271 (c) 2003 Playmore - JAMMA PCB */
	DRIVER( kof2003 )	/* 0271 (c) 2003 Playmore */
	DRIVER( kof2003h )	/* 0271 (c) 2003 Playmore */
	DRIVER( kf2k3bl )	/* bootleg */
	DRIVER( kf2k3bla )	/* bootleg */
	DRIVER( kf2k3pl )	/* bootleg */
	DRIVER( kf2k3upl )	/* bootleg */
	DRIVER( samsh5sp )	/* 0272 (c) 2004 Playmore */
	DRIVER( samsh5sph )	/* 0272 (c) 2004 Playmore */
	DRIVER( samsh5spn )	/* 0272 (c) 2004 Playmore */

	/* The BrezzaSoft games don't have proper ID codes */
	DRIVER( jockeygp )
	DRIVER( vliner )
	DRIVER( vlinero )

	/* Nor does Digger Man */
	DRIVER( diggerma )  /* No Game ID (unlicensed), (c) 2000 Kyle Hodgetts, prototype */

	/* Neo Print */
	DRIVER( 98best44 )

	/* new added */
	DRIVER( kof97hfa )	/* hackrom, based by kof97 */
	DRIVER( ss2hfa )	/* hackrom, based by samsho2 */
	DRIVER( sokhfa )	/* hackrom, based by ragnagrd */
	DRIVER( crswd2bl )	/* Crossed Sword 2, bootleg of NGCD version */
	DRIVER( 3countba )	/* 3 Count Bout / Fire Suplex (NGM-043) */
	DRIVER( fatfury2a )	/* Fatal Fury 2 / Garou Densetsu 2 - arata-naru tatakai (NGM-047) */
	DRIVER( fatfury3a )	/* Fatal Fury 3 - Road to the Final Victory / Garou Densetsu 3 - arata-naru tatakai (NGM-069) (NGH-069) (alternate set) */
	DRIVER( alpham2p )	/* Alpha Mission 2 /ASO 2 (prototype) */
	DRIVER( b2b )		/* Bang Bang Busters (NCI release) */
	DRIVER( bangbedp )	/* Bang Bead (prototype) */
	DRIVER( burningfp )	/* Burning Fight (prototype) */
	DRIVER( jockeygpa )	/* Jockey Grand Prix (set 2) */
	DRIVER( garouh )	/* Garou - Mark of the Wolves (NGM-2530 ~ NGH-2530) */
	DRIVER( ironclad )	/* Choutetsu Brikin'ger - Iron clad (prototype) */


#endif	/* DRIVER_RECURSIVE */
