#pragma once
#include "../../gui_main/gui3.hpp"

/* main includes */
#include "../base/base.hpp"

namespace ui {
	class group_box;
	class window : public base {
	public:
		window( std::string name, vec2_t_ui* m_pos, vec2_t_ui m_size, std::vector<std::string> m_tabs = { }, int* m_selected = 0 );

		vec2_t_ui* m_win_pos;
		std::vector<std::string> m_tabs;

		int* m_selected;
		void add_group( group_box* group );
	private:
		void draw( );
		void draw_tabs( );

		void install( );
		void handle_position( );

		//aditions.
		std::string name;
		vec2_t_ui size;
	};
}