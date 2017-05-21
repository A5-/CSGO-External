#include "..\include.h"

Interface* g_pInterface = new Interface();

void Interface::OnSetup()
{
	m_hSteamApi = process->LoadRemote( "steam_api.dll" );

	if( !m_hSteamApi )
		return;

	m_hTier0 = process->LoadRemote( "tier0.dll" );

	if( !m_hTier0 )
		return;

	m_hVstdlib = process->LoadRemote( "vstdlib.dll" );

	if( !m_hVstdlib )
		return;

	m_hPhonon3d = process->LoadRemote( "phonon3d.dll" );

	if ( !m_hPhonon3d )
		return;

	m_hClient = process->LoadRemote( "client.dll" );

	if( !m_hClient )
		return;

	m_hEngine = process->LoadRemote( "engine.dll" );

	if( !m_hEngine )
		return;

	CreateInterfaceFn ClientFactory = ( CreateInterfaceFn )GetProcAddress( m_hClient, "CreateInterface" );

	if( !ClientFactory )
		return;

	m_pClient = ( CHLClient* )ClientFactory( "VClient018", nullptr );

	if( !m_pClient )
		return;

	m_pEntList = ClientFactory( "VClientEntityList003", nullptr );

	if( !m_pEntList )
		return;

	CreateInterfaceFn EngineFactory = ( CreateInterfaceFn )GetProcAddress( m_hEngine, "CreateInterface" );

	if( !EngineFactory )
		return;

	m_pEngine = ( CEngineClient* )EngineFactory( "VEngineClient014", nullptr );

	if( !m_pEngine )
		return;
}

void Interface::OnShutdown()
{
	if( m_hEngine )
	{
		FreeLibrary( m_hEngine );
	}

	if( m_hClient )
	{
		FreeLibrary( m_hClient );
	}

	if( m_hSteamApi )
	{
		FreeLibrary( m_hSteamApi );
	}
}