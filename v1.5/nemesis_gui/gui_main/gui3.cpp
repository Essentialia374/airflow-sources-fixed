#include "gui3.hpp"
#include "../indicators/indicators.hpp"
#include "../../../lua.hpp"
/* controls */
#include "../controls_main/window/window3.hpp"
#include "../controls_main/group/group3.hpp"
#include "../../rpg_game.hpp"
#include <codecvt>

ui::details ui::m_details;
ui::input ui::m_input;
ui::control ui::m_control;
ui::external_rendering ui::m_ext_render;

static int slot_cfg = 0;
static int lua_cfg = 0;
std::string config_name;
std::string skin_filter;
std::string new_cfg;

namespace ui {
	bool details::get_menu_state( ) {
		return menu_show;
	}

	void details::set_menu_starte( const bool m_state ) {
		menu_show = m_state;
	}

	vec2_t_ui input::get_mouse_pos( ) {
		return m_mouse_pos;
	}

	void input::set_mouse_pos( const vec2_t_ui m_pos ) {
		m_mouse_pos = vec2_t_ui{ m_pos.x, m_pos.y };
	}

	void input::pool_input( ) {
		for ( size_t i{ 0 }; i < 256; i++ ) {
			helpers_input.m_prev_key_state[ i ] = helpers_input.m_key_state[ i ];
			helpers_input.m_key_state[ i ] = GetAsyncKeyState( i );
		}
	}

	bool input::key_pressed( const uintptr_t m_key ) {
		return helpers_input.m_key_state[ m_key ] && !helpers_input.m_prev_key_state[ m_key ];
	}

	bool input::mouse_pointer( const vec2_t_ui m_pos, const vec2_t_ui m_size ) {
		std::pair<bool, bool> m_should_return{
			get_mouse_pos( ).x > m_pos.x && get_mouse_pos( ).y > m_pos.y,
			get_mouse_pos( ).x < m_pos.x + m_size.x && get_mouse_pos( ).y < m_pos.y + m_size.y
		};

		return m_should_return.first && m_should_return.second;
	}

	void input::set_mouse_wheel( const float m_mouse ) {
		next_menu_wheel = m_mouse;
	}

	float input::get_mouse_wheel( ) {
		return next_menu_wheel;
	}

	int control::get_index( ) {
		return m_index;
	}

	void control::set_index( const uintptr_t m_last ) {
		m_index = m_last;
	}

	bool control::is_possible( ) {
		return m_control.m_opened[ control_type::combo ] == -1 &&
			m_control.m_opened[ control_type::multicombo ] == -1 &&
			m_control.m_opened[ control_type::bind ] == -1;
	}

	void external_rendering::install( ) {
		for ( size_t i{ 0 }; i < rect_draw.size( ); i++ )
			renderr->rect_fill_init( rect_draw[ i ].pos_x, rect_draw[ i ].pos_y, rect_draw[ i ].m_width, rect_draw[ i ].m_height, rect_draw[ i ].color );

		for ( size_t i{ 0 }; i < gradient_h_draw.size( ); i++ )
			renderr->gradient_horizontal_init( gradient_h_draw[ i ].pos_x, gradient_h_draw[ i ].pos_y, gradient_h_draw[ i ].m_width, gradient_h_draw[ i ].m_height, gradient_h_draw[ i ].color, gradient_h_draw[ i ].color2 );

		for ( size_t i{ 0 }; i < gradient_v_draw.size( ); i++ )
			renderr->gradient_vertical_init( gradient_v_draw[ i ].pos_x, gradient_v_draw[ i ].pos_y, gradient_v_draw[ i ].m_width, gradient_v_draw[ i ].m_height, gradient_v_draw[ i ].color, gradient_v_draw[ i ].color2 );

		for ( size_t i{ 0 }; i < outline_draw.size( ); i++ )
			renderr->rect_init( outline_draw[ i ].pos_x, outline_draw[ i ].pos_y, outline_draw[ i ].m_width, outline_draw[ i ].m_height, outline_draw[ i ].color );

		for ( size_t i{ 0 }; i < rect_draw2.size( ); i++ )
			renderr->rect_fill_init( rect_draw2[ i ].pos_x, rect_draw2[ i ].pos_y, rect_draw2[ i ].m_width, rect_draw2[ i ].m_height, rect_draw2[ i ].color );

		for ( size_t i{ 0 }; i < text_draw.size( ); i++ )
			renderr->text(  text_draw[ i ].pos_x, text_draw[ i ].pos_y, renderr->fonts.menu_font, text_draw[ i ].m_text, text_draw[ i ].color );

		rect_draw.clear( );
		outline_draw.clear( );
		gradient_h_draw.clear( );
		gradient_v_draw.clear( );
		rect_draw2.clear( );
		text_draw.clear( );
	}

	vec2_t_ui window_position = vec2_t_ui( 350, 460 );
	std::vector<std::string> tabs = { "Aimbot", "Anti-aim", "Visuals", "Skins", "Misc" };
	std::vector<std::string> test_combo2 = { "Distance", "Crosshair", "Damage", "Health", "Lagcomp", "Height" };
	std::vector<std::string> test_combo3 = { "Off", "Down", "Up", "Random", "Ideal" };
	std::vector<std::string> test_combo4 = { "Off", "Jiter", "Rotate", "Random" };
	std::vector<std::string> test_combo5 = { "Off", "Static", "Away crosshair", "Away distance" };
	std::vector<std::string> t4 = { "Automatic", "Backwards", "Right", "Left" };
	std::vector<std::string> test_combo6 = { "Off", "Left", "Right", "Opposite","Z", "Twisted" };
	std::vector<std::string> test_combo7 = { "Max", "Break", "Random", "Break step" };
	std::vector<std::string> test_combo8 = { "Off", "Default", "Relative", "Jitter","Rotate", "Random", "Local view" };
	std::vector<std::string> test_combo9 = { "Normal", "Flat", "Wireframe Animated", "Glow" };
	std::vector<std::string> test_combo10 = { "Slot 1", "Slot 2", "Slot 3", "Slot 4", "Slot 5", "Slot 6", "Slot 7" };
	std::vector<std::string> autoBuyPrimary = { "Auto", "Scout", "AWP" };
	std::vector<std::string> autoBuySecondary = { "Dualies", "Heavy pistols" };
	std::vector<std::string> t1 = { "Intensive", "Smart", "High" };
	std::vector<std::string> t2 = { "Disabled", "Viewangles", "Directional" };
	std::vector<std::string> t3 = { "SlidePeek", "Always" };
	std::vector<std::string> t19 = { "Default", "Simulate" };
	std::vector<std::string> t6 = { "Offensive", "Defensive", "Nemesis" };
	std::vector<std::string> t9 = { XOR( "none" ), XOR( "full" ), XOR( "quick" ), XOR( "alternative" ) };
	std::vector<std::string> t8 = { "Rain", "Snow" };
	std::vector<std::string> t14 = { "Normal", "Color modulate" };
	std::vector<std::string> t15 = { "Default", "Experimental", "Memory test" };
	std::vector<std::string> t17 = { "Midget", "Boreal animfix", "Shuffle" };
	std::vector<std::string> t16 = { "Enemy", "Local" };
	std::vector<std::string> t18 = { "Default", "Customizable" };
	std::vector<std::string> t11 = { "Pattern #1", "Pattern #2", "Pattern #3", "Pattern #4","Pattern #5","Pattern #6", "Pattern #7" };
	std::vector<std::string> t12 = { XOR( "Off" ), XOR( "Bayonet" ), XOR( "Bowie" ), XOR( "Butterfly" ), XOR( "Falchion" ), XOR( "Flip" ), XOR( "Gut" ), XOR( "Huntsman" ), XOR( "Karambit" ), XOR( "M9 Bayonet" ), XOR( "Daggers" ) };
	std::vector<std::string> t13 = { XOR( "Off" ), XOR( "Bloodhound" ), XOR( "Sport" ), XOR( "Driver" ), XOR( "Handwraps" ), XOR( "Moto" ), XOR( "Specialist" ) };
	//std::vector<std::string> t10 = lua_deathmesis::g_lua->get_lua( );
	std::vector<std::string> t7 = { "Triangle", "Diamond", "Circle", "Hexagonal" };
	std::vector<std::string> t5 = { "Off",XOR( "Tibet" ),XOR( "Daylight" ),XOR( "Cloudy" ),XOR( "Night 1" ),XOR( "Vertigo Blue HD" ),XOR( "Vertigo" ),XOR( "Vietnam" ),XOR( "Dusty Sky" ),XOR( "Jungle" ),XOR( "Nuke" ),XOR( "Office" ) };
	std::vector<std::string> hah = { "None" };
	std::vector<std::string> stopbs = { ( "None" ),( "Full stop" ),( "Minimal stop" ) };
	std::vector<std::string> stop_conditions = { ( "None" ),( "Always" ),( "Lethal health" ) };
	std::vector<std::string> stop_mode = { ( "Full stop" ), ( "Minimal stop" ) };
	std::vector<std::string> t01 = { ( "Default" ), ( "Custom" ) };
	std::vector<std::string> t02 = { ( "Default" ), ( "Smart-scan" ), "Selective" };
	std::string string_from_vector( const std::vector<std::string>& pieces ) {
		std::stringstream ss;

		for ( std::vector<std::string>::const_iterator itr = pieces.begin( );
			itr != pieces.end( );
			++itr ) {
			ss << *itr;
		}

		return ss.str( );
	}

	void save( ) {
		if ( !cfg::g_cfg->files_cfg.size( ) )
			return;

		settings.Save( cfg::g_cfg->files_cfg[ slot_cfg ] );
	}

	void save_game( ) {
		nem::g_games->save_progress( );
	}

	void create( ) {
		//settings.CreateConfig( config_name );
		settings.Save( config_name );
		settings.GetConfigs( );

		// clear the last name
		config_name.clear( );
	}

	void load( ) {
		if ( !cfg::g_cfg->files_cfg.size( ) )
			return;

		settings.Load( slot_cfg );
	}

	void load_lua_gui( ) {
		if ( !lua_deathmesis::g_lua->files.size( ) )
			return;

		lua_deathmesis::g_lua->load_lua( lua_deathmesis::g_lua->files[ lua_cfg ] );
		//g_config.config_files( );
	}

	void force_update( ) {
		cfg::g_cfg->foce_upate = true;
	}

	std::unordered_map<std::string, GLOVE_SKIN> gloveSkinMap = {
		{"Amphibious", GLOVE_SKIN::S_Amphibious},
		{"Arboreal", GLOVE_SKIN::S_Arboreal},
		{"Arid", GLOVE_SKIN::S_Arid},
		{"Badlands", GLOVE_SKIN::S_Badlands},
		{"Boom!", GLOVE_SKIN::S_Boom},
		{"Bronze Morph", GLOVE_SKIN::S_BronzeMorph},
		{"Bronzed", GLOVE_SKIN::S_Bronzed},
		{"Buckshot", GLOVE_SKIN::S_Buckshot},
		{"Case Hardened", GLOVE_SKIN::S_CaseHardened},
		{"Charred", GLOVE_SKIN::S_Charred},
		{"Cobalt Skulls", GLOVE_SKIN::S_CobaltSkulls},
		{"Convoy", GLOVE_SKIN::S_Convoy},
		{"Cool Mint", GLOVE_SKIN::S_CoolMint},
		{"Crimson Kimono", GLOVE_SKIN::S_CrimsonKimono},
		{"Crimson Weave", GLOVE_SKIN::S_CrimsonWeave},
		{"Crimson Web", GLOVE_SKIN::S_CrimsonWeb},
		{"Diamondback", GLOVE_SKIN::S_Diamondback},
		{"Duct Tape", GLOVE_SKIN::S_DuctTape},
		{"Eclipse", GLOVE_SKIN::S_Eclipse},
		{"Emerald Web", GLOVE_SKIN::S_EmeraldWeb},
		{"Emerald", GLOVE_SKIN::S_Emerald},
		{"Fade", GLOVE_SKIN::S_Fade},
		{"Forest DDPAT", GLOVE_SKIN::S_ForestDDPAT},
		{"Foundation", GLOVE_SKIN::S_Foundation},
		{"Guerrilla", GLOVE_SKIN::S_Guerrilla},
		{"Hedge Maze", GLOVE_SKIN::S_HedgeMaze},
		{"Mangrove", GLOVE_SKIN::S_Mangrove},
		{"Pandora's Box", GLOVE_SKIN::S_PandorasBox},
		{"Rattler", GLOVE_SKIN::S_Rattler},
		{"Slaughter", GLOVE_SKIN::S_Slaughter},
		{"Snakebite", GLOVE_SKIN::S_Snakebite},
		{"Spruce DDPAT", GLOVE_SKIN::S_SpruceDDPAT},
		{"Superconductor", GLOVE_SKIN::S_Superconductor},
		{"Transport", GLOVE_SKIN::S_Transport},
		{"Turtle", GLOVE_SKIN::S_Turtle},
		{"Vice", GLOVE_SKIN::S_Vice},
	};


	std::unordered_map<std::string, GLOBAL_SKIN> skin_names = {
		{ "Doppler Red", S_DOPPLER_RED },
		{ "Doppler Blue", S_DOPPLER_BLUE },
		{ "Doppler Black Pearl", S_DOPPLER_BLACK_PEARL },
		{ "Doppler Phase 1", S_DOPPLER_PHASE_1 },
		{ "Doppler Phase 2", S_DOPPLER_PHASE_2 },
		{ "Doppler Phase 3", S_DOPPLER_PHASE_3 },
		{ "Doppler Phase 4", S_DOPPLER_PHASE_4 },
		{ "Gamma Doppler Emerald", S_GAMMA_DOPPLER_EMERALD },
		{ "Gamma Doppler Phase 1", S_GAMMA_DOPPLER_PHASE_1 },
		{ "Gamma Doppler Phase 2", S_GAMMA_DOPPLER_PHASE_2 },
		{ "Gamma Doppler Phase 3", S_GAMMA_DOPPLER_PHASE_3 },
		{ "Gamma Doppler Phase 4", S_GAMMA_DOPPLER_PHASE_4 },
		{ "Whiteout", S_WHITEOUT }
	};


	void details::install( ) {
		m_input.pool_input( );

		if ( m_input.key_pressed( VK_INSERT ) )
			set_menu_starte( !get_menu_state( ) );

		// i wont press insert all time in debugging
		 if ( !get_menu_state( ) )
			return;

		 m_control.set_index( 0 );
		 m_control.color_picker_opened = false;

		//opened states
		m_control.m_opened[ m_control.control_type::combo ] = -1;
		m_control.m_opened[ m_control.control_type::multicombo ] = -1;
		m_control.m_opened[ m_control.control_type::bind ] = -1;

		auto g_window = new window( "project alpha", &window_position, { 800, 600 }, tabs, &m_index ); {
			auto group = new group_box( "MAIN", 0, 15, 80, 380, 248 , 0 ); {
				g_window->add_group( group );
				group->handle( new check_box( "Enable", &settings.enable ) );
				group->handle( new check_box( "Show viewangles", &settings.silent ) );
				group->handle( new check_box( "Autoscope", &settings.autoscopee ) );
				//group->handle( new check_box( "Print debug logs", &settings.draw_debug ) );

				group->handle( new combo( "Record selection", &settings.best_aim_selection, t19 ) );
				group->handle( new combo( "Sorting", &settings.selection, test_combo2 ) );

				auto aim_twe = new multi_combo( "Aimbot tweaks" );
				aim_twe->add_item( "Decrease hitchance seeds", &settings.decrease_hitchance_seeds );
				aim_twe->add_item( "Reserve points", &settings.reserve_points );
				aim_twe->add_item( "Control recoil", &settings.norecoil );
				aim_twe->add_item( "Clear older records", &settings.clear_older_records );
				aim_twe->add_item( "Disable safe choke", &settings.disable_safe_choke );
				group->handle( aim_twe );
	
				group->handle( new check_box( "Detect cheat", &settings.cheat_detector ) );
#ifdef DEV_
				group->handle( new check_box( "Receieve net msg", &settings.receive_msg ) );
#endif

				group->handle_scroll( );
			} delete group;

			auto group1 = new group_box( "HITBOXES", 1, 15, 80 + 10 + 248, 380, 248, 0 ); {
				g_window->add_group( group1 );

				auto hitbox_selection = new multi_combo( "Hitscan" );
				hitbox_selection->add_item( "Head", &settings.hitbox[ 0 ] );
				hitbox_selection->add_item( "Chest", &settings.hitbox[ 1 ] );
				hitbox_selection->add_item( "Body", &settings.hitbox[ 2 ] );
				hitbox_selection->add_item( "Arms", &settings.hitbox[ 3 ] );
				hitbox_selection->add_item( "Legs", &settings.hitbox[ 4 ] );
				group1->handle( hitbox_selection );

				static bool aa_back = false;
				group1->handle( new check_box( "Hitscan override key", &aa_back, true ) );
				group1->handle( new key_bind( &settings.o_hv_key, &settings.o_hv_key_mode ) );

				auto hitbox_ovv = new multi_combo( "Hitscan override" );
				hitbox_ovv->add_item( "Head", &settings.hitbox_override[ 0 ] );
				hitbox_ovv->add_item( "Chest", &settings.hitbox_override[ 1 ] );
				hitbox_ovv->add_item( "Body", &settings.hitbox_override[ 2 ] );
				hitbox_ovv->add_item( "Arms", &settings.hitbox_override[ 3 ] );
				hitbox_ovv->add_item( "Legs", &settings.hitbox_override[ 4 ] );
				group1->handle( hitbox_ovv );

				group1->handle( new combo( "Multipoint scan type", &settings.multipoint_type, t02 ) );

				auto multiss = new multi_combo( "Multipoint" );
				multiss->add_item( "Head", &settings.multipoint[ 0 ] );
				multiss->add_item( "Chest", &settings.multipoint[ 1 ] );
				multiss->add_item( "Body", &settings.multipoint[ 2 ] );
				multiss->add_item( "Legs", &settings.multipoint[ 3 ] );
				group1->handle( multiss );

				//group1->handle( new combo( "Hitbox scale", &settings.scan_mode, t1 ) );
				group1->handle( new check_box( "Ignore unsafe if lethal", &settings.deeal_more_damage ) );
				group1->handle( new check_box( "Static point-scale", &settings.static_pscale ) );
				if ( settings.static_pscale ) {
					group1->handle( new slider( "Hitbox scale", &settings.scale, 0, 100, "%" ) );
					group1->handle( new slider( "Body scale", &settings.body_scale, 0, 100, "%" ) );
				}

				std::string autom;
				std::string autom2;
				if ( settings.minimal_damage == 0 ) {
					autom = " Automatic";
				}

				if ( settings.penetrate_minimal_damage == 0 ) {
					autom2 = " Automatic";
				}

				group1->handle( new slider( "Visible minimum damage" + autom, &settings.minimal_damage, 0, 100, "hp" ) );
				group1->handle( new slider( "Wallbang minimum damage" + autom2, &settings.penetrate_minimal_damage, 0, 100, "hp" ) );

				group1->handle( new check_box( "Override minimum damage key", &aa_back, true ) );
				group1->handle( new key_bind( &settings.o_dmg_key, &settings.o_dmg_key_mode ) );
				if ( settings.o_dmg_key > 0 ) {
					group1->handle( new slider( "Override minimum damage", &settings.o_dmg_overr, 0, 100, "hp" ) );
				}

				group1->handle_scroll( );
			} delete group1;

			auto group2 = new group_box( "ACCURACY", 2, 15 + 380 + 10, 80 , 380, 340 , 0 ); {
				g_window->add_group( group2 );
				group2->handle( new check_box( "Resolver", &settings.correct ) );
				group2->handle( new combo( "Resolver mode", &settings.resolver_Mode, t15 ) );

				group2->handle( new check_box( "Fakelag correction", &settings.lagfix ) );


				group2->handle( new check_box( "Hitchance", &settings.hitchance ) );
				if ( settings.hitchance ) {
					//auto HCCO = new multi_combo( "Hitchance conditions" );
					//HCCO->add_item( "Double-tap hitchance", &settings.doubletap_hc );
					//HCCO->add_item( "Override hitchance in air", &settings.custom_hc_in_auir );
					//HCCO->add_item( "Accuracy boost", &settings.accbost );
					//group2->handle( HCCO );

					group2->handle( new slider( "Hitchance", &settings.hitchance_amount, 0, 100, "%" ) );

					if ( settings.doubletap_hc ) {
						group2->handle( new slider( "Double-tap hitchance", &settings.dt_hc, 0, 100, "%" ) );
					}

					if ( settings.custom_hc_in_auir ) {
						group2->handle( new slider( "Hitchance in air", &settings.hcInair, 0, 100, "%" ) );
					}

					if ( settings.accbost ) {
						group2->handle( new slider( "Accuracy boost", &settings.acuracy_bost, 0, 100, "%" ) );
					}

					/*
										static bool aa_back = false;
					group2->handle( new check_box( "Override hitchance key", &aa_back, true ) );
					group2->handle( new key_bind( &settings.o_hc_key, &settings.o_hc_key_mode ) );

					group2->handle( new slider( "Override hitchance", &settings.o_hc_overr, 0, 100, "hp" ) );
					*/
				}

				group2->handle( new combo( "Autostop conditions", &settings.m_auto_stop_conditions, stop_conditions ) );
				group2->handle( new combo( "Autostop mode", &settings.m_auto_stop_type, stop_mode ) );
				group2->handle( new combo( "Autostop between shots", &settings.m_between_shots_auto_stop_type, stopbs ) );


				auto baim_co = new multi_combo( "Baim conditions" );
				baim_co->add_item( "While double-shoting", &settings.while_doubletaping );
				baim_co->add_item( "In air", &settings.while_enemy_in_air );
				baim_co->add_item( "While broken-lc", &settings.when_lc_broke );
				baim_co->add_item( "When fake-angles", &settings.while_fake_shooting );
				baim_co->add_item( "After x misses", &settings.on_x_misses );
				group2->handle( baim_co );

				if ( settings.on_x_misses ) {
					group2->handle( new slider( "X misses", &settings.x_misses, 0, 15, "%" ) );
				}

				// for testing
				group2->handle( new check_box( "Force head while move", &settings.force_head_while_moving, true ) );

				static bool aa = false;
				group2->handle( new check_box( "Force baim", &aa, true ) );
				group2->handle( new key_bind( &settings.o_fb_key, &settings.o_fb_key_mode ) );

				group2->handle_scroll( );
			} delete group2;

			auto group3 = new group_box( "EXPLOITS", 3,15 + 380 + 10, 80 + 340 + 10 , 380, 155, 0 ); {
				g_window->add_group( group3 );
				group3->handle( new check_box( "Double tap", &settings.m_enabled ) );
				group3->handle( new key_bind( &settings.m_dt_key, &settings.dtstyle ) );
				group3->handle( new check_box( "Extend teleport", &settings.dt_defensive_teleport ) );
				group3->handle( new check_box( "Offensive charge", &settings.less_tp ) );			
			} delete group3;

			auto group4 = new group_box( "MAIN", 4, 15, 80, 380, 248, 1 ); {
				g_window->add_group( group4 );
				group4->handle( new check_box( "Enabled", &settings.enable_aa ) );
				auto disaa = new multi_combo( "Disable anti-aim conditions" ); {
					disaa->add_item( "Round end", &settings.disable_aa_condition[ 0 ] );
					disaa->add_item( "Round start", &settings.disable_aa_condition[ 1 ] );
				} group4->handle( disaa );

				group4->handle( new combo( "Pitch", &settings.pitch_stand, test_combo3 ) );
				//group4->handle( new combo( "Base yaw", &settings.base_angle_stand, test_combo5 ) );	
				group4->handle( new combo( "Real yaw", &settings.yaw_stand, test_combo4 ) );

				if ( settings.yaw_stand == 1 ) {
					group4->handle( new slider( "Jitter range", &settings.jitter_range_stand, 0, 180, "%" ) );
				}


				group4->handle_scroll( );
			} delete group4;

			auto group5 = new group_box( "FAKELAG", 5, 15, 80 + 10 + 248, 380, 248, 1 ); {
				g_window->add_group( group5 );
				group5->handle( new check_box( "Enabled", &settings.lag_enable ) );
				group5->handle( new check_box( "Adaptive choke", &settings.lag_land ) );
				group5->handle( new combo( "Mode", &settings.lag_mode, test_combo7 ) );
				group5->handle( new slider( "Ticks", &settings.lag_limit, 0, 14, "t" ) );
			} delete group5;


			auto group6 = new group_box( "ANGLE", 6, 15 + 380 + 10, 80, 380, 340 + 10 + 155, 1 ); {
				g_window->add_group( group6 );
				auto aatwk = new multi_combo( "Anti-aim tweaks" ); {
					aatwk->add_item( "Edge", &settings.anti_aim_tweaks[ 0 ] );
					aatwk->add_item( "Freestanding", &settings.anti_aim_tweaks[ 1 ] );
					aatwk->add_item( "Break last-move", &settings.anti_aim_tweaks[ 4 ] );
					aatwk->add_item( "Disable lby in air", &settings.anti_aim_tweaks[ 5 ] );
					aatwk->add_item( "Manual anti-aim", &settings.anti_aim_tweaks[ 6 ] );
				} group6->handle( aatwk );

				

				group6->handle( new combo( "Lby breaker type", &settings.breaker_mode, t18 ) );
				switch (  settings.breaker_mode ) {
					case 0: {
						group6->handle( new combo( "Fake body", &settings.body_fake_stand, test_combo6 ) );
					} break;
					case 1: {
						group6->handle( new check_box( "Lby breaker", &settings.lby_breaker ) );
						group6->handle( new check_box( "Static angle", &settings.static_angle ) );
						group6->handle( new check_box( "Optimal adjust", &settings.optimalbreak ) );

						group6->handle( new slider( "First break", &settings.break_lby_first, 0, 120, "°" ) );
						group6->handle( new slider( "Break amount", &settings.break_lby, 0, 120, "°" ) );
					} break;
				}

				if ( settings.yaw_stand > 0 )
					group6->handle( new combo( "Fake Yaw", &settings.fake_yaw, test_combo8 ) );

				if ( settings.fake_yaw == 2 ) {
					group6->handle( new slider( "Relative angle", &settings.fake_relative, 0, 180, "°" ) );
				}

				auto distor = new multi_combo( "Distortion" ); {
					distor->add_item( "Shift", &settings.distortion_shift );
					distor->add_item( "Force turn", &settings.distortion_fturn );			
					distor->add_item( "Ignore manual", &settings.ignoreidst_in_manual );			
				} group6->handle( distor );

				if ( settings.distortion_shift ) {
					group6->handle( new slider( "Distortion angle", &settings.distorio_angle, -180, 180, "°" ) );
				}

				bool ciocan = false;
				bool ciocan2 = false;
				bool ciocan3 = false;
				group6->handle( new check_box( "Fake-duck", &ciocan, true ) );
				group6->handle( new key_bind( &settings.fd_keu, &settings.fdstyle ) );

				group6->handle( new check_box( "Exploit angle", &ciocan2, true ) );
				group6->handle( new key_bind( &settings.expwl2_key, &settings.expwl2_keys ) );

				group6->handle( new check_box( "Fake walk", &ciocan3, true ) );
				group6->handle( new key_bind( &settings.fw_key, &settings.fw_key_keys ) );
				group6->handle( new slider( "Fakewalk speed", &settings.fw_speed, 0, 150, "°" ) );

				if ( settings.anti_aim_tweaks[ 6 ] ) {
					static bool aa_left = false;
					static bool aa_right = false;
					static bool aa_back = false;
					group6->handle( new check_box( "Manual left", &aa_left, true ) );
					group6->handle( new key_bind( &settings.aa_left, &settings.tkey_style ) );

					group6->handle( new check_box( "Manual right", &aa_right, true ) );
					group6->handle( new key_bind( &settings.aa_right, &settings.tkey_style ) );

					group6->handle( new check_box( "Manual back", &aa_back, true ) );
					group6->handle( new key_bind( &settings.aa_back, &settings.tkey_style ) );
				}

		/*
		
				static bool aa_back2 = false;
				group6->handle( new check_box( "Exploit-walk", &aa_back2 ) );
				group6->handle( new key_bind( &settings.expwl_key, &settings.expwl_keys ) );*/


			} delete group6;

			auto group7 = new group_box( "ENEMY", 7, 15, 80, 380, 248, 2 ); {
				g_window->add_group( group7 );

				/*
				static int group = 0;
				group7->handle( new combo( "Esp group", &group, t16 ) );
				switch ( group ) {
					case 0: {

					} break;
					case 1: {

					} break;
				}
				*/


				group7->handle( new check_box( "Enabled", &settings.enable_esp ) );

				group7->handle( new check_box( "Box", &settings.box_enemy ) );
				group7->handle( new color_selector( &settings.box_color, &settings.box_hue, true ) );

				group7->handle( new check_box( "Name", &settings.name_esp ) );
				group7->handle( new color_selector( &settings.name_color, &settings.name_hue, true ) );

				group7->handle( new check_box( "Health", &settings.health_esp ) );
				if ( settings.health_esp ) {
					group7->handle( new check_box( "Health color override", &settings.override_hbar ) );
					group7->handle( new color_selector( &settings.hbar, &settings.hbar_hue, true ) );
				}

				group7->handle( new check_box( "Ammo", &settings.ammo_esp ) );
				group7->handle( new color_selector( &settings.ammo_color, &settings.ammo_hue, true ) );

				group7->handle( new check_box( "Weapon", &settings.weapon_esp ) );
				group7->handle( new color_selector( &settings.weapon_color, &settings.weapoon_hue, true ) );

				//group7->handle( new check_box( "Shot timer", &settings.shot_timer ) );
				//group7->handle( new color_selector( &settings.shotimer, &settings.shotimer_hue, true ) );
				

				// dcrash
				group7->handle( new check_box( "Skeleton [!]", &settings.skel1 ) );
				group7->handle( new color_selector( &settings.skele_color, &settings.skel_hue, true ) );

				group7->handle( new check_box( "Skeleton history [!]", &settings.skel2 ) );
				group7->handle( new color_selector( &settings.skele2_color, &settings.skel2_hue, true ) );

				auto flgs = new multi_combo( "Flags" );
				flgs->add_item( "Money", &settings.flags[ 0 ] );
				flgs->add_item( "Armor", &settings.flags[ 1 ] );
				flgs->add_item( "Scoped", &settings.flags[ 2 ] );
				flgs->add_item( "Resolver", &settings.flags[ 3 ] );
				flgs->add_item( "Vulnerable", &settings.flags[ 4 ] );
				flgs->add_item( "Break lc", &settings.break_lc_flag );
				//flgs->add_item( "Hit", &settings.hit_flag );
				group7->handle( flgs );

				group7->handle( new check_box( "Glow", &settings.enemy_glow ) );
				group7->handle( new color_selector( &settings.glow_enemy, &settings.glow_hue, true ) );
				group7->handle( new check_box( "Chams", &settings.enemy_chams ) );
				//if ( !settings.enemy_chams_whl_hid )
				group7->handle( new color_selector( &settings.chams_enemy_vis, &settings.chams_huee, true ) );
				if ( settings.enemy_chams ) {
					group7->handle( new check_box( "Illuminate", &settings.iluminate_chams ) );
					group7->handle( new color_selector( &settings.iluminate_Enemy_col, &settings.ilchams_hue, true ) );

					group7->handle( new combo( "Material", &settings.materials_enemy, test_combo9 ) );
					group7->handle( new check_box( "While hidden", &settings.enemy_chams_whl_hid ) );
					group7->handle( new color_selector( &settings.chams_enemy_invis, &settings.ilchams_hue, true ) );
				}
				group7->handle( new check_box( "Disable model occulusion", &settings.disable_occulusion ) );
				group7->handle( new check_box( "Dormant", &settings.dormant ) );
				group7->handle( new check_box( "Skeleton matrix [!]", &settings.skel3 ) );
				group7->handle( new check_box( "Hitmatrix", &settings.hit_hitmatrix ) );
				group7->handle( new color_selector( &settings.hitmatrix_color, &settings.hitmatrix_hue, true ) );

				group7->handle_scroll( );
			} delete group7;

			auto group8 = new group_box( "MISC", 8, 15, 80 + 10 + 248, 380, 248, 2 ); {
				g_window->add_group( group8 );
				group8->handle( new check_box( "Penetration reticle", &settings.pen_crosshair ) );
				group8->handle( new check_box( "Bullet tracers", &settings.impact_beams ) );
				group8->handle( new color_selector( &settings.impact_beams_color, &settings.lbulletc_hue, true ) );
				group8->handle( new check_box( "Hitmarkers", &settings.hitmarker ) );
				if ( settings.hitmarker ) {
					group8->handle( new combo( "Hitsound", &settings.hitsoundtype, t01 ) );
					if ( settings.hitsoundtype == 1 ) {
						const auto GetScripts = [ ]( ) {
							std::string dir = "C:\\nemesis.technology\\";
							for ( auto& file_path : std::filesystem::directory_iterator( dir ) ) {
								if ( !file_path.path( ).string( ).empty( ) ) {
									if ( file_path.path( ).string( ).find( XorStr( ".wav" ) ) != std::string::npos ) {
										cfg::g_cfg->m_vec_hitsounds.emplace_back( file_path.path( ).string( ).erase( 0, dir.length( ) ) );
									}
								}
							}
						};

						if ( cfg::g_cfg->m_vec_hitsounds.empty( ) ) {
							GetScripts( );
						}

						if ( !cfg::g_cfg->m_vec_hitsounds.empty( ) ) {
							group8->handle( new combo( "Custom hitsounds", &settings.customhitscoun, cfg::g_cfg->m_vec_hitsounds ) );
						}
					}

				}

				group8->handle( new check_box( "World hitmarkers", &settings.hitmarker_3d ) );
				group8->handle( new check_box( "Extended killfeed", &settings.preserve_kilfed ) );
				group8->handle( new check_box( "Planted c4", &settings.planted_c4 ) );
				group8->handle( new check_box( "Debug panel", &settings.debug_panel ) );
				//group8->handle( new check_box( "Christmas [fps drops]", &settings.christmans ) );
				group8->handle( new check_box( "Rodgolls", &settings.rod_gollss ) );
				group8->handle( new check_box( "Spread crosshair", &settings.spread_crfosh ) );
	
				group8->handle( new check_box( "Aspect ratio", &settings.aspect_ratio ) );
				if ( settings.aspect_ratio ) {
					group8->handle( new slider( "Screen ratio", &settings.aspect_r, 0, 150, "%" ) ); // this is not linked to anything
				}
				group8->handle_scroll( );
			} delete group8;


			auto group9 = new group_box( "LOCAL", 9, 15 + 380 + 10, 80, 380, 248, 2 ); {
				g_window->add_group( group9 );
				static bool show_esp_local = false;;
				//group9->handle( new check_box( "Show local esp", &show_esp_local ) );
				//if ( show_esp_local ) {
				//	group9->handle( new check_box( "Enabled", &settings.local_esp ) );
				//	group9->handle( new check_box( "Box", &settings.boudingbox_local ) );
				//	group9->handle( new check_box( "Name", &settings.namelocal ) );
				//}

				group9->handle( new check_box( "Local stickman", &settings.local_stickman ) );
				group9->handle( new check_box( "Funny animfix", &settings.midget ) );
				if ( settings.midget ) {
					group9->handle( new combo( "Funky mode", &settings.break_mode, t17 ) );
					if ( settings.break_mode == 1 ) {
						group9->handle( new check_box( "Funny switcher", &settings.funky_shwiter ) );

						auto break_pn = new multi_combo( "Funky fix" );
						break_pn->add_item( "Head", &settings.head_funky );
						break_pn->add_item( "Chest", &settings.chest_funky );
						break_pn->add_item( "Arms", &settings.arms_funky );
						break_pn->add_item( "Legs", &settings.legs_funky );
						group9->handle( break_pn );

						group9->handle( new slider( "Funky distance", &settings.funky_time, 0, 100, "°" ) ); // if u put - 100 and have funky_shwiter on it will crash
					}
				}

				static bool thirdperson = false;
				group9->handle( new check_box( "Thirdperson", &thirdperson, true ) );
				group9->handle( new key_bind( &settings.tp_key, &settings.tkey_style ) );

				if ( settings.tp_key > 0 ) {
					group9->handle( new check_box( "Override cam_idealdist", &settings.override_cam_idealdist ) );
					if ( settings.override_cam_idealdist ) {
						group9->handle( new slider( "Thirdperson distance", &settings.thirdperson_distance, 0, 130, "°" ) );
					}
				}

				group9->handle( new check_box( "Glow", &settings.local_glow ) );
				group9->handle( new color_selector( &settings.localg_color, &settings.lglow_hue, true ) );

				group9->handle( new check_box( "Chams", &settings.local_chams ) );
				group9->handle( new color_selector( &settings.localc_color, &settings.local_col_hue, true ) );
				if ( settings.local_chams ) {
					group9->handle( new combo( "Material", &settings.local_materials, test_combo9 ) );

					group9->handle( new check_box( "Illuminate", &settings.local_illuminate ) );
					group9->handle( new color_selector( &settings.local_illuminate_co, &settings.local_ill_hue, true ) );
				}

				group9->handle( new check_box( "Weapon hams", &settings.weapon_chamss ) );
				group9->handle( new color_selector( &settings.weaponc_color, &settings.weapon_col_hue, true ) );
				if ( settings.weapon_chamss ) {
					group9->handle( new combo( "Weapon material", &settings.weapon_materials, test_combo9 ) );
				
					group9->handle( new check_box( "Illuminate weapons", &settings.weapon_illuminate ) );
					group9->handle( new color_selector( &settings.weapon_illuminate_co, &settings.weapon_ill_hue, true ) );
				}


				group9->handle( new slider( "Field of view", &settings.fov_amount, 0, 130, "°" ) );
				group9->handle( new slider( "Model field of view", &settings.v_fov_am, 0, 130, "°" ) );
				group9->handle( new slider( "Field of view x", &settings.viewmodel_offset_x, -100, 100, "°" ) );
				group9->handle( new slider( "Field of view y", &settings.viewmodel_offset_y, -100, 100, "°" ) );
				group9->handle( new slider( "Field of view z", &settings.viewmodel_offset_z, -100, 100, "°" ) );

				auto anim_tgewak = new multi_combo( "Local tweaks" );
				anim_tgewak->add_item( "Disable static-legs", &settings.animation_tweaks[ 0 ] );
				anim_tgewak->add_item( "Interpolate animations", &settings.animation_tweaks[ 1 ] );
				anim_tgewak->add_item( "Disable legs shadows", &settings.disable_foot_shadows );
				group9->handle( anim_tgewak );

				//group9->handle( new check_box( "Disable legs shadows", &settings.disable_foot_shadows ) );

				auto ind = new multi_combo( "Other visuals" );
				ind->add_item( "Indicators", &settings.indicators[ 0 ] );
				ind->add_item( "Keybinds", &settings.indicators[ 1 ] );
				//ind->add_item( "Watermark", &settings.indicators[ 1 ] );
				group9->handle( ind );

				group9->handle_scroll( );
			} delete group9;

			auto group10 = new group_box( "WORLD", 10, 15 + 380 + 10, 80 + 10 + 248, 380, 248, 2 ); {
				g_window->add_group( group10 );
				auto flgs = new multi_combo( "Removals" );
				flgs->add_item( "Scope", &settings.noscope );
				flgs->add_item( "Smoke", &settings.nosmoke );
				flgs->add_item( "Flash", &settings.noflashs );
				flgs->add_item( "Teammates", &settings.remove_team );
				flgs->add_item( "Zoom", &settings.fov_while_scoped );
				group10->handle( flgs );

				group10->handle( new combo( "Skybox", &settings.new_sky, t5 ) );

				auto wrld_mod = new multi_combo( "World modulation" );
				wrld_mod->add_item( "Sunset", &settings.sunsettt );
				wrld_mod->add_item( "Fullbright", &settings.fullbrigan );
				wrld_mod->add_item( "Modulate sky", &settings.modulate_sky );
				group10->handle( wrld_mod );

				if ( settings.modulate_sky ) {
					group10->handle( new check_box( "Modulate sky", &settings.modulate_sky, true ) );
					group10->handle( new color_selector( &settings.modulatesky_color, &settings.hue2, true ) );
				}

				group10->handle( new slider( "Prop alpha", &settings.prop_alpha, 0, 100, "%" ) );
				group10->handle( new combo( "World modulation type", &settings.world_modulation_type, t14 ) );
				switch ( settings.world_modulation_type ) {
					case 0: {
						group10->handle( new slider( "World dim", &settings.darkness, 0, 100, "%" ) );
					} break;
					case 1: {
						group10->handle( new check_box( "Modulate world", &settings.modulate_world ) );
						group10->handle( new color_selector( &settings.mod_world, &settings.modworld_hue, true ) );

					} break;
				}

				static bool f_update = false;
				group10->handle( new check_box( "Force update", &f_update ) );
				if ( f_update ) {
					Visuals::ModulateWorld( );
					f_update = false;
				}

				group10->handle( new check_box( "Modulate fire", &settings.ModulateFire ) );
				if ( settings.ModulateFire )
					group10->handle( new slider( "Fire hue", &settings.FireHue, 0, 7, "%" ) );

				group10->handle( new check_box( "Modulate light", &settings.rAmbient ) );
				if ( settings.rAmbient ) {
					group10->handle( new slider( "Model ambient", &settings.AmbientAmount, 0, 10, "%" ) );
				}


				group10->handle( new check_box( "Projectile esp", &settings.proj ) );
				group10->handle( new color_selector( &settings.proj_color, &settings.hue3, true ) );

				group10->handle( new check_box( "Grenade glow", &settings.grenade_glow_b ) );
				group10->handle( new color_selector( &settings.grenade_glow, &settings.grenade_glow_hue, true ) );

				group10->handle( new check_box( "Grenade simulation", &settings.trajectory ) );
				group10->handle( new color_selector( &settings.trajcolor, &settings.trajcolor_hue, true ) );

				group10->handle( new check_box( "Weapon esp", &settings.items ) );
				group10->handle( new color_selector( &settings.item_color, &settings.hue1, true ) );
				//group10->handle( new check_box( "Grenade esp", &settings.items ) );
				group10->handle_scroll( );
			} delete group10;

			auto group711 = new group_box( "SKINS", 14, 15, 80, 380, 248 + 248 + +10, 3 ); {
				g_window->add_group( group711 );
				group711->handle( new check_box( "Enable", &settings.skinchanger ) );
				group711->handle( new check_box( "Switch knife hand", &settings.switch_knfie_hand ) );
				group711->handle( new combo( "Knife changer", &settings.knife_changer, t12 ) );
				group711->handle( new combo( "Glove changer", &settings.glove_changer, t13 ) );

				static bool once = false;
				if ( !once ) {
					for ( int i = S_Charred; i <= S_Mogul; i++ ) {
						switch ( static_cast< GLOVE_SKIN >( i ) ) {
							case S_Amphibious:    cfg::g_cfg->glove_paints.push_back( "Amphibious" );     break;
							case S_Arboreal:      cfg::g_cfg->glove_paints.push_back( "Arboreal" );       break;
							case S_Arid:          cfg::g_cfg->glove_paints.push_back( "Arid" );           break;
							case S_Badlands:      cfg::g_cfg->glove_paints.push_back( "Badlands" );       break;
							case S_Boom:          cfg::g_cfg->glove_paints.push_back( "Boom!" );          break;
							case S_BronzeMorph:   cfg::g_cfg->glove_paints.push_back( "Bronze Morph" );   break;
							case S_Bronzed:       cfg::g_cfg->glove_paints.push_back( "Bronzed" );        break;
							case S_Buckshot:      cfg::g_cfg->glove_paints.push_back( "Buckshot" );       break;
							case S_CaseHardened:  cfg::g_cfg->glove_paints.push_back( "Case Hardened" );  break;
							case S_Charred:       cfg::g_cfg->glove_paints.push_back( "Charred" );        break;
							case S_CobaltSkulls:  cfg::g_cfg->glove_paints.push_back( "Cobalt Skulls" );  break;
							case S_Convoy:        cfg::g_cfg->glove_paints.push_back( "Convoy" );         break;
							case S_CoolMint:      cfg::g_cfg->glove_paints.push_back( "Cool Mint" );      break;
							case S_CrimsonKimono: cfg::g_cfg->glove_paints.push_back( "Crimson Kimono" ); break;
							case S_CrimsonWeave:  cfg::g_cfg->glove_paints.push_back( "Crimson Weave" );  break;
							case S_CrimsonWeb:    cfg::g_cfg->glove_paints.push_back( "Crimson Web" );    break;
							case S_Diamondback:   cfg::g_cfg->glove_paints.push_back( "Diamondback" );    break;
							case S_DuctTape:      cfg::g_cfg->glove_paints.push_back( "Duct Tape" );      break;
							case S_Eclipse:       cfg::g_cfg->glove_paints.push_back( "Eclipse" );        break;
							case S_EmeraldWeb:    cfg::g_cfg->glove_paints.push_back( "Emerald Web" );    break;
							case S_Emerald:       cfg::g_cfg->glove_paints.push_back( "Emerald" );        break;
							case S_Fade:          cfg::g_cfg->glove_paints.push_back( "Fade" );           break;
							case S_ForestDDPAT:   cfg::g_cfg->glove_paints.push_back( "Forest DDPAT" );   break;
							case S_Foundation:    cfg::g_cfg->glove_paints.push_back( "Foundation" );     break;
							case S_Guerrilla:     cfg::g_cfg->glove_paints.push_back( "Guerrilla" );      break;
							case S_HedgeMaze:     cfg::g_cfg->glove_paints.push_back( "Hedge Maze" );     break;
							case S_Mangrove:      cfg::g_cfg->glove_paints.push_back( "Mangrove" );       break;
							case S_PandorasBox:   cfg::g_cfg->glove_paints.push_back( "Pandora's Box" );  break;
							case S_Rattler:       cfg::g_cfg->glove_paints.push_back( "Rattler" );        break;
							case S_Slaughter:     cfg::g_cfg->glove_paints.push_back( "Slaughter" );      break;
							case S_Snakebite:     cfg::g_cfg->glove_paints.push_back( "Snakebite" );      break;
							case S_SpruceDDPAT:   cfg::g_cfg->glove_paints.push_back( "Spruce DDPAT" );   break;
							case S_Superconductor: cfg::g_cfg->glove_paints.push_back( "Superconductor" ); break;
							case S_Transport:     cfg::g_cfg->glove_paints.push_back( "Transport" );      break;
							case S_Turtle:        cfg::g_cfg->glove_paints.push_back( "Turtle" );         break;
							case S_Vice:          cfg::g_cfg->glove_paints.push_back( "Vice" );           break;
						}
					}
					once = true;
				}

				static int main_case = 0;
				group711->handle( new list_box( "Glove list", &settings.skin_glove_selected, 173, 8, cfg::g_cfg->glove_paints ) );
				auto selected_paint = cfg::g_cfg->glove_paints[ settings.skin_glove_selected ];
				int selected_skin;
				
				auto it = gloveSkinMap.find( cfg::g_cfg->glove_paints[ settings.skin_glove_selected ] );
				if ( it != gloveSkinMap.end( ) ) {
					settings.glove_skin = it->second;

					static bool once2 = false;
					if ( !once2 ) {
						g_notify.add( std::to_string( it->second ) );
						once2 = true;
					}

					if ( GetAsyncKeyState( VK_HOME ) )
						once2 = false;
				}
				else {
					// handle unexpected input
				}


				group711->handle( new button( "Force update", [ ]( ) { force_update( ); } ) );
				group711->handle_scroll( );
			} delete group711;
			
			auto group721 = new group_box( "DETAIL", 15, 15 + 380 + 10, 80, 380, 248 + 248 + +10, 3 ); {
				g_window->add_group( group721 );

				static bool once = false;
				if ( !once ) {
					// handle once
					for ( int i = S_WHITEOUT; i <= S_GAMMA_DOPPLER_PHASE_4; i++ ) {
						switch ( static_cast< GLOBAL_SKIN >( i ) ) {
						case S_DOPPLER_RED:
							cfg::g_cfg->global_paints.push_back( "Doppler Red" );
							break;
						case S_DOPPLER_BLUE:
							cfg::g_cfg->global_paints.push_back( "Doppler Blue" );
							break;
						case S_DOPPLER_BLACK_PEARL:
							cfg::g_cfg->global_paints.push_back( "Doppler Black Pearl" );
							break;
						case S_DOPPLER_PHASE_1:
							cfg::g_cfg->global_paints.push_back( "Doppler Phase 1" );
							break;
						case S_DOPPLER_PHASE_2:
							cfg::g_cfg->global_paints.push_back( "Doppler Phase 2" );
							break;
						case S_DOPPLER_PHASE_3:
							cfg::g_cfg->global_paints.push_back( "Doppler Phase 3" );
							break;
						case S_DOPPLER_PHASE_4:
							cfg::g_cfg->global_paints.push_back( "Doppler Phase 4" );
							break;
						case S_GAMMA_DOPPLER_EMERALD:
							cfg::g_cfg->global_paints.push_back( "Gamma Doppler Emerald" );
							break;
						case S_GAMMA_DOPPLER_PHASE_1:
							cfg::g_cfg->global_paints.push_back( "Gamma Doppler Phase 1" );
							break;
						case S_GAMMA_DOPPLER_PHASE_2:
							cfg::g_cfg->global_paints.push_back( "Gamma Doppler Phase 2" );
							break;
						case S_GAMMA_DOPPLER_PHASE_3:
							cfg::g_cfg->global_paints.push_back( "Gamma Doppler Phase 3" );
							break;
						case S_GAMMA_DOPPLER_PHASE_4:
							cfg::g_cfg->global_paints.push_back( "Gamma Doppler Phase 4" );
							break;
	
						case S_WHITEOUT:
							cfg::g_cfg->global_paints.push_back( "Whiteout" );
							break;
						default:
							// handle any unexpected cases here
							break;
						}
					}

					once = true;
				}

				if ( !g_skins.m_paint_kits.empty( ) ) {
					group721->handle( new list_box( "Paint list", &settings.paint_selected, 407, 18, g_skins.m_paint_kits, skin_filter ) );
				}
				group721->handle( new text_input( &skin_filter, "Skin name", "Search the name of an item." ) );
				
				
				auto it = g_skins.skins_map.find( g_skins.m_paint_kits[ settings.paint_selected ] );
				if ( it != g_skins.skins_map.end( ) ) {
					if ( g_cl.m_weapon_id == Weapons_t::DEAGLE ) {
						settings.skin_deagle = it->second;
					}
					else if ( g_cl.m_weapon_id == Weapons_t::GLOCK ) {
						settings.skin_glock = it->second;
					}
					else if ( g_cl.m_weapon_id == Weapons_t::ELITE ) {
						settings.skin_elite = it->second;
					}
					else if ( g_cl.m_weapon_id == Weapons_t::AWP ) {
						settings.skin_awp = it->second;
					}
					else if ( g_cl.m_weapon_id == Weapons_t::SSG08 ) {
						settings.skin_ssg = it->second;
					}
					else if ( g_cl.m_weapon_id == Weapons_t::SCAR20 ) {
						settings.skin_auto_ct = it->second;
					}
					else if ( g_cl.m_weapon_id == Weapons_t::G3SG1 ) {
						settings.skin_auto_t = it->second;
					}
					else if ( g_cl.m_weapon_id == Weapons_t::USPS ) {
						settings.skin_usp = it->second;
					}
					else if ( g_cl.m_weapon_id == Weapons_t::REVOLVER ) {
						settings.skin_revolver = it->second;
					}
					else if ( g_cl.m_weapon_id == Weapons_t::TEC9 ) {
						settings.skin_tec = it->second;
					}
					else if ( g_cl.m_weapon_id == Weapons_t::KNIFE_BAYONET ||
						g_cl.m_weapon_id == Weapons_t::KNIFE_FLIP ||
						g_cl.m_weapon_id == Weapons_t::KNIFE_GUT ||
						g_cl.m_weapon_id == Weapons_t::KNIFE_KARAMBIT ||
						g_cl.m_weapon_id == Weapons_t::KNIFE_M9_BAYONET ||
						g_cl.m_weapon_id == Weapons_t::KNIFE_HUNTSMAN ||
						g_cl.m_weapon_id == Weapons_t::KNIFE_FALCHION ||
						g_cl.m_weapon_id == Weapons_t::KNIFE_BOWIE ||
						g_cl.m_weapon_id == Weapons_t::KNIFE_BUTTERFLY ||
						g_cl.m_weapon_id == Weapons_t::KNIFE_SHADOW_DAGGERS ||
						g_cl.m_weapon_id == Weapons_t::GLOVE_T_SIDE ||
						g_cl.m_weapon_id == Weapons_t::GLOVE_CT_SIDE ) {
						settings.skin_knif = it->second;
					}

					static bool once2 = false;
					if ( !once2 ) {
						g_notify.add( std::to_string( it->second ) );
						once2 = true;
					}

					if ( GetAsyncKeyState( VK_HOME ) )
						once2 = false;
				}
				else {
					// handle unexpected
				}
				group721->handle_scroll( );
			} delete group721;

			auto group71 = new group_box( "RESTRICTIONS", 11, 15, 80, 380, 248, 4 ); {
				g_window->add_group( group71 );
				group71->handle( new check_box( "Filter console", &settings.filter_it ) );
				group71->handle( new check_box( "Modify menu theme", &settings.modify_menu_color ) );
				group71->handle( new color_selector( &settings.accent_color, &settings.m_color_hue, true ) );
				group71->handle( new check_box( "Drops", &settings.rain_drop_menu ) );
				group71->handle( new color_selector( &settings.snoww, &settings.snoww_hue, true ) );
				if ( settings.rain_drop_menu ) {
					group71->handle( new combo( "Drops shape", &settings.draw_rain_or_snow, t8 ) );
					group71->handle( new slider( "Drops amount", &settings.max_drops, 10, 300, "%" ) ); // this is not linked to anything
				}

				group71->handle( new combo( "Menu pattern", &settings.menu_backround, t11 ) );

				/*
				group71->handle( new check_box( "Customize csgo sounds", &settings.customize_sounds ) ); // testt this
				if ( settings.customize_sounds ) {

					group71->handle( new slider( "Weapon volume", &settings.weapon_volume, 0, 100, "%" ) ); // this is not linked to anything
					group71->handle( new slider( "Steps volume", &settings.footstep_volume, 0, 100, "%" ) ); // this is not linked to anything
					group71->handle( new slider( "Other volume", &settings.other_volume, 0, 100, "%" ) ); // this is not linked to anything

				}
				*/
				group71->handle_scroll( );
			} delete group71;

			auto group81 = new group_box( "MOVEMENT", 12, 15, 80 + 10 + 248, 380, 248, 4 ); {
				g_window->add_group( group81 );
				group81->handle( new check_box( "Automatic hop", &settings.dick_hop ) );
				group81->handle( new check_box( "Airstrafe", &settings.dick_strafe ) );
				if ( settings.dick_strafe ) {
					group81->handle( new combo( "Autostrafe type", &settings.movement_mode, t2 ) );
					if ( settings.movement_mode == 2 ) {
						group81->handle( new slider( "Strafe smooth", &settings.auto_strafe_smooth, 0, 100, "%" ) ); // this is not linked to anything
					}
				}

				group81->handle( new check_box( "Clantag", &settings.clantag ) );

				bool ciocan = false;
				group81->handle( new check_box( "Auto-peek", &ciocan ) );
				group81->handle( new key_bind( &settings.apeek_key, &settings.apeek_mo_ke ) );
				group81->handle( new check_box( "Auto-peek visualize", &settings.apeek_visualize ) );
				group81->handle( new color_selector( &settings.apeek, &settings.apeek_hue, true ) );
				group81->handle( new combo( "Visualize shape", &settings.visualsi_tyl, t7 ) );

				group81->handle( new check_box( "Accurate walk", &settings.accurate_walk ) );
				if ( settings.accurate_walk ) {
					group81->handle( new slider( "Speed", &settings.accurate_speed, 0.0f, 16.0f, "%" ) );
				}
				group81->handle( new check_box( "Slidewalk", &settings.slidewalk ) );
				if ( settings.slidewalk ) {
					group81->handle( new combo( "Slidewalk type", &settings.slide_mode, t3 ) );
				}

				group81->handle( new check_box( "Always on fake-ping", &settings.fakelon ) );
				group81->handle( new key_bind( &settings.fakel_key, &settings.fakel_keys ) );
				if ( !settings.fakelon ) {
					group81->handle( new slider( "Fake ping", &settings.how_much_fakel, 0, 500, "%" ) );
				}

				auto evt = new multi_combo( "Events" );
				evt->add_item( "Damage given", &settings.events[ 0 ] );
				evt->add_item( "Purchace", &settings.events[ 1 ] );
				evt->add_item( "Bomb", &settings.events[ 2 ] );
				group81->handle( evt );

				group81->handle( new check_box( "Buy bot", &settings.enableBuyBot ) );
				if ( settings.enableBuyBot ) {
					group81->handle( new combo( "Auto buy primary", &settings.buyPrimary, autoBuyPrimary ) );
					group81->handle( new combo( "Auto buy secondary", &settings.buySecondary, autoBuySecondary ) );

					auto autoBuyExtra = new multi_combo( "Auto buy extra" );
					autoBuyExtra->add_item( "Armor", &settings.buyExtra[ 0 ] );
					autoBuyExtra->add_item( "Zeus", &settings.buyExtra[ 1 ] );
					autoBuyExtra->add_item( "Defuser", &settings.buyExtra[ 2 ] );
					autoBuyExtra->add_item( "Grenade", &settings.buyExtra[ 3 ] );
					autoBuyExtra->add_item( "Molotov", &settings.buyExtra[ 4 ] );
					autoBuyExtra->add_item( "Smoke", &settings.buyExtra[ 5 ] );
					group81->handle( autoBuyExtra );
				}
				group81->handle_scroll( );
			} delete group81;


			static bool lua_page = false;
			static bool config_page = false;

			auto group91 = new group_box( config_page ? "CONFIG" : "LUA", 13, 15 + 380 + 10, 80, 380, 340 + 10 + 155, 4 ); {
				g_window->add_group( group91 );

				group91->handle( new check_box( config_page ? "Switch to lua" : "Switch to config", &lua_page ) );
				if ( lua_page ) {
					config_page = false;
				}
				else if ( !lua_page ) {
					config_page = true;
				}

				if ( config_page ) {
					group91->handle( new list_box( "Config list", &slot_cfg, 170, 8, cfg::g_cfg->files_cfg.size( ) <= 0 ? hah : cfg::g_cfg->files_cfg ) );
					
					// run once the update
					static bool once = false;
					if ( !once ) {
						settings.GetConfigs( );
						once = true;
					}

					group91->handle( new button( "Refresh", [ ]( ) { settings.GetConfigs( ); } ) );

					group91->handle( new text_input( &config_name, "New config name", "Enter the name of a new config." ) );
					group91->handle( new button( "Create", [ ]( ) { create( ); } ) );
					group91->handle( new button( "Save", [ ]( ) { save( ); } ) );
					group91->handle( new button( "Save Game", [ ]( ) { save_game( ); } ) );
					group91->handle( new button( "Load", [ ]( ) { load( ); } ) );

					lua_page = false;
				}
				else if ( lua_page  ) {
					config_page = false;
					
					// run once the update
					static bool once = false;
					if ( !once ) {
						lua_deathmesis::g_lua->get_lua( );
						once = true;
					}

					group91->handle( new list_box( "Lua list", &lua_cfg, 200, 8, lua_deathmesis::g_lua->files.size( ) <= 0 ? hah : lua_deathmesis::g_lua->files ) );
					group91->handle( new button( "Refresh", [ ]( ) { lua_deathmesis::g_lua->get_lua( ); } ) );
					group91->handle( new button( "Load", [ ]( ) { load_lua_gui(); } ) );
				}


				group91->handle_scroll( );
			} delete group91;

		} delete g_window;

		// install externals
		m_ext_render.install( );
		
		//indicators::indicator( );
		//indicators::watermark( );
	}
}