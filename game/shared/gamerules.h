//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#ifndef GAMERULES_H
#define GAMERULES_H
#ifdef _WIN32
#pragma once
#endif

// Debug history should be disabled in release builds
//#define DISABLE_DEBUG_HISTORY	

#ifdef CLIENT_DLL

	#include "c_baseentity.h"
	
	#define CGameRules C_GameRules
	#define CGameRulesProxy C_GameRulesProxy

#else
	
	#include "baseentity.h"
	#include "recipientfilter.h"

#endif

#include "igamesystem.h"
#include "gamerules_register.h"

class CBaseCombatWeapon;
class CBaseCombatCharacter;
class CBasePlayer;
class CItem;
class CAmmoDef;

// weapon respawning return codes
enum
{	
	GR_NONE = 0,
	
	GR_WEAPON_RESPAWN_YES,
	GR_WEAPON_RESPAWN_NO,
	
	GR_ITEM_RESPAWN_YES,
	GR_ITEM_RESPAWN_NO,
};

// Player relationship return codes
enum
{
	GR_NOTTEAMMATE = 0,
	GR_TEAMMATE,
	GR_ENEMY,
	GR_ALLY,
	GR_NEUTRAL,
};

// This class has the data tables and gets the CGameRules data to the client.
class CGameRulesProxy : public CBaseEntity
{
public:
	DECLARE_CLASS( CGameRulesProxy, CBaseEntity );
	DECLARE_NETWORKCLASS();

	CGameRulesProxy();
	~CGameRulesProxy();

	// ALWAYS transmit to all clients.
	virtual int UpdateTransmitState( void );

	// CGameRules chains its NetworkStateChanged calls to here, since this
	// is the actual entity that will send the data.
	static void NotifyNetworkStateChanged();

private:
	
	static CGameRulesProxy *s_pGameRulesProxy;
};


abstract_class CGameRules : public CAutoGameSystemPerFrame
{
public:
	DECLARE_CLASS_GAMEROOT( CGameRules, CAutoGameSystemPerFrame );

	virtual char const *Name() { return "CGameRules"; }

	// Stuff shared between client and server.

	CGameRules(void);
	virtual ~CGameRules( void );

	// Damage Queries - these need to be implemented by the various subclasses (single-player, multi-player, etc).
	// The queries represent queries against damage types and properties.
	virtual bool	Damage_IsTimeBased( int iDmgType ) = 0;			// Damage types that are time-based.
	virtual bool	Damage_ShouldGibCorpse( int iDmgType ) = 0;		// Damage types that gib the corpse.
	virtual bool	Damage_ShowOnHUD( int iDmgType ) = 0;			// Damage types that have client HUD art.
	virtual bool	Damage_NoPhysicsForce( int iDmgType ) = 0;		// Damage types that don't have to supply a physics force & position.
	virtual bool	Damage_ShouldNotBleed( int iDmgType ) = 0;		// Damage types that don't make the player bleed.
	//Temp: These will go away once DamageTypes become enums.
	virtual int		Damage_GetTimeBased( void ) = 0;				// Actual bit-fields.
	virtual int		Damage_GetShouldGibCorpse( void ) = 0;
	virtual int		Damage_GetShowOnHud( void ) = 0;					
	virtual int		Damage_GetNoPhysicsForce( void )= 0;
	virtual int		Damage_GetShouldNotBleed( void ) = 0;

	virtual bool SwitchToNextBestWeapon( CBaseCombatCharacter *pPlayer, CBaseCombatWeapon *pCurrentWeapon ); // Switch to the next best weapon
	virtual CBaseCombatWeapon *GetNextBestWeapon( CBaseCombatCharacter *pPlayer, CBaseCombatWeapon *pCurrentWeapon ); // I can't use this weapon anymore, get me the next best one.
	virtual bool ShouldCollide( int collisionGroup0, int collisionGroup1 );

	virtual int DefaultFOV( void ) { return 90; }

	// This function is here for our CNetworkVars.
	inline void NetworkStateChanged()
	{
		// Forward the call to the entity that will send the data.
		CGameRulesProxy::NotifyNetworkStateChanged();
	}

	inline void NetworkStateChanged( void *pVar )
	{
		// Forward the call to the entity that will send the data.
		CGameRulesProxy::NotifyNetworkStateChanged();
	}

	// Get the view vectors for this mod.
	virtual const CViewVectors* GetViewVectors() const;

// Functions to verify the single/multiplayer status of a game
	virtual bool IsMultiplayer( void ) = 0;// is this a multiplayer game? (either coop or deathmatch)

	virtual bool InRoundRestart( void ) { return false; }

	// Allow thirdperson camera.
	virtual bool AllowThirdPersonCamera( void ) { return true; }

	virtual void ClientCommandKeyValues( edict_t *pEntity, KeyValues *pKeyValues ) {} 

	// IsConnectedUserInfoChangeAllowed allows the clients to change
	// cvars with the FCVAR_NOT_CONNECTED rule if it returns true
	virtual bool IsConnectedUserInfoChangeAllowed( CBasePlayer *pPlayer )
	{ 
		Assert( !IsMultiplayer() );
		return true; 
	}

#ifdef CLIENT_DLL

	virtual bool AllowMapParticleEffect( const char *pszParticleEffect ) { return true; }

	virtual bool AllowWeatherParticles( void ) { return true; }

	virtual bool AllowMapVisionFilterShaders( void ) { return false; }
	virtual const char* TranslateEffectForVisionFilter( const char *pchEffectType, const char *pchEffectName ) { return pchEffectName; }

	virtual bool IsLocalPlayer( int nEntIndex );

	virtual void ModifySentChat( char *pBuf, int iBufSize ) { return; }
	
#else

    virtual void Status( void (*print) (const char *fmt, ...) ) {}	
	virtual void GetTaggedConVarList( KeyValues *pCvarTagList ) {}

// CBaseEntity overrides.
public:

// Setup
	
	// Called when game rules are destroyed by CWorld
	virtual void LevelShutdown( void ) { return; };

	virtual void Precache( void ) { return; };
	
	// Called each frame. This just forwards the call to Think().
	virtual void FrameUpdatePostEntityThink();

	virtual void Think( void ) = 0;// GR_Think - runs every server frame, should handle any timer tasks, periodic events, etc.
	virtual bool IsAllowedToSpawn( CBaseEntity *pEntity ) = 0;  // Can this item spawn (eg NPCs don't spawn in deathmatch).

	// Called at the end of GameFrame (i.e. after all game logic has run this frame)
	virtual void EndGameFrame( void );

	virtual bool FAllowFlashlight( void ) = 0; // Are players allowed to switch on their flashlight?

// Functions to verify the single/multiplayer status of a game
	virtual bool IsTeamplay( void ) { return FALSE; };// is this deathmatch game being played with team rules?
	virtual const char *GetGameDescription( void ) { return "BrainBread 2"; }  // this is the game name that gets seen in the server browser
	
// Client connection/disconnection
	virtual bool ClientConnected( edict_t *pEntity, const char *pszName, const char *pszAddress, char *reject, int maxrejectlen ) = 0;// a client just connected to the server (player hasn't spawned yet)
	virtual void InitHUD( CBasePlayer *pl ) = 0;		// the client dll is ready for updating
	virtual void ClientDisconnected( edict_t *pClient ) = 0;// a client just disconnected from the server
	
// Client damage rules
	virtual float FlPlayerFallDamage( CBasePlayer *pPlayer ) = 0;// this client just hit the ground after a fall. How much damage?
	virtual bool  FPlayerCanTakeDamage( CBasePlayer *pPlayer, CBaseEntity *pAttacker, const CTakeDamageInfo &info ) { return TRUE; } // can this player take damage from this attacker?	

	virtual bool ShouldUseRobustRadiusDamage(CBaseEntity *pEntity) { return false; }
	virtual void  RadiusDamage( const CTakeDamageInfo &info, const Vector &vecSrc, float flRadius, int iClassIgnore, CBaseEntity *pEntityIgnore );
	// Let the game rules specify if fall death should fade screen to black
	virtual bool  FlPlayerFallDeathDoesScreenFade( CBasePlayer *pl ) { return FALSE; }

	virtual bool AllowDamage( CBaseEntity *pVictim, const CTakeDamageInfo &info ) = 0;


// Client spawn/respawn control
	virtual void PlayerSpawn( CBasePlayer *pPlayer ) = 0;// called by CBasePlayer::Spawn just before releasing player into the game
	virtual void PlayerThink( CBasePlayer *pPlayer ) = 0; // called by CBasePlayer::PreThink every frame, before physics are run and after keys are accepted
	virtual CBaseEntity *GetPlayerSpawnSpot( CBasePlayer *pPlayer );// Place this player on their spawnspot and face them the proper direction.
	virtual bool IsSpawnPointValid( CBaseEntity *pSpot, CBasePlayer *pPlayer );

	virtual bool ClientCommand( CBaseEntity *pEdict, const CCommand &args );  // handles the user commands;  returns TRUE if command handled properly
	virtual void ClientSettingsChanged( CBasePlayer *pPlayer );		 // the player has changed cvars

// Client kills/scoring
	virtual int IPointsForKill( CBasePlayer *pAttacker, CBasePlayer *pKilled ) = 0;// how many points do I award whoever kills this player?
	virtual void PlayerKilled( CBasePlayer *pVictim, const CTakeDamageInfo &info ) = 0;// Called each time a player dies
	virtual void DeathNotice(CBaseEntity *pVictim, const CTakeDamageInfo &info) { } // Call this from within a GameRules class to report an obituary.

// Weapon Damage
	// Determines how much damage Player's attacks inflict
	virtual float AdjustPlayerDamageInflicted( float damage ) { return damage; }
	virtual void  AdjustPlayerDamageTaken( CTakeDamageInfo *pInfo ) {}; // Base class does nothing.

// Weapon spawn/respawn control
	virtual int WeaponShouldRespawn(CBaseCombatWeapon *pWeapon) { return TRUE; } // should this weapon respawn?
	virtual Vector VecWeaponRespawnSpot(CBaseCombatWeapon *pWeapon) { return vec3_origin; } // where in the world should this weapon respawn?

// Item retrieval
	virtual bool CanHaveItem( CBasePlayer *pPlayer, CItem *pItem ) = 0;// is this player allowed to take this item?
	virtual void PlayerGotItem( CBasePlayer *pPlayer, CItem *pItem ) = 0;// call each time a player picks up an item (battery, healthkit)

// Item spawn/respawn control
	virtual int ItemShouldRespawn(CItem *pItem) { return TRUE; } // Should this item respawn?
	virtual float FlItemRespawnTime(CItem *pItem) { return 0.0f; } // when may this item respawn?
	virtual Vector VecItemRespawnSpot(CItem *pItem) { return vec3_origin; } // where in the world should this item respawn?
	virtual QAngle VecItemRespawnAngles(CItem *pItem) { return vec3_angle; } // what angles should this item use when respawing?

// AI Definitions
	virtual void			InitDefaultAIRelationships( void ) { return; }

// Teamplay stuff	
	virtual int PlayerRelationship(CBaseEntity *pPlayer, CBaseEntity *pTarget) { return GR_NOTTEAMMATE; } // What is the player's relationship with this entity?
	virtual bool PlayerCanHearChat( CBasePlayer *pListener, CBasePlayer *pSpeaker ) = 0;
	virtual void CheckChatText( CBasePlayer *pPlayer, char *pText ) { return; }

	virtual void ChangePlayerTeam( CBasePlayer *pPlayer, const char *pTeamName, bool bKill, bool bGib ) {}
	virtual void UpdateClientData( CBasePlayer *pPlayer ) { };

// Sounds
	virtual bool PlayTextureSounds( void ) { return TRUE; }
	virtual bool PlayFootstepSounds( CBasePlayer *pl ) { return TRUE; }

// NPCs
	virtual bool FAllowNPCs( void ) = 0;//are NPCs allowed

	// Immediately end a multiplayer game
	virtual void EndMultiplayerGame( void ) {}
				    
	// trace line rules
	virtual float WeaponTraceEntity( CBaseEntity *pEntity, const Vector &vecStart, const Vector &vecEnd, unsigned int mask, trace_t *ptr );

	// Setup g_pPlayerResource (some mods use a different entity type here).
	virtual void CreateStandardEntities();

	// Team name, etc shown in chat and dedicated server console
	virtual const char *GetChatPrefix( bool bTeamOnly, CBasePlayer *pPlayer );

	// Location name shown in chat
	virtual const char *GetChatLocation( bool bTeamOnly, CBasePlayer *pPlayer ) { return NULL; }

	// VGUI format string for chat, if desired
	virtual const char *GetChatFormat( bool bTeamOnly, CBasePlayer *pPlayer ) { return NULL; }

	// Whether props that are on fire should get a DLIGHT.
	virtual bool ShouldBurningPropsEmitLight() { return false; }

	virtual bool CanEntityBeUsePushed( CBaseEntity *pEnt ) { return true; }

	virtual void CreateCustomNetworkStringTables( void ) { }

	virtual void ResetMapCycleTimeStamp( void ){ return; }

	virtual void OnNavMeshLoad( void ) { return; }

	virtual void ProcessVerboseLogOutput( void ){}

#endif

	virtual const char *GetGameTypeName( void ){ return NULL; }
	virtual int GetGameType( void ){ return 0; }

	virtual bool ShouldDrawHeadLabels(){ return true; }

	virtual void ClientSpawned( edict_t * pPlayer ) { return; }

	virtual void OnFileReceived( const char * fileName, unsigned int transferID ) { return; }

	virtual bool IsHolidayActive( /*EHoliday*/ int eHoliday ) const { return false; }
	virtual bool IsManualMapChangeOkay( const char **pszReason ) { return true; }

#ifndef CLIENT_DLL
private:
	float m_flNextVerboseLogOutput;
#endif // CLIENT_DLL
};

#ifndef CLIENT_DLL
	void InstallGameRules();
	
	// Create user messages for game here, calls into static player class creation functions
	void RegisterUserMessages( void );
#endif

//-----------------------------------------------------------------------------
// Gets us at the game rules
//-----------------------------------------------------------------------------

extern CGameRules *g_pGameRules;

inline CGameRules* GameRules()
{
	return g_pGameRules;
}

#endif // GAMERULES_H
