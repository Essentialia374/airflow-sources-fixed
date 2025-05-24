#pragma once
#include "../group/group3.hpp"

namespace ui {
	class color_selector : public base {
	public:
		/* drawing shits and values */
		color_selector( color_t* m_Select, float* m_Hue, bool m_Alpha = false );

		void draw( );
		void update( );

		vec2_t_ui m_pos;
		vec2_t_ui m_cursor_pos;
	private:
		/* values */
		bool m_focused;
		bool m_alpha_bar;

		float* m_hue;
		float m_alpha;

		color_t* m_select;
	};
}