#include "window3.hpp"
#include <ctime>
#include "../group/group3.hpp"

/* using namespace ui -> we could use that but, yeah */
namespace ui {
	window::window( std::string name, vec2_t_ui* m_pos, vec2_t_ui m_size, std::vector<std::string> m_tabs, int* m_selected ) {
		this->m_win_pos = m_pos;
		this->size = m_size;
		this->name = name;
		this->m_selected = m_selected;
		this->m_tabs = m_tabs;

		this->install( );
	}

	void window::install( ) {
		handle_position( );
		draw( );
		draw_tabs( );
	}

	void window::handle_position( ) {
		/* drag & offset */
		static bool m_drag{ false };
		static vec2_t_ui m_offset{ };

		/* delta calculation */
		vec2_t_ui delta = m_input.get_mouse_pos( ) - m_offset;

		/* drag system */ {
			/* dont drag */
			if ( m_drag && !GetAsyncKeyState( VK_LBUTTON ) ) {
				m_drag = false;
			}

			/* drag delta */
			if ( m_drag && GetAsyncKeyState( VK_LBUTTON ) ) {
				*this->m_win_pos = delta;
			}

			/* get mouse pointer */
			if ( m_input.mouse_pointer( vec2_t_ui( this->m_win_pos->x, this->m_win_pos->y ), vec2_t_ui( 210, 30 ) ) ) {
				m_drag = true;
				m_offset = m_input.get_mouse_pos( ) - *this->m_win_pos;
			}
		}

		/* iritate window */
		if ( this->m_win_pos->x < 0 ) {
			this->m_win_pos->x = 0;
		}

		if ( this->m_win_pos->y < 0 ) {
			this->m_win_pos->y = 0;
		}
	}

	void window::draw_tabs( ) {
		/* if we are not drawing any tabs dont draw */
		if ( this->m_tabs.empty( ) )
			return;

		bool is_possible = !m_control.color_picker_opened && !m_control.m_animate[ 0 ] &&
			!m_control.m_animate[ 1 ] && !m_control.m_animate[ 2 ] && !m_control.m_animate[ 3 ] && m_control.is_possible( );

		/* main int's */
		static int m_alpha[ 6 ];
		static int m_switched{ 0 };
		static int m_next_alpha{ 255 };
		static int m_changed;

		if ( m_switched != *this->m_selected ) {
			m_next_alpha = -1;
			m_switched = *this->m_selected;
		}

		/* alpha sys */ {
			if ( m_next_alpha != 255 ) {
				m_next_alpha += 5;
			}

			if ( m_next_alpha > 255 ) {
				m_next_alpha = 255;
			}

			if ( m_next_alpha < 0 ) {
				m_next_alpha = 0;
			}

			/* clamp alpha */
			std::clamp<int>( m_next_alpha, 0, 255 );
		}

		/* run tabs */
		for ( int i = 0; i < this->m_tabs.size( ); i++ ) {
			auto tab_size = ( ( this->size.x / 2 ) - 30 ) / this->m_tabs.size( );
			RECT tab_position = { this->m_win_pos->x + 430 + ( i * tab_size ), this->m_win_pos->y + 5, tab_size, 30 };

			// sizer
			short int re_sizer = 0;

			// sizer
			if ( i == 3 )
				re_sizer = 8;
			else if ( i == 0 )
				re_sizer = -5;
			else if ( i == 4 )
				re_sizer = 26;

			if ( is_possible && m_input.mouse_pointer( { (float)tab_position.left + 20, ( float )tab_position.top }, { ( float )tab_position.right, ( float )tab_position.bottom } ) ) {
				if ( m_input.key_pressed( VK_LBUTTON ) ) {
					*this->m_selected = i;
				}
			}
			color_t color = i == *m_selected ? settings.modify_menu_color ? settings.accent_color : accent : color_t( 149, 149, 149, 255 );

			auto size_text = renderr->get_text_width( this->m_tabs[ i ].c_str( ), renderr->fonts.main_font );
			s_text( tab_position.left - re_sizer + ( tab_position.right / 2 ), tab_position.top + 20, renderr->fonts.second_font, this->m_tabs[ i ], color, false, 2, color_t( 5, 5, 5 ) );
		}
	}

	void render_checkerboard_pattern( int x, int y, int width, int height, int grid_size, Color col1, Color col2 )
	{
		for ( int i = 0; i < height - 40; i += grid_size )
		{
			for ( int j = 0; j < width; j += grid_size )
			{
				if ( ( i / grid_size + j / grid_size ) % 2 == 0 )
				{
					s_rect( x + j, ( y + 40 ) + i, grid_size, grid_size, col1 );
				}
				else
				{
					s_rect( x + j, ( y + 40 ) + i, grid_size, grid_size, col2 );
				}
			}
		}
	}

	void window::draw( ) {
		s_rect_filled( this->m_win_pos->x, this->m_win_pos->y, this->size.x, this->size.y, color_t( 20, 20, 20, 255 ) );
		
		auto m_x = this->m_win_pos->x;
		auto m_y = this->m_win_pos->y;
		auto m_width = this->size.x;
		auto width = this->size.x;
		auto m_height = this->size.y;
		auto height = this->size.y;
		auto m_alpha = 255;

		switch ( settings.menu_backround ) {
			case 0: {
				int grid_step = 5;
				int grid_size = 3;
				Color col_bg1( 25, 25, 25, 255 );
				Color col_bg2( 20, 20, 20, 183 );

				for ( int i = 0; i < m_width; i += grid_step * grid_size )
				{
					// Ensure that the final square in the grid is fully within the canvas
					int x_max = std::fmin( i + grid_step * grid_size, m_width );

					for ( int j = 0; j < m_height; j += grid_step * grid_size )
					{
						// Ensure that the final square in the grid is fully within the canvas
						int y_max = std::fmin( j + grid_step * grid_size, m_height );

						// Draw a grid of squares
						for ( int ii = i; ii < x_max; ii += grid_step )
						{
							for ( int jj = j; jj < y_max; jj += grid_step )
							{
								Color col = ( ( ii + jj ) / grid_step ) % 2 == 0 ? col_bg1 : col_bg2;
								int width = std::fmin( grid_step, x_max - ii );
								int height = std::fmin( grid_step, y_max - jj );
								s_rect_filled( m_x + ii, m_y + jj, width, height, col );
							}
						}
					}
				}
			} break;
			case 1: {
				// Define the grid size
				int grid_size = 5;

				// Iterate through each checkered square
				for ( int io = 0; io < m_width / ( 2 * grid_size ); io++ )
				{
					for ( int jo = 0; jo < ( m_height - 32 ) / ( grid_size * 2 ); jo++ )
					{
						// Calculate the position of the current square
						int x = m_x + ( io * grid_size * 2 );
						int y = m_y + 32 + ( jo * grid_size * 2 );

						// Alternate the colors of the square
						if ( ( io + jo ) % 2 == 0 )
						{
							s_rect( x, y, grid_size, grid_size, Color( 20, 20, 20, 255 ) );
						}
						else
						{
							s_rect( x, y, grid_size, grid_size, Color( 25, 25, 25, 255 ) );
						}
					}
				}
			} break;
			case 2: {
				render_checkerboard_pattern( m_x, m_y, m_width, m_height, 10, Color( 20, 20, 20 ), Color( 40, 40, 40 ) );
			} break;
			case 3: {
				int square_size = 32; // 36
				for ( int i = 0; i < 17; i++ ) {
					for ( int j = 0; j < 25; j++ ) {
						int x = m_x + j * square_size;
						int y = m_y + i * square_size;
						if ( ( i + j ) % 2 == 0 ) {
							s_rect( x, y + 56, square_size, square_size, Color( 25, 25, 25, m_alpha ) );
						}
						else {
							s_rect( x, y + 56, square_size, square_size, Color( 10, 10, 10, m_alpha ) );
						}
					}
				}
			} break;
			case 4: {
				// Define the size of each square on the chess table
				int square_size = 32;

				// Define the colors for the light and dark squares
				Color light_square_color( 25, 25, 25, m_alpha );
				Color dark_square_color( 35, 35, 35, m_alpha );

				// Define the colors for the shadows
				Color shadow_color( 5, 5, 5, 100 );
				Color highlight_color( 75, 75, 75, 100 );

				// Loop through each square on the chess table
				for ( int i = 0; i < 17; i++ ) {
					for ( int j = 0; j < 25; j++ ) {
						int x = m_x + j * square_size;
						int y = m_y + i * square_size;

						// Determine if this square is a light or dark square
						bool is_light_square = ( i + j ) % 2 == 0;

						// Draw the shadow for this square
						if ( is_light_square ) {
							s_rect( x + square_size / 2, y + square_size / 2 + 56, square_size / 2, square_size / 2, shadow_color );
						}
						else {
							s_rect( x + square_size / 2, y + square_size / 2 + 56, square_size / 2, square_size / 2, highlight_color );
						}

						// Draw the square itself
						s_rect( x, y + 56, square_size, square_size, is_light_square ? light_square_color : dark_square_color );
					}
				}
			} break;
			case 5: {
				// Define the size of each square on the checkerboard
				int square_size = 32;

				// Define the colors for the light and dark squares
				Color light_square_color( 25, 25, 25, m_alpha );
				Color dark_square_color( 10, 10, 10, m_alpha );

				// Define the colors for the shadows
				Color shadow_color( 5, 5, 5, 100 );
				Color highlight_color( 45, 45, 45, 100 );

				// Loop through each square on the checkerboard
				for ( int i = 0; i < 17; i++ ) {
					for ( int j = 0; j < 25; j++ ) {
						int x = m_x + j * square_size;
						int y = m_y + i * square_size;

						// Determine the color of the current square
						Color current_color = ( i + j ) % 2 == 0 ? light_square_color : dark_square_color;

						// Draw the current square
						s_rect( x, y + 56, square_size, square_size, current_color );

						// Draw a shadow and highlight effect around the square
						s_rect( x, y + 56, square_size, 2, shadow_color );
						s_rect( x, y + 56 + square_size - 2, square_size, 2, highlight_color );
						s_rect( x, y + 56, 2, square_size, shadow_color );
						s_rect( x + square_size - 2, y + 56, 2, square_size, highlight_color );
					}
				}
			} break;
			case 6: {
				for ( int i = 0; i < m_width; i += 10 )
				{
					for ( int j = 0; j < m_height; j += 10 )
					{
						if ( ( i + j ) % 20 == 0 )
						{
							s_line( m_x + i, m_y + j, m_x + i + 10, m_y + j + 10, Color( 30, 30, 30, m_alpha ) );
						}
						else
						{
							s_line( m_x + i, m_y + j + 10, m_x + i + 10, m_y + j, Color( 30, 30, 30, m_alpha ) );
						}
					}
				}
			} break;
		}

		s_gradient( this->m_win_pos->x, this->m_win_pos->y, this->size.x, 60, color_t( 24, 24, 24, 255 ), color_t( 16, 16, 16, 255 ), false );
		s_rect_filled( this->m_win_pos->x + 1, this->m_win_pos->y + 1, this->size.x - 2, 1, settings.modify_menu_color ? settings.accent_color : accent );
		s_rect_filled( this->m_win_pos->x + 1, this->m_win_pos->y + 60, this->size.x - 2, 2, color_t( 40, 40, 40, 255 ) );
		s_rect_filled( this->m_win_pos->x + 1, this->m_win_pos->y + 45, 180, 1, color_t( 45, 45, 45, 255 ) );

		//s_rect_filled( this->m_win_pos->x + 25 - 5, this->m_win_pos->y + 15, 4, 40, color_t( 140, 140, 140 ) );
		//s_rect_filled( this->m_win_pos->x + ( 25 / 2 ) + 1 - 5, this->m_win_pos->y + 26, 30, 4, color_t( 140, 140, 140 ) );
		s_text( this->m_win_pos->x + 5, this->m_win_pos->y + 20, renderr->fonts.main_font, "DEATHMESIS", settings.modify_menu_color ? settings.accent_color : accent, false, 2, color_t( 5, 5, 5 ) );
		s_text( this->m_win_pos->x + 5, this->m_win_pos->y + 32, renderr->fonts.main_font, "DEVELOPED BY", color_t( 255, 255, 255, 100 ), false, 2, color_t( 5, 5, 5 ) );

		auto developed_width = renderr->get_text_width( "DEVELOPED BY", renderr->fonts.main_font );
		s_text( this->m_win_pos->x + 5 + developed_width + 4, this->m_win_pos->y + 32, renderr->fonts.main_font, "DUTU1337", settings.modify_menu_color ? settings.accent_color : accent, false, 2, color_t( 5, 5, 5 ) );

		s_rect( this->m_win_pos->x, this->m_win_pos->y, this->size.x, this->size.y, color_t( 40, 40, 40, 255 ) );
	}

	void window::add_group( group_box* group ) {
		group->set_visible( group->m_tab == *this->m_selected );
		if ( group->get_visible( ) ) {
			group->set_pos( *this->m_win_pos );
			group->install( );
		}
	}
}