#include "base.hpp"

base* g_base;
void base::destroy( base* m_element ) {
	delete m_element;
}