/*
===========================================================================
Copyright (C) 2000 - 2013, Raven Software, Inc.
Copyright (C) 2001 - 2013, Activision, Inc.
Copyright (C) 2013 - 2015, OpenJK contributors

This file is part of the OpenJK source code.

OpenJK is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License version 2 as
published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, see <http://www.gnu.org/licenses/>.
===========================================================================
*/

//g_itemLoad.cpp
//reads in ext_data\items.dat to bg_itemlist[]

#include "g_local.h"
#include "g_items.h"
#include <cgame\cg_local.h>

constexpr auto PICKUPSOUND = "sound/weapons/w_pkup.wav";

extern gitem_t bg_itemlist[];

struct itemParms_s
{
	int itemNum;
} itemParms;

static void IT_ClassName(const char** hold_buf);
static void IT_Count(const char** hold_buf);
static void IT_Icon(const char** hold_buf);
static void IT_Min(const char** hold_buf);
static void IT_Max(const char** hold_buf);
static void IT_Name(const char** hold_buf);
static void IT_PickupSound(const char** hold_buf);
static void IT_Tag(const char** hold_buf);
static void IT_Type(const char** hold_buf);
static void IT_WorldModel(const char** hold_buf);

using itemParms_t = struct
{
	const char* parmName;
	void (*func)(const char** hold_buf);
};

constexpr auto IT_PARM_MAX = 10;

itemParms_t ItemParms[IT_PARM_MAX] =
{
	{"itemname", IT_Name},
	{"classname", IT_ClassName},
	{"count", IT_Count},
	{"icon", IT_Icon},
	{"min", IT_Min},
	{"max", IT_Max},
	{"pickupsound", IT_PickupSound},
	{"tag", IT_Tag},
	{"type", IT_Type},
	{"worldmodel", IT_WorldModel},
};

static void IT_SetDefaults()
{
	bg_itemlist[itemParms.itemNum].mins[0] = -16;
	bg_itemlist[itemParms.itemNum].mins[1] = -16;
	bg_itemlist[itemParms.itemNum].mins[2] = -2;

	bg_itemlist[itemParms.itemNum].maxs[0] = 16;
	bg_itemlist[itemParms.itemNum].maxs[1] = 16;
	bg_itemlist[itemParms.itemNum].maxs[2] = 16;

	bg_itemlist[itemParms.itemNum].pickup_sound = PICKUPSOUND; //give it a default sound
	bg_itemlist[itemParms.itemNum].precaches = nullptr;
	bg_itemlist[itemParms.itemNum].sounds = nullptr;
}

static void IT_Name(const char** hold_buf)
{
	int itemNum;
	const char* token_str;

	if (COM_ParseString(hold_buf, &token_str))
	{
		return;
	}

	if (!Q_stricmp(token_str, "ITM_NONE"))
		itemNum = ITM_NONE;
	else if (!Q_stricmp(token_str, "ITM_STUN_BATON_PICKUP"))
		itemNum = ITM_STUN_BATON_PICKUP;
	else if (!Q_stricmp(token_str, "ITM_SABER_PICKUP"))
		itemNum = ITM_SABER_PICKUP;
	else if (!Q_stricmp(token_str, "ITM_BRYAR_PISTOL_PICKUP"))
		itemNum = ITM_BRYAR_PISTOL_PICKUP;
	else if (!Q_stricmp(token_str, "ITM_BLASTER_PICKUP"))
		itemNum = ITM_BLASTER_PICKUP;
	else if (!Q_stricmp(token_str, "ITM_DISRUPTOR_PICKUP"))
		itemNum = ITM_DISRUPTOR_PICKUP;
	else if (!Q_stricmp(token_str, "ITM_BOWCASTER_PICKUP"))
		itemNum = ITM_BOWCASTER_PICKUP;
	else if (!Q_stricmp(token_str, "ITM_REPEATER_PICKUP"))
		itemNum = ITM_REPEATER_PICKUP;
	else if (!Q_stricmp(token_str, "ITM_DEMP2_PICKUP"))
		itemNum = ITM_DEMP2_PICKUP;
	else if (!Q_stricmp(token_str, "ITM_FLECHETTE_PICKUP"))
		itemNum = ITM_FLECHETTE_PICKUP;
	else if (!Q_stricmp(token_str, "ITM_ROCKET_LAUNCHER_PICKUP"))
		itemNum = ITM_ROCKET_LAUNCHER_PICKUP;
	else if (!Q_stricmp(token_str, "ITM_THERMAL_DET_PICKUP"))
		itemNum = ITM_THERMAL_DET_PICKUP;
	else if (!Q_stricmp(token_str, "ITM_TRIP_MINE_PICKUP"))
		itemNum = ITM_TRIP_MINE_PICKUP;
	else if (!Q_stricmp(token_str, "ITM_DET_PACK_PICKUP"))
		itemNum = ITM_DET_PACK_PICKUP;
	else if (!Q_stricmp(token_str, "ITM_BOT_LASER_PICKUP"))
		itemNum = ITM_BOT_LASER_PICKUP;
	else if (!Q_stricmp(token_str, "ITM_EMPLACED_GUN_PICKUP"))
		itemNum = ITM_EMPLACED_GUN_PICKUP;
	else if (!Q_stricmp(token_str, "ITM_DROIDEKA_PICKUP"))
		itemNum = ITM_DROIDEKA_PICKUP;
	else if (!Q_stricmp(token_str, "ITM_TURRET_PICKUP"))
		itemNum = ITM_TURRET_PICKUP;
	else if (!Q_stricmp(token_str, "ITM_MELEE"))
		itemNum = ITM_MELEE;
	else if (!Q_stricmp(token_str, "ITM_ATST_MAIN_PICKUP"))
		itemNum = ITM_ATST_MAIN_PICKUP;
	else if (!Q_stricmp(token_str, "ITM_ATST_SIDE_PICKUP"))
		itemNum = ITM_ATST_SIDE_PICKUP;
	else if (!Q_stricmp(token_str, "ITM_TIE_FIGHTER_PICKUP"))
		itemNum = ITM_TIE_FIGHTER_PICKUP;
	else if (!Q_stricmp(token_str, "ITM_RAPID_FIRE_CONC_PICKUP"))
		itemNum = ITM_RAPID_FIRE_CONC_PICKUP;
	else if (!Q_stricmp(token_str, "ITM_JAWA_PICKUP"))
		itemNum = ITM_JAWA_PICKUP;
	else if (!Q_stricmp(token_str, "ITM_TUSKEN_RIFLE_PICKUP"))
		itemNum = ITM_TUSKEN_RIFLE_PICKUP;
	else if (!Q_stricmp(token_str, "ITM_TUSKEN_STAFF_PICKUP"))
		itemNum = ITM_TUSKEN_STAFF_PICKUP;
	else if (!Q_stricmp(token_str, "ITM_SCEPTER_PICKUP"))
		itemNum = ITM_SCEPTER_PICKUP;
	else if (!Q_stricmp(token_str, "ITM_NOGHRI_STICK_PICKUP"))
		itemNum = ITM_NOGHRI_STICK_PICKUP;
	else if (!Q_stricmp(token_str, "ITM_BATTLEDROID_PICKUP"))
		itemNum = ITM_BATTLEDROID_PICKUP;
	else if (!Q_stricmp(token_str, "ITM_THEFIRSTORDER_PICKUP"))
		itemNum = ITM_THEFIRSTORDER_PICKUP;
	else if (!Q_stricmp(token_str, "ITM_CLONECARBINE_PICKUP"))
		itemNum = ITM_CLONECARBINE_PICKUP;
	else if (!Q_stricmp(token_str, "ITM_REBELBLASTER_PICKUP"))
		itemNum = ITM_REBELBLASTER_PICKUP;
	else if (!Q_stricmp(token_str, "ITM_CLONERIFLE_PICKUP"))
		itemNum = ITM_CLONERIFLE_PICKUP;
	else if (!Q_stricmp(token_str, "ITM_CLONECOMMANDO_PICKUP"))
		itemNum = ITM_CLONECOMMANDO_PICKUP;
	else if (!Q_stricmp(token_str, "ITM_REBELRIFLE_PICKUP"))
		itemNum = ITM_REBELRIFLE_PICKUP;
	else if (!Q_stricmp(token_str, "ITM_REY_PICKUP"))
		itemNum = ITM_REY_PICKUP;
	else if (!Q_stricmp(token_str, "ITM_JANGO_PICKUP"))
		itemNum = ITM_JANGO_PICKUP;
	else if (!Q_stricmp(token_str, "ITM_BOBA_PICKUP"))
		itemNum = ITM_BOBA_PICKUP;
	else if (!Q_stricmp(token_str, "ITM_CLONEPISTOL_PICKUP"))
		itemNum = ITM_CLONEPISTOL_PICKUP;
	else if (!Q_stricmp(token_str, "ITM_ROTARY_CANNON_PICKUP"))
		itemNum = ITM_ROTARY_CANNON_PICKUP;
	else if (!Q_stricmp(token_str, "ITM_SBD_BLASTER_PICKUP"))
		itemNum = ITM_SBD_BLASTER_PICKUP;
	else if (!Q_stricmp(token_str, "ITM_DUALPISTOL_PICKUP"))
		itemNum = ITM_DUALPISTOL_PICKUP;
	else if (!Q_stricmp(token_str, "ITM_DUALCLONEPISTOLPISTOL_PICKUP"))
		itemNum = ITM_DUALCLONEPISTOLPISTOL_PICKUP;
	else if (!Q_stricmp(token_str, "ITM_WRIST_BLASTER_PICKUP"))
		itemNum = ITM_WRIST_BLASTER_PICKUP;

	//ammo
	else if (!Q_stricmp(token_str, "ITM_AMMO_FORCE_PICKUP"))
		itemNum = ITM_AMMO_FORCE_PICKUP;
	else if (!Q_stricmp(token_str, "ITM_AMMO_BLASTER_PICKUP"))
		itemNum = ITM_AMMO_BLASTER_PICKUP;
	else if (!Q_stricmp(token_str, "ITM_AMMO_POWERCELL_PICKUP"))
		itemNum = ITM_AMMO_POWERCELL_PICKUP;
	else if (!Q_stricmp(token_str, "ITM_AMMO_METAL_BOLTS_PICKUP"))
		itemNum = ITM_AMMO_METAL_BOLTS_PICKUP;
	else if (!Q_stricmp(token_str, "ITM_AMMO_ROCKETS_PICKUP"))
		itemNum = ITM_AMMO_ROCKETS_PICKUP;
	else if (!Q_stricmp(token_str, "ITM_AMMO_EMPLACED_PICKUP"))
		itemNum = ITM_AMMO_EMPLACED_PICKUP;
	else if (!Q_stricmp(token_str, "ITM_AMMO_THERMAL_PICKUP"))
		itemNum = ITM_AMMO_THERMAL_PICKUP;
	else if (!Q_stricmp(token_str, "ITM_AMMO_TRIPMINE_PICKUP"))
		itemNum = ITM_AMMO_TRIPMINE_PICKUP;
	else if (!Q_stricmp(token_str, "ITM_AMMO_DETPACK_PICKUP"))
		itemNum = ITM_AMMO_DETPACK_PICKUP;
	else if (!Q_stricmp(token_str, "ITM_FORCE_HEAL_PICKUP"))
	{
		itemNum = ITM_FORCE_HEAL_PICKUP;
	}
	else if (!Q_stricmp(token_str, "ITM_FORCE_LEVITATION_PICKUP"))
	{
		itemNum = ITM_FORCE_LEVITATION_PICKUP;
	}
	else if (!Q_stricmp(token_str, "ITM_FORCE_SPEED_PICKUP"))
	{
		itemNum = ITM_FORCE_SPEED_PICKUP;
	}
	else if (!Q_stricmp(token_str, "ITM_FORCE_PUSH_PICKUP"))
	{
		itemNum = ITM_FORCE_PUSH_PICKUP;
	}
	else if (!Q_stricmp(token_str, "ITM_FORCE_PULL_PICKUP"))
	{
		itemNum = ITM_FORCE_PULL_PICKUP;
	}
	else if (!Q_stricmp(token_str, "ITM_FORCE_TELEPATHY_PICKUP"))
	{
		itemNum = ITM_FORCE_TELEPATHY_PICKUP;
	}
	else if (!Q_stricmp(token_str, "ITM_FORCE_GRIP_PICKUP"))
	{
		itemNum = ITM_FORCE_GRIP_PICKUP;
	}
	else if (!Q_stricmp(token_str, "ITM_FORCE_LIGHTNING_PICKUP"))
	{
		itemNum = ITM_FORCE_LIGHTNING_PICKUP;
	}
	else if (!Q_stricmp(token_str, "ITM_FORCE_SABERTHROW_PICKUP"))
	{
		itemNum = ITM_FORCE_SABERTHROW_PICKUP;
	}
	else if (!Q_stricmp(token_str, "ITM_BATTERY_PICKUP"))
	{
		itemNum = ITM_BATTERY_PICKUP;
	}
	else if (!Q_stricmp(token_str, "ITM_SEEKER_PICKUP"))
		itemNum = ITM_SEEKER_PICKUP;
	else if (!Q_stricmp(token_str, "ITM_SHIELD_PICKUP"))
		itemNum = ITM_SHIELD_PICKUP;
	else if (!Q_stricmp(token_str, "ITM_BACTA_PICKUP"))
		itemNum = ITM_BACTA_PICKUP;
	else if (!Q_stricmp(token_str, "ITM_DATAPAD_PICKUP"))
		itemNum = ITM_DATAPAD_PICKUP;
	else if (!Q_stricmp(token_str, "ITM_BINOCULARS_PICKUP"))
		itemNum = ITM_BINOCULARS_PICKUP;
	else if (!Q_stricmp(token_str, "ITM_SENTRY_GUN_PICKUP"))
		itemNum = ITM_SENTRY_GUN_PICKUP;
	else if (!Q_stricmp(token_str, "ITM_LA_GOGGLES_PICKUP"))
		itemNum = ITM_LA_GOGGLES_PICKUP;
	else if (!Q_stricmp(token_str, "ITM_CLOAK_PICKUP"))
		itemNum = ITM_CLOAK_PICKUP;
	else if (!Q_stricmp(token_str, "ITM_BARRIER_PICKUP"))
		itemNum = ITM_BARRIER_PICKUP;
	else if (!Q_stricmp(token_str, "ITM_GRAPPLEHOOK_PICKUP"))
		itemNum = ITM_GRAPPLEHOOK_PICKUP;
	else if (!Q_stricmp(token_str, "ITM_BLASTER_PISTOL_PICKUP"))
		itemNum = ITM_BLASTER_PISTOL_PICKUP;
	else if (!Q_stricmp(token_str, "ITM_CONCUSSION_RIFLE_PICKUP"))
		itemNum = ITM_CONCUSSION_RIFLE_PICKUP;
	else if (!Q_stricmp(token_str, "ITM_MEDPAK_PICKUP"))
		itemNum = ITM_MEDPAK_PICKUP;
	else if (!Q_stricmp(token_str, "ITM_SHIELD_SM_PICKUP"))
		itemNum = ITM_SHIELD_SM_PICKUP;
	else if (!Q_stricmp(token_str, "ITM_SHIELD_LRG_PICKUP"))
		itemNum = ITM_SHIELD_LRG_PICKUP;
	else if (!Q_stricmp(token_str, "ITM_GOODIE_KEY_PICKUP"))
		itemNum = ITM_GOODIE_KEY_PICKUP;
	else if (!Q_stricmp(token_str, "ITM_SECURITY_KEY_PICKUP"))
		itemNum = ITM_SECURITY_KEY_PICKUP;
	else
	{
		itemNum = 0;
		gi.Printf("WARNING: bad itemname in external item data '%s'\n", token_str);
	}

	itemParms.itemNum = itemNum;
	//	++bg_numItems;

	IT_SetDefaults();
}

static void IT_ClassName(const char** hold_buf)
{
	const char* token_str;

	if (COM_ParseString(hold_buf, &token_str))
	{
		return;
	}

	int len = strlen(token_str);
	len++;
	if (len > 32)
	{
		len = 32;
		gi.Printf("WARNING: weaponclass too long in external ITEMS.DAT '%s'\n", token_str);
	}

	bg_itemlist[itemParms.itemNum].classname = G_NewString(token_str);

	//	Q_strncpyz(bg_itemlist[itemParms.itemNum].classname,token_str,len);
}

static void IT_WorldModel(const char** hold_buf)
{
	const char* token_str;

	if (COM_ParseString(hold_buf, &token_str))
	{
		return;
	}

	int len = strlen(token_str);
	len++;
	if (len > 64)
	{
		len = 64;
		gi.Printf("WARNING: world model too long in external ITEMS.DAT '%s'\n", token_str);
	}

	bg_itemlist[itemParms.itemNum].world_model = G_NewString(token_str);
}

static void IT_Tag(const char** hold_buf)
{
	int tag;
	const char* token_str;

	if (COM_ParseString(hold_buf, &token_str))
	{
		return;
	}

	if (!Q_stricmp(token_str, "WP_NONE"))
		tag = WP_NONE;
	else if (!Q_stricmp(token_str, "WP_STUN_BATON"))
		tag = WP_STUN_BATON;
	else if (!Q_stricmp(token_str, "WP_SABER"))
		tag = WP_SABER;
	else if (!Q_stricmp(token_str, "WP_BLASTER_PISTOL"))
		tag = WP_BLASTER_PISTOL;
	else if (!Q_stricmp(token_str, "WP_BRYAR_PISTOL"))
		tag = WP_BRYAR_PISTOL;
	else if (!Q_stricmp(token_str, "WP_BLASTER"))
		tag = WP_BLASTER;
	else if (!Q_stricmp(token_str, "WP_DISRUPTOR"))
		tag = WP_DISRUPTOR;
	else if (!Q_stricmp(token_str, "WP_BOWCASTER"))
		tag = WP_BOWCASTER;
	else if (!Q_stricmp(token_str, "WP_REPEATER"))
		tag = WP_REPEATER;
	else if (!Q_stricmp(token_str, "WP_DEMP2"))
		tag = WP_DEMP2;
	else if (!Q_stricmp(token_str, "WP_FLECHETTE"))
		tag = WP_FLECHETTE;
	else if (!Q_stricmp(token_str, "WP_ROCKET_LAUNCHER"))
		tag = WP_ROCKET_LAUNCHER;
	else if (!Q_stricmp(token_str, "WP_CONCUSSION"))
		tag = WP_CONCUSSION;
	else if (!Q_stricmp(token_str, "WP_THERMAL"))
		tag = WP_THERMAL;
	else if (!Q_stricmp(token_str, "WP_TRIP_MINE"))
		tag = WP_TRIP_MINE;
	else if (!Q_stricmp(token_str, "WP_DET_PACK"))
		tag = WP_DET_PACK;
	else if (!Q_stricmp(token_str, "WP_BOT_LASER"))
		tag = WP_BOT_LASER;
	else if (!Q_stricmp(token_str, "WP_EMPLACED_GUN"))
		tag = WP_EMPLACED_GUN;
	else if (!Q_stricmp(token_str, "WP_DROIDEKA"))
		tag = WP_DROIDEKA;
	else if (!Q_stricmp(token_str, "WP_SBD_BLASTER"))
		tag = WP_SBD_BLASTER;
	else if (!Q_stricmp(token_str, "WP_DUAL_PISTOL"))
		tag = WP_DUAL_PISTOL;
	else if (!Q_stricmp(token_str, "WP_DUAL_CLONEPISTOL"))
		tag = WP_DUAL_CLONEPISTOL;
	else if (!Q_stricmp(token_str, "WP_MELEE"))
		tag = WP_MELEE;
	else if (!Q_stricmp(token_str, "WP_TURRET"))
		tag = WP_TURRET;
	else if (!Q_stricmp(token_str, "WP_ATST_MAIN"))
		tag = WP_ATST_MAIN;
	else if (!Q_stricmp(token_str, "WP_ATST_SIDE"))
		tag = WP_ATST_SIDE;
	else if (!Q_stricmp(token_str, "WP_TIE_FIGHTER"))
		tag = WP_TIE_FIGHTER;
	else if (!Q_stricmp(token_str, "WP_RAPID_FIRE_CONC"))
		tag = WP_RAPID_FIRE_CONC;
	else if (!Q_stricmp(token_str, "WP_JAWA"))
		tag = WP_JAWA;
	else if (!Q_stricmp(token_str, "WP_TUSKEN_RIFLE"))
		tag = WP_TUSKEN_RIFLE;
	else if (!Q_stricmp(token_str, "WP_TUSKEN_STAFF"))
		tag = WP_TUSKEN_STAFF;
	else if (!Q_stricmp(token_str, "WP_SCEPTER"))
		tag = WP_SCEPTER;
	else if (!Q_stricmp(token_str, "WP_NOGHRI_STICK"))
		tag = WP_NOGHRI_STICK;
	else if (!Q_stricmp(token_str, "WP_BATTLEDROID"))
		tag = WP_BATTLEDROID;
	else if (!Q_stricmp(token_str, "WP_THEFIRSTORDER"))
		tag = WP_THEFIRSTORDER;
	else if (!Q_stricmp(token_str, "WP_CLONECARBINE"))
		tag = WP_CLONECARBINE;
	else if (!Q_stricmp(token_str, "WP_REBELBLASTER"))
		tag = WP_REBELBLASTER;
	else if (!Q_stricmp(token_str, "WP_CLONERIFLE"))
		tag = WP_CLONERIFLE;
	else if (!Q_stricmp(token_str, "WP_CLONECOMMANDO"))
		tag = WP_CLONECOMMANDO;
	else if (!Q_stricmp(token_str, "WP_REBELRIFLE"))
		tag = WP_REBELRIFLE;
	else if (!Q_stricmp(token_str, "WP_REY"))
		tag = WP_REY;
	else if (!Q_stricmp(token_str, "WP_JANGO"))
		tag = WP_JANGO;
	else if (!Q_stricmp(token_str, "WP_BOBA"))
		tag = WP_BOBA;
	else if (!Q_stricmp(token_str, "WP_CLONEPISTOL"))
		tag = WP_CLONEPISTOL;
	else if (!Q_stricmp(token_str, "WP_ROTARY_CANNON"))
		tag = WP_ROTARY_CANNON;
	else if (!Q_stricmp(token_str, "WP_WRIST_BLASTER"))
		tag = WP_WRIST_BLASTER;

	else if (!Q_stricmp(token_str, "AMMO_FORCE"))
		tag = AMMO_FORCE;
	else if (!Q_stricmp(token_str, "AMMO_BLASTER"))
		tag = AMMO_BLASTER;
	else if (!Q_stricmp(token_str, "AMMO_POWERCELL"))
		tag = AMMO_POWERCELL;
	else if (!Q_stricmp(token_str, "AMMO_METAL_BOLTS"))
		tag = AMMO_METAL_BOLTS;
	else if (!Q_stricmp(token_str, "AMMO_ROCKETS"))
		tag = AMMO_ROCKETS;
	else if (!Q_stricmp(token_str, "AMMO_EMPLACED"))
		tag = AMMO_EMPLACED;
	else if (!Q_stricmp(token_str, "AMMO_THERMAL"))
		tag = AMMO_THERMAL;
	else if (!Q_stricmp(token_str, "AMMO_TRIPMINE"))
		tag = AMMO_TRIPMINE;
	else if (!Q_stricmp(token_str, "AMMO_DETPACK"))
		tag = AMMO_DETPACK;
	else if (!Q_stricmp(token_str, "FP_HEAL"))
	{
		tag = FP_HEAL;
	}
	else if (!Q_stricmp(token_str, "FP_LEVITATION"))
	{
		tag = FP_LEVITATION;
	}
	else if (!Q_stricmp(token_str, "FP_SPEED"))
	{
		tag = FP_SPEED;
	}
	else if (!Q_stricmp(token_str, "FP_PUSH"))
	{
		tag = FP_PUSH;
	}
	else if (!Q_stricmp(token_str, "FP_PULL"))
	{
		tag = FP_PULL;
	}
	else if (!Q_stricmp(token_str, "FP_TELEPATHY"))
	{
		tag = FP_TELEPATHY;
	}
	else if (!Q_stricmp(token_str, "FP_GRIP"))
	{
		tag = FP_GRIP;
	}
	else if (!Q_stricmp(token_str, "FP_LIGHTNING"))
	{
		tag = FP_LIGHTNING;
	}
	else if (!Q_stricmp(token_str, "FP_SABERTHROW"))
	{
		tag = FP_SABERTHROW;
	}
	else if (!Q_stricmp(token_str, "FP_STASIS"))
	{
		tag = FP_STASIS;
	}
	else if (!Q_stricmp(token_str, "FP_DESTRUCTION"))
	{
		tag = FP_DESTRUCTION;
	}
	else if (!Q_stricmp(token_str, "FP_GRASP"))
	{
		tag = FP_GRASP;
	}
	else if (!Q_stricmp(token_str, "FP_BLAST"))
	{
		tag = FP_BLAST;
	}
	else if (!Q_stricmp(token_str, "FP_REPULSE"))
	{
		tag = FP_REPULSE;
	}
	else if (!Q_stricmp(token_str, "FP_LIGHTNING_STRIKE"))
	{
		tag = FP_LIGHTNING_STRIKE;
	}
	else if (!Q_stricmp(token_str, "FP_FEAR"))
	{
		tag = FP_FEAR;
	}
	else if (!Q_stricmp(token_str, "FP_DEADLYSIGHT"))
	{
		tag = FP_DEADLYSIGHT;
	}
	else if (!Q_stricmp(token_str, "FP_PROJECTION"))
	{
		tag = FP_PROJECTION;
	}
	else if (!Q_stricmp(token_str, "ITM_BATTERY_PICKUP"))
	{
		tag = ITM_BATTERY_PICKUP;
	}
	else if (!Q_stricmp(token_str, "INV_SEEKER"))
	{
		tag = INV_SEEKER;
	}
	else if (!Q_stricmp(token_str, "ITM_SHIELD_PICKUP"))
	{
		tag = ITM_SHIELD_PICKUP;
	}
	else if (!Q_stricmp(token_str, "INV_BACTA_CANISTER"))
	{
		tag = INV_BACTA_CANISTER;
	}
	else if (!Q_stricmp(token_str, "ITM_DATAPAD_PICKUP"))
	{
		tag = ITM_DATAPAD_PICKUP;
	}
	else if (!Q_stricmp(token_str, "INV_ELECTROBINOCULARS"))
	{
		tag = INV_ELECTROBINOCULARS;
	}
	else if (!Q_stricmp(token_str, "INV_SENTRY"))
	{
		tag = INV_SENTRY;
	}
	else if (!Q_stricmp(token_str, "INV_LIGHTAMP_GOGGLES"))
	{
		tag = INV_LIGHTAMP_GOGGLES;
	}
	else if (!Q_stricmp(token_str, "INV_GOODIE_KEY"))
	{
		tag = INV_GOODIE_KEY;
	}
	else if (!Q_stricmp(token_str, "INV_SECURITY_KEY"))
	{
		tag = INV_SECURITY_KEY;
	}
	else if (!Q_stricmp(token_str, "ITM_MEDPAK_PICKUP"))
	{
		tag = ITM_MEDPAK_PICKUP;
	}
	else if (!Q_stricmp(token_str, "ITM_SHIELD_SM_PICKUP"))
	{
		tag = ITM_SHIELD_SM_PICKUP;
	}
	else if (!Q_stricmp(token_str, "ITM_SHIELD_LRG_PICKUP"))
	{
		tag = ITM_SHIELD_LRG_PICKUP;
	}
	else if (!Q_stricmp(token_str, "INV_CLOAK"))
	{
		tag = INV_CLOAK;
	}
	else if (!Q_stricmp(token_str, "INV_BARRIER"))
	{
		tag = INV_BARRIER;
	}
	else if (!Q_stricmp(token_str, "INV_GRAPPLEHOOK"))
	{
		tag = INV_GRAPPLEHOOK;
	}
	else
	{
		tag = WP_BRYAR_PISTOL;
		//This error was slipping through too much, causing runaway exceptions and shutting down, so now it's a real error when not in Final
#ifndef FINAL_BUILD
		G_Error("ERROR: bad tagname in external item data '%s'\n", token_str);
#else
		gi.Printf("WARNING: bad tagname in external item data '%s'\n", token_str);
#endif
	}

	bg_itemlist[itemParms.itemNum].giTag = tag;
}

static void IT_Type(const char** hold_buf)
{
	int type;
	const char* token_str;

	if (COM_ParseString(hold_buf, &token_str))
	{
		return;
	}

	if (!Q_stricmp(token_str, "IT_BAD"))
		type = IT_BAD;
	else if (!Q_stricmp(token_str, "IT_WEAPON"))
		type = IT_WEAPON;
	else if (!Q_stricmp(token_str, "IT_AMMO"))
		type = IT_AMMO;
	else if (!Q_stricmp(token_str, "IT_ARMOR"))
		type = IT_ARMOR;
	else if (!Q_stricmp(token_str, "IT_HEALTH"))
		type = IT_HEALTH;
	else if (!Q_stricmp(token_str, "IT_HOLDABLE"))
		type = IT_HOLDABLE;
	else if (!Q_stricmp(token_str, "IT_BATTERY"))
		type = IT_BATTERY;
	else if (!Q_stricmp(token_str, "IT_HOLOCRON"))
		type = IT_HOLOCRON;
	else
	{
		type = IT_BAD;
		gi.Printf("WARNING: bad itemname in external item data '%s'\n", token_str);
	}

	bg_itemlist[itemParms.itemNum].giType = static_cast<itemType_t>(type);
}

static void IT_Icon(const char** hold_buf)
{
	const char* token_str;

	if (COM_ParseString(hold_buf, &token_str))
	{
		return;
	}

	int len = strlen(token_str);
	len++;
	if (len > 32)
	{
		len = 32;
		gi.Printf("WARNING: icon too long in external ITEMS.DAT '%s'\n", token_str);
	}

	bg_itemlist[itemParms.itemNum].icon = G_NewString(token_str);
}

static void IT_Count(const char** hold_buf)
{
	int token_int;

	if (COM_ParseInt(hold_buf, &token_int))
	{
		SkipRestOfLine(hold_buf);
		return;
	}

	if (token_int < 0 || token_int > 1000) // FIXME :What are the right values?
	{
		gi.Printf("WARNING: bad Count in external item data '%d'\n", token_int);
		return;
	}
	bg_itemlist[itemParms.itemNum].quantity = token_int;
}

static void IT_Min(const char** hold_buf)
{
	int token_int;

	for (int i = 0; i < 3; ++i)
	{
		if (COM_ParseInt(hold_buf, &token_int))
		{
			SkipRestOfLine(hold_buf);
			return;
		}

		bg_itemlist[itemParms.itemNum].mins[i] = token_int;
	}
}

static void IT_Max(const char** hold_buf)
{
	int token_int;

	for (int i = 0; i < 3; ++i)
	{
		if (COM_ParseInt(hold_buf, &token_int))
		{
			SkipRestOfLine(hold_buf);
			return;
		}

		bg_itemlist[itemParms.itemNum].maxs[i] = token_int;
	}
}

static void IT_PickupSound(const char** hold_buf)
{
	const char* token_str;

	if (COM_ParseString(hold_buf, &token_str))
	{
		return;
	}

	int len = strlen(token_str);
	len++;
	if (len > 32)
	{
		len = 32;
		gi.Printf("WARNING: Pickup Sound too long in external ITEMS.DAT '%s'\n", token_str);
	}

	bg_itemlist[itemParms.itemNum].pickup_sound = G_NewString(token_str);
}

static void IT_ParseWeaponParms(const char** hold_buf)
{
	int i;

	while (hold_buf)
	{
		const char* token = COM_ParseExt(hold_buf, qtrue);

		if (!Q_stricmp(token, "}")) // End of data for this weapon
			break;

		// Loop through possible parameters
		for (i = 0; i < IT_PARM_MAX; ++i)
		{
			if (!Q_stricmp(token, ItemParms[i].parmName))
			{
				ItemParms[i].func(hold_buf);
				break;
			}
		}

		if (i < IT_PARM_MAX) // Find parameter???
		{
			continue;
		}

		Com_Printf("^3WARNING: bad parameter in external item data '%s'\n", token);
		SkipRestOfLine(hold_buf);
	}
}

static void IT_ParseParms(const char* buffer)
{
	const char* hold_buf;

	//	bg_numItems = 0;
	hold_buf = buffer;
	COM_BeginParseSession();

	while (hold_buf)
	{
		const char* token = COM_ParseExt(&hold_buf, qtrue);

		if (!Q_stricmp(token, "{"))
		{
			IT_ParseWeaponParms(&hold_buf);
		}
	}

	COM_EndParseSession();

	//	--bg_numItems;
}

void IT_LoadItemParms()
{
	char* buffer = nullptr;

	gi.FS_ReadFile("ext_data/items.dat", reinterpret_cast<void**>(&buffer));

	IT_ParseParms(buffer);

	gi.FS_FreeFile(buffer); //let go of the buffer
}

extern void cgi_Cvar_Register(vmCvar_t* vmCvar, const char* varName, const char* defaultValue, int flags);
extern cvar_t* g_AllowWeather;
extern cvar_t* com_rend2;

void IT_LoadWeatherParms(void)
{
	vmCvar_t mapname;
	const char* info = CG_ConfigString(CS_SERVERINFO);
	const char* s = Info_ValueForKey(info, "mapname");

	if (com_rend2->integer == 1 &&
		(strcmp(s, "yavin1") == 0
			|| strcmp(s, "demo") == 0
			|| strcmp(s, "jodemo") == 0
			|| strcmp(s, "01nar") == 0
			|| strcmp(s, "md2_bd_ch") == 0
			|| strcmp(s, "md_sn_intro_jedi") == 0
			|| strcmp(s, "md_ch_battledroids") == 0
			|| strcmp(s, "md_ep4_intro") == 0
			|| strcmp(s, "secbase") == 0
			|| strcmp(s, "level0") == 0
			|| strcmp(s, "JKG_ImperialBase") == 0
			|| strcmp(s, "kejim_post") == 0)) // dont add weather on these maps when rend2 is on.
	{
		return;
	}

	if (g_AllowWeather->integer != 1)
	{
		return;
	}

	cgi_Cvar_Register(&mapname, "mapname", "", CVAR_SERVERINFO | CVAR_ROM);

	gi.SendConsoleCommand(va("execq Weather/%s", mapname.string, mapname.string, mapname.string));
}