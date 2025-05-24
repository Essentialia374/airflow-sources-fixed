#pragma once
#include "../d3d9 render/render_d3d9.hpp"
#define color_t Color
#include "../../config/config_new.h"
#include "../../../includes.h"

namespace ui {
	//desvu framework
	inline constexpr int m_maximum = 1000;

	// external
	namespace external {
		struct e_draw_rect {
			e_draw_rect( float pos_x, float pos_y, float m_width, float m_height, color_t color ) {
				this->pos_x = pos_x;
				this->pos_y = pos_y;
				this->m_width = m_width;
				this->m_height = m_height;
				this->color = color;
			}

			float pos_x;
			float pos_y;
			float m_width;
			float m_height;
			color_t color;
		};

		struct e_draw_text {
			e_draw_text( float pos_x, float pos_y, std::string m_text, color_t color ) {
				this->pos_x = pos_x;
				this->pos_y = pos_y;
				this->m_text = m_text;
				this->color = color;
			}

			float pos_x;
			float pos_y;
			std::string m_text;
			color_t color;
		};


		struct e_draw_gradient {
			e_draw_gradient( float pos_x, float pos_y, float m_width, float m_height, color_t color, color_t color2 ) {
				this->pos_x = pos_x;
				this->pos_y = pos_y;
				this->m_width = m_width;
				this->m_height = m_height;
				this->color = color;
				this->color2 = color2;
			}

			float pos_x;
			float pos_y;
			float m_width;
			float m_height;
			color_t color;
			color_t color2;
		};
	}

	//class details | main
	class details {
	private:
		//menu state * tabs
		int menu_show{ };
		int m_index{ };
	public:
		//constructor.
		~details( void ) = default;

		bool get_menu_state( );
		void install( );
		void set_menu_starte( const bool menu_state );

		// default colros.
		color_t backround = color_t( 20, 20, 20 );
		color_t accent = color_t( 255, 0, 0 );
	};

	//class input
	class input {
	private:
		vec2_t_ui m_mouse_pos{ };
		float next_menu_pos{ };
		float next_menu_wheel{};
	public:
		//voids.
		void pool_input( );
		void set_mouse_pos( const vec2_t_ui m_pos );
		void set_mouse_wheel( const float m_mouse );

		//vec2.
		vec2_t_ui get_mouse_pos( );

		// bools.
		bool key_pressed( const uintptr_t m_key );
		bool mouse_pointer( const vec2_t_ui m_pos, const vec2_t_ui m_size );

		//ffloats.
		float get_mouse_wheel( );

		//additions.
		struct {
			bool m_key_state[ 256 ]{};
			bool m_prev_key_state[ 256 ]{};
		} helpers_input;
	};

	//class control
	class control {
	private:
		int m_index{ };
	public:
		enum control_type {
			combo,
			multicombo,
			color_selection,
			textbox,
			bind,
			size
		};

		bool m_animate[ 5 ];
		int get_index( );
		void set_index( const uintptr_t m_last );
		int m_opened[ control_type::size ];
		bool m_opened_state[ control_type::size ][ m_maximum ];
		bool is_possible( );
		bool color_picker_opened;
	};

	//external rendering
	class external_rendering {
	public:
		void install( );

		//vectors.
		std::vector< external::e_draw_text > text_draw;
		std::vector< external::e_draw_rect > outline_draw;
		std::vector< external::e_draw_rect > rect_draw;
		std::vector< external::e_draw_rect > rect_draw2;
		std::vector< external::e_draw_gradient > gradient_v_draw;
		std::vector< external::e_draw_gradient > gradient_h_draw;
	};

	extern ui::details m_details;
	extern ui::control m_control;
	extern ui::external_rendering m_ext_render;
	extern ui::input m_input;
}

#define s_rect_filled renderr->rect_fill_init
#define s_gradient renderr->gradient
#define s_rect renderr->rect_init
#define s_line renderr->line_init
#define s_text renderr->text
#define accent color_t(52, 152, 235)