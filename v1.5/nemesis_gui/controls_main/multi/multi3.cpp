#include "multi3.hpp"

namespace ui {
	static auto stored_index = -1;
	static bool state = false;

	static int m_time;
	static int m_alpha[ ui::m_maximum ];
	static int animation[ ui::m_maximum ];
	static int animation_time[ ui::m_maximum ];

	multi_combo::multi_combo( std::string name ) {
		this->name = name;
		this->items = items;
	}

	void multi_combo::update( ) {
		if ( animation_time[ ui::m_control.get_index( ) ] == 100 && state && stored_index == ui::m_control.get_index( ) && ui::m_input.key_pressed( VK_LBUTTON ) && !ui::m_input.mouse_pointer( { pos.x + 5, pos.y + 40 }, { 350, ( float )( items.size( ) * 23 ) } ) ) {
			state = !state;
			stored_index = -1;
		}

		if ( ui::m_input.key_pressed( VK_ESCAPE ) ) {
			state = false;
		}

		if ( !ui::m_control.m_animate[ 0 ] && animation[ ui::m_control.get_index( ) ] == 0 && !ui::m_control.color_picker_opened && ui::m_control.m_opened[ ui::m_control.multicombo ] == -1 && m_time == -1 && ui::m_input.key_pressed( VK_LBUTTON ) && ui::m_input.mouse_pointer( { pos.x + 5, pos.y + 16 }, { 350, 23 } ) && !ui::m_control.m_animate[ 3 ] ) {
			state = !state;
			stored_index = ui::m_control.get_index( );
		}


		/* we are going to setup the main animation and time */
		if ( m_time >= 12 )
			m_time = 12;
		else if ( m_time <= 0 )
			m_time = 0;

		/* we are going to setup the open state */
		if ( state )
			m_time++;
		else
			m_time--;

		/* other checks for sanity */
		ui::m_control.m_opened[ ui::m_control.multicombo ] = m_time;

		/* check for stored index and setup trought state */
		if ( stored_index == ui::m_control.get_index( ) )
			focused = state;

		/* check the focus and setup it */
		ui::m_control.m_opened_state[ ui::m_control.multicombo ][ ui::m_control.get_index( ) ] = focused;
	}

	void multi_combo::draw( ) {
		auto arrow = [ ]( float x, float y, bool turn ) {
			if ( turn )
			{
				for ( float i = 7; i >= 4; --i ) {
					float offset = 7 - i;
					s_line( x + offset, y + offset, x + offset + std::clamp( i - offset, ( float )0, ( float )10 ), y + offset, color_t( 170, 170, 170 ) );
				}
			}
			else
			{
				s_line( x + 3, y + 1, x + 4, y + 1, color_t( 170, 170, 170 ) );
				s_line( x + 2, y + 2, x + 5, y + 2, color_t( 170, 170, 170 ) );
				s_line( x + 1, y + 3, x + 6, y + 3, color_t( 170, 170, 170 ) );
				s_line( x, y + 4, x + 7, y + 4, color_t( 170, 170, 170 ) );
			}
		};

		auto combo_drawing = [ ]( float x, float y, float w, float h, std::string name, std::string selected, bool focus ) -> void {
			s_gradient( x, y, w, h, color_t( 41, 41, 41 ), color_t( 49, 49, 49 ), false );
			s_rect( x, y, w, h, color_t( 15, 15, 15, 155 ) );
			s_text( x, y - 15, renderr->fonts.menu_font, name, color_t( 120, 120, 120, 255 ), false, 0 );

			if ( !focus )
				s_text( x + 8, y + 5, renderr->fonts.menu_font, selected, color_t( 120, 120, 120, 255 ), false, 0 );
			else
				s_text( x + 8, y + 5, renderr->fonts.menu_font, "To close this please press ESC!", color_t( 120, 120, 120, 255 ), false, 0 );
		};

		std::string to_display;
		for ( int i = 0; i < items.size( ); i++ ) {
			if ( *items[ i ].value )
			{
				if ( to_display != "" )
					to_display.append( ", " );

				to_display.append( items[ i ].name );
			}
		}

		if ( to_display == "" )
			to_display.append( "None" );

		auto TextSize = renderr->get_text_width( to_display.c_str( ), renderr->fonts.menu_font );
		if ( pos.x + 2 + TextSize > pos.x + 100 )
		{
			to_display.resize( 10 );
			to_display.append( "..." );
		}

		/* after that we are going to setup open combo so stay close */
		combo_drawing( pos.x + 5, pos.y + 14, 350, 23, name, to_display, focused );
		arrow( pos.x + 337, pos.y + 23, !focused );

		/* setup focus and animations */
		auto setup_focus = [ ]( bool focused, int animation[ 1000 ], int animation_time[ 1000 ] ) -> void {
			/* focused setup */
			if ( focused ) {
				animation[ ui::m_control.get_index( ) ] += 10;
				animation_time[ ui::m_control.get_index( ) ] += 15;
			}
			else {
				animation[ ui::m_control.get_index( ) ] -= 10;
				animation_time[ ui::m_control.get_index( ) ] -= 15;
			}

			if ( animation_time[ ui::m_control.get_index( ) ] > 255 )
				animation_time[ ui::m_control.get_index( ) ] = 255;

			if ( animation_time[ ui::m_control.get_index( ) ] < 0 )
				animation_time[ ui::m_control.get_index( ) ] = 0;

			if ( animation[ ui::m_control.get_index( ) ] > 100 )
				animation[ ui::m_control.get_index( ) ] = 100;

			if ( animation[ ui::m_control.get_index( ) ] < 0 )
				animation[ ui::m_control.get_index( ) ] = 0;

			std::clamp<int>( animation[ ui::m_control.get_index( ) ], 0, 100 );
			std::clamp<int>( animation_time[ ui::m_control.get_index( ) ], 0, 255 );
		};
		setup_focus( focused, animation, animation_time );

		
		if ( animation[ ui::m_control.get_index( ) ] != 0 ) {
			for ( size_t i{ 0 }; i < items.size( ); i++ ) {
				if ( ui::m_input.key_pressed( VK_LBUTTON ) && ui::m_input.mouse_pointer( { pos.x, pos.y + 40 + ( i * 23 ) }, { 350, 23 } ) ) {
					if ( animation[ ui::m_control.get_index( ) ] == 100 ) {
						*items[ i ].value = !*items[ i ].value;
					}
				}

				color_t m_Select = ui::m_input.mouse_pointer( { pos.x, pos.y + 41 + ( i * 23 ) }, { 350, 23 } ) || *items[ i ].value ? settings.modify_menu_color ? settings.accent_color : accent : color_t( 120, 120, 120, 150 );
				ui::m_ext_render.text_draw.insert( ui::m_ext_render.text_draw.begin( ), ui::external::e_draw_text( pos.x + 10, pos.y + 45 + ( 23 * i ), items[ i ].name, m_Select ) );
				ui::m_ext_render.rect_draw.insert( ui::m_ext_render.rect_draw.begin( ), ui::external::e_draw_rect( pos.x + 6, pos.y + 41 + ( 23 * i ), 350 - 2, 24, *items[ i ].value ? color_t( 40, 40, 40 ) : color_t( 35, 35, 35, 0 ) ) );
			}
			ui::m_ext_render.rect_draw.insert( ui::m_ext_render.rect_draw.begin( ), ui::external::e_draw_rect( pos.x + 6, pos.y + 42, 350 - 2, ( ( 23 * items.size( ) ) + 2 ) * ( animation[ ui::m_control.get_index( ) ] / 100.f ), color_t( 35, 35, 35 ) ) );
			ui::m_ext_render.rect_draw.insert( ui::m_ext_render.rect_draw.begin( ), ui::external::e_draw_rect( pos.x + 5, pos.y + 41, 350, ( 23 * items.size( ) + 4 ) * ( animation[ ui::m_control.get_index( ) ] / 100.f ), color_t( 15, 15, 15, 155 ) ) );
		}
		ui::m_control.m_animate[ 2 ] = state;
		
	}

	/* add item to multibox related */
	void multi_combo::add_item( std::string m_Name, bool* m_Value ) {
		/* pushback the item into multibox */
		items.push_back( multi_constructor( m_Name, m_Value ) );
	}
}