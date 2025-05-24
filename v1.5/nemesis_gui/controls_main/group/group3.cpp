#include "group3.hpp"

namespace ui {
	static int m_slide[ m_maximum ];
	static bool m_slide_move[ m_maximum ];
	static float m_slide_offset[ m_maximum ];

	group_box::group_box( std::string name, int m_group_num, int pos_x, int pos_y, int width, int height, int m_current_tab, uint32_t m_flags ) {
		this->m_name = name;
		this->m_group_num = m_group_num;

		this->m_pos.x = pos_x;
		this->m_pos.y = pos_y;

		this->m_size.x = width;
		this->m_size.y = height; 

		this->m_tab = m_current_tab;
	}

	/* visible */
	bool group_box::get_visible( ) {
		return m_visible;
	}

	void group_box::set_visible( bool m_vis ) {
		m_visible = m_vis;
	}

	/* limit */
	void group_box::set_limit( ) {
		renderr->set_viewport(
			{ ( DWORD )m_pos.x, ( DWORD )m_pos.y + 17, ( DWORD )m_size.x, ( DWORD )m_size.y - 17, 0.f, 1.f }
		);
	}

	/* hovered */
	bool group_box::is_hovered( ) {
		return ui::m_input.mouse_pointer(
			{ m_pos.x , m_pos.y + 20 }, { m_size.x, m_size.y - 17 }
		);
	}

	/* position */
	vec2_t_ui group_box::get_pos( ) {
		return m_pos;
	}

	void group_box::set_pos( vec2_t_ui m_position ) {
		m_pos += m_position;
	}

	void group_box::install( ) {
		m_elements.offset_x = this->m_pos.x + 10;
		m_elements.offset_y[ this->m_group_num ] = this->m_pos.y + 22 + m_slide[ this->m_group_num ];
		m_slide_offset[ m_group_num ] = 0;

		/* render */
		draw( );
	}

	void group_box::draw( ) {
		s_rect_filled( this->m_pos.x, this->m_pos.y, this->m_size.x, this->m_size.y, color_t( 30, 30, 30 ) );
		s_gradient( this->m_pos.x, this->m_pos.y, this->m_size.x, 15, settings.modify_menu_color ? settings.accent_color.alpha( 70 ) : accent.alpha( 70 ), settings.modify_menu_color ? settings.accent_color : accent, true );
		s_text( this->m_pos.x + 5, this->m_pos.y + 2, renderr->fonts.group_font, this->m_name.c_str( ), color_t( 220, 220, 220 ), false, 1, color_t( 10, 10, 10 ).alpha( 150 ) );
		s_rect( this->m_pos.x, this->m_pos.y, this->m_size.x, this->m_size.y, color_t( 16, 16, 16 ).alpha( 255 ) );
	}

	void group_box::handle( check_box* check ) {
		if ( get_visible( ) ) {
			D3DVIEWPORT9 m_old = renderr->get_viewport( );
			set_limit( );
			check->m_pos = { m_elements.offset_x, m_elements.offset_y[ m_group_num ] };

			if ( is_hovered( ) )
				check->update( );

			/* draw */
			check->draw( );

			m_elements.offset_y[ m_group_num ] += 22;
			m_slide_offset[ m_group_num ] += 22;

			/* set index */
			ui::m_control.set_index( ui::m_control.get_index( ) + 1 );
			renderr->set_viewport( m_old );
		}
		delete check;
	}

	void group_box::handle( slider* slide ) {
		if ( get_visible( ) ) {
			D3DVIEWPORT9 m_old = renderr->get_viewport( );
			set_limit( );

			slide->pos = { m_elements.offset_x, m_elements.offset_y[ m_group_num ] };

			if ( is_hovered( ) ) {
				slide->update( );
				slide->m_allow_draw = true;
			}
			else {
				slide->m_allow_draw = false;
			}

			slide->draw( );

			m_elements.offset_y[ m_group_num ] += 32;
			m_slide_offset[ m_group_num ] += 32;
			ui::m_control.set_index( ui::m_control.get_index( ) + 1 );
			renderr->set_viewport( m_old );
		}
		delete slide;
	}

	void group_box::handle( key_bind* key_bind ) {
		if ( get_visible( ) ) {
			D3DVIEWPORT9 m_Old = renderr->get_viewport( );
			set_limit( );

			key_bind->pos = { m_elements.offset_x + 355, m_elements.offset_y[ m_group_num ] - 19 };

			if ( is_hovered( ) )
				key_bind->update( );

			key_bind->draw( );

			if ( !ui::m_control.m_opened_state[ ui::m_control.bind ][ ui::m_control.get_index( ) ] ) {
				if ( is_hovered( ) )
					key_bind->update_list( );
			}
			else
				key_bind->update_list( );

			key_bind->init_list( );
			ui::m_control.set_index( ui::m_control.get_index( ) + 1 );
			renderr->set_viewport( m_Old );
		}

		delete key_bind;
	}

	void group_box::handle( color_selector* color_selector ) {
		if ( get_visible( ) ) {
			D3DVIEWPORT9 m_Old = renderr->get_viewport( );
			set_limit( );

			color_selector->m_pos = { m_elements.offset_x + 339, m_elements.offset_y[ m_group_num ] - 17 };
			if ( !ui::m_control.m_opened_state[ ui::m_control.color_selection ][ ui::m_control.get_index( ) ] ) {
				if ( is_hovered( ) )
					color_selector->update( );
			}
			else
				color_selector->update( );

			color_selector->draw( );
			ui::m_control.set_index( ui::m_control.get_index( ) + 1 );
			renderr->set_viewport( m_Old );
		}

		delete color_selector;
	}
	void group_box::handle( list_box* m_List )
	{
		if ( get_visible( ) )
		{
			D3DVIEWPORT9 m_Old = renderr->get_viewport( );
			set_limit( );
			m_List->m_Pos = { m_elements.offset_x + 2, m_elements.offset_y[ m_group_num ] };

			m_List->Update( );
			m_List->Draw( );

			m_elements.offset_y[ m_group_num ] += m_List->pulica_size + 23; // 250 + 20 = size_y + 20
			m_slide_offset[ m_group_num ] += m_List->pulica_size + 23;

			ui::m_control.set_index( ui::m_control.get_index( ) + 1 );
			renderr->set_viewport( m_Old );
		}

		delete m_List;
	}

	void group_box::handle( text_input* m_Input )
	{
		if ( get_visible( ) )
		{
			D3DVIEWPORT9 m_Old = renderr->get_viewport( );
			set_limit( );

			m_Input->m_Pos = { m_elements.offset_x, m_elements.offset_y[ m_group_num ] };

			if ( !ui::m_control.m_opened_state[ ui::m_control.textbox ][ ui::m_control.get_index( ) ] ) {
				if ( is_hovered( ) )
					m_Input->Update( );
			}
			else
				m_Input->Update( );

			m_Input->Draw( );

			m_elements.offset_y[ m_group_num ] += 40;
			m_slide_offset[ m_group_num ] += 40;

			ui::m_control.set_index( ui::m_control.get_index( ) + 1 );

			renderr->set_viewport( m_Old );
		}

		delete m_Input;
	}


	void group_box::handle( button* button ) {
		if ( get_visible( ) ) {
			D3DVIEWPORT9 m_Old = renderr->get_viewport( );
			set_limit( );

			button->m_pos = { m_elements.offset_x,
				m_elements.offset_y[ m_group_num ]
			};

			if ( is_hovered( ) )
				button->update( );

			button->draw( );

			m_elements.offset_y[ m_group_num ] += 40;
			m_slide_offset[ m_group_num ] += 40;

			ui::m_control.set_index( ui::m_control.get_index( ) + 1 );

			renderr->set_viewport( m_Old );
		}

		delete button;
	}

	void group_box::handle( combo* combo ) {
		if ( get_visible( ) ) {
			D3DVIEWPORT9 m_old = renderr->get_viewport( );

			/* initialize combo pos in menu elements */
			combo->pos = { m_elements.offset_x, m_elements.offset_y[ m_group_num ] };

			/* do some sanity checks */
			if ( !ui::m_control.m_opened_state[ ui::m_control.combo ][ ui::m_control.get_index( ) ] ) {
				if ( is_hovered( ) ) {
					combo->update( );
				}
			}
			else if ( ui::m_control.m_opened_state[ ui::m_control.combo ][ ui::m_control.get_index( ) ] ) {
				combo->update( );
			}

			/* this is going to be initialized after we do some checks */
			set_limit( );
			renderr->set_viewport( m_old );

			/* do some sanity checks after setting viewport */
			if ( !ui::m_control.m_opened_state[ ui::m_control.combo ][ ui::m_control.get_index( ) ] )
				set_limit( ); /* set the limit of drawing even when the open state is not true */

			/* initialize the drawing */
			combo->draw( );

			/* finish the initialization */
			m_elements.offset_y[ m_group_num ] += 41;
			m_slide_offset[ m_group_num ] += 41;
			ui::m_control.set_index( ui::m_control.get_index( ) + 1 );
			renderr->set_viewport( m_old );
		} delete combo;
	}

	void group_box::handle( multi_combo* multi_combo ) {
		if ( get_visible( ) ) {
			D3DVIEWPORT9 m_old = renderr->get_viewport( );

			/* initialize combo pos in menu elements */
			multi_combo->pos = { m_elements.offset_x, m_elements.offset_y[ m_group_num ] };

			/* do some sanity checks */
			if ( !ui::m_control.m_opened_state[ ui::m_control.multicombo ][ ui::m_control.get_index( ) ] ) {
				if ( is_hovered( ) ) {
					multi_combo->update( );
				}
			}
			else if ( ui::m_control.m_opened_state[ ui::m_control.multicombo ][ ui::m_control.get_index( ) ] ) {
				multi_combo->update( );
			}

			/* this is going to be initialized after we do some checks */
			set_limit( );	
			renderr->set_viewport( m_old );

			/* do some sanity checks after setting viewport */
			if ( !ui::m_control.m_opened_state[ ui::m_control.multicombo ][ ui::m_control.get_index( ) ] )
				set_limit( ); /* set the limit of drawing even when the open state is not true */

			/* initialize the drawing */
			multi_combo->draw( );

			/* finish the initialization */
			m_elements.offset_y[ m_group_num ] += 41;
			m_slide_offset[ m_group_num ] += 41;
			ui::m_control.set_index( ui::m_control.get_index( ) + 1 );
			renderr->set_viewport( m_old );
		} delete multi_combo;
	}


	void group_box::handle_scroll( ) {
		if ( !get_visible( ) ) return;
		if ( !( m_slide_offset[ m_group_num ] > m_size.y - 24 ) ) {
			m_slide[ m_group_num ] = 0;
			return;
		}

		bool is_color_picker = ui::m_control.color_picker_opened;
		bool is_hovered = ui::m_input.mouse_pointer(
			{ m_pos.x, m_pos.y }, { m_size.x, m_size.y + 20 }
		);

		auto scroll_process = [ this ]( ) {
			if ( m_slide[ m_group_num ] > 0 )
				m_slide[ m_group_num ] = 0;

			if ( m_slide[ m_group_num ] < ( m_size.y - 19 ) - m_slide_offset[ m_group_num ] )
				m_slide[ m_group_num ] = ( m_size.y - 19 ) - m_slide_offset[ m_group_num ];
		};

		if ( !is_color_picker && ui::m_control.is_possible( ) && is_hovered && ui::m_input.get_mouse_wheel( ) != 0 ) {
			m_slide[ m_group_num ] += ui::m_input.get_mouse_wheel( ) * 6;
			ui::m_input.set_mouse_wheel( 0 );
			scroll_process( );
		}

		int n_max = m_slide_offset[ m_group_num ] - m_size.y + 29.f;
		int m_scroll_process = ( ( float )m_slide[ m_group_num ] / ( float )m_slide_offset[ m_group_num ] ) * ( m_size.y - 20.f ) * ( -1 );
		float m_scroll_pos_max = n_max / ( float )m_slide_offset[ m_group_num ] * ( m_size.y - 20.f );
		static int m_old[ ui::m_maximum ];

		if ( m_old[ m_group_num ] != n_max ) {
			scroll_process( );
			m_old[ m_group_num ] = n_max;
		}

		s_rect_filled( m_pos.x + m_size.x - 4, m_pos.y + 16 + m_scroll_process, 3, m_size.y - m_scroll_pos_max - 10, color_t( 48, 48, 48 ) );
	}
}