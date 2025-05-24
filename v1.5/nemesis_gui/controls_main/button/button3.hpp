#pragma once
#include "../group/group3.hpp"

namespace ui {
	class button : public base {
	public:
		button( std::string m_name, std::function< void( ) > function );

		void draw( );
		void update( );

		vec2_t_ui m_pos;
	private:
		std::string m_name;
		std::function< void( ) > function = { };

		bool m_focused;
	};
}