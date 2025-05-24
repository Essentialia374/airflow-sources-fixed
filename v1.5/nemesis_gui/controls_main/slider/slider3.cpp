#include "slider3.hpp"

namespace ui {
	static int m_alpha[ ui::m_maximum ];
	static int m_alpha_text[ ui::m_maximum ];
	static int m_alpha_text2[ ui::m_maximum ];

	slider::slider( std::string name, float* value, int min, int max, std::string suffix ) {
		this->name = name;
		this->value = value;
		this->min = min;
		this->max = max;
		this->suffix = suffix;
	}

	void slider::update( ) {
		static auto stored_index = -1;
		auto delta = max - min;

		/* is possible */
		if ( ui::m_control.is_possible( ) && !ui::m_control.color_picker_opened ) {
			if ( ui::m_input.key_pressed( VK_LBUTTON ) & 1 && ui::m_input.mouse_pointer( vec2_t_ui( pos.x + 5, pos.y + 15 ), vec2_t_ui( 350, 10 ) ) )
				stored_index = ui::m_control.get_index( );
		}

		/* updating controls */
		if ( GetAsyncKeyState( VK_LBUTTON ) && stored_index == ui::m_control.get_index( ) ) {
			*value = min + delta * ( ui::m_input.get_mouse_pos( ).x - pos.x ) / 350;

			if ( *value < min )
				*value = min;

			if ( *value > max )
				*value = max;
		}

		/* update index */
		if ( !GetAsyncKeyState( VK_LBUTTON ) )
			stored_index = -1;
	}

	/* draw slider */
	void slider::draw( ) {
		/* is hovered */
		bool is_hovered = ui::m_input.mouse_pointer( { pos.x, pos.y + 13 }, { 350, 15 } );

		/* now sliding */
		auto min_delta = *value - min;
		auto delta = max - min;
		auto total = ( min_delta / delta ) * 350;

		std::string value_text;
		if ( is_hovered )
			value_text = ": " + std::to_string( ( int )*value ) + suffix;
		else
			value_text = "";

		s_text( pos.x + 5, pos.y, renderr->fonts.menu_font, name.c_str( ) + value_text, color_t( 120, 120, 120, 255 ), false, 0 );
		s_rect_filled( pos.x + 5, pos.y + 15, 350, 10, color_t( 60, 60, 60, 255 ) );

		s_gradient( pos.x + 5, pos.y + 15, total, 10, settings.modify_menu_color ? settings.accent_color : accent, color_t( 25, 25, 25, 255 ), false );

		s_rect( pos.x + 5, pos.y + 15, 350, 10, color_t( 15, 15, 15, 155 ) );
	}
}