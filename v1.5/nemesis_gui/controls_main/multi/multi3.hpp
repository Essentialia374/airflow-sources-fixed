#pragma once
#include "../group/group3.hpp"

namespace ui {
	/* multibox constructor */
	struct multi_constructor {
		multi_constructor( std::string name, bool* value ) {
			this->name = name;
			this->value = value;
		}

		std::string name;
		bool* value;
	};

	class multi_combo : public base {
	public:
		multi_combo( );
		multi_combo( std::string name );

		void update( );
		void draw( );
		void add_item( std::string name, bool* value );
		vec2_t_ui pos;
	private:
		std::vector< multi_constructor > items;
		std::string name;
		bool focused;
	};
}