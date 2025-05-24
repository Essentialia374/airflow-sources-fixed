#pragma once
#include "../base/base.hpp"
#include "../window/window3.hpp"
#include "../checkbox/checkbox3.hpp"
#include "../slider/slider3.hpp"
#include "../combo/combo3.hpp"
#include "../multi/multi3.hpp"
#include "../keybind/keybind3.hpp"
#include "../color_picker/color_picker.hpp"
#include "../button/button3.hpp"
#include "../listbox/listbox.hpp"

namespace ui {
	class check_box;
	class slider;
	class combo;
	class multi_combo;
	class color_selector;
	class key_bind;
	class list_box;
	class text_input;
	class button;
	class group_box : public base {
	public:
		enum group_flags {
			group_no_draw = 1 << 0,
			group_no_draw_text = 1 << 1
		};

		/* constructor. */
		group_box( std::string name, int m_group_num, int pos_x, int pos_y, int width, int height, int m_current_tab = 0, uint32_t m_flags = 0 );
		void install( );
		void draw( );

		void set_pos( vec2_t_ui m_position );
		vec2_t_ui get_pos( );

		vec2_t_ui m_size;
		vec2_t_ui m_pos;

		bool get_visible( );
		void set_visible( bool m_vis );
		bool m_visible;

		int m_tab;
		int m_group_num; // used for animation

		void set_limit( );
		bool is_hovered( );

		// oofsets
		struct {
			float offset_x;
			float offset_y[ m_maximum ];
		} m_elements;
	public:
		void handle( check_box* check );
		void handle( slider* slide );
		void handle( combo* combo );
		void handle( multi_combo* multi_combo );
		void handle( key_bind* key_bind );
		void handle( color_selector* color_selector );
		void handle( button* button );
		void handle( list_box* list_box );
		void handle( text_input* text_input );

		void handle_scroll( );
	private:
		std::string m_name;
	};
}