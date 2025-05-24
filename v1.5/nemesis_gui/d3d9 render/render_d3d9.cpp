#include "render_d3d9.hpp"

/* main class insc */
d3d9_render* renderr;

void d3d9_render::init_device( ) {
	/* setup shaders */
	dev->SetVertexShader( nullptr );
	dev->SetPixelShader( nullptr );
	dev->SetFVF( D3DFVF_XYZRHW | D3DFVF_DIFFUSE );
	dev->SetRenderState( D3DRS_LIGHTING, false );
	dev->SetRenderState( D3DRS_FOGENABLE, false );
	dev->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	dev->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );

	/* setup states */
	dev->SetRenderState( D3DRS_ZENABLE, D3DZB_FALSE );
	dev->SetRenderState( D3DRS_SCISSORTESTENABLE, true );
	dev->SetRenderState( D3DRS_ZWRITEENABLE, false );
	dev->SetRenderState( D3DRS_STENCILENABLE, false );

	/* setup states for fonts */
	dev->SetRenderState( D3DRS_MULTISAMPLEANTIALIAS, true );
	dev->SetRenderState( D3DRS_ANTIALIASEDLINEENABLE, true );

	/* setup states for render system */
	dev->SetRenderState( D3DRS_ALPHABLENDENABLE, true );
	dev->SetRenderState( D3DRS_ALPHATESTENABLE, false );
	dev->SetRenderState( D3DRS_SEPARATEALPHABLENDENABLE, true );
	dev->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	dev->SetRenderState( D3DRS_SRCBLENDALPHA, D3DBLEND_INVDESTALPHA );
	dev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	dev->SetRenderState( D3DRS_DESTBLENDALPHA, D3DBLEND_ONE );

	/* setup states for color */
	dev->SetRenderState( D3DRS_SRGBWRITEENABLE, false );
	dev->SetRenderState( D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA );

	/* setup states for textures */
	dev->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
	dev->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	dev->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
	dev->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );
	dev->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	dev->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );

	/* init samples */
	dev->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
	dev->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
}

/* renderr->init_text setup */
int d3d9_render::get_text_width( const char* text, LPD3DXFONT font ) {
	RECT rcRect = { 0, 0, 0, 0 };
	if ( font )
	{
		font->DrawText( NULL, text, strlen( text ), &rcRect, DT_CALCRECT,
			D3DCOLOR_XRGB( 0, 0, 0 ) );
	}

	return rcRect.right - rcRect.left;
}

int d3d9_render::get_text_height( const char* text, LPD3DXFONT font ) {
	RECT rcRect = { 0, 0, 0, 0 };
	if ( font )
	{
		font->DrawText( NULL, text, strlen( text ), &rcRect, DT_CALCRECT,
			D3DCOLOR_XRGB( 0, 0, 0 ) );
	}

	return rcRect.top - rcRect.bottom;
}

#define red r()
#define green g()
#define blue b()
#define alpha a()

void d3d9_render::text( float x, float y, LPD3DXFONT font, std::string text, color_t color, bool centered, int drop_shadow, color_t shadow_color ) {
	if ( centered )
		x -= ( get_text_width( text.c_str( ), font ) / 2 );

	RECT DrawArea = { x, y, 1920, DrawArea.top + 200 };

	if ( drop_shadow == 1 )
	{
		RECT DrawArea2 = { x + 1, y + 1, 1920, DrawArea.top + 200 };
		font->DrawTextA( NULL, text.c_str( ), -1, &DrawArea2, 0, D3DCOLOR_RGBA( shadow_color.r( ), shadow_color.g( ), shadow_color.b( ), shadow_color.a( ) ) );
	}
	else if ( drop_shadow == 2 )
	{
		RECT DrawArea2 = { x + 1.f, y, 1920, DrawArea.top + 200 };
		RECT DrawArea3 = { x - 1.f, y, 1920, DrawArea.top + 200 };
		RECT DrawArea4 = { x, y + 1.f, 1920, DrawArea.top + 200 };
		RECT DrawArea5 = { x, y - 1.f, 1920, DrawArea.top + 200 };
		font->DrawTextA( NULL, text.c_str( ), -1, &DrawArea2, 0, D3DCOLOR_RGBA( shadow_color.red, shadow_color.green, shadow_color.blue, shadow_color.alpha ) );
		font->DrawTextA( NULL, text.c_str( ), -1, &DrawArea3, 0, D3DCOLOR_RGBA( shadow_color.red, shadow_color.green, shadow_color.blue, shadow_color.alpha ) );
		font->DrawTextA( NULL, text.c_str( ), -1, &DrawArea4, 0, D3DCOLOR_RGBA( shadow_color.red, shadow_color.green, shadow_color.blue, shadow_color.alpha ) );
		font->DrawTextA( NULL, text.c_str( ), -1, &DrawArea5, 0, D3DCOLOR_RGBA( shadow_color.red, shadow_color.green, shadow_color.blue, shadow_color.alpha ) );
	}

	font->DrawTextA( NULL, text.c_str( ), -1, &DrawArea, 0, D3DCOLOR_RGBA( color.red, color.green, color.blue, color.alpha ) );
}

/* renderr->gradient setup */

void d3d9_render::gradient_vertical( vec2_t_ui a, vec2_t_ui b, color_t c_a, color_t c_b ) {
	b += a;

	vertice_t verts[ 4 ] = {
		{ a.x, a.y, 0.01f, 0.01f, D3DCOLOR_RGBA( c_a.red, c_a.green, c_a.blue, c_a.alpha ) },
		{ b.x, a.y, 0.01f, 0.01f, D3DCOLOR_RGBA( c_a.red, c_a.green, c_a.blue, c_a.alpha ) },
		{ a.x, b.y, 0.01f, 0.01f, D3DCOLOR_RGBA( c_b.red, c_b.green, c_b.blue, c_b.alpha ) },
		{ b.x, b.y, 0.01f, 0.01f, D3DCOLOR_RGBA( c_b.red, c_b.green, c_b.blue, c_b.alpha ) }
	};

	// Calculate the y-coordinate range of the gradient
	float y_range = static_cast< float >( b.y - a.y );

	// Lerp between the two colors based on the y-coordinate of each vertex
	for ( int i = 0; i < 1; i++ ) {
		float lerp_factor = ( verts[ i ].y - a.y ) / y_range;
		lerp_factor = lerp_factor * lerp_factor; // Apply a square function to lerp_factor to make the second color blend more heavily towards the first color at the top of the gradient
		verts[ i ].c = D3DCOLOR_RGBA(
			static_cast< int >( ( 1.0f - lerp_factor ) * c_a.red + lerp_factor * c_b.red ),
			static_cast< int >( ( 1.0f - lerp_factor ) * c_a.green + lerp_factor * c_b.green ),
			static_cast< int >( ( 1.0f - lerp_factor ) * c_a.blue + lerp_factor * c_b.blue ),
			static_cast< int >( ( 1.0f - lerp_factor ) * c_a.alpha + lerp_factor * c_b.alpha )
		);
	}

	dev->SetTexture( 0, nullptr );
	dev->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, &verts, sizeof( vertice_t ) );
}


void d3d9_render::gradient_vertical_init( float x, float y, float w, float h, color_t c_a, color_t c_b ) {
	gradient_vertical( { x, y }, { w, h }, c_a, c_b );
}

void d3d9_render::gradient( float x, float y, float w, float h, color_t c_a, color_t c_b, bool direction ) {
	if ( direction ) { // horizontal
		gradient_horizontal_init( x, y, w, h, c_a, c_b );
	}
	else if ( !direction ) { // vertical
		gradient_vertical_init( x, y, w, h, c_a, c_b );
	}
}

/* renderr->line setup */
void d3d9_render::line( vec2_t_ui a, vec2_t_ui b, color_t c ) {
	vertice_t verts[ 2 ] = {
		{ a.x, a.y, 0.01f, 0.01f, D3DCOLOR_RGBA( c.red, c.green, c.blue, c.alpha ) },
	{ b.x, b.y, 0.01f, 0.01f, D3DCOLOR_RGBA( c.red, c.green, c.blue, c.alpha ) }
	};

	dev->SetTexture( 0, nullptr );
	dev->DrawPrimitiveUP( D3DPT_LINELIST, 1, &verts, 20 );
}

void d3d9_render::line_init( float x, float y, float x1, float x2, color_t c ) {
	line( { x, y }, { x1, x2 }, c );
}

/* renderr->rect setup */
void d3d9_render::rect( vec2_t_ui a, vec2_t_ui b, color_t c ) {
	b += a;

	b.x -= 1;
	b.y -= 1;

	vertice_t verts[ 5 ] = {
		{ float( a.x ), float( a.y ), 0.01f, 0.01f, D3DCOLOR_RGBA( c.red, c.green, c.blue, c.alpha ) },
		{ float( b.x ), float( a.y ), 0.01f, 0.01f, D3DCOLOR_RGBA( c.red, c.green, c.blue, c.alpha ) },
		{ float( b.x ), float( b.y ), 0.01f, 0.01f, D3DCOLOR_RGBA( c.red, c.green, c.blue, c.alpha ) },
		{ float( a.x ), float( b.y ), 0.01f, 0.01f, D3DCOLOR_RGBA( c.red, c.green, c.blue, c.alpha ) },
		{ float( a.x ), float( a.y ), 0.01f, 0.01f, D3DCOLOR_RGBA( c.red, c.green, c.blue, c.alpha ) }
	};

	dev->SetTexture( 0, nullptr );
	dev->DrawPrimitiveUP( D3DPT_LINESTRIP, 4, &verts, 20 );
}

void d3d9_render::rect_init( float x, float y, float w, float h, color_t c ) {
	rect( { x, y }, { w + 1, h + 1 }, c );
}

/* renderr->filled_rect setup */
void d3d9_render::rect_fill( vec2_t_ui a, vec2_t_ui b, color_t c ) {
	b += a;

	vertice_t verts[ 4 ] = {
		{ a.x, a.y, 0.01f, 0.01f, D3DCOLOR_RGBA( c.red, c.green, c.blue, c.alpha ) },
	{ b.x, a.y, 0.01f, 0.01f, D3DCOLOR_RGBA( c.red, c.green, c.blue, c.alpha ) },
	{ a.x, b.y, 0.01f, 0.01f, D3DCOLOR_RGBA( c.red, c.green, c.blue, c.alpha ) },
	{ b.x, b.y, 0.01f, 0.01f, D3DCOLOR_RGBA( c.red, c.green, c.blue, c.alpha ) }
	};

	dev->SetTexture( 0, nullptr );
	dev->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, &verts, 20 );
}

void d3d9_render::rect_fill_init( float x, float y, float w, float h, color_t c ) {
	rect_fill( { x, y }, { w, h }, c );
}

void d3d9_render::gradient_horizontal( vec2_t_ui a, vec2_t_ui b, color_t c_a, color_t c_b )
{
	b += a;

	vertice_t verts[ 4 ] = {
		{ a.x, a.y, 0.01f, 0.01f, D3DCOLOR_RGBA( c_a.red, c_a.green, c_a.blue, c_a.alpha ) },
		{ b.x, a.y, 0.01f, 0.01f, D3DCOLOR_RGBA( c_b.red, c_b.green, c_b.blue, c_b.alpha ) },
		{ a.x, b.y, 0.01f, 0.01f, D3DCOLOR_RGBA( c_a.red, c_a.green, c_a.blue, c_a.alpha ) },
		{ b.x, b.y, 0.01f, 0.01f, D3DCOLOR_RGBA( c_b.red, c_b.green, c_b.blue, c_b.alpha ) }
	};

	dev->SetTexture( 0, nullptr );
	dev->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, &verts, 20 );
}

void d3d9_render::gradient_horizontal_init( float x, float y, float w, float h, color_t c_a, color_t c_b )
{
	gradient_horizontal( { x, y }, { w, h }, c_a, c_b );
}

void d3d9_render::rect_outlined( float x, float y, float w, float h, color_t c, color_t c2 ) {
	rect_init( x, y, w, h, c );
	rect_init( x - 1, y - 1, w + 2, h + 2, c2 );
	rect_init( x + 1, y + 1, w - 2, h - 2, c2 );
}
