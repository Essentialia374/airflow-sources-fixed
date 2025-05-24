#include "input_sys.hpp"

void UTILS::InitializeInput( const std::string szWindowName ) {
	for ( int i = 0; i < 256; i++ ) {
		m_bPrevKeyState[ i ] = m_bKeyState[ i ];
		m_bKeyState[ i ] = GetAsyncKeyState( i );
	}
		
	POINT vMousePos;
	GetCursorPos( &vMousePos );
	ScreenToClient( FindWindow( nullptr, szWindowName.data( ) ), &vMousePos );

	m_vPrevMousePos = m_vMousePos;
	m_vMousePos = { ( float )vMousePos.x, ( float )vMousePos.y };

	if ( !m_bInputLoopStarted )
		m_bInputLoopStarted = true;
}

bool UTILS::KeybindMethod( const int iKey, const int iMethod, bool* pToggled, bool old_method )
{
	switch ( iMethod )
	{
	case 0: // Always on
		return true;
		break;
	case 1: // Hold
		return GetAsyncKeyState( iKey );
		break;
	case 2: // Toggle
	{
		if ( old_method ) {
			if ( KeyPressed( iKey ) )
				*pToggled = !*pToggled;

			return *pToggled;
		}
		else {
			return GetKeyState( iKey );
		}
	}
	break;
	case 3: // Force off
		return !KeyDown( iKey );
		break;
	}
	return false;
}

bool UTILS::KeyPressed( const int key )
{
	return m_bKeyState[ key ] && !m_bPrevKeyState[ key ];
}

bool UTILS::KeyDown( const int key )
{
	return m_bKeyState[ key ];
}

bool UTILS::KeyReleased( const int key )
{
	return !m_bKeyState[ key ] && m_bPrevKeyState[ key ];
}

bool UTILS::MouseInRegion( const int x, const int y, const int w, const int h )
{
	return m_vMousePos.x > x && m_vMousePos.y > y && m_vMousePos.x < w + x && m_vMousePos.y < h + y;
}

std::vector<std::string> UTILS::SplitStr( const char* str, const char separator )
{
	std::vector<std::string> output;
	std::string substring;
	std::istringstream stream{ str };

	while ( std::getline( stream, substring, separator ) )
		output.push_back( substring );

	return output;
}

uint32_t UTILS::Hash( const char* str, const uint32_t value )
{
	return *str ? Hash( str + 1, ( value ^ *str ) * 0x1000193ull ) : value;
}