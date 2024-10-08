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

extern vec3_t forward_vec, vright_vec, up;
extern vec3_t muzzle;
extern vec3_t muzzle2;

void WP_TraceSetStart(const gentity_t* ent, vec3_t start);
gentity_t* create_missile(vec3_t org, vec3_t dir, float vel, int life, gentity_t* owner, qboolean alt_fire = qfalse);
void WP_Stick(gentity_t* missile, const trace_t* trace, float fudge_distance = 0.0f);
void WP_Explode(gentity_t* self);
void WP_ExplosiveDie(gentity_t* self, gentity_t* inflictor, gentity_t* attacker, int damage, int means_of_death, int d_flags, int hit_loc);
bool WP_MissileTargetHint(gentity_t* shooter, vec3_t start, vec3_t out);
void ViewHeightFix(const gentity_t* ent);
qboolean LogAccuracyHit(const gentity_t* target, const gentity_t* attacker);
extern qboolean G_BoxInBounds(const vec3_t point, const vec3_t mins, const vec3_t maxs, const vec3_t bounds_mins, const vec3_t bounds_maxs);
extern qboolean jedi_dodge_evasion(gentity_t* self, gentity_t* shooter, trace_t* tr, int hit_loc);
extern qboolean jedi_disruptor_dodge_evasion(gentity_t* self, gentity_t* shooter, trace_t* tr, int hit_loc);
extern qboolean jedi_npc_disruptor_dodge_evasion(gentity_t* self, gentity_t* shooter, trace_t* tr, int hit_loc);
extern qboolean PM_DroidMelee(int npc_class);
extern void G_Knockdown(gentity_t* self, gentity_t* attacker, const vec3_t push_dir, float strength,
	qboolean break_saber_lock);
extern qboolean G_HasKnockdownAnims(const gentity_t* ent);

extern gentity_t* ent_list[MAX_GENTITIES];

extern int g_rocketLockEntNum;
extern int g_rocketLockTime;
extern int g_rocketSlackTime;

int G_GetHitLocFromTrace(trace_t* trace, int mod);

// Specific weapon functions

void WP_ATSTMainFire(gentity_t* ent);
void WP_ATSTSideAltFire(gentity_t* ent);
void WP_ATSTSideFire(gentity_t* ent);
void WP_FireBryarPistol(gentity_t* ent, qboolean alt_fire);
void WP_FireBryarPistolDuals(gentity_t* ent, qboolean alt_fire, qboolean second_pistol);
void WP_FireJawaPistol(gentity_t* ent, qboolean alt_fire);
void WP_FireSBDPistol(gentity_t* ent, qboolean alt_fire);
void WP_FireBlasterMissile(gentity_t* ent, vec3_t start, vec3_t dir, qboolean alt_fire);
void WP_FireBlaster(gentity_t* ent, qboolean alt_fire);
void WP_FireBattleDroidMissile(gentity_t* ent, vec3_t start, vec3_t dir, qboolean alt_fire);
void WP_FireBattleDroid(gentity_t* ent, qboolean alt_fire);
void WP_FireFirstOrderMissile(gentity_t* ent, vec3_t start, vec3_t dir, qboolean alt_fire);
void WP_FireFirstOrder(gentity_t* ent, qboolean alt_fire);
void WP_FireCloneMissile(gentity_t* ent, vec3_t start, vec3_t dir, qboolean alt_fire);
void WP_FireClone(gentity_t* ent, qboolean alt_fire);
void WP_FireRebelBlasterMissile(gentity_t* ent, vec3_t start, vec3_t dir, qboolean alt_fire);
void WP_FireRebelBlaster(gentity_t* ent, qboolean alt_fire);
void WP_FireRebelBlasterDuals(gentity_t* ent, qboolean alt_fire, qboolean second_pistol);
void WP_FireCloneRifleMissile(gentity_t* ent, vec3_t start, vec3_t dir, qboolean alt_fire);
void WP_FireCloneRifle(gentity_t* ent, qboolean alt_fire);
void WP_FireCloneCommandoMissile(gentity_t* ent, vec3_t start, vec3_t dir, qboolean alt_fire);
void WP_FireCloneCommando(gentity_t* ent, qboolean alt_fire);
void WP_FireRebelRifleMissile(gentity_t* ent, vec3_t start, vec3_t dir, qboolean alt_fire);
void WP_FireRebelRifle(gentity_t* ent, qboolean alt_fire);
void WP_FireBobaRifleMissile(gentity_t* ent, vec3_t start, vec3_t dir, qboolean alt_fire);
void WP_FireBobaRifle(gentity_t* ent, qboolean alt_fire);
void WP_FireWristPistol(gentity_t* ent, qboolean alt_fire);
void WP_FireReyPistolDuals(gentity_t* ent, qboolean alt_fire, qboolean second_pistol);
void WP_FireReyPistol(gentity_t* ent, qboolean alt_fire);
void WP_FireJangoPistolMissile(gentity_t* ent, vec3_t start, vec3_t dir, qboolean alt_fire);
void WP_FireJangoPistol(gentity_t* ent, qboolean alt_fire, qboolean second_pistol);
void WP_FireJangoDualPistolMissile(gentity_t* ent, vec3_t start, vec3_t dir, qboolean alt_fire);
void WP_FireJangoDualPistolMissileDuals(gentity_t* ent, vec3_t start, vec3_t dir, qboolean alt_fire);
void WP_FireJangoDualPistol(gentity_t* ent, qboolean alt_fire);
void WP_FireDroidekaDualPistol(gentity_t* ent, qboolean alt_fire);
void WP_FireJangoFPPistolDuals(gentity_t* ent, qboolean alt_fire, qboolean second_pistol);
void WP_FireDroidekaFPPistolDuals(gentity_t* ent, qboolean alt_fire, qboolean second_pistol);
void WP_FireClonePistol(gentity_t* ent, qboolean alt_fire);
void WP_FireClonePistolDuals(gentity_t* ent, qboolean alt_fire, qboolean second_pistol);
void WP_FireRotaryCannonMissile(gentity_t* ent, vec3_t start, vec3_t dir, qboolean alt_fire);
void WP_FireRotaryCannon(gentity_t* ent, qboolean alt_fire);
void WP_FireMandoClonePistolDuals(gentity_t* ent, qboolean alt_fire, qboolean second_pistol);
void WP_BotLaser(gentity_t* ent);
void WP_FireBowcaster(gentity_t* ent, qboolean alt_fire);
void WP_Concussion(gentity_t* ent, qboolean alt_fire);
void WP_FireDEMP2(gentity_t* ent, qboolean alt_fire);
void charge_stick(gentity_t* self, gentity_t* other, const trace_t* trace);
void WP_FireDetPack(gentity_t* ent, qboolean alt_fire);
void WP_FireDisruptor(gentity_t* ent, qboolean alt_fire);
void WP_FireTurboLaserMissile(gentity_t* ent, vec3_t start, vec3_t dir);
void WP_EmplacedFire(gentity_t* ent);
void prox_mine_think(gentity_t* ent);
void prox_mine_stick(gentity_t* self, gentity_t* other, const trace_t* trace);
void WP_FireFlechette(gentity_t* ent, qboolean alt_fire);
void WP_Melee(gentity_t* ent);
void WP_FireNoghriStick(gentity_t* ent);
void WP_FireRepeater(gentity_t* ent, qboolean alt_fire);
void rocketThink(gentity_t* ent);
void WP_FireRocket(gentity_t* ent, qboolean alt_fire);
void WP_FireStunBaton(gentity_t* ent, qboolean alt_fire);
void thermalDetonatorExplode(gentity_t* ent);
void thermal_die(gentity_t* self, gentity_t* inflictor, gentity_t* attacker, int damage, int mod, int d_flags,
	int hit_loc);
qboolean WP_LobFire(const gentity_t* self, vec3_t start, vec3_t target, vec3_t mins, vec3_t maxs, int clipmask,
	vec3_t velocity, qboolean trace_path, int ignore_ent_num, int enemy_num,
	float ideal_speed = 0, qboolean must_hit = qfalse);
void WP_ThermalThink(gentity_t* ent);
gentity_t* WP_FireThermalDetonator(gentity_t* ent, qboolean alt_fire);
gentity_t* WP_DropThermal(gentity_t* ent);
void touchLaserTrap(gentity_t* ent, gentity_t* other, const trace_t* trace);
void CreateLaserTrap(gentity_t* laser_trap, vec3_t start, gentity_t* owner);
void WP_PlaceLaserTrap(gentity_t* ent, qboolean alt_fire);
void WP_FireTuskenRifle(gentity_t* ent);
qboolean NPC_IsNotHavingEnoughForceSight(const gentity_t* self);
