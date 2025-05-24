#pragma once

/* c++ includes */
#include "../../../includes.h"
#include "../../vectuwu.hpp"
#include "../gui_main/gui3.hpp"
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "d3d9.lib")
#include <d3dx9.h>
#include <d3d9.h>
#define color_t Color

/* vertices */
struct vertice_t {
	float x, y, z, rhw;
	int c;
};

/* main class */
class d3d9_render {
public:
	d3d9_render( IDirect3DDevice9* device ) {
		this->dev = device;
		this->vertex_shader = vertex_shader;

		device->GetViewport( &screen_size );
	}

	IDirect3DVertexShader9* vertex_shader;
	D3DVIEWPORT9 screen_size;
public:
	D3DVIEWPORT9 get_viewport( ) {
		D3DVIEWPORT9 vp;
		dev->GetViewport( &vp );
		return vp;
	}

	/* viewports */
	void set_viewport( D3DVIEWPORT9 vp ) {
		dev->SetViewport( &vp );
	}
public:
	/* fonts */
	struct {
		LPD3DXFONT main_font;
		LPD3DXFONT menu_font;
		LPD3DXFONT indicators;

		LPD3DXFONT name_esp_font;
		LPD3DXFONT pixel_font;
		LPD3DXFONT smallest_font;

		LPD3DXFONT group_font;
		LPD3DXFONT second_font;
	} fonts;
public:
	/* elements */
	void init_device( );

	int get_text_width( const char* text, LPD3DXFONT font );
	int get_text_height( const char* text, LPD3DXFONT font );
	void text( float x, float y, LPD3DXFONT font, std::string text, color_t color, bool centered = false, int drop_shadow = 0, color_t shadow_color = color_t( ) );

	void line( vec2_t_ui a, vec2_t_ui b, color_t c );
	void line_init( float x, float y, float x1, float x2, color_t c );

	void rect_fill( vec2_t_ui a, vec2_t_ui b, color_t c );
	void rect_fill_init( float x, float y, float w, float h, color_t c );
	void rect_outlined( float x, float y, float w, float h, color_t c, color_t c2 );

	void rect( vec2_t_ui a, vec2_t_ui b, color_t c );
	void rect_init( float x, float y, float w, float h, color_t c );

	void gradient_vertical( vec2_t_ui a, vec2_t_ui b, color_t c_a, color_t c_b );
	void gradient_vertical_init( float x, float y, float w, float h, color_t c_a, color_t c_b );

	void gradient_horizontal( vec2_t_ui a, vec2_t_ui b, color_t c_a, color_t c_b );
	void gradient_horizontal_init( float x, float y, float w, float h, color_t c_a, color_t c_b );

	/*

		void triangle( vec2_t pos1, vec2_t pos2, vec2_t pos3, color_t color );
		void triangle_fill( vec2_t pos1, vec2_t pos2, vec2_t pos3, color_t color );

		void polygon( const std::vector<std::pair<int, int>>& verticies, color_t color, bool line = false );
		void circle3d( const vec3_t& pos, int rad, int segments, color_t color, bool line = false );

	*/
	void gradient( float x, float y, float w, float h, color_t c_a, color_t c_b, bool direction );
private:
	IDirect3DDevice9* dev;
	bool b_ready = false;
};
extern d3d9_render* renderr;