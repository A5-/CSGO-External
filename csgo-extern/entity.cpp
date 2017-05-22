#include "include.h"

/*I have a big dick so the skull fucking will hurt*/

Entity LocalEntity;
Entity EntityList[ 65 ];

Entity::Entity()
{
	m_iIndex = NULL;

	m_dwPointer = NULL;
	m_dwBoneMatrix = NULL;

	m_iTeamNum = NULL;
	m_iHealth = NULL;
	m_iFlags = NULL;


	m_bDead = true;
}

Entity::~Entity()
{

}

void Entity::Update( int iIndex )
{
	Clear();

	m_iIndex = iIndex;

	if( !m_iIndex )
		return;

	m_dwPointer = process->Read<DWORD_PTR>( client->GetImage() + g_pStatic->EntityListBase + 40 + m_iIndex * 8 );

	if( !m_dwPointer )
		return;

	m_dwPointer -= 8;

	m_iHealth = process->Read<int>( m_dwPointer + g_pDynamic->m_iHealth );

	m_dwBoneMatrix		= process->Read<DWORD_PTR>( m_dwPointer + g_pDynamic->m_BoneMatrix );

	if( !m_dwBoneMatrix )
		return;

	process->Read( m_dwBoneMatrix, m_BoneMatrix, sizeof( Matrix3x4[ 128 ] ) );

	m_vOrigin				= process->Read<Vector3>( m_dwPointer + g_pDynamic->m_vecOrigin );
	m_vEye					= m_vOrigin + process->Read<Vector3>( m_dwPointer + g_pDynamic->m_vecViewOffset );
	m_vEyeAngles			= process->Read<Vector3>( m_dwPointer + g_pDynamic->m_angEyeAngles );
	m_vVelocity				= process->Read<Vector3>( m_dwPointer + g_pDynamic->m_vecVelocity );
	m_vPunch				= process->Read<Vector3>( m_dwPointer + g_pDynamic->m_aimPunchAngle );
	m_iTeamNum				= process->Read<int>( m_dwPointer + g_pDynamic->m_iTeamNum );
	m_iFlags				= process->Read<int>( m_dwPointer + g_pDynamic->m_fFlags );
	m_iCrossHairID			= process->Read<int>( m_dwPointer + 0xAA70 );				
	m_hActiveWeapon			= process->Read<int>( m_dwPointer + g_pDynamic->m_hActiveWeapon );
	m_iCompetitiveRanking	= process->Read<int>( g_pStatic->GameResources + g_pDynamic->m_iCompetitiveRanking + m_iIndex * 4 );
	m_iCompetitiveWins		= process->Read<int>( g_pStatic->GameResources + g_pDynamic->m_iCompetitiveWins + m_iIndex * 4 );
	m_bDead					= process->Read<bool>( m_dwPointer + g_pDynamic->m_lifeState );
	m_bIsDormant			= process->Read<bool>( m_dwPointer + 0xE9 );
	m_i64SpottedByMask		= process->Read<INT64>( m_dwPointer + 0x97C );
	m_bSpotted				= process->Read<bool>( m_dwPointer + 0x939 );

	// Player name with player_info_t
	// credits to brinkz

	DWORD_PTR ECX, EDX, EAX, Info;

	int iEntIndex = m_iIndex - 1;

	if( !g_pStatic->EnginePointer )
		return;

	ECX = process->Read<DWORD_PTR>( g_pStatic->EnginePointer + 0x523C );

	if( !ECX )
		return;

	EDX = process->Read<DWORD_PTR>( ECX + 0x3C );

	if( !EDX )
		return;

	EAX = process->Read<DWORD_PTR>( EDX + 0xC );

	if( !EAX )
		return;

	Info = process->Read<DWORD_PTR>( EAX + 0x20 + iEntIndex * 0x34 );

	if( !Info )
		return;

	process->Read( Info + 0x10, m_pName, 64 );
	process->Read( Info + 0x94, m_pSteamID, 64 );

}

DWORD_PTR Entity::GetPointer()
{
	return m_dwPointer;
}

DWORD_PTR Entity::GetLocalPointer()
{
	return m_dwLocalPointer;
}

DWORD_PTR Entity::GetBoneMatrix()
{
	return m_dwBoneMatrix;
}

Matrix3x4 Entity::GetBoneMatrix( int iBone )
{
	return m_BoneMatrix[ iBone ];
}

Vector3 Entity::GetPunchAngles()
{
	return m_vPunch;
}

Vector3 Entity::GetOrigin()
{
	return m_vOrigin;
}

Vector3 Entity::GetEyeAngles()
{
	return m_vEyeAngles;
}

Vector3 Entity::GetEyePosition()
{
	return m_vEye;
}

Vector3 Entity::GetVelocity()
{
	return m_vVelocity;
}

Vector3 Entity::GetBonePosition( int iBone )
{
	return Vector3( m_BoneMatrix[ iBone ].m[ 0 ][ 3 ], m_BoneMatrix[ iBone ].m[ 1 ][ 3 ], m_BoneMatrix[ iBone ].m[ 2 ][ 3 ] );
}

int Entity::GetActiveWeapon()
{
	int weaponId = process->Read<int>( GetActiveWeaponBase() + g_pDynamic->m_iWeaponId );
	if( weaponId > 50 || weaponId < 0 ) // If number is greater than array size return 0 to prevent crash
		weaponId = 42;	// set weapon ID to knife

	return weaponId;
}

DWORD_PTR Entity::GetActiveWeaponBase()
{ 
	DWORD_PTR dwBaseCombatWeaponIndex = process->Read< DWORD_PTR >( m_dwPointer + g_pDynamic->m_hActiveWeapon );
	dwBaseCombatWeaponIndex &= 0xFFF;
	return process->Read<DWORD_PTR>( ( g_pStatic->EntityList + dwBaseCombatWeaponIndex * 0x10 ) - 0x10 );
}

bool Entity::IsActiveWeaponNotVaild()
{
	int WeaponId = LocalEntity.GetActiveWeapon();

	return( WeaponId == WEAPON_KNIFEGG || WeaponId == WEAPON_KNIFE || WeaponId == WEAPON_FLASHBANG || WeaponId == WEAPON_HEGRENADE || WeaponId == WEAPON_SMOKEGRENADE
			|| WeaponId == WEAPON_MOLOTOV || WeaponId == WEAPON_DECOY || WeaponId == WEAPON_INCGRENADE || WeaponId == WEAPON_C4 );
}

bool Entity::IsActiveWeaponPistol()
{
	int WeaponId = LocalEntity.GetActiveWeapon();

	return( WeaponId == WEAPON_DEAGLE || WeaponId == WEAPON_ELITE || WeaponId == WEAPON_FIVESEVEN || WeaponId == WEAPON_GLOCK
			|| WeaponId == WEAPON_P228 || WeaponId == WEAPON_USP || WeaponId == WEAPON_TEC9 || WeaponId == WEAPON_TASER || WeaponId == WEAPON_HKP2000 );
}

int Entity::GetTeamNum()
{
	return m_iTeamNum;
}

int Entity::GetHealth()
{
	return m_iHealth;
}

int Entity::GetCrosshairEnt()
{
	return m_iCrossHairID;
}

int Entity::GetMatchMakingRank()
{
	if( m_iCompetitiveRanking > 18 || m_iCompetitiveRanking < 0 )
		return 0;
	return m_iCompetitiveRanking;
}

int Entity::GetMatchMakingWins()
{
	return m_iCompetitiveWins;
}

int Entity::GetFlags()
{
	return m_iFlags;
}

float Entity::GetFlashDuration()
{
	return process->Read<float>( m_dwPointer + g_pDynamic->m_flFlashDuration );
}

void Entity::SetFlashDuration()
{
	process->Write<float>( m_dwPointer + g_pDynamic->m_flFlashDuration, 0.0f );
}

int Entity::GetTickBase()
{
	return process->Read<int>( m_dwPointer + 0x17CC );
}

bool Entity::IsValid()
{
	if( !GetPointer() )
		return false;

	if( !GetBoneMatrix() )
		return false;

	if( !GetTeamNum() )
		return false;

	if( IsDead() )
		return false;

	if( IsDormant() )
		return false;

	if( GetOrigin().IsZero() )
		return false;

	if( GetHealth() < 1 )
		return false;

	return true;
}

bool Entity::IsPlayer()
{
	if( !GetPointer() )
		return false;

	if( !GetName() )
		return false;

	if( !GetTeamNum() )
		return false;

	return true;
}

bool Entity::IsDead()
{
	return m_bDead;
}

bool Entity::IsDormant()
{
	return m_bIsDormant;
}

bool Entity::IsSpottedByMask()
{
	return ( m_i64SpottedByMask & ( 1 << ( g_pEngine->GetLocalPlayer() - 1 ) ) );
}

bool Entity::IsSpotted()
{
	return m_bSpotted;
}

void Entity::CheckVisible( int bone )
{	
	std::future<bool> ret = std::async( std::launch::async, &BSPMap::Visible, g_pBSP, LocalEntity.GetEyePosition(), GetBonePosition( bone ) );
	m_bVisible = ret.get(); // Spawns an async thread then return value to m_bVisible
}

bool Entity::IsVisible()
{
	return m_bVisible;
}

const char* Entity::GetName()
{
	return m_pName;
}

const char* Entity::GetSteamID()
{
	return m_pSteamID;
}

void Entity::Clear()
{
	m_iIndex = NULL;
	m_flFlashDuration = NULL;
	m_dwPointer = NULL;
	m_dwBoneMatrix = NULL;

	memset( m_BoneMatrix, NULL, sizeof( Matrix3x4[ 128 ] ) );

	m_vOrigin = Vector3( 0, 0, 0 );
	m_vEye = Vector3( 0, 0, 0 );

	m_iTeamNum = NULL;
	m_iHealth = NULL;
	m_iFlags = NULL;
	m_iCrossHairID = NULL;
	m_hActiveWeapon = NULL;
	m_iWeaponId = NULL;
	m_iCompetitiveRanking = NULL;
	m_iCompetitiveWins = NULL;

	m_bDead = true;
	m_i64SpottedByMask = NULL;

	m_flFlashDuration = NULL;
	m_flServerTime = NULL;

	memset( m_pName, NULL, sizeof( char[ 64 ] ) );
}
