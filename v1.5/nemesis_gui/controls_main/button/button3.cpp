#include "button3.hpp"

static short int stored_index = -1;
static bool m_state = false;
static int m_alpha[ ui::m_maximum ];

namespace ui {
	/* button setup */
	button::button( std::string m_name, std::function< void( ) > function ) {
		/* setup vars */
		this->m_name = m_name;
		this->function = function;
	}
	
	/* update buton */
	void button::update( ) {
		/* update key */
		if ( !ui::m_input.key_pressed( VK_LBUTTON ) && ui::m_input.mouse_pointer( { m_pos.x + 7, m_pos.y + 6 }, { 350, 32 } ) )
		{
			if ( m_state )
				function( );

			m_state = false;
		}

		/* check colorpicker */
		if ( !ui::m_control.color_picker_opened && ui::m_control.is_possible( ) && ui::m_input.key_pressed( VK_LBUTTON ) && ui::m_input.mouse_pointer( { m_pos.x + 5, m_pos.y + 6 }, { 350, 35 } ) ) {
			m_state = true;
			stored_index = ui::m_control.get_index( );
		}

		if ( stored_index == ui::m_control.get_index( ) )
			m_focused = m_state;

	}

	/* draw button */
	void button::draw( ) {
		auto text_size = renderr->get_text_width( this->m_name.c_str( ), renderr->fonts.menu_font );

		s_gradient( m_pos.x + 7, m_pos.y + 3, 350, 32, color_t( 107, 107, 107, 255 ), color_t( 72, 72, 72, 255 ), false );
		s_text( m_pos.x + ( 350 / 2 ), m_pos.y + 12, renderr->fonts.menu_font, this->m_name, color_t( 50, 50, 50, 235 ), true );
		s_rect( m_pos.x + 7, m_pos.y + 3, 350, 32, color_t( 15, 15, 15, 155 ) );

	}
}