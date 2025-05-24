#pragma once
#include "../gui_main/gui3.hpp"

namespace indicators {
	void supportive_rectangles( );
	void indicator( );
	void keybind( );
	void watermark( );

	// supportive angles.
	inline bool local_player_death{};
	inline bool player_died[ 64 ]{};
}