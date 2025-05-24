#include "keybind3.hpp"

namespace ui {
	static short int stored_index = -1;
	static bool m_state = false;
	static int m_key = -1;

	std::string m_name[ ui::m_maximum ];
	static bool is_good = false;

	char m_buffer[ 128 ];
	extern const char* m_keys[ 254 ];

	static short int stored_index_list = -1;
	static bool m_state_list = false;

	static int m_time;
	static int m_alpha[ ui::m_maximum ];

	static int m_animation[ ui::m_maximum ];
	static int m_animation_text[ ui::m_maximum ];

	std::vector<std::string> m_Items = { "Always", "Hold on", "Toggle", "Hold off" };

	key_bind::key_bind( int* value, int* bind_type ) {
		this->value = value;
		this->bind_type = bind_type;
	}

	void key_bind::update_list( ) {
		auto text_size = renderr->get_text_width( m_name[ ui::m_control.get_index( ) ].c_str( ), renderr->fonts.menu_font );
		if ( m_animation[ ui::m_control.get_index( ) ] == 100 && m_state_list && stored_index_list == ui::m_control.get_index( ) && ui::m_input.key_pressed( VK_LBUTTON ) && !ui::m_input.mouse_pointer( { pos.x + 20, pos.y + 4 }, { 90, ( float )( m_Items.size( ) * 18 ) } ) ) {
			m_state_list = !m_state_list;
			stored_index_list = -1;
		}

		if ( !ui::m_control.m_animate[ 1 ] && !ui::m_control.m_animate[ 0 ] && !ui::m_control.m_animate[ 2 ] && ui::m_control.is_possible( ) && m_animation[ ui::m_control.get_index( ) ] == 0 && m_time == -1&& ui::m_input.mouse_pointer( { pos.x - text_size, pos.y }, { 35, 10 } ) && ui::m_input.key_pressed( VK_RBUTTON ) ) {
			m_state_list = !m_state_list;
			stored_index_list = ui::m_control.get_index( );
		}

		if ( m_time >= 12 )
			m_time = 12;

		else if ( m_time <= 0 )
			m_time = 0;

		if ( m_state_list )
			m_time++;
		else
			m_time--;

		ui::m_control.m_opened[ ui::m_control.bind ] = m_time;
		if ( stored_index_list == ui::m_control.get_index( ) )
			focused_list = m_state_list;

		ui::m_control.m_opened_state[ ui::m_control.bind ][ ui::m_control.get_index( ) ] = focused_list;
	}

	void key_bind::init_list( ) {
		if ( focused_list ) {
			m_animation[ ui::m_control.get_index( ) ] += 10;
			m_animation_text[ ui::m_control.get_index( ) ] += 15;
		}
		else {
			m_animation[ ui::m_control.get_index( ) ] -= 10;
			m_animation_text[ ui::m_control.get_index( ) ] -= 15;
		}

		if ( m_animation_text[ ui::m_control.get_index( ) ] > 255 )
			m_animation_text[ ui::m_control.get_index( ) ] = 255;

		if ( m_animation_text[ ui::m_control.get_index( ) ] < 0 )
			m_animation_text[ ui::m_control.get_index( ) ] = 0;

		if ( m_animation[ ui::m_control.get_index( ) ] > 100 )
			m_animation[ ui::m_control.get_index( ) ] = 100;

		if ( m_animation[ ui::m_control.get_index( ) ] < 0 )
			m_animation[ ui::m_control.get_index( ) ] = 0;

		std::clamp<int>( m_animation[ ui::m_control.get_index( ) ], 0, 100 );
		std::clamp<int>( m_animation_text[ ui::m_control.get_index( ) ], 0, 255 );

		if ( m_animation[ ui::m_control.get_index( ) ] != 0 ) {
			for ( size_t i{ 0 }; i < m_Items.size( ); i++ ) {
				if ( ui::m_input.key_pressed( VK_LBUTTON ) && ui::m_input.mouse_pointer( { pos.x + 5, pos.y + 4 + ( i * 18 ) }, { 88, 15 } ) ) {
					if ( m_animation[ ui::m_control.get_index( ) ] == 100 ){
						*bind_type = i;
						m_state_list = !m_state_list;
						stored_index_list = -1;
					}
				}

				color_t m_Select = ui::m_input.mouse_pointer( { pos.x + 5, pos.y + 4 + ( i * 18 ) }, { 88, 15 } ) || *bind_type == i ? settings.modify_menu_color ? settings.accent_color : accent : color_t( 120, 120, 120, 255 );
				ui::m_ext_render.text_draw.insert( ui::m_ext_render.text_draw.begin( ), ui::external::e_draw_text( pos.x + 24, pos.y + 4 + ( 18 * i ), m_Items[ i ], m_Select ) );
			}
			ui::m_ext_render.rect_draw.insert( ui::m_ext_render.rect_draw.begin( ), ui::external::e_draw_rect( pos.x + 4 + 1, pos.y + 1, 89 - 2, ( ( 18 * m_Items.size( ) ) + 2 ) * ( m_animation[ ui::m_control.get_index( ) ] / 100.f ), color_t( 60, 60, 60, 255 ) ) );
			ui::m_ext_render.rect_draw.insert( ui::m_ext_render.rect_draw.begin( ), ui::external::e_draw_rect( pos.x + 4, pos.y, 89, ( 18 * m_Items.size( ) + 4 ) * ( m_animation[ ui::m_control.get_index( ) ] / 100.f ), color_t( 15, 15, 15, 155 ) ) );
		}

		ui::m_control.m_animate[ 3 ] = m_state_list;
	}

	void key_bind::update( )
	{
		auto TextSize = renderr->get_text_width( m_name[ ui::m_control.get_index( ) ].c_str( ), renderr->fonts.menu_font );
		if ( !ui::m_control.color_picker_opened && ui::m_control.is_possible( ) && GetAsyncKeyState( VK_LBUTTON ) && ui::m_input.mouse_pointer( { pos.x - TextSize, pos.y }, { 35, 10 } ) ) {
			if ( !m_state )
				m_state = true;

			stored_index = ui::m_control.get_index( );
		}

		if ( stored_index == ui::m_control.get_index( ) )
			focused = m_state;

		if ( focused ) {
			for ( int i = 0; i < 255; i++ ) {
				if ( ui::m_input.key_pressed( i ) ) {
					if ( i == VK_ESCAPE ) {
						*value = -1;
						stored_index = -1;
						return;
					}

					*value = i;
					stored_index = -1;
					return;
				}
			}
		}

		std::string Container = "[ ";
		std::string NextContainer = " ]";

		if ( focused ) {
			m_name[ ui::m_control.get_index( ) ] = "[ ... ]";
		}
		else {
			if ( *value >= 0 ) {
				m_name[ ui::m_control.get_index( ) ] = Container + m_keys[ *value ] + NextContainer;

				if ( m_name[ ui::m_control.get_index( ) ].c_str( ) ) {
					is_good = true;
				}
				else {
					if ( GetKeyNameText( *value << 16, m_buffer, 127 ) )
					{
						m_name[ ui::m_control.get_index( ) ] = m_buffer;
						is_good = true;
					}
				}
			}

			if ( !is_good ) {
				m_name[ ui::m_control.get_index( ) ] = "[ NONE ]";
			}
		}
	}

	void key_bind::draw( ) {
		if ( m_name[ ui::m_control.get_index( ) ] == "" )
			m_name[ ui::m_control.get_index( ) ] = "[ NONE ]";

		auto TextSize = renderr->get_text_width( m_name[ ui::m_control.get_index( ) ].c_str( ), renderr->fonts.menu_font );
		s_text( pos.x - TextSize, pos.y, renderr->fonts.menu_font, m_name[ ui::m_control.get_index( ) ], color_t( 120, 120, 120, 255 ), false, 0 );
	}

	const char* m_keys[ 254 ] = {
		"NONE", "M1", "M2", "BRK", "M3", "M4", "M5",
		"NONE", "Bspc", "Tab", "NONE", "NONE", "NONE", "enter", "NONE", "NONE", "Shift",
		"Ctrl", "Alt", "pau", "Caps", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE",
		"esc", "NONE", "NONE", "NONE", "NONE", "space", "Pgup", "Pgdown", "End", "Home", "Left",
		"up", "right", "down", "NONE", "prnt", "NONE", "prtscr", "ins", "del", "NONE", "0", "1",
		"2", "3", "4", "5", "6", "7", "8", "9", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE",
		"NONE", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U",
		"V", "W", "X", "Y", "Z", "Lftwin", "Rghtwin", "NONE", "NONE", "NONE", "Num0", "Num1",
		"Num2", "Num3", "Num4", "Num5", "Num6", "Num7", "Num8", "Num9", "*", "+", "_", "-", ".", "/", "F1", "F2", "F3",
		"F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12", "f13", "f14", "f15", "f16", "F17", "F18", "F19", "F20",
		"f21",
		"f22", "f23", "f24", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE",
		"num lock", "scroll lock", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE",
		"NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "lshft", "rshft", "lctrl",
		"rctrl", "lmenu", "rmenu", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE",
		"NONE", "NONE", "NONE", "ntrk", "ptrk", "stop", "play", "NONE", "NONE",
		"NONE", "NONE", "NONE", "NONE", ";", "+", ",", "-", ".", "/?", "~", "NONE", "NONE",
		"NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE",
		"NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE",
		"NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "{", "\\|", "}", "'\"", "NONE",
		"NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE",
		"NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE",
		"NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE",
		"NONE", "NONE"
	};
}