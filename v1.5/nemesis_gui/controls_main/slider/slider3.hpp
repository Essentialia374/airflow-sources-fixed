#pragma once
#include "../group/group3.hpp"

namespace ui {
	class slider : public base {
	public:
		slider( std::string name, float* value, int min = 0, int max = 100, std::string suffix = "%" ); // we are going to use suffix % but we will look for prefix later

		/* we are going to do the draw /update function down */
		void draw( );
		void update( );

		vec2_t_ui pos;
		bool m_allow_draw;
	private:
		std::string name;
		std::string suffix;

		float* value;
		int min;
		int max;
	};
}