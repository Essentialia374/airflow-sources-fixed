#pragma once
#include "../group/group3.hpp"

namespace ui {
	class combo : public base {
	public:
		combo( std::string name, int* value, std::vector<std::string> items );

		void update( );
		void draw( );
		void draw_box( ); // if that are going to p and initialize, not sure yet

		vec2_t_ui pos;
		bool focused;
	private:
		std::string name;
		int* value;

		std::vector<std::string> items;
	};
}