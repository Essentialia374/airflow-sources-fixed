#include "indicators.hpp"

const float ind_speed = 0.1f;
float lerp( float t, float a, float b ) {
	return ( 1 - t ) * a + t * b;
}

void gain_mouse_pos( vec2_t& last, vec2_t& new_ ) {
	POINT p_mouse_pos;
	GetCursorPos( &p_mouse_pos );
	ScreenToClient( FindWindow( 0, XOR( "Counter-Strike: Global Offensive" ) ), &p_mouse_pos );
	last = new_;
	new_ = vec2_t( static_cast< int >( p_mouse_pos.x ), static_cast< int >( p_mouse_pos.y ) );
}

char* local_player_died[ ] = {
	"How do you manage to die that easily?",
	"Damn god do you even have a brain?",
	"Please use your keyboard to play this game.",
	"I think your brain might have left for vacantion.",
	"I just cant imagine how can you be that bad at this game.",
	"When i see how you play i get an horible headcache"
};


namespace indicators {
	void supportive_rectangles( ) {
		// booleans and etc for once runing
		static bool once_rand = false;
		static bool once_display = false;
		static int display_time = 0;
		static const int max_time = 600;


		vec2_t position = vec2_t( 600, 200 );

		static vec2_t main_mouse, last_mouse, s_drag;
		bool is_dragging;
		auto x = position.x;
		auto y = position.y;

		gain_mouse_pos( main_mouse, last_mouse );

		// dragging logic.
		if ( main_mouse.x > position.x - s_drag.x && main_mouse.y > position.y - s_drag.y && main_mouse.x < ( position.x - s_drag.x ) + 200 && main_mouse.y < ( position.y - s_drag.y ) + 20 && GetAsyncKeyState( VK_LBUTTON ) ) {
			s_drag.x += main_mouse.x - last_mouse.x;
			s_drag.y += main_mouse.y - last_mouse.y;
			is_dragging = true;
		}

		x -= s_drag.x;
		y -= s_drag.y;


		// drawing
		s_rect_filled( x, y, 200, 200, color_t( 20, 20, 20 ) );

		// eyes
		for ( int i = 0; i < 2; i++ ) {
			s_rect_filled( x + 100 + ( i * ( 200 / 2 ) - 30 * 2 ), y + 30, 10, 10, color_t( 0, 0, 0 ) );
			s_rect_filled( x + 103 + ( i * ( 200 / 2 ) - 30 * 2 ), y + 33, 5, 5, color_t( 0, 162, 232 ) );
			s_rect( x + 100 + ( i * ( 200 / 2 ) - 30 * 2 ), y + 30, 10, 10, color_t( 255, 255, 255 ) );
		}

		// nose
		s_rect( x + 95, y + 60, 10, 35, color_t( 255, 255, 255 ) );

		if ( !local_player_death && !player_died ) { // render mounth simple when local is alive or no kill
			s_rect_filled( x + 25, y + 150, 150, 5, color_t( 255, 255, 255 ) );

			// reset everything
			once_rand = false;
			once_display = false;
		}

		if ( local_player_death && !player_died ) {
			s_rect_filled( x + 30, y + 150, 140, 5, color_t( 255, 255, 255 ) );
			s_rect_filled( x + 25, y + 156, 5, 5, color_t( 255, 255, 255 ) );
			s_rect_filled( x + 25 + 145, y + 156, 5, 5, color_t( 255, 255, 255 ) );

			static int msg_index = 0;
			if ( !once_rand ) {
				msg_index = rand( ) % 6;
				once_rand = true;
			}

			if ( !once_display ) {
				// using main logic/my logic
				if ( display_time < max_time ) {
					display_time++;
				}

				// log it out
				//std::cout << std::to_string( display_time ) + " display time\n";

				if ( display_time < max_time )
					s_text( x + 220, y + 100, renderr->fonts.menu_font, local_player_died[ msg_index ], color_t( 255, 255, 255 ) );

				if ( display_time == max_time ) {
					once_display = true;
					display_time = 0;

					// memory leak fix up
				}
			}
		}
		else if ( player_died && !local_player_death ) {
			s_rect_filled( x + 30, y + 150, 140, 5, color_t( 255, 255, 255 ) );
			s_rect_filled( x + 25, y + 150 - 6, 5, 5, color_t( 255, 255, 255 ) );
			s_rect_filled( x + 25 + 145, y + 150 - 6, 5, 5, color_t( 255, 255, 255 ) );
		}
	}

	void indicator( ) {
		if ( !settings.indicators[ 0 ] )
			return;

		if ( !g_cl.m_local )
			return;

		// do not render while local is dead
		if ( !g_cl.m_local->alive( ) )
			return;

		// we re going to do dymanic position
		vec2_t position = vec2_t( 10, 530 );
		static vec2_t main_mouse, last_mouse, s_drag;
		bool is_dragging;
		auto x = position.x;
		auto y = position.y;

		gain_mouse_pos( main_mouse, last_mouse );

		// dragging logic.
		if ( main_mouse.x > position.x - s_drag.x && main_mouse.y > position.y - s_drag.y && main_mouse.x < ( position.x - s_drag.x ) + 200 && main_mouse.y < ( position.y - s_drag.y ) + 20 && GetAsyncKeyState( VK_LBUTTON ) ) {
			s_drag.x += main_mouse.x - last_mouse.x;
			s_drag.y += main_mouse.y - last_mouse.y;
			is_dragging = true;
		}

		x -= s_drag.x;
		y -= s_drag.y;

		/* thats indicators */
		static bool enabled = true;
		int amount = 0;
		if ( enabled ) { // ad there if_local->is_alive( )
			amount++;
			amount++;
		}
		int height = ( 5 + amount ) * 11 + 12;
		auto add_indicator = [ ]( int add_x, int add_y, std::string indicator_name, const float progress ) -> void {
			std::transform( indicator_name.begin( ), indicator_name.end( ), indicator_name.begin( ), ::toupper );
			s_text( add_x + 7, 19 + add_y, renderr->fonts.indicators, indicator_name, color_t( 255, 255, 255 ), false, 0 );
			s_rect_filled( add_x + 200 - 116 - 10, 21 + add_y, 130, 8, color_t( 20, 20, 20, 100 ) );
			s_gradient( add_x + 200 - 116 - 10, 21 + add_y, progress * 130, 8, Color( 35, 35, 35, 150 ), settings.modify_menu_color ? settings.accent_color : accent, true );
		};

		s_rect_filled( x, y, 210, height, color_t( 35, 35, 35, 150 ) );
		s_rect_filled( x, y + 1, 210, 17, color_t( 20, 20, 20 ) );

		std::string indicators = "indicators";
		std::transform( indicators.begin( ), indicators.end( ), indicators.begin( ), ::toupper );
		auto text_size_indicators = renderr->get_text_width( indicators.c_str( ), renderr->fonts.indicators );
		int centerX = x + ( 210 - text_size_indicators ) / 2;
		s_text( centerX, y + 4, renderr->fonts.indicators, indicators, color_t( 255, 255, 255 ), false, 0 );

		s_rect_filled( x, y + 1, 210, 1, settings.modify_menu_color ? settings.accent_color : accent );
		s_rect( x, y, 210, height, color_t( 15, 15, 15 ) );

		{ // lby
			static float value = 0.f;
			float change = std::abs( math::NormalizedAngle( g_cl.m_body - g_cl.m_angle.y ) );
			float new_value = std::clamp( change, 0.f, change / 180.f ); // ugh
			value = lerp( ind_speed, value, new_value );

			add_indicator( x, y + 0, "fake yaw", value );
		}


		{ // fakelag

			if ( settings.lag_enable && settings.lag_limit > 2 ) {
				static float value = 0.f;
				auto chocked_command = g_csgo.m_cl->m_choked_commands;
				auto fl_value = ( int )settings.lag_limit;

				static int choked = 0;
				if ( chocked_command )
					choked++;
				else
					choked = 0;

				choked = std::clamp( choked, 0, fl_value );
				value = lerp( ind_speed, value, float( choked / fl_value ) );

				add_indicator( x, y + 14, "fakelag", value );
			}
		}

		{ /* doubletap */
			static float value = 0.f;

			int custom_shift{};
			int shift_amm{};

			if ( g_tickbase_control.is_charged( ) )
				shift_amm = 14;
			else
				shift_amm = 0;

			float new_value = std::clamp( ( float )shift_amm, 0.f, ( float )shift_amm / 14.f ); // uh?
			value = lerp( ind_speed, value, new_value );

			add_indicator( x, y + 14 + 14, "exploit", value );
		}

		{ /* inaccuracy */
			static float value = 0.f;
			float new_value;
			new_value = 0; // it was crash bro fuck it, random it xdd hah

			value = lerp( ind_speed, value, new_value );

			add_indicator( x, y + 14 + 14 + 14, "inaccuracy", value );
		}

		{ // velocity

			static float value = 0.f;
			static auto maxvel = g_csgo.m_cvar->FindVar( HASH( "sv_maxvelocity" ) );

			if ( maxvel ) {

				float newvalue = std::clamp( g_cl.m_local->m_vecVelocity( ).length_2d( ), 0.f, 300.f ) / 300.f; /* FOR NOW cause more retarded servers do max vel at 3500*/
				value = lerp( ind_speed, value, newvalue );

				add_indicator( x, y + 14 + 14 + 14 + 14, "velocity", value );
			}

		}
	}

	void keybind( ) {
		if ( !settings.indicators[ 1 ] )
			return;

		if ( !g_cl.m_local )
			return;

		struct key_binds_t {
			std::string text;
			std::string mode;
		};
		std::vector< key_binds_t > keys{ };

		// keybind_t
		key_binds_t key{ };

		// we re going to do dymanic position
		vec2_t position = vec2_t( 10, 624 );
		static vec2_t main_mouse, last_mouse, s_drag;
		bool is_dragging;
		auto x = position.x;
		auto y = position.y;

		gain_mouse_pos( main_mouse, last_mouse );

		// dragging logic.
		if ( main_mouse.x > position.x - s_drag.x && main_mouse.y > position.y - s_drag.y && main_mouse.x < ( position.x - s_drag.x ) + 200 && main_mouse.y < ( position.y - s_drag.y ) + 20 && GetAsyncKeyState( VK_LBUTTON ) ) {
			s_drag.x += main_mouse.x - last_mouse.x;
			s_drag.y += main_mouse.y - last_mouse.y;
			is_dragging = true;
		}

		x -= s_drag.x;
		y -= s_drag.y;

		auto translate_mode = [ ]( int style ) -> std::string {
			switch ( style ) {
				case 0: {
					return "active";
				} break;
				case 1: {
					return "hold";
				} break;
				case 2: {
					return "toggled";
				} break;
				case 3: {
					return "force off";
				} break;
			}
		};


		if ( g_tickbase_control.m_double_tap ) {
			key_binds_t ind{ };
			ind.mode = translate_mode( settings.dtstyle );
			ind.text = "double-tap";
			keys.push_back( ind );
		}

		int pixel = keys.empty( ) ? 18 : 18 + ( 14 * keys.size( ) ) + 1;

		s_rect_filled( x, y, 210, pixel, color_t( 35, 35, 35, 150 ) );
		s_rect_filled( x, y + 1, 210, 17, color_t( 20, 20, 20 ) );

		std::string indicators2 = "keybinds";
		std::transform( indicators2.begin( ), indicators2.end( ), indicators2.begin( ), ::toupper );
		auto text_size_indicators2 = renderr->get_text_width( indicators2.c_str( ), renderr->fonts.indicators );
		int centerX = x + ( 210 - text_size_indicators2 ) / 2;
		s_text( centerX, y + 4, renderr->fonts.indicators, indicators2, color_t( 255, 255, 255 ), false, 0 );

		s_rect_filled( x, y + 1, 210, 1, settings.modify_menu_color ? settings.accent_color : accent );
		s_rect( x, y, 210, pixel, color_t( 15, 15, 15 ) );

		for ( size_t i{ }; i < keys.size( ); ++i ) {
			auto& indicator = keys[ i ];

			std::string indicators2 = indicator.mode;
			std::transform( indicators2.begin( ), indicators2.end( ), indicators2.begin( ), ::toupper );
			auto text_size_indicators2 = renderr->get_text_width( indicators2.c_str( ), renderr->fonts.indicators );

			int render_x = x + 7; // Starting position of the text
			int render_y = y + pixel - 13;
			int text_render_pos = x + 216 - text_size_indicators2; // Position to render the text

			s_text( x + 5, y + pixel - 13, renderr->fonts.indicators, indicator.text, color_t( 255, 255, 255 ), false, 0 );
			s_text( text_render_pos, y + pixel - 16, renderr->fonts.indicators, indicator.mode, color_t( 255, 255, 255 ), false, 0 );
		}

	}


	void watermark( ) {
		s_rect_filled( 1650, 10, 195, 18, color_t( 35, 35, 35, 150 ) );
		s_rect_filled( 1650, 10, 195, 18, color_t( 20, 20, 20 ) );

		std::string indicators = "deathrow.technology | debug | dutu1337";
		std::transform( indicators.begin( ), indicators.end( ), indicators.begin( ), ::toupper );
		auto text_size_indicators = renderr->get_text_width( indicators.c_str( ), renderr->fonts.indicators );

		s_text( 1660, 15, renderr->fonts.indicators, indicators, color_t( 255, 255, 255 ), false, 0 );

		s_rect_filled( 1650, 11, 195, 1, settings.modify_menu_color ? settings.accent_color : accent );
		s_rect( 1650, 10, 195, 18, color_t( 15, 15, 15, 155 ) );

	}
}