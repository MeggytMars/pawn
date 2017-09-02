/*  
 *  Version: MPL 1.1
 *  
 *  The contents of this file are subject to the Mozilla Public License Version
 *  1.1 (the "License"); you may not use this file except in compliance with
 *  the License. You may obtain a copy of the License at
 *  [url]http://www.mozilla.org/MPL/[/url]
 *  
 *  Software distributed under the License is distributed on an "AS IS" basis,
 *  WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 *  for the specific language governing rights and limitations under the
 *  License.
 *  
 *  The Original Code is the sscanf 2.0 SA:MP plugin.
 *  
 *  The Initial Developer of the Original Code is Alex "Y_Less" Cole.
 *  Portions created by the Initial Developer are Copyright (C) 2010
 *  the Initial Developer. All Rights Reserved.
 *  
 *  Contributor(s):
 *  
 *  Special Thanks to:
 *  
 *      SA:MP Team past, present and future
 */

#if defined _inc_a_npc
	#pragma library sscanf
#elseif !defined _inc_a_samp
	#error Please include <a_npc> or <a_samp> first.
#endif

#define SSCANF:%0(%1) sscanf_%0(%1);public sscanf_%0(%1)

#if defined sscanf
	#error sscanf (possibly the PAWN version) already defined.
#endif

native sscanf(const data[], const format[], {Float,_}:...);
native unformat(const data[], const format[], {Float,_}:...) = sscanf;
native SSCANF_Init(players, invalid, len);
native SSCANF_Join(playerid, const name[], npc);
native SSCANF_Leave(playerid);

native SSCANF_Option(const name[], value);

stock const
	SSCANF_QUIET[] = "SSCANF_QUIET",
	OLD_DEFAULT_NAME[] = "OLD_DEFAULT_NAME",
	MATCH_NAME_PARTIAL[] = "MATCH_NAME_PARTIAL",
	CELLMIN_ON_MATCHES[] = "CELLMIN_ON_MATCHES",
	OLD_DEFAULT_KUSTOM[] = "OLD_DEFAULT_KUSTOM",
	OLD_DEFAULT_CUSTOM[] = "OLD_DEFAULT_CUSTOM";

static stock
	bool:SSCANF_gInit = false,
	SSCANF_g_sPlayers[MAX_PLAYERS char];

#if defined _inc_a_npc
	forward SSCANF_PlayerCheck();
	
	/*
	  OnNPCModeInit

	  Called when the script starts if it is a NPC mode, sets up the system,
	  then calls the "real" OnNPCModeInit (using the new ALS 2 hook method).
	*/
	
	public OnNPCModeInit()
	{
		SSCANF_Init(MAX_PLAYERS, INVALID_PLAYER_ID, MAX_PLAYER_NAME);
		#if !defined SSCANF_NO_PLAYERS
			// Initialise the system.
			SSCANF_PlayerCheck();
			SetTimer("SSCANF_PlayerCheck", 1, 1);
		#endif
		#if defined SSCANF_OnNPCModeInit
			SSCANF_OnNPCModeInit();
		#endif
		return 1;
	}
	
	#if defined _ALS_OnNPCModeInit
		#undef OnNPCModeInit
	#else
		#define _ALS_OnNPCModeInit
	#endif
	#define OnNPCModeInit SSCANF_OnNPCModeInit
	#if defined SSCANF_OnNPCModeInit
		forward SSCANF_OnNPCModeInit();
	#endif
	
	/*
	  SSCANF_PlayerCheck

	  NPC modes have no "OnPlayerConnect callback, so we need to simulate one.
	*/
	
	#if !defined SSCANF_NO_PLAYERS
		public SSCANF_PlayerCheck()
		{
			for (new i = 0; i != MAX_PLAYERS; ++i)
			{
				if (IsPlayerConnected(i))
				{
					if (!SSCANF_g_sPlayers{i})
					{
						new
							name[MAX_PLAYER_NAME];
						GetPlayerName(i, name, sizeof (name));
						// We have no way to know if they are an NPC or not!
						SSCANF_Join(i, name, 0);
						SSCANF_g_sPlayers{i} = 1;
					}
				}
				else
				{
					if (SSCANF_g_sPlayers{i})
					{
						SSCANF_Leave(i);
						SSCANF_g_sPlayers{i} = 0;
					}
				}
			}
		}
	#endif
#else
	/*
	  OnFilterScriptInit

	  Called when the script starts if it is a filterscript, sets up the system,
	  then calls the "real" OnFilterScriptInit (using the new ALS 2 hook
	  method).
	*/
	
	public OnFilterScriptInit()
	{
		SSCANF_Init(GetMaxPlayers(), INVALID_PLAYER_ID, MAX_PLAYER_NAME);
		SSCANF_gInit = true;
		#if defined SSCANF_OnFilterScriptInit
			SSCANF_OnFilterScriptInit();
		#endif
		return 1;
	}
	
	#if defined _ALS_OnFilterScriptInit
		#undef OnFilterScriptInit
	#else
		#define _ALS_OnFilterScriptInit
	#endif
	#define OnFilterScriptInit SSCANF_OnFilterScriptInit
	#if defined SSCANF_OnFilterScriptInit
		forward SSCANF_OnFilterScriptInit();
	#endif
	
	/*
	  OnGameModeInit

	  Called when the script starts if it is a gamemode.  This callback is also
	  called in filterscripts so we don't want to reinitialise the system in
	  that case.
	*/
	
	public OnGameModeInit()
	{
		if (!SSCANF_gInit)
		{
			SSCANF_Init(GetMaxPlayers(), INVALID_PLAYER_ID, MAX_PLAYER_NAME);
			SSCANF_gInit = true;
		}
		#if defined SSCANF_OnGameModeInit
			SSCANF_OnGameModeInit();
		#endif
		return 1;
	}
	
	#if defined _ALS_OnGameModeInit
		#undef OnGameModeInit
	#else
		#define _ALS_OnGameModeInit
	#endif
	#define OnGameModeInit SSCANF_OnGameModeInit
	#if defined SSCANF_OnGameModeInit
		forward SSCANF_OnGameModeInit();
	#endif
	
	/*
	  OnPlayerConnect

	  Called when a player connects.  Actually increments an internal count so
	  that if a script ends and "OnPlayerDisconnect" is called then "sscanf"
	  still knows that the player is really connected.  Also stores their name
	  internally.
	*/
	
	public OnPlayerConnect(playerid)
	{
		new
			name[MAX_PLAYER_NAME];
		GetPlayerName(playerid, name, sizeof (name));
		SSCANF_Join(playerid, name, IsPlayerNPC(playerid));
		#if defined SSCANF_OnPlayerConnect
			SSCANF_OnPlayerConnect(playerid);
		#endif
		return 1;
	}
	
	#if defined _ALS_OnPlayerConnect
		#undef OnPlayerConnect
	#else
		#define _ALS_OnPlayerConnect
	#endif
	#define OnPlayerConnect SSCANF_OnPlayerConnect
	#if defined SSCANF_OnPlayerConnect
		forward SSCANF_OnPlayerConnect(playerid);
	#endif
	
	/*
	  OnPlayerDisconnect

	  Called when a player disconnects, or when a script is ended.
	*/
	
	public OnPlayerDisconnect(playerid, reason)
	{
		#if defined SSCANF_OnPlayerDisconnect
			SSCANF_OnPlayerDisconnect(playerid, reason);
		#endif
		SSCANF_Leave(playerid);
		return 1;
	}
	
	#if defined _ALS_OnPlayerDisconnect
		#undef OnPlayerDisconnect
	#else
		#define _ALS_OnPlayerDisconnect
	#endif
	#define OnPlayerDisconnect SSCANF_OnPlayerDisconnect
	#if defined SSCANF_OnPlayerDisconnect
		forward SSCANF_OnPlayerDisconnect(playerid, reason);
	#endif
#endif

#define SSCANF_Init
#define SSCANF_Join
#define SSCANF_Leave

#define extract%0->%1; EXTRN%1;unformat(_:EXTRZ:EXTRV:EXTRX:%0,##,%1,,);
#define unformat(_:EXTRZ:EXTRV:EXTRX:%0,##,%1);%2else if (unformat(_:EXTRV:EXTRX:%0,##,%1))

#define EXTRV:EXTRX:%0<%3>##,%9new%1,%2) EXTRY:%0##P<%3>,|||%1|||%2)
#define EXTRX:%0##,%9new%1,%2) EXTRY:%0##,|||%1|||%2)
#define EXTRY: EXTR8:EXTR9:EXTR0:EXTR1:EXTR2:EXTR3:EXTR4:

#define EXTR8:EXTR9:EXTR0:EXTR1:EXTR2:EXTR3:EXTR4:%0##%1,%2|||%6:%3=%9|||%4) %6_EXTRO:%0##%1,%2|||%3=%9|||%4)
#define EXTR9:EXTR0:EXTR1:EXTR2:EXTR3:EXTR4:%0##%1,%2|||%3=%9|||%4) __EXTRO:%0##%1,%2|||%3=%9|||%4)
#define EXTR0:EXTR1:EXTR2:EXTR3:EXTR4:%0##%1,%2|||%6:%3[%7]|||%4) %6_EXTRW:%0##%1,%2|||%3[%7]|||%4)
#define EXTR1:EXTR2:EXTR3:EXTR4:%0##%1,%2|||%3[%7]|||%4) __EXTRW:%0##%1,%2|||%3|||%4)
#define EXTR2:EXTR3:EXTR4:%0##%1,%2|||%6:%3|||%4) %6_EXTRN:%0##%1,%2|||%3|||%4)
#define EXTR3:EXTR4:%0##%1,,%2||||||%4) %0##%1,%2)
#define EXTR4:%0##%1,%2|||%3|||%4) __EXTRN:%0##%1,%2|||%3|||%4)

// Optional specifiers.
#define __EXTRO:%0##%1,%2|||%3=%9|||%4,%5) EXTRY:%0##%1I"("#%9")"#,%2,%3|||%4|||%5)
#define Float_EXTRO:%0##%1,%2|||%3=%9|||%4,%5) EXTRY:%0##%1F"("#%9")"#,%2,%3|||%4|||%5)
#define player_EXTRO:%0##%1,%2|||%3=%9|||%4,%5) EXTRY:%0##%1U"("#%9")"#,%2,%3|||%4|||%5)
#define string_EXTRO:%0##%1,%2|||%3[%7]=%9|||%4,%5) EXTRY:%0##%1S"("#%9")"#[%7],%2,%3|||%4|||%5)

// Normal specifiers (the double underscore is to work for "_:".
#define __EXTRN:%0##%1,%2|||%3|||%4,%5) EXTRY:%0##%1i,%2,%3|||%4|||%5)
#define Float_EXTRN:%0##%1,%2|||%3|||%4,%5) EXTRY:%0##%1f,%2,%3|||%4|||%5)
#define player_EXTRN:%0##%1,%2|||%3|||%4,%5) EXTRY:%0##%1u,%2,%3|||%4|||%5)
//#define string_EXTRW:%0##%1,%2|||%3[%7]|||%4,%5) EXTRY:%0##%1s[%7],%2,%3|||%4|||%5)

// Array versions of normal specifiers.
#define __EXTRW:%0##%1,%2|||%3[%7]|||%4,%5) EXTRY:%0##%1a<i>[%7],%2,%3|||%4|||%5)
#define Float_EXTRW:%0##%1,%2|||%3[%7]|||%4,%5) EXTRY:%0##%1a<f>[%7],%2,%3|||%4|||%5)
#define player_EXTRW:%0##%1,%2|||%3[%7]|||%4,%5) EXTRY:%0##%1a<u>[%7],%2,%3|||%4|||%5)
#define string_EXTRW:%0##%1,%2|||%3[%7]|||%4,%5) EXTRY:%0##%1s[%7],%2,%3|||%4|||%5)

// Get rid of excess leading space which causes warnings.
#define EXTRN%0new%1; new%1;

#if !defined stringg
    #define stringg:
#endif

#define player:

#define hex:
#define hex_EXTRO:%0##%1,%2|||%3=%9|||%4,%5) EXTRY:%0##%1H"("#%9")"#,%2,%3|||%4|||%5)
#define hex_EXTRN:%0##%1,%2|||%3|||%4,%5) EXTRY:%0##%1h,%2,%3|||%4|||%5)
#define hex_EXTRW:%0##%1,%2|||%3[%7]|||%4,%5) EXTRY:%0##%1a<h>[%7],%2,%3|||%4|||%5)

#define bin:
#define bin_EXTRO:%0##%1,%2|||%3=%9|||%4,%5) EXTRY:%0##%1B"("#%9")"#,%2,%3|||%4|||%5)
#define bin_EXTRN:%0##%1,%2|||%3|||%4,%5) EXTRY:%0##%1b,%2,%3|||%4|||%5)
#define bin_EXTRW:%0##%1,%2|||%3[%7]|||%4,%5) EXTRY:%0##%1a<b>[%7],%2,%3|||%4|||%5)

#define kustom:%0<%1> %0
#define kustom_EXTRO:%0##%1,%2|||%3<%8>=%9|||%4,%5) EXTRY:%0##%1K<%8>"("#%9")"#,%2,%3|||%4|||%5)
#define kustom_EXTRN:%0##%1,%2|||%3<%8>|||%4,%5) EXTRY:%0##%1k<%8>,%2,%3|||%4|||%5)
//#define bin_EXTRW:%0##%1,%2|||%3[%7]|||%4,%5) EXTRY:%0##%1a<b>[%7],%2,%3|||%4|||%5)

SSCANF:weapon(stringg[])
{
	// This function is VERY basic, needs VASTLY improving to detect variations.
	if ('0' <= stringg[0] <= '9')
	{
		new
			ret = strval(stringg);
		if (0 <= ret <= 18 || 22 <= ret <= 46)
		{
			return ret;
		}
	}
	else if (!strcmp(stringg, "Unarmed")) return 0;
	else if (!strcmp(stringg, "Brass Knuckles")) return 1;
	else if (!strcmp(stringg, "Golf Club")) return 2;
	else if (!strcmp(stringg, "Night Stick")) return 3;
	else if (!strcmp(stringg, "Knife")) return 4;
	else if (!strcmp(stringg, "Baseball Bat")) return 5;
	else if (!strcmp(stringg, "Shovel")) return 6;
	else if (!strcmp(stringg, "Pool cue")) return 7;
	else if (!strcmp(stringg, "Katana")) return 8;
	else if (!strcmp(stringg, "Chainsaw")) return 9;
	else if (!strcmp(stringg, "Purple Dildo")) return 10;
	else if (!strcmp(stringg, "White Dildo")) return 11;
	else if (!strcmp(stringg, "Long White Dildo")) return 12;
	else if (!strcmp(stringg, "White Dildo 2")) return 13;
	else if (!strcmp(stringg, "Flowers")) return 14;
	else if (!strcmp(stringg, "Cane")) return 15;
	else if (!strcmp(stringg, "Grenades")) return 16;
	else if (!strcmp(stringg, "Tear Gas")) return 17;
	else if (!strcmp(stringg, "Molotovs")) return 18;
	else if (!strcmp(stringg, "Pistol")) return 22;
	else if (!strcmp(stringg, "Silenced Pistol")) return 23;
	else if (!strcmp(stringg, "Desert Eagle")) return 24;
	else if (!strcmp(stringg, "Shotgun")) return 25;
	else if (!strcmp(stringg, "Sawn Off Shotgun")) return 26;
	else if (!strcmp(stringg, "Combat Shotgun")) return 27;
	else if (!strcmp(stringg, "Micro Uzi")) return 28;
	else if (!strcmp(stringg, "Mac 10")) return 28;
	else if (!strcmp(stringg, "MP5")) return 29;
	else if (!strcmp(stringg, "AK47")) return 30;
	else if (!strcmp(stringg, "M4")) return 31;
	else if (!strcmp(stringg, "Tec9")) return 32;
	else if (!strcmp(stringg, "Rifle")) return 33;
	else if (!strcmp(stringg, "Sniper Rifle")) return 34;
	else if (!strcmp(stringg, "RPG")) return 35;
	else if (!strcmp(stringg, "Missile Launcher")) return 36;
	else if (!strcmp(stringg, "Flame Thrower")) return 37;
	else if (!strcmp(stringg, "Minigun")) return 38;
	else if (!strcmp(stringg, "Sachel Charges")) return 39;
	else if (!strcmp(stringg, "Detonator")) return 40;
	else if (!strcmp(stringg, "Spray Paint")) return 41;
	else if (!strcmp(stringg, "Fire Extinguisher")) return 42;
	else if (!strcmp(stringg, "Camera")) return 43;
	else if (!strcmp(stringg, "Nightvision Goggles")) return 44;
	else if (!strcmp(stringg, "Thermal Goggles")) return 45;
	else if (!strcmp(stringg, "Parachute")) return 46;
	return -1;
}

SSCANF:vehicle(stringg[])
{
	// This function is VERY basic, needs VASTLY improving to detect variations.
	if ('0' <= stringg[0] <= '9')
	{
		new
			ret = strval(stringg);
		if (400 <= ret <= 611)
		{
			return ret;
		}
	}
	else if (!strcmp(stringg, "Landstalker")) return 400;
	else if (!strcmp(stringg, "Bravura")) return 401;
	else if (!strcmp(stringg, "Buffalo")) return 402;
	else if (!strcmp(stringg, "Linerunner")) return 403;
	else if (!strcmp(stringg, "Perenniel")) return 404;
	else if (!strcmp(stringg, "Sentinel")) return 405;
	else if (!strcmp(stringg, "Dumper")) return 406;
	else if (!strcmp(stringg, "Firetruck")) return 407;
	else if (!strcmp(stringg, "Trashmaster")) return 408;
	else if (!strcmp(stringg, "Stretch")) return 409;
	else if (!strcmp(stringg, "Manana")) return 410;
	else if (!strcmp(stringg, "Infernus")) return 411;
	else if (!strcmp(stringg, "Voodoo")) return 412;
	else if (!strcmp(stringg, "Pony")) return 413;
	else if (!strcmp(stringg, "Mule")) return 414;
	else if (!strcmp(stringg, "Cheetah")) return 415;
	else if (!strcmp(stringg, "Ambulance")) return 416;
	else if (!strcmp(stringg, "Leviathan")) return 417;
	else if (!strcmp(stringg, "Moonbeam")) return 418;
	else if (!strcmp(stringg, "Esperanto")) return 419;
	else if (!strcmp(stringg, "Taxi")) return 420;
	else if (!strcmp(stringg, "Washington")) return 421;
	else if (!strcmp(stringg, "Bobcat")) return 422;
	else if (!strcmp(stringg, "Mr Whoopee")) return 423;
	else if (!strcmp(stringg, "BF Injection")) return 424;
	else if (!strcmp(stringg, "Hunter")) return 425;
	else if (!strcmp(stringg, "Premier")) return 426;
	else if (!strcmp(stringg, "Enforcer")) return 427;
	else if (!strcmp(stringg, "Securicar")) return 428;
	else if (!strcmp(stringg, "Banshee")) return 429;
	else if (!strcmp(stringg, "Predator")) return 430;
	else if (!strcmp(stringg, "Bus")) return 431;
	else if (!strcmp(stringg, "Rhino")) return 432;
	else if (!strcmp(stringg, "Barracks")) return 433;
	else if (!strcmp(stringg, "Hotknife")) return 434;
	else if (!strcmp(stringg, "Article Trailer")) return 435;
	else if (!strcmp(stringg, "Previon")) return 436;
	else if (!strcmp(stringg, "Coach")) return 437;
	else if (!strcmp(stringg, "Cabbie")) return 438;
	else if (!strcmp(stringg, "Stallion")) return 439;
	else if (!strcmp(stringg, "Rumpo")) return 440;
	else if (!strcmp(stringg, "RC Bandit")) return 441;
	else if (!strcmp(stringg, "Romero")) return 442;
	else if (!strcmp(stringg, "Packer")) return 443;
	else if (!strcmp(stringg, "Monster")) return 444;
	else if (!strcmp(stringg, "Admiral")) return 445;
	else if (!strcmp(stringg, "Squallo")) return 446;
	else if (!strcmp(stringg, "Seasparrow")) return 447;
	else if (!strcmp(stringg, "Pizzaboy")) return 448;
	else if (!strcmp(stringg, "Tram")) return 449;
	else if (!strcmp(stringg, "Article Trailer 2")) return 450;
	else if (!strcmp(stringg, "Turismo")) return 451;
	else if (!strcmp(stringg, "Speeder")) return 452;
	else if (!strcmp(stringg, "Reefer")) return 453;
	else if (!strcmp(stringg, "Tropic")) return 454;
	else if (!strcmp(stringg, "Flatbed")) return 455;
	else if (!strcmp(stringg, "Yankee")) return 456;
	else if (!strcmp(stringg, "Caddy")) return 457;
	else if (!strcmp(stringg, "Solair")) return 458;
	else if (!strcmp(stringg, "Berkley's RC Van")) return 459;
	else if (!strcmp(stringg, "Skimmer")) return 460;
	else if (!strcmp(stringg, "PCJ-600")) return 461;
	else if (!strcmp(stringg, "Faggio")) return 462;
	else if (!strcmp(stringg, "Freeway")) return 463;
	else if (!strcmp(stringg, "RC Baron")) return 464;
	else if (!strcmp(stringg, "RC Raider")) return 465;
	else if (!strcmp(stringg, "Glendale")) return 466;
	else if (!strcmp(stringg, "Oceanic")) return 467;
	else if (!strcmp(stringg, "Sanchez")) return 468;
	else if (!strcmp(stringg, "Sparrow")) return 469;
	else if (!strcmp(stringg, "Patriot")) return 470;
	else if (!strcmp(stringg, "Quad")) return 471;
	else if (!strcmp(stringg, "Coastguard")) return 472;
	else if (!strcmp(stringg, "Dinghy")) return 473;
	else if (!strcmp(stringg, "Hermes")) return 474;
	else if (!strcmp(stringg, "Sabre")) return 475;
	else if (!strcmp(stringg, "Rustler")) return 476;
	else if (!strcmp(stringg, "ZR-350")) return 477;
	else if (!strcmp(stringg, "Walton")) return 478;
	else if (!strcmp(stringg, "Regina")) return 479;
	else if (!strcmp(stringg, "Comet")) return 480;
	else if (!strcmp(stringg, "BMX")) return 481;
	else if (!strcmp(stringg, "Burrito")) return 482;
	else if (!strcmp(stringg, "Camper")) return 483;
	else if (!strcmp(stringg, "Marquis")) return 484;
	else if (!strcmp(stringg, "Baggage")) return 485;
	else if (!strcmp(stringg, "Dozer")) return 486;
	else if (!strcmp(stringg, "Maverick")) return 487;
	else if (!strcmp(stringg, "SAN News Maverick")) return 488;
	else if (!strcmp(stringg, "Rancher")) return 489;
	else if (!strcmp(stringg, "FBI Rancher")) return 490;
	else if (!strcmp(stringg, "Virgo")) return 491;
	else if (!strcmp(stringg, "Greenwood")) return 492;
	else if (!strcmp(stringg, "Jetmax")) return 493;
	else if (!strcmp(stringg, "Hotring Racer")) return 494;
	else if (!strcmp(stringg, "Sandking")) return 495;
	else if (!strcmp(stringg, "Blista Compact")) return 496;
	else if (!strcmp(stringg, "Police Maverick")) return 497;
	else if (!strcmp(stringg, "Boxville")) return 498;
	else if (!strcmp(stringg, "Benson")) return 499;
	else if (!strcmp(stringg, "Mesa")) return 500;
	else if (!strcmp(stringg, "RC Goblin")) return 501;
	else if (!strcmp(stringg, "Hotring Racer")) return 502;
	else if (!strcmp(stringg, "Hotring Racer")) return 503;
	else if (!strcmp(stringg, "Bloodring Banger")) return 504;
	else if (!strcmp(stringg, "Rancher")) return 505;
	else if (!strcmp(stringg, "Super GT")) return 506;
	else if (!strcmp(stringg, "Elegant")) return 507;
	else if (!strcmp(stringg, "Journey")) return 508;
	else if (!strcmp(stringg, "Bike")) return 509;
	else if (!strcmp(stringg, "Mountain Bike")) return 510;
	else if (!strcmp(stringg, "Beagle")) return 511;
	else if (!strcmp(stringg, "Cropduster")) return 512;
	else if (!strcmp(stringg, "Stuntplane")) return 513;
	else if (!strcmp(stringg, "Tanker")) return 514;
	else if (!strcmp(stringg, "Roadtrain")) return 515;
	else if (!strcmp(stringg, "Nebula")) return 516;
	else if (!strcmp(stringg, "Majestic")) return 517;
	else if (!strcmp(stringg, "Buccaneer")) return 518;
	else if (!strcmp(stringg, "Shamal")) return 519;
	else if (!strcmp(stringg, "Hydra")) return 520;
	else if (!strcmp(stringg, "FCR-900")) return 521;
	else if (!strcmp(stringg, "NRG-500")) return 522;
	else if (!strcmp(stringg, "HPV1000")) return 523;
	else if (!strcmp(stringg, "Cement Truck")) return 524;
	else if (!strcmp(stringg, "Towtruck")) return 525;
	else if (!strcmp(stringg, "Fortune")) return 526;
	else if (!strcmp(stringg, "Cadrona")) return 527;
	else if (!strcmp(stringg, "FBI Truck")) return 528;
	else if (!strcmp(stringg, "Willard")) return 529;
	else if (!strcmp(stringg, "Forklift")) return 530;
	else if (!strcmp(stringg, "Tractor")) return 531;
	else if (!strcmp(stringg, "Combine Harvester")) return 532;
	else if (!strcmp(stringg, "Feltzer")) return 533;
	else if (!strcmp(stringg, "Remington")) return 534;
	else if (!strcmp(stringg, "Slamvan")) return 535;
	else if (!strcmp(stringg, "Blade")) return 536;
	else if (!strcmp(stringg, "Freight (Train)")) return 537;
	else if (!strcmp(stringg, "Brownstreak (Train)")) return 538;
	else if (!strcmp(stringg, "Vortex")) return 539;
	else if (!strcmp(stringg, "Vincent")) return 540;
	else if (!strcmp(stringg, "Bullet")) return 541;
	else if (!strcmp(stringg, "Clover")) return 542;
	else if (!strcmp(stringg, "Sadler")) return 543;
	else if (!strcmp(stringg, "Firetruck LA")) return 544;
	else if (!strcmp(stringg, "Hustler")) return 545;
	else if (!strcmp(stringg, "Intruder")) return 546;
	else if (!strcmp(stringg, "Primo")) return 547;
	else if (!strcmp(stringg, "Cargobob")) return 548;
	else if (!strcmp(stringg, "Tampa")) return 549;
	else if (!strcmp(stringg, "Sunrise")) return 550;
	else if (!strcmp(stringg, "Merit")) return 551;
	else if (!strcmp(stringg, "Utility Van")) return 552;
	else if (!strcmp(stringg, "Nevada")) return 553;
	else if (!strcmp(stringg, "Yosemite")) return 554;
	else if (!strcmp(stringg, "Windsor")) return 555;
	else if (!strcmp(stringg, "Monster \"A\"")) return 556;
	else if (!strcmp(stringg, "Monster \"B\"")) return 557;
	else if (!strcmp(stringg, "Uranus")) return 558;
	else if (!strcmp(stringg, "Jester")) return 559;
	else if (!strcmp(stringg, "Sultan")) return 560;
	else if (!strcmp(stringg, "Stratum")) return 561;
	else if (!strcmp(stringg, "Elegy")) return 562;
	else if (!strcmp(stringg, "Raindance")) return 563;
	else if (!strcmp(stringg, "RC Tiger")) return 564;
	else if (!strcmp(stringg, "Flash")) return 565;
	else if (!strcmp(stringg, "Tahoma")) return 566;
	else if (!strcmp(stringg, "Savanna")) return 567;
	else if (!strcmp(stringg, "Bandito")) return 568;
	else if (!strcmp(stringg, "Freight Flat Trailer (Train)")) return 569;
	else if (!strcmp(stringg, "Streak Trailer (Train)")) return 570;
	else if (!strcmp(stringg, "Kart")) return 571;
	else if (!strcmp(stringg, "Mower")) return 572;
	else if (!strcmp(stringg, "Dune")) return 573;
	else if (!strcmp(stringg, "Sweeper")) return 574;
	else if (!strcmp(stringg, "Broadway")) return 575;
	else if (!strcmp(stringg, "Tornado")) return 576;
	else if (!strcmp(stringg, "AT400")) return 577;
	else if (!strcmp(stringg, "DFT-30")) return 578;
	else if (!strcmp(stringg, "Huntley")) return 579;
	else if (!strcmp(stringg, "Stafford")) return 580;
	else if (!strcmp(stringg, "BF-400")) return 581;
	else if (!strcmp(stringg, "Newsvan")) return 582;
	else if (!strcmp(stringg, "Tug")) return 583;
	else if (!strcmp(stringg, "Petrol Trailer")) return 584;
	else if (!strcmp(stringg, "Emperor")) return 585;
	else if (!strcmp(stringg, "Wayfarer")) return 586;
	else if (!strcmp(stringg, "Euros")) return 587;
	else if (!strcmp(stringg, "Hotdog")) return 588;
	else if (!strcmp(stringg, "Club")) return 589;
	else if (!strcmp(stringg, "Freight Box Trailer (Train)")) return 590;
	else if (!strcmp(stringg, "Article Trailer 3")) return 591;
	else if (!strcmp(stringg, "Andromada")) return 592;
	else if (!strcmp(stringg, "Dodo")) return 593;
	else if (!strcmp(stringg, "RC Cam")) return 594;
	else if (!strcmp(stringg, "Launch")) return 595;
	else if (!strcmp(stringg, "Police Car (LSPD)")) return 596;
	else if (!strcmp(stringg, "Police Car (SFPD)")) return 597;
	else if (!strcmp(stringg, "Police Car (LVPD)")) return 598;
	else if (!strcmp(stringg, "Police Ranger")) return 599;
	else if (!strcmp(stringg, "Picador")) return 600;
	else if (!strcmp(stringg, "S.W.A.T.")) return 601;
	else if (!strcmp(stringg, "Alpha")) return 602;
	else if (!strcmp(stringg, "Phoenix")) return 603;
	else if (!strcmp(stringg, "Glendale Shit")) return 604;
	else if (!strcmp(stringg, "Sadler Shit")) return 605;
	else if (!strcmp(stringg, "Baggage Trailer \"A\"")) return 606;
	else if (!strcmp(stringg, "Baggage Trailer \"B\"")) return 607;
	else if (!strcmp(stringg, "Tug Stairs Trailer")) return 608;
	else if (!strcmp(stringg, "Boxville")) return 609;
	else if (!strcmp(stringg, "Farm Trailer")) return 610;
	else if (!strcmp(stringg, "Utility Trailer")) return 611;
	return -1;
}

// Fix the compiler crash when both the PAWN and Plugin versions of sscanf are
// found by renaming the old version at declaration.  (fixes.inc compatible
// naming scheme: "BAD_Function()").
#define sscanf(%0:...) BAD_sscanf(%0:...)
