#pragma once
#include "../group/group3.hpp"

namespace ui {
	class check_box : public base {
	public:
		check_box( std::string name, bool* value, bool clean_lable = false );

		void draw( );
		void update( );

		vec2_t_ui m_pos;

	private:
		std::string name;
		bool* value;
		bool clean_label;
	};
}