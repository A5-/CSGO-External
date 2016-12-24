#include "..\include.h"

Misc* g_pMisc = new Misc();

void Misc::BunnyHop()
{
	if( GetAsyncKeyState( VK_SPACE ) && m_bhop )
		if( LocalEntity.GetFlags() & FL_ONGROUND )
			SendMessage( process->GetWindow(), WM_MOUSEWHEEL, -45, NULL );
}

void Misc::SetBHop()
{
	m_bhop = !m_bhop;
}

bool Misc::GetBHop()
{
	return m_bhop;
}

void Misc::AutoStrafeLeftDirection( int direction )
{
	switch( direction )
	{
	case 1:
		process->WriteProtected<BYTE>( g_pStatic->ConvarMoveRight, 0 );
		process->WriteProtected<BYTE>( g_pStatic->ConvarMoveLeft, 255 );
		return;
	case 2:
		process->WriteProtected<BYTE>( g_pStatic->ConvarMoveForward, 0 );
		process->WriteProtected<BYTE>( g_pStatic->ConvarMoveBackward, 255 );
		return;
	case 3:
		process->WriteProtected<BYTE>( g_pStatic->ConvarMoveRight, 255 );
		process->WriteProtected<BYTE>( g_pStatic->ConvarMoveLeft, 0 );
		return;
	case 4:
		process->WriteProtected<BYTE>( g_pStatic->ConvarMoveForward, 255 );
		process->WriteProtected<BYTE>( g_pStatic->ConvarMoveBackward, 0 );
		return;
	}
}

void Misc::AutoStrafeRightDirection( int direction )
{
	switch( direction )
	{
	case 1:
		process->WriteProtected<BYTE>( g_pStatic->ConvarMoveRight, 255 );
		process->WriteProtected<BYTE>( g_pStatic->ConvarMoveLeft, 0 );
		return;
	case 2:
		process->WriteProtected<BYTE>( g_pStatic->ConvarMoveForward, 255 );
		process->WriteProtected<BYTE>( g_pStatic->ConvarMoveBackward, 0 );
		return;
	case 3:
		process->WriteProtected<BYTE>( g_pStatic->ConvarMoveRight, 0 );
		process->WriteProtected<BYTE>( g_pStatic->ConvarMoveLeft, 255 );
		return;
	case 4:
		process->WriteProtected<BYTE>( g_pStatic->ConvarMoveForward, 0 );
		process->WriteProtected<BYTE>( g_pStatic->ConvarMoveBackward, 255 );
		return;
	}
}
void Misc::AutoStrafeNullDirection()
{
	process->WriteProtected<BYTE>( g_pStatic->ConvarMoveLeft, 0 );
	process->WriteProtected<BYTE>( g_pStatic->ConvarMoveRight, 0 );
	process->WriteProtected<BYTE>( g_pStatic->ConvarMoveForward, 0 );
	process->WriteProtected<BYTE>( g_pStatic->ConvarMoveBackward, 0 );
}

void Misc::AutoStrafe()
{
	if( LocalEntity.GetFlags() == ( FL_INAIR ) && !m_autostrafe == 0 || LocalEntity.GetFlags() == ( FL_INAIRDUCKING ) && !m_autostrafe == 0 ) // http://en.wikipedia.org/wiki/Operator_%28computer_programming%29
	{
		POINT cursor_pos;
		int x, y;
		g_pOverlay->GetCenterScreen( &x, &y );
		GetCursorPos( &cursor_pos );
		m_strafecheck = false;
		if( !( GetKeyState( 0x44 ) & KEYDOWN ) )
			if( !( GetKeyState( 0x41 ) & KEYDOWN ) )
				if( !( GetKeyState( 0x53 ) & KEYDOWN ) )
					if( !( GetKeyState( 0x57 ) & KEYDOWN ) )
					{
						if( cursor_pos.x > x )
						{
							Misc::AutoStrafeRightDirection( m_autostrafe );
							m_strafecheck = true;
						}

						else if( cursor_pos.x < x )
						{
							Misc::AutoStrafeLeftDirection( m_autostrafe );
							m_strafecheck = true;
						}
						else if( cursor_pos.x = x )
						{
							Misc::AutoStrafeNullDirection();
							m_strafecheck = false;
						}
					}
	}
	else if( !GetAsyncKeyState( VK_SPACE ) && m_strafecheck )
	{
		Misc::AutoStrafeLeftDirection( m_autostrafe );
		if( ( GetKeyState( 0x44 ) & KEYDOWN ) )
			process->WriteProtected<BYTE>( g_pStatic->ConvarMoveRight, 255 );

		if( ( GetKeyState( 0x41 ) & KEYDOWN ) )
			process->WriteProtected<BYTE>( g_pStatic->ConvarMoveLeft, 255 );

		m_strafecheck = false;
	}


}

void Misc::SetAutoStrafe( int direction )
{
	m_autostrafe = direction;
}

int Misc::GetAutoStrafe()
{
	return m_autostrafe;
}

void Misc::NoFlash()
{
	if( LocalEntity.GetFlashDuration() > 0.0f && m_noflash )
		LocalEntity.SetFlashDuration();
}

void Misc::SetNoFlash()
{
	m_noflash = !m_noflash;
}

bool Misc::GetNoFlash()
{
	return m_noflash;
}

void Misc::FakeLag()
{
	if( m_fakelag )
	{
		g_pEngine->SetSendPacket( false );
		Sleep( 2 ); // we want to freeze 12 ticks worth of packets, this should do 
		g_pEngine->SetSendPacket( true );
	}

	if( m_toggleFakelag ) // make sure we set it on
	{
		g_pEngine->SetSendPacket( true );
		m_toggleFakelag = false;
	}

}

void Misc::SetFakeLag()
{
	m_fakelag = !m_fakelag;
}

bool Misc::GetFakeLag()
{
	return m_fakelag;
}

void Misc::NoHands() // Have to set before you join a game
{

	char nohands[ 256 ] = { 0 };
	process->Write<char[ 256 ]>( LocalEntity.GetPointer() + g_pDynamic->m_szArmsModel, nohands );
	g_pEngine->ForceUpdate();
}

void Misc::SetNoHands()
{
	m_nohands = !m_nohands;
}

bool Misc::GetNoHands()
{
	return m_nohands;
}

void Misc::SkinChanger()
{
	if( !m_skinchanger )
		return;

	if( LocalEntity.IsActiveWeaponNotVaild() )
		return;

	FallbackSkin_t oldskin = process->Read<FallbackSkin_t>( LocalEntity.GetActiveWeaponBase() + 0x14F0 );


	FallbackSkin.quality = 0;
	FallbackSkin.prefab = 0;
	FallbackSkin.paintkitId = m_skinId;
	FallbackSkin.seed = rand() % 420911;	 // fuck phone idgaf about unrandom
	FallbackSkin.wear = 0.001f;
	FallbackSkin.stattrack = 420000;

	char customName[ 32 ] = "Gamerfood Memes";

	while( process->Read<int>( LocalEntity.GetActiveWeaponBase() + 0x14F8 ) != FallbackSkin.paintkitId ) // this loop should allow skins to change when you buy them
	{
		process->Write<int[ 2 ]>( LocalEntity.GetActiveWeaponBase() + 0x1330, { -1, -1 } );	// item id low / high	
		process->Write<char[ 32 ]>( LocalEntity.GetActiveWeaponBase() + 0x13AC, customName );	// weapon custom name
		process->Write<FallbackSkin_t>( LocalEntity.GetActiveWeaponBase() + 0x14F0, FallbackSkin );
	}

	if( GetAsyncKeyState( 0x50 ) ) // if skin didn't update when you bought them or you picked up a skin, press P to force an update but will also reset all other skins to default
		process->Write<int>( g_pStatic->EnginePointer + 0x154, -1 ); // force update

}

void Misc::SetSkinChanger()
{
	m_skinchanger = !m_skinchanger;
}

bool Misc::GetSkinChanger()
{
	return m_skinchanger;
}

void Misc::SetSkinID( int skinId )
{
	m_skinId = skinId;
}

int Misc::GetSkinID()
{
	return m_skinId;
}

void Misc::Derank()
{
	if( !m_derank )
		return;

	int moveLeft = rand() % 200,
		moveRight = rand() % 200,
		shoot = rand() % 100;

	if( moveLeft == 0 )
	{
		process->WriteProtected<BYTE>( g_pStatic->ConvarMoveLeft, 255 );
		process->WriteProtected<BYTE>( g_pStatic->ConvarMoveRight, 0 );
	}

	if( moveRight == 0 )
	{
		process->WriteProtected<BYTE>( g_pStatic->ConvarMoveLeft, 0 );
		process->WriteProtected<BYTE>( g_pStatic->ConvarMoveRight, 255 );
	}

	if( shoot == 0 )
	{
		process->WriteProtected<BYTE>( g_pStatic->ConvarAttack, 5 );
		Sleep( 1 );
		process->WriteProtected<BYTE>( g_pStatic->ConvarAttack, 4 );
	}
}

void Misc::SetDerank()
{
	m_derank = !m_derank;
}

void Misc::InsultGen()
{
	if( !m_insultgen )
		return;

	char str[ 255 ];
	strcpy( str, "say " );
	strcat( str, g_pWebShit->ReadInsult().c_str() );

	g_pEngine->ClientCMD( str );
	Sleep( 500 );
}

void Misc::SetInsultGen()
{
	m_insultgen = !m_insultgen;
}

void Misc::IrcChatter()
{
	if (!m_ircchatter)
		return;

	char str[255];
	if (str)
	{
		strcpy(str, "say ");
		strcat(str, g_pWebShit->ReadIrc().c_str());
		g_pEngine->ClientCMD(str);
	}
	Sleep(1000); //big sleeps
}

void Misc::SetIrcChatter()
{
	m_ircchatter = !m_ircchatter;
}

void Misc::ReadYiff()
{
	if( m_readyiff )
		return;

	std::ifstream yifffile;
	yifffile.open( "yiff.txt" );
	std::string line;

	while( std::getline( yifffile, line ) )
		m_yifflines.push_back( line );
	
	yifffile.close();
}

void Misc::Yiff()
{
	if( !m_yiff )
		return;

	int line = rand() % m_yifflines.size() - 1;
	std::string target = EntityList[ rand() % 9 + 1 ].GetName(),
				owner  = EntityList[ rand() % 9 + 1 ].GetName(),
				self   = LocalEntity.GetName();

	char str[ 512 ];
	strcpy( str, "say " );

	while( m_yifflines[ line ].find( "$TARGET" ) != std::string::npos )
		m_yifflines[ line ].replace( m_yifflines[ line ].find( "$TARGET" ), 7, target );

	while( m_yifflines[ line ].find( "$OWNER" ) != std::string::npos )
		m_yifflines[ line ].replace( m_yifflines[ line ].find( "$OWNER" ), 6, owner );

	while( m_yifflines[ line ].find( "$SELF" ) != std::string::npos )
		m_yifflines[ line ].replace( m_yifflines[ line ].find( "$SELF" ), 5, self );

	strcat( str, m_yifflines[ line ].c_str() );

	g_pEngine->ClientCMD( str );
	Sleep( 500 );
}

void Misc::SetYiff()
{
	m_yiff = !m_yiff;
}
