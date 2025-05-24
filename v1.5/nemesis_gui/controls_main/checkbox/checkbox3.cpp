#include "checkbox3.hpp"

// get darker color
Color darker( Color color, int amount ) {
	int r = std::fmax( 0, color.r( ) - amount );
	int g = std::fmax( 0, color.g( ) - amount );
	int b = std::fmax( 0, color.b( ) - amount );
	return Color( r, g, b, color.a( ) );
}

namespace ui {
	check_box::check_box( std::string name, bool* value, bool clean_label ) {
		this->name = name;
		this->value = value;
		this->clean_label = clean_label;
	}

	void check_box::update( ) {
		auto m_size = renderr->get_text_width( name.c_str( ), renderr->fonts.menu_font );
		if ( ui::m_control.is_possible( ) && !ui::m_control.color_picker_opened && !ui::m_control.m_animate[ 0 ] ) {
			if ( ui::m_input.mouse_pointer( { m_pos.x, m_pos.y }, { 100, 20 } ) && ui::m_input.key_pressed( VK_LBUTTON ) ) 
				*this->value = !*this->value;	
		}
	}

	void check_box::draw( ) {
		int x_pos{};
		if ( this->clean_label )
			x_pos = 5;
		else
			x_pos = 25;


		if ( !this->clean_label ) {
			s_gradient( m_pos.x + 5, m_pos.y + 3, 11, 11, color_t( 52, 52, 52, 255 ), color_t( 41, 41, 41, 255 ), false );
		}

		s_text( m_pos.x + x_pos, m_pos.y + 2, renderr->fonts.menu_font, this->name, color_t( 120, 120, 120, 255 ) );

		// we want to render it only when we dont have clean_label
		if ( !this->clean_label ) {
			if ( *value ) {
				s_gradient( m_pos.x + 5, m_pos.y + 3, 11, 11, settings.modify_menu_color ? settings.accent_color : accent, color_t( 25, 25, 25, 255 ), false );
			}

			s_rect( m_pos.x + 5, m_pos.y + 3, 11, 11, color_t( 15, 15, 15, 150 ) );
		}
	}
}