//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: Default schedules.
//
//=============================================================================//

#ifndef AI_DEFAULT_H
#define AI_DEFAULT_H
#ifdef _WIN32
#pragma once
#endif

//=========================================================
// These are the schedule types
//=========================================================
enum 
{
	SCHED_NONE = 0,
	SCHED_IDLE_STAND,
	SCHED_IDLE_WALK,
	SCHED_WAKE_ANGRY,
	SCHED_ALERT_FACE,
	SCHED_ALERT_FACE_BESTSOUND,
	SCHED_ALERT_REACT_TO_COMBAT_SOUND,
	SCHED_ALERT_STAND,
	SCHED_ALERT_WALK,
	SCHED_INVESTIGATE_SOUND,
	SCHED_COMBAT_FACE,
	SCHED_COMBAT_SWEEP,
	SCHED_FEAR_FACE,
	SCHED_COMBAT_STAND,
	SCHED_COMBAT_WALK,
	SCHED_CHASE_ENEMY,
	SCHED_CHASE_ENEMY_FAILED,
	SCHED_VICTORY_DANCE,
	SCHED_TARGET_FACE,
	SCHED_TARGET_CHASE,
	SCHED_SMALL_FLINCH,
	SCHED_BIG_FLINCH,
	SCHED_BACK_AWAY_FROM_ENEMY,
	SCHED_MOVE_AWAY_FROM_ENEMY,
	SCHED_BACK_AWAY_FROM_SAVE_POSITION,
	SCHED_TAKE_COVER_FROM_ENEMY,
	SCHED_TAKE_COVER_FROM_BEST_SOUND,
	SCHED_FLEE_FROM_BEST_SOUND,
	SCHED_TAKE_COVER_FROM_ORIGIN,
	SCHED_FAIL_TAKE_COVER,
	SCHED_RUN_FROM_ENEMY,
	SCHED_RUN_FROM_ENEMY_FALLBACK,
	SCHED_MOVE_TO_WEAPON_RANGE,
	SCHED_ESTABLISH_LINE_OF_FIRE,
	SCHED_ESTABLISH_LINE_OF_FIRE_FALLBACK,
	SCHED_PRE_FAIL_ESTABLISH_LINE_OF_FIRE,
	SCHED_FAIL_ESTABLISH_LINE_OF_FIRE,
	SCHED_SHOOT_ENEMY_COVER,
	SCHED_COWER, // usually a last resort!
	SCHED_MELEE_ATTACK1,
	SCHED_MELEE_ATTACK2,
	SCHED_RANGE_ATTACK1,
	SCHED_RANGE_ATTACK2,
	SCHED_SPECIAL_ATTACK1,
	SCHED_SPECIAL_ATTACK2,
	SCHED_STANDOFF,
	SCHED_ARM_WEAPON,
	SCHED_DISARM_WEAPON,
	SCHED_HIDE_AND_RELOAD,
	SCHED_RELOAD,
	SCHED_AMBUSH,
	SCHED_DIE,
	SCHED_DIE_RAGDOLL,
	SCHED_WAIT_FOR_SCRIPT,
	SCHED_AISCRIPT,
	SCHED_SCRIPTED_WALK,
	SCHED_SCRIPTED_RUN,
	SCHED_SCRIPTED_CUSTOM_MOVE,
	SCHED_SCRIPTED_WAIT,
	SCHED_SCRIPTED_FACE,
	SCHED_SCENE_GENERIC,

	SCHED_MOVE_TO_TARGET, // Triggered by aiscripted schedule, mostly.
	SCHED_MOVE_TO_TARGET_VITAL,
	SCHED_MOVE_AWAY,
	SCHED_MOVE_AWAY_FAIL,
	SCHED_MOVE_AWAY_END,
	SCHED_NPC_FREEZE,
	SCHED_FALL_TO_GROUND,
	SCHED_DROPSHIP_DUSTOFF,

	SCHED_FLINCH_PHYSICS,

	SCHED_FAIL,
	SCHED_FAIL_NOSTOP,

	SCHED_RUN_FROM_ENEMY_MOB,

	SCHED_DUCK_DODGE,
	SCHED_SLEEP,

	// ======================================
	// IMPORTANT: This must be the last enum
	// ======================================
	LAST_SHARED_SCHEDULE
};

#endif // AI_DEFAULT_H
