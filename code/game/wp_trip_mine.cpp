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

#include "g_local.h"
#include "b_local.h"
#include "g_functions.h"
#include "wp_saber.h"
#include "w_local.h"

//-----------------------
//	Laser Trip Mine
//-----------------------

constexpr auto PROXIMITY_STYLE = 1;
constexpr auto TRIPWIRE_STYLE = 2;

//---------------------------------------------------------
void touchLaserTrap(gentity_t* ent, gentity_t* other, const trace_t* trace)
//---------------------------------------------------------
{
	ent->s.eType = ET_GENERAL;

	// a tripwire so add draw line flag
	VectorCopy(trace->plane.normal, ent->movedir);

	// make it shootable
	VectorSet(ent->mins, -4, -4, -4);
	VectorSet(ent->maxs, 4, 4, 4);

	ent->clipmask = MASK_SHOT;
	ent->contents = CONTENTS_SHOTCLIP;
	ent->takedamage = qtrue;
	ent->health = 15;

	ent->e_DieFunc = dieF_WP_ExplosiveDie;
	ent->e_TouchFunc = touchF_NULL;

	// so we can trip it too
	ent->activator = ent->owner;
	ent->owner = nullptr;

	WP_Stick(ent, trace);

	if (ent->count == TRIPWIRE_STYLE)
	{
		constexpr vec3_t mins = { -4, -4, -4 }, maxs = { 4, 4, 4 }; //FIXME: global these
		trace_t tr;
		VectorMA(ent->currentOrigin, 32, ent->movedir, ent->s.origin2);
		gi.trace(&tr, ent->s.origin2, mins, maxs, ent->currentOrigin, ent->s.number, MASK_SHOT, G2_RETURNONHIT, 0);
		VectorCopy(tr.endpos, ent->s.origin2);

		ent->e_ThinkFunc = thinkF_laserTrapThink;
	}
	else
	{
		ent->e_ThinkFunc = thinkF_WP_prox_mine_think;
	}

	ent->nextthink = level.time + LT_ACTIVATION_DELAY;
}

// copied from flechette prox above...which will not be used if this gets approved
//---------------------------------------------------------
void WP_prox_mine_think(gentity_t* ent)
//---------------------------------------------------------
{
	qboolean blow = qfalse;

	// first time through?
	if (ent->count)
	{
		// play activated warning
		ent->count = 0;
		ent->s.eFlags |= EF_PROX_TRIP;
		G_Sound(ent, G_SoundIndex("sound/weapons/laser_trap/warning.wav"));
	}

	// if it isn't time to auto-explode, do a small proximity check
	if (ent->delay > level.time)
	{
		const int count = G_RadiusList(ent->currentOrigin, PROX_MINE_RADIUS_CHECK, ent, qtrue, ent_list);

		for (int i = 0; i < count; i++)
		{
			if (ent_list[i]->client && ent_list[i]->health > 0 && ent->activator && ent_list[i]->s.number != ent->
				activator->s.number)
			{
				blow = qtrue;
				break;
			}
		}
	}
	else
	{
		// well, we must die now
		blow = qtrue;
	}

	if (blow)
	{
		//		G_Sound( ent, G_SoundIndex( "sound/weapons/flechette/warning.wav" ));
		ent->e_ThinkFunc = thinkF_WP_Explode;
		ent->nextthink = level.time + 200;
	}
	else
	{
		// we probably don't need to do this thinking logic very often...maybe this is fast enough?
		ent->nextthink = level.time + 500;
	}
}

//---------------------------------------------------------
void laserTrapThink(gentity_t* ent)
//---------------------------------------------------------
{
	vec3_t end;
	constexpr vec3_t maxs = { 4, 4, 4 };
	constexpr vec3_t mins = { -4, -4, -4 };
	trace_t tr;

	// turn on the beam effect
	if (!(ent->s.eFlags & EF_FIRING))
	{
		// arm me
		G_Sound(ent, G_SoundIndex("sound/weapons/laser_trap/warning.wav"));
		ent->s.loopSound = G_SoundIndex("sound/weapons/laser_trap/hum_loop.wav");
		ent->s.eFlags |= EF_FIRING;
	}

	ent->e_ThinkFunc = thinkF_laserTrapThink;
	ent->nextthink = level.time + FRAMETIME;

	// Find the main impact point
	VectorMA(ent->s.pos.trBase, 2048, ent->movedir, end);
	gi.trace(&tr, ent->s.origin2, mins, maxs, end, ent->s.number, MASK_SHOT, G2_RETURNONHIT, 0);

	const gentity_t* traceEnt = &g_entities[tr.entityNum];

	// Adjust this so that the effect has a relatively fresh endpoint
	VectorCopy(tr.endpos, ent->pos4);

	if (traceEnt->client || tr.startsolid)
	{
		// go boom
		WP_Explode(ent);
		ent->s.eFlags &= ~EF_FIRING; // don't draw beam if we are dead
	}
	else
	{
		/*
		// FIXME: they need to avoid the beam!
		AddSoundEvent( ent->owner, ent->currentOrigin, ent->splashRadius*2, AEL_DANGER );
		AddSightEvent( ent->owner, ent->currentOrigin, ent->splashRadius*2, AEL_DANGER, 50 );
		*/
	}
}

//---------------------------------------------------------
void CreateLaserTrap(gentity_t* laser_trap, vec3_t start, gentity_t* owner)
//---------------------------------------------------------
{
	if (!VALIDSTRING(laser_trap->classname))
	{
		// since we may be coming from a map placed trip mine, we don't want to override that class name....
		//	That would be bad because the player drop code tries to limit number of placed items...so it would have removed map placed ones as well.
		laser_trap->classname = "tripmine";
	}

	laser_trap->splashDamage = weaponData[WP_TRIP_MINE].splashDamage;
	laser_trap->splashRadius = weaponData[WP_TRIP_MINE].splashRadius;
	laser_trap->damage = weaponData[WP_TRIP_MINE].damage;
	laser_trap->methodOfDeath = MOD_LASERTRIP;
	laser_trap->splashMethodOfDeath = MOD_LASERTRIP; //? SPLASH;

	laser_trap->s.eType = ET_MISSILE;
	laser_trap->svFlags = SVF_USE_CURRENT_ORIGIN;
	laser_trap->s.weapon = WP_TRIP_MINE;

	laser_trap->owner = owner;
	//	VectorSet( laserTrap->mins, -LT_SIZE, -LT_SIZE, -LT_SIZE );
	//	VectorSet( laserTrap->maxs, LT_SIZE, LT_SIZE, LT_SIZE );
	laser_trap->clipmask = CONTENTS_SOLID | CONTENTS_BODY | CONTENTS_SHOTCLIP; //MASK_SHOT;

	laser_trap->s.pos.trTime = level.time; // move a bit on the very first frame
	VectorCopy(start, laser_trap->s.pos.trBase);
	VectorCopy(start, laser_trap->currentOrigin);

	VectorCopy(start, laser_trap->pos2); // ?? wtf ?

	laser_trap->fxID = G_EffectIndex("tripMine/explosion");

	laser_trap->e_TouchFunc = touchF_touchLaserTrap;

	laser_trap->s.radius = 60;
	VectorSet(laser_trap->s.modelScale, 1.0f, 1.0f, 1.0f);
	gi.G2API_InitGhoul2Model(laser_trap->ghoul2, weaponData[WP_TRIP_MINE].missileMdl,
		G_ModelIndex(weaponData[WP_TRIP_MINE].missileMdl),
		NULL_HANDLE, NULL_HANDLE, 0, 0);
}

//---------------------------------------------------------
static void WP_RemoveOldTraps(const gentity_t* ent)
//---------------------------------------------------------
{
	gentity_t* found = nullptr;
	int trapcount = 0;
	int foundLaserTraps[MAX_GENTITIES] = { ENTITYNUM_NONE };

	// see how many there are now
	while ((found = G_Find(found, FOFS(classname), "tripmine")) != nullptr)
	{
		if (found->activator != ent) // activator is really the owner?
		{
			continue;
		}
		foundLaserTraps[trapcount++] = found->s.number;
	}

	// now remove first ones we find until there are only 9 left
	found = nullptr;
	const int trapcount_org = trapcount;
	int lowestTimeStamp = level.time;

	while (trapcount > 9)
	{
		int removeMe = -1;
		for (int i = 0; i < trapcount_org; i++)
		{
			if (foundLaserTraps[i] == ENTITYNUM_NONE)
			{
				continue;
			}
			found = &g_entities[foundLaserTraps[i]];
			if (found->setTime < lowestTimeStamp)
			{
				removeMe = i;
				lowestTimeStamp = found->setTime;
			}
		}
		if (removeMe != -1)
		{
			//remove it... or blow it?
			if (&g_entities[foundLaserTraps[removeMe]] == nullptr)
			{
				break;
			}
			G_FreeEntity(&g_entities[foundLaserTraps[removeMe]]);
			foundLaserTraps[removeMe] = ENTITYNUM_NONE;
			trapcount--;
		}
		else
		{
			break;
		}
	}
}

//---------------------------------------------------------
void WP_PlaceLaserTrap(gentity_t* ent, const qboolean alt_fire)
//---------------------------------------------------------
{
	// limit to 10 placed at any one time
	WP_RemoveOldTraps(ent);

	//FIXME: surface must be within 64
	gentity_t* laserTrap = G_Spawn();

	if (laserTrap)
	{
		vec3_t start;
		// now make the new one
		VectorCopy(muzzle, start);
		WP_TraceSetStart(ent, start);
		//make sure our start point isn't on the other side of a wall

		CreateLaserTrap(laserTrap, start, ent);

		// set player-created-specific fields
		laserTrap->setTime = level.time; //remember when we placed it

		laserTrap->s.eFlags |= EF_MISSILE_STICK;
		laserTrap->s.pos.trType = TR_GRAVITY;
		VectorScale(forward_vec, LT_VELOCITY, laserTrap->s.pos.trDelta);

		if (alt_fire)
		{
			laserTrap->count = PROXIMITY_STYLE;
			laserTrap->delay = level.time + 40000; // will auto-blow in 40 seconds.
			laserTrap->methodOfDeath = MOD_LASERTRIP_ALT;
			laserTrap->splashMethodOfDeath = MOD_LASERTRIP_ALT; //? SPLASH;
		}
		else
		{
			laserTrap->count = TRIPWIRE_STYLE;
		}
	}
}