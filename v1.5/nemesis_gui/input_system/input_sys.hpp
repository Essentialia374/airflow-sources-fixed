#pragma once
#include "../../../includes.h"

namespace UTILS
{
	void InitializeInput( const std::string szWindowName );

	bool KeybindMethod( const int iKey, const int iMethod, bool* pToggled, bool old_method = false );
	bool KeyPressed( const int iKey );
	bool KeyDown( const int iKey );
	bool KeyReleased( const int iKey );

	bool MouseInRegion( const int iX, const int iY, const int iW, const int iH );

	std::vector<std::string> SplitStr( const char* szName, char szSeperator );
	uint32_t Hash( const char* szName, uint32_t uHash = 0x811c9dc5 );

	inline bool m_bKeyState[ 256 ];
	inline bool m_bPrevKeyState[ 256 ];
	inline bool m_bInputLoopStarted = false;

	inline vec2_t m_vPrevMousePos, m_vMousePos;
}