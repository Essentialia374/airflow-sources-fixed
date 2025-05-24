#pragma once
#include "../group/group3.hpp"

namespace ui {
	class key_bind : public base {
	public:
		key_bind( int* value, int* bind_type );

		void update( );
		void draw( );

		void update_list( );
		void init_list( );

		vec2_t_ui pos;
	private:
		int* value;
		int* bind_type;

		bool focused;
		bool focused_list;
	};
}