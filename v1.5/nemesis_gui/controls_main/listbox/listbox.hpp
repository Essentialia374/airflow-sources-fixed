#pragma once
#include "../group/group3.hpp"

namespace ui {
	class list_box : public base {
	public:
		list_box( std::string m_Name, int* m_Select, int size_y, int items, std::vector<std::string>& m_Items );
		list_box( std::string m_Name, int* m_Select, int size_y, int items, std::vector<std::string>& m_Items, std::string& m_Filter );
		void Draw( );
		void Update( );

		vec2_t m_Pos;
		int pulica_size;
		int item_to_dra;
	private:

		void GetItems( );
		void Scroll( int m_Limit );

		std::string ToLower( std::string String );
		int* m_Select;

		std::vector<std::string> m_LowerItems;
		std::vector<std::string>* m_Items;
		int size_y;
		int items_to_draw;
		std::string* m_Filter;
		std::string m_PreviousFilter = "123";

		std::string m_Name;
		std::vector< std::pair<std::string, int >> m_Temp;

	};

	class text_input : public base {
	public:

		text_input( std::string* m_Label, std::string name_put, std::string name_in );

		void Update( );
		void Draw( );

		vec2_t m_Pos;

	private:

		std::string* m_Label;
		bool m_Focused;
		std::string name_put;
		std::string name_in;

	};
}