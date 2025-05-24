#pragma once
#include "../../gui_main/gui3.hpp"

class base {
	virtual void destroy( base* m_element );
};
extern base* g_base;