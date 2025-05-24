#include "color_picker.hpp"

/* animatino shits and values / strings */
static auto stored_index = -1;
static bool m_state = false;

struct RGB { double r, g, b; };
struct HSV { double h, s, v; };

static int m_alpha_in[ ui::m_maximum ];
HSV rgb_to_hsv( const RGB& In )
{
	HSV m_Result;
	double Min, Max, Delta;

	Min = In.r < In.g ? In.r : In.g;
	Min = Min < In.b ? Min : In.b;

	Max = In.r > In.g ? In.r : In.g;
	Max = Max > In.b ? Max : In.b;

	m_Result.v = Max;
	Delta = Max - Min;

	if ( Delta < 0.0001 ) {
		m_Result.s = 0;
		m_Result.h = 0;

		return m_Result;
	}

	if ( Max > 0 ) {
		m_Result.s = ( Delta / Max );
	}
	else {
		m_Result.s = 0;
		m_Result.h = NAN;

		return m_Result;
	}

	if ( In.r >= Max ) {
		m_Result.h = ( In.g - In.b ) / Delta;
	}
	else {
		if ( In.g >= Max ) {
			m_Result.h = 2 + ( In.b - In.r ) / Delta;
		}
		else {
			m_Result.h = 4 + ( In.r - In.g ) / Delta;
		}
	}

	m_Result.h *= 60;

	if ( m_Result.h < 0 )
		m_Result.h += 360;

	return m_Result;
}
RGB hsv_to_rgb( const HSV& In )
{
	RGB m_Result;

	double HH, P, Q, T, FF;
	long i;

	if ( In.s <= 0 ) {
		m_Result.r = In.v;
		m_Result.g = In.v;
		m_Result.b = In.v;

		return m_Result;
	}

	HH = ( In.h >= 360 ? 0 : In.h ) / 60;
	i = ( long )HH;

	FF = HH - i;

	P = In.v * ( 1 - In.s );
	Q = In.v * ( 1 - ( In.s * FF ) );
	T = In.v * ( 1 - ( In.s * ( 1 - FF ) ) );

	switch ( i ) {
	case 0:
		m_Result.r = In.v;
		m_Result.g = T;
		m_Result.b = P;

		break;

	case 1:
		m_Result.r = Q;
		m_Result.g = In.v;
		m_Result.b = P;

		break;

	case 2:
		m_Result.r = P;
		m_Result.g = In.v;
		m_Result.b = T;

		break;

	case 3:
		m_Result.r = P;
		m_Result.g = Q;
		m_Result.b = In.v;

		break;

	case 4:
		m_Result.r = T;
		m_Result.g = P;
		m_Result.b = In.v;

		break;

	case 5:
	default:
		m_Result.r = In.v;
		m_Result.g = P;
		m_Result.b = Q;

		break;

	}

	return m_Result;
}

/* get color */
static color_t bar_colors[ 7 ] = {
	color_t( 255, 0, 0, 255 ),
	color_t( 255, 255, 0, 255 ),
	color_t( 0, 255, 0, 255 ),
	color_t( 0, 255, 255, 255 ),
	color_t( 0, 0, 255, 255 ),
	color_t( 255, 0, 255, 255 ),
	color_t( 255, 0, 0, 255 )
};

namespace ui {
	color_selector::color_selector( color_t* m_Select, float* m_Hue, bool m_alpha ) {
		this->m_select = m_Select;
		this->m_alpha_bar = m_alpha;

		HSV NewColor = rgb_to_hsv( { ( double )( ( float )m_Select->r() / 255.f ),( double )( ( float )m_Select->g( ) / 255.f ),( double )( ( float )m_Select->g( ) / 255.f ) } );

		this->m_hue = m_Hue;
		this->m_cursor_pos.x = ( float )NewColor.s * 150.f;
		this->m_cursor_pos.y = 150.f - ( ( float )NewColor.v * 150.f );

		if ( m_alpha )
			this->m_alpha = ( float )m_Select->a( ) / 255.f;
		else
			this->m_alpha = 1.0f;
	}

	void color_selector::update( )
	{
		if ( m_state && stored_index == ui::m_control.get_index( ) && ui::m_input.key_pressed( VK_LBUTTON ) && !ui::m_input.mouse_pointer( { m_pos.x + 23, m_pos.y }, { 180, float( m_alpha_bar ? 180 : 164 ) } ) )	{
			m_state = !m_state;
			stored_index = -1;
		}

		if ( ui::m_control.is_possible( ) && ui::m_input.mouse_pointer( { m_pos.x, m_pos.y }, { 17, 9 } ) && ui::m_input.key_pressed( VK_LBUTTON ) && !ui::m_control.m_animate[ 3 ] ) {
			m_state = !m_state;
			stored_index = ui::m_control.get_index( );
		}

		if ( stored_index == ui::m_control.get_index( ) )
			m_focused = m_state;

		ui::m_control.m_animate[ 0 ] = m_state;
		ui::m_control.color_picker_opened = m_focused;
		ui::m_control.m_opened_state[ ui::m_control.color_selection ][ ui::m_control.get_index( ) ] = m_focused;

		if ( m_focused )
		{
			if ( GetAsyncKeyState( VK_LBUTTON ) )
			{
				if ( ui::m_input.mouse_pointer( { m_pos.x + 30, m_pos.y + 6 }, { 150, 156 } ) ) {
					m_cursor_pos.x = ui::m_input.get_mouse_pos( ).x - ( m_pos.x + 30 );
					m_cursor_pos.y = ui::m_input.get_mouse_pos( ).y - ( m_pos.y + 7 );

					if ( m_cursor_pos.x < 0.f )
						m_cursor_pos.x = 0.f;

					if ( m_cursor_pos.x > 156.f )
						m_cursor_pos.x = 156.f;

					if ( m_cursor_pos.y < 0.f )
						m_cursor_pos.y = 0.f;

					if ( m_cursor_pos.y > 156.f )
						m_cursor_pos.y = 156.f;
				}
				else if ( ui::m_input.mouse_pointer( { m_pos.x + 187, m_pos.y + 7 }, { 10, 155 } ) ) // m_pos.x + 187, m_pos.y + 7, 8, 155
				{
					if ( m_alpha_bar )
					{
						m_alpha = ( ui::m_input.get_mouse_pos( ).y - ( m_pos.y + 7 ) ) / 155.f;

						if ( m_alpha < 0.f )
							m_alpha = 0.f;
								
						if ( m_alpha > 1.f )
							m_alpha = 1.f;

						if ( m_alpha > 0.99f )
							m_alpha = 1.f;
					}
				}
				
				else if ( ui::m_input.mouse_pointer( { m_pos.x + 30, m_pos.y + 167 }, { 150, 10 } ) ) // m_pos.x + 30, m_pos.y + 167 + ( 25 * 0 ), 150, 8
				{
					*m_hue = ( ui::m_input.get_mouse_pos( ).x - ( m_pos.x + 30 ) ) / 150.f;

					if ( *m_hue < 0.f )
						*m_hue = 0.f;

					if ( *m_hue > 1.f )
						*m_hue = 1.f;
				}
			

				float m_Sat = m_cursor_pos.x / 150.f;
				float m_Bright = 1.f - ( m_cursor_pos.y / 150.f );

				RGB NewColor = hsv_to_rgb( { *m_hue * 360.f, m_Sat, m_Bright } );
				*m_select = color_t( ( int )( NewColor.r * 255.f ), ( int )( NewColor.g * 255.f ), ( int )( NewColor.b * 255.f ), ( int )( m_alpha * 255.f ) );/*(int)*/
			}
		}
	}

	void color_selector::draw( ) {
		s_rect_filled( m_pos.x, m_pos.y, 16, 8, *m_select );
		s_rect( m_pos.x, m_pos.y, 16, 8, color_t( 15, 15, 15, 155 ) );


		if ( m_focused )
			m_alpha_in[ ui::m_control.get_index( ) ] += 10;

		else
			m_alpha_in[ ui::m_control.get_index( ) ] -= 10;

		if ( m_alpha_in[ ui::m_control.get_index( ) ] > 255 )
			m_alpha_in[ ui::m_control.get_index( ) ] = 255;

		if ( m_alpha_in[ ui::m_control.get_index( ) ] < 0 )
			m_alpha_in[ ui::m_control.get_index( ) ] = 0;

		std::clamp<int>( m_alpha_in[ ui::m_control.get_index( ) ], 0, 255 );

		if ( m_alpha_in[ ui::m_control.get_index( ) ] > 0 )
		{
			/* alpha bar */
			ui::m_ext_render.outline_draw.insert( ui::m_ext_render.outline_draw.begin( ), ui::external::e_draw_rect( m_pos.x + 187, m_pos.y + 7, 8, 155, color_t( 15, 15, 15, m_alpha_in[ ui::m_control.get_index( ) ] ) ) );	
			ui::m_ext_render.gradient_v_draw.insert( ui::m_ext_render.gradient_v_draw.begin( ), ui::external::e_draw_gradient( m_pos.x + 187, m_pos.y + 7, 8, 155, m_select->alpha( m_select->a( ) - 255 ), *m_select ) );

			ui::m_ext_render.rect_draw2.insert( ui::m_ext_render.rect_draw2.begin( ), ui::external::e_draw_rect( m_pos.x + 186, m_pos.y + 7 + ( 155.f * m_alpha ), 11, 3, color_t( 153, 153, 159, m_alpha_in[ ui::m_control.get_index( ) ] ) ) );
			ui::m_ext_render.outline_draw.insert( ui::m_ext_render.outline_draw.begin( ), ui::external::e_draw_rect( m_pos.x + 185, m_pos.y + 6 + ( 155.f * m_alpha ), 12, 4, color_t( 15, 15, 15, m_alpha_in[ ui::m_control.get_index( ) ] ) ) );

			ui::m_ext_render.outline_draw.insert( ui::m_ext_render.outline_draw.begin( ), ui::external::e_draw_rect( m_pos.x + 187, m_pos.y + 7 + ( 155.f * m_alpha ), 8, 1, color_t( 153, 153, 159, m_alpha_in[ ui::m_control.get_index( ) ] ) ) );
			ui::m_ext_render.outline_draw.insert( ui::m_ext_render.outline_draw.begin( ), ui::external::e_draw_rect( m_pos.x + 187, m_pos.y + 6 + ( 155.f * m_alpha ), 8, 1, color_t( 153, 153, 159, m_alpha_in[ ui::m_control.get_index( ) ] ) ) );

			/* main render */
			ui::m_ext_render.rect_draw.insert( ui::m_ext_render.rect_draw.begin( ), ui::external::e_draw_rect( m_pos.x + 23, m_pos.y, 180, 180, color_t( 35, 35, 35, m_alpha_in[ ui::m_control.get_index( ) ] ) ) );
			ui::m_ext_render.outline_draw.insert( ui::m_ext_render.outline_draw.begin( ), ui::external::e_draw_rect( m_pos.x + 23, m_pos.y, 180, 180, color_t( 15, 15, 15, m_alpha_in[ ui::m_control.get_index( ) ] ) ) );

			/* color palete */
			ui::m_ext_render.outline_draw.insert( ui::m_ext_render.outline_draw.begin( ), ui::external::e_draw_rect( m_pos.x + 29, m_pos.y + 6, 151, 156, color_t( 15, 15, 15, m_alpha_in[ ui::m_control.get_index( ) ] ) ) );
			RGB hue_color_t = hsv_to_rgb( { ( double )*m_hue * 360.f, 1.f, 1.f } );
			ui::m_ext_render.gradient_h_draw.insert( ui::m_ext_render.gradient_h_draw.begin( ), ui::external::e_draw_gradient( m_pos.x + 30, m_pos.y + 7, 150, 156, color_t( 255, 255, 255, m_alpha_in[ ui::m_control.get_index( ) ] ), color_t(
				( int )( hue_color_t.r * 255.0f ),
				( int )( hue_color_t.g * 255.0f ),
				( int )( hue_color_t.b * 255.0f ),
				m_alpha_in[ ui::m_control.get_index( ) ]
			) ) );
			ui::m_ext_render.gradient_v_draw.insert( ui::m_ext_render.gradient_v_draw.begin( ), ui::external::e_draw_gradient( m_pos.x + 30, m_pos.y + 7, 150, 156, color_t( 0, 0, 0, 0 ), color_t( 0, 0, 0, 255 ) ) );

			/* hue palete */
			ui::m_ext_render.outline_draw.insert( ui::m_ext_render.outline_draw.begin( ), ui::external::e_draw_rect( m_pos.x + 30, m_pos.y + 167 + ( 25 * 0 ), 150, 8, color_t( 15, 15, 15, m_alpha_in[ ui::m_control.get_index( ) ] ) ) );
			ui::m_ext_render.gradient_h_draw.insert( ui::m_ext_render.gradient_h_draw.begin( ), ui::external::e_draw_gradient( m_pos.x + 30+ ( 25 * 0 ), m_pos.y + 167 , 25, 8, bar_colors[ 0 ], bar_colors[ 1 ] ) );
			ui::m_ext_render.gradient_h_draw.insert( ui::m_ext_render.gradient_h_draw.begin( ), ui::external::e_draw_gradient( m_pos.x + 30 + ( 25 * 1 ), m_pos.y + 167, 25, 8, bar_colors[ 1 ], bar_colors[ 2 ] ) );
			ui::m_ext_render.gradient_h_draw.insert( ui::m_ext_render.gradient_h_draw.begin( ), ui::external::e_draw_gradient( m_pos.x + 30 + ( 25 * 2 ), m_pos.y + 167, 25, 8, bar_colors[ 2 ], bar_colors[ 3 ] ) );
			ui::m_ext_render.gradient_h_draw.insert( ui::m_ext_render.gradient_h_draw.begin( ), ui::external::e_draw_gradient( m_pos.x + 30 + ( 25 * 3 ), m_pos.y + 167, 25, 8, bar_colors[ 3 ], bar_colors[ 4 ] ) );
			ui::m_ext_render.gradient_h_draw.insert( ui::m_ext_render.gradient_h_draw.begin( ), ui::external::e_draw_gradient( m_pos.x + 30 + ( 25 * 4 ), m_pos.y + 167, 25, 8, bar_colors[ 4 ], bar_colors[ 5 ] ) );
			ui::m_ext_render.gradient_h_draw.insert( ui::m_ext_render.gradient_h_draw.begin( ), ui::external::e_draw_gradient( m_pos.x + 30 + ( 25 * 5 ), m_pos.y + 167, 25, 8, bar_colors[ 5 ], bar_colors[ 6 ] ) );

			ui::m_ext_render.rect_draw2.insert( ui::m_ext_render.rect_draw2.begin( ), ui::external::e_draw_rect( m_pos.x + 30 + ( 148.f * ( m_cursor_pos.x / 150.0f ) ), m_pos.y + 6 + ( 148.f * ( m_cursor_pos.y / 150.f ) ), 4, 4, color_t( 130, 130, 170, m_alpha_in[ ui::m_control.get_index( ) ] ) ) );


			ui::m_ext_render.rect_draw2.insert( ui::m_ext_render.rect_draw2.begin( ), ui::external::e_draw_rect( m_pos.x + 30 + ( 150.f * *m_hue ), m_pos.y + 166, 3, 11, color_t( 154, 152, 155, m_alpha_in[ ui::m_control.get_index( ) ] ) ) );
			ui::m_ext_render.outline_draw.insert( ui::m_ext_render.outline_draw.begin( ), ui::external::e_draw_rect( m_pos.x + 29 + ( 150.f * *m_hue ), m_pos.y + 165, 4, 12, color_t( 15, 15, 15, m_alpha_in[ ui::m_control.get_index( ) ] ) ) );		
		}
	}
}