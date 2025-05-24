#include "listbox.hpp"

namespace ui {
	static int m_ScrollPos[ ui::m_maximum ];
	static bool m_ScrollState[ ui::m_maximum ];

	static int m_Alpha[ ui::m_maximum ];

	list_box::list_box( std::string m_Name, int* m_Select, int size_y, int items, std::vector<std::string>& m_Items )
	{
		this->m_Name = m_Name;
		this->m_Select = m_Select;
		this->m_Items = &m_Items;
		this->size_y = size_y;
		this->pulica_size = size_y;
		this->items_to_draw = items;
	}

	list_box::list_box( std::string m_Name, int* m_Select, int size_y, int items, std::vector<std::string>& m_Items, std::string& m_Filter )
	{
		this->m_Name = m_Name;
		this->m_Select = m_Select;
		this->m_Items = &m_Items;
		this->m_Filter = &m_Filter;
		this->size_y = size_y;
		this->pulica_size = size_y;
		this->items_to_draw = items;
	}

	void list_box::Update( )
	{
		GetItems( );
	}

	void list_box::Scroll( int m_Limit )
	{
		static size_t m_Lost[ ui::m_maximum ];
		if ( m_Lost[ ui::m_control.get_index( ) ] != m_Temp.size( ) ) {
			m_ScrollPos[ ui::m_control.get_index( ) ] = 0;
			m_Lost[ ui::m_control.get_index( ) ] = m_Temp.size( );
		}

		bool isHovered = ui::m_input.mouse_pointer
		(
			{ m_Pos.x + 6, m_Pos.y + 18 },
			{ 120, (float)size_y - 5 } // 250 - 5 = 245
		);

		if ( !m_ScrollState[ ui::m_control.get_index( ) ] && isHovered && m_Temp.size( ) > this->items_to_draw - 1 )
		{
			float ratio = this->items_to_draw / float( m_Temp.size( ) );
			float some_shit = m_Pos.y;

			float size_ratio = this->items_to_draw / float( m_Temp.size( ) );
			size_ratio *= 120;

			float height_delta = some_shit + size_ratio - ( 120 );
			if ( height_delta > 0 )
				some_shit -= height_delta;

			float pos_ratio = float( some_shit ) / float( 120 );
			float m_prikol = pos_ratio * m_Temp.size( );


			if ( ui::m_input.get_mouse_wheel( ) != 0 && isHovered )
			{
				m_ScrollPos[ ui::m_control.get_index( ) ] += ui::m_input.get_mouse_wheel( ) * ( -1 );
				ui::m_input.set_mouse_wheel( 0 );

				if ( m_ScrollPos[ ui::m_control.get_index( ) ] < 0 )
					m_ScrollPos[ ui::m_control.get_index( ) ] = 0;

				if ( m_ScrollPos[ ui::m_control.get_index( ) ] > ( int )m_prikol )
					m_ScrollPos[ ui::m_control.get_index( ) ] = ( int )m_prikol;
			}
		}
	}

	void list_box::Draw( )
	{
		s_text( m_Pos.x + 5, m_Pos.y + 4, renderr->fonts.menu_font, m_Name, color_t( 120, 120, 120, 255 ), false, 0 );
		s_rect_filled( m_Pos.x + 5, m_Pos.y + 18, 350, this->size_y, color_t( 45, 45, 45 ) );
		s_rect( m_Pos.x + 5, m_Pos.y + 18, 350, this->size_y, color_t( 15, 15, 15, 150 ) );

		if ( !m_Temp.empty( ) ) {
			int DrawnItems = 0;
			int ItemsToDraw = this->items_to_draw;

		//	D3DVIEWPORT9 m_Old = renderr->get_viewport( );
			//renderr->set_viewport( { ( DWORD )m_Pos.x + 6, ( DWORD )m_Pos.y + 18, ( DWORD )351, ( DWORD )250, 0.f, 1.f } );
		

			for ( int i{ m_ScrollPos[ ui::m_control.get_index( ) ] }; ( i < m_Temp.size( ) && DrawnItems < ItemsToDraw ); i++ ) {
				bool isHovered = ui::m_input.mouse_pointer
				(
					{ m_Pos.x + 12, ( m_Pos.y + 19 + DrawnItems * 22 ) },
					{ 120, 15 }
				);

				if ( isHovered && ui::m_input.key_pressed( VK_LBUTTON )
					&& ui::m_control.is_possible( ) && !ui::m_control.m_animate[ 0 ] && !ui::m_control.color_picker_opened )
					*m_Select = m_Temp[ i ].second; 

				color_t m_Color = isHovered && !ui::m_control.m_animate[ 0 ] && !ui::m_control.m_animate[ 1 ] && !ui::m_control.m_animate[ 2 ]
					&& !ui::m_control.m_animate[ 3 ] || *m_Select == m_Temp[ i ].second ? settings.modify_menu_color ? settings.accent_color : accent : color_t( 120, 120, 120 );

				//s_rect_filled( m_Pos.x + this->size_y - 4, m_Pos.y + 16 + m_scroll_process, 3, this->size_y - m_scroll_pos_max - 10, color_t( 48, 48, 48 ) );
				s_rect_filled( m_Pos.x + 5, m_Pos.y + 18 + ( DrawnItems * 22 ), 350, 24, *m_Select == m_Temp[ i ].second ? color_t( 60, 60, 60 ) : color_t( 35, 35, 35, 0 ) );
				s_text( m_Pos.x + 12, ( m_Pos.y + 24 + DrawnItems * 22 ), renderr->fonts.menu_font, m_Temp[ i ].first, m_Color, false, 0 );
				DrawnItems++;
			}

			//renderr->set_viewport( m_Old );
			Scroll( 0 );
		}
	}

	std::string list_box::ToLower( std::string String )
	{
		std::transform( String.begin( ), String.end( ), String.begin( ), ( int( * )( int ) )::tolower );
		return String;
	}

	void list_box::GetItems( )
	{
		if ( m_LowerItems.empty( ) || m_LowerItems.size( ) != m_Items->size( ) )
		{
			m_LowerItems.clear( );
			for ( int i = 0; i < m_Items->size( ); i++ )
			{
				std::string temp_s = ( *m_Items )[ i ];
				m_LowerItems.push_back( temp_s );
			}
		}

		if ( !m_Filter && m_Temp.empty( ) || m_Temp.size( ) != m_Items->size( ) && ( !m_Filter || m_Filter->empty( ) ) )
		{
			m_Temp.clear( );
			for ( int i = 0; i < m_Items->size( ); i++ ) {
				m_Temp.emplace_back( ( *m_Items )[ i ], i );
			}
		}

		// Assume m_Filter is a std::string variable
		if ( !m_Filter )
			return;

		if ( m_PreviousFilter == *m_Filter )
			return;

		m_PreviousFilter = *m_Filter;
		const auto l_filter = ToLower( *m_Filter );

		// Convert the first letter of m_Filter to uppercase (if it is a lowercase letter)
		if ( !m_Filter->empty( ) ) {
			if ( std::islower( ( *m_Filter )[ 0 ] ) ) {
				( *m_Filter )[ 0 ] = std::toupper( ( *m_Filter )[ 0 ] );
			}
		}

		m_Temp.clear( );

		for ( int i = 0; i < m_Items->size( ); i++ )
		{
			if ( m_Filter != nullptr )
			{
				std::string temp_s = m_Items->at( i ); // use the original case-sensitive version of the item
				const auto l_item = ToLower( temp_s ); // convert the item to lowercase for comparison
				if ( !( l_item.find( l_filter ) != std::string::npos ) )
					continue;
			}

			m_Temp.emplace_back( m_Items->at( i ), i );
		}



	}


	extern const char* LowerCase[ 254 ];
	extern const char* UpperCase[ 254 ];

	static short int StoredIndex = -1;
	static bool m_State = false;

	float m_Blink;
	static int m_Alpha2[ ui::m_maximum ];

	text_input::text_input( std::string* m_Label, std::string name_put, std::string name_in )
	{
		this->m_Label = m_Label;
		this->name_put = name_put;
		this->name_in = name_in;
	}

	void text_input::Update( )
	{
		if ( m_State && StoredIndex == ui::m_control.get_index( ) && ui::m_input.key_pressed( VK_LBUTTON ) )
		{
			m_State = !m_State;
			StoredIndex = -1;
		}

		if ( !ui::m_control.color_picker_opened && ui::m_control.is_possible( ) && ui::m_input.key_pressed( VK_LBUTTON )
			&& ui::m_input.mouse_pointer( { m_Pos.x + 7, m_Pos.y + 13 }, { 170, 18 } ) )
		{
			m_State = !m_State;
			StoredIndex = ui::m_control.get_index( );
		}

		if ( StoredIndex == ui::m_control.get_index( ) )
			m_Focused = m_State;

		ui::m_control.m_opened_state[ ui::m_control.textbox ][ ui::m_control.get_index( ) ] = m_Focused;

		if ( m_Focused )
		{
			std::string m_Str = *m_Label;

			for ( int i = 0; i < 255; i++ ) {
				if ( ui::m_input.key_pressed( i ) ) {
					if ( i == VK_ESCAPE || i == VK_RETURN || i == VK_INSERT ) {
						StoredIndex = -1;
						return;
					}

					if ( strlen( m_Str.c_str( ) ) != 0 ) {
						if ( GetAsyncKeyState( VK_BACK ) ) {
							*m_Label = m_Str.substr( 0, strlen( m_Str.c_str( ) ) - 1 );
						}
					}

					if ( strlen( m_Str.c_str( ) ) < 27 && i != NULL && UpperCase[ i ] != nullptr ) {
						if ( GetAsyncKeyState( VK_SHIFT ) ) {
							*m_Label = m_Str + UpperCase[ i ];
						}
						else {
							*m_Label = m_Str + LowerCase[ i ];
						}

						return;
					}

					if ( strlen( m_Str.c_str( ) ) < 27 && i == 32 ) {
						*m_Label = m_Str + " ";
						return;
					}
				}
			}
		}
	}

	void text_input::Draw( )
	{
		std::string ToDraw = *m_Label;

		if ( GetTickCount64( ) >= m_Blink )
			m_Blink = GetTickCount64( ) + 800;

		if ( m_Focused && GetTickCount64( ) > ( m_Blink - 400 ) )
			ToDraw += "_";

		s_rect_filled( m_Pos.x + 7, m_Pos.y + 13, 350, 23, color_t( 45, 45, 45 ) );
		s_rect( m_Pos.x + 7, m_Pos.y + 13, 350, 23, color_t( 15, 15, 15, 155 ) );
		s_text( m_Pos.x + 11, m_Pos.y + 18, renderr->fonts.menu_font, ToDraw, color_t( 120, 120, 120, 255 ), false, 0 );

		s_text( m_Pos.x + 11, m_Pos.y - 2, renderr->fonts.menu_font, this->name_put, color_t( 120, 120, 120, 255 ), false, 0 );

		if ( !m_Focused && ToDraw.empty( ) ) {
			s_text( m_Pos.x + 11, m_Pos.y + 18, renderr->fonts.menu_font, this->name_in, color_t( 103, 103, 103, 235 ), false, 0 );
		}
	}


	const char* LowerCase[ 254 ] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x",
	"y", "z", nullptr, nullptr, nullptr, nullptr, nullptr, "0", "1", "2", "3", "4", "5", "6",
	"7", "8", "9", nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, ";", "+", ",", "-", ".", "/?", "~", nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, "[", "\\", "]", "'", nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };

	const char* UpperCase[ 254 ] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X",
	"Y", "Z", nullptr, nullptr, nullptr, nullptr, nullptr, "0", "1", "2", "3", "4", "5", "6",
	"7", "8", "9", nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, ";", "+", ",", "_", ".", "?", "~", nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, "{", "|", "}", "\"", nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
}