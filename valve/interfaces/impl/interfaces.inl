#pragma once

#include "../interfaces.hpp"

namespace supremacy::valve {
	__forceinline client_class_t* c_client::all_classes( ) {
		using fn_t = client_class_t*( __thiscall* )( decltype( this ) );

		return ( *reinterpret_cast< fn_t** >( this ) )[ 8u ]( this );
	}

	__forceinline bool c_engine::in_game( ) {
		using fn_t = bool( __thiscall* )( decltype( this ) );

		return ( *reinterpret_cast< fn_t** >( this ) )[ 26u ]( this );
	}

	__forceinline int c_engine::index_for_uid( const int uid ) {
		using fn_t = int( __thiscall* )( decltype( this ), const int );

		return ( *reinterpret_cast< fn_t** >( this ) )[ 9u ]( this, uid );
	}

	__forceinline float c_engine::last_time_stamp() {
		using fn_t = float(__thiscall*)(decltype(this));

		return (*reinterpret_cast<fn_t**>(this))[14u](this);
	}

	__forceinline std::optional< player_info_t > c_engine::player_info( const int index ) {
		using fn_t = bool( __thiscall* )( decltype( this ), const int, player_info_t* const );

		player_info_t ret{};
		if ( !( *reinterpret_cast< fn_t** >( this ) )[ 8u ]( this, index, &ret ) )
			return std::nullopt;

		return ret;
	}

	__forceinline net_channel_info_t* c_engine::net_channel_info( ) {
		using fn_t = net_channel_info_t*( __thiscall* )( decltype( this ) );

		return ( *reinterpret_cast< fn_t** >( this ) )[ 78u ]( this );
	}

	__forceinline qangle_t c_engine::view_angles( ) {
		using fn_t = void( __thiscall* )( decltype( this ), qangle_t& );

		qangle_t ret{};

		( *reinterpret_cast< fn_t** >( this ) )[ 18u ]( this, ret );

		return ret;
	}

	__forceinline void c_engine::set_view_angles( const qangle_t& angles ) {
		using fn_t = void( __thiscall* )( decltype( this ), const qangle_t& );

		return ( *reinterpret_cast< fn_t** >( this ) )[ 19u ]( this, angles );
	}

	__forceinline void c_engine::fire_events( ) {
		using fn_t = void( __thiscall* )( decltype( this ) );

		return ( *reinterpret_cast< fn_t** >( this ) )[ 59u ]( this );
	}

	__forceinline const mat4x4_t& c_engine::w2s_matrix( ) {
		using fn_t = const mat4x4_t&( __thiscall* )( decltype( this ) );

		return ( *reinterpret_cast< fn_t** >( this ) )[ 37u ]( this );
	}

	__forceinline void c_engine::exec_cmd( const char* cmd ) {
		using fn_t = void( __thiscall* )( decltype( this ), const char* );

		return ( *reinterpret_cast< fn_t** >( this ) )[ 108u ]( this, cmd );
	}

	__forceinline void c_engine::unrestricted_cmd(const char* cmd, const bool delayed = false) {
		using fn_t = void(__thiscall*)(decltype(this), const char*, bool);

		return (*reinterpret_cast<fn_t**>(this))[114u](this, cmd, delayed);
	}

	__forceinline int c_engine::local_player_index( ) {
		using fn_t = int( __thiscall* )( decltype( this ) );

		return ( *reinterpret_cast< fn_t** >( this ) )[ 12u ]( this );
	}

	__forceinline std::uintptr_t* c_engine::bsp_tree_query( ) {
		using fn_t = std::uintptr_t*( __thiscall* )( decltype( this ) );

		return ( *reinterpret_cast< fn_t** >( this ) )[ 43u ]( this );
	}

	__forceinline c_entity* c_entity_list::find_entity( const int index ) {
		using fn_t = c_entity*( __thiscall* )( decltype( this ), const int );

		return ( *reinterpret_cast< fn_t** >( this ) )[ 3u ]( this, index );
	}

	__forceinline c_entity* c_entity_list::find_entity( const e_ent_handle handle ) {
		using fn_t = c_entity*( __thiscall* )( decltype( this ), const e_ent_handle );

		return ( *reinterpret_cast< fn_t** >( this ) )[ 4u ]( this, handle );
	}

	__forceinline int c_entity_list::highest_index( ) {
		using fn_t = int( __thiscall* )( decltype( this ) );

		return ( *reinterpret_cast< fn_t** >( this ) )[ 6u ]( this );
	}

	__forceinline void c_move_helper::set_host( c_player* const player ) {
		using fn_t = void( __thiscall* )( decltype( this ), c_player* const );

		return ( *reinterpret_cast< fn_t** >( this ) )[ 1u ]( this, player );
	}

	__forceinline void prediction_t::update(
		const int start_frame, const bool valid_frame,
		const int in_acked, int out_cmd
	) {
		using fn_t = void( __thiscall* )(
			decltype( this ), const int,
			const bool, const int, const int
		);

		return ( *reinterpret_cast< fn_t** >( this ) )[ 3u ](
			this, start_frame, valid_frame, in_acked, out_cmd
		);
	}

	__forceinline void prediction_t::setup_move(
		c_player* const player, user_cmd_t* const user_cmd,
		c_move_helper* const move_helper, move_data_t* const move_data
	) {
		using fn_t = void( __thiscall* )(
			decltype( this ), c_player* const, user_cmd_t* const,
			c_move_helper* const, move_data_t* const
		);

		return ( *reinterpret_cast< fn_t** >( this ) )[ 20u ](
			this, player, user_cmd, move_helper, move_data
		);
	}

	__forceinline void prediction_t::finish_move(
		c_player* const player,
		user_cmd_t* const user_cmd, move_data_t* const move_data
	) {
		using fn_t = void( __thiscall* )(
			decltype( this ), c_player* const,
			user_cmd_t* const, move_data_t* const
		);

		return ( *reinterpret_cast< fn_t** >( this ) )[ 21u ](
			this, player, user_cmd, move_data
		);
	}

	__forceinline void c_movement::process_movement(
		c_player* const player, move_data_t* const move_data
	) {
		using fn_t = void( __thiscall* )(
			decltype( this ), c_player* const, move_data_t* const
		);

		return ( *reinterpret_cast< fn_t** >( this ) )[ 1u ](
			this, player, move_data
		);
	}

	__forceinline void c_movement::start_track_prediction_errors( c_player* const player ) {
		using fn_t = void( __thiscall* )( decltype( this ), c_player* const );

		return ( *reinterpret_cast< fn_t** >( this ) )[ 3u ]( this, player );
	}

	__forceinline void c_movement::finish_track_prediction_errors( c_player* const player ) {
		using fn_t = void( __thiscall* )( decltype( this ), c_player* const );

		return ( *reinterpret_cast< fn_t** >( this ) )[ 4u ]( this, player );
	}

	__forceinline cvar_t* c_cvar::find_var( const char* name ) {
		using fn_t = cvar_t*( __thiscall* )( decltype( this ), const char* );

		return ( *reinterpret_cast< fn_t** >( this ) )[ 16u ]( this, name );
	}

	__forceinline void c_cvar::con_print( const std::uint32_t& clr, const char* str ) {
		using fn_t = void( __cdecl* )( decltype( this ), const std::uint32_t&, const char*, ... );

		return ( *reinterpret_cast< fn_t** >( this ) )[ 25u ]( this, clr, str );
	}

	__forceinline void c_cvar::con_print_format(const std::uint32_t& clr, const char* format, ...) {
		auto temp = std::vector<char>{};
		auto length = std::size_t{ 63 };
		va_list args;
		while (temp.size() <= length) {
			temp.resize(length + 1);
			va_start(args, format);
			const auto status = std::vsnprintf(temp.data(), temp.size(), format, args);
			va_end(args);

			length = static_cast<std::size_t>(status);
		}
		std::string out{ temp.data(), length };

		using fn_t = void(__cdecl*)(decltype(this), const std::uint32_t&, const char*, ...);

		return (*reinterpret_cast<fn_t**>(this))[25u](this, clr, out.c_str());
	}

	__forceinline e_mask c_engine_trace::point_contents(
		const vec3_t& point, const e_mask mask, c_entity** const entity
	) {
		using fn_t = e_mask( __thiscall* )(
			decltype( this ), const vec3_t&, const e_mask, c_entity** const
		);

		return ( *reinterpret_cast< fn_t** >( this ) )[ 0u ]( this, point, mask, entity );
	}

	__forceinline void c_engine_trace::clip_ray_to_entity(
		const ray_t& ray, const e_mask mask,
		c_entity* const entity, trace_t* const trace
	) {
		using fn_t = void( __thiscall* )(
			decltype( this ), const ray_t&, const e_mask,
			c_entity* const , trace_t* const
		);

		return ( *reinterpret_cast< fn_t** >( this ) )[ 3u ](
			this, ray, mask, entity, trace
		);
	}

	__forceinline void c_engine_trace::trace_ray(
		const ray_t& ray, const e_mask mask,
		trace_filter_t* const filter, trace_t* const trace
	) {
		using fn_t = void( __thiscall* )(
			decltype( this ), const ray_t&, const e_mask,
			trace_filter_t* const, trace_t* const
		);

		return ( *reinterpret_cast< fn_t** >( this ) )[ 5u ](
			this, ray, mask, filter, trace
		);
	}

	__forceinline surface_data_t* c_surface_data::find( const int index ) {
		using fn_t = surface_data_t*( __thiscall* )( decltype( this ), const int );

		return ( *reinterpret_cast< fn_t** >( this ) )[ 5u ]( this, index );
	}

	__forceinline user_cmd_t* input_t::user_cmd( const int slot, const int seq_number ) {
		using fn_t = user_cmd_t*( __thiscall* )( decltype( this ), const int, const int );

		return ( *reinterpret_cast< fn_t** >( this ) )[ 8u ]( this, slot, seq_number );
	}

	__forceinline void c_game_event_mgr::add_listener(
		base_event_listener_t* listener, const char* event_name, const bool server_side
	) {
		using fn_t = void( __thiscall* )( decltype( this ), base_event_listener_t*, const char*, const bool );

		return ( *reinterpret_cast< fn_t** >( this ) )[ 3u ]( this, listener, event_name, server_side );
	}

	__forceinline void studio_render_t::set_alpha( const float value ) {
		using fn_t = void( __thiscall* )( decltype( this ), const float );

		return ( *reinterpret_cast< fn_t** >( this ) )[ 28u ]( this, value );
	}

	__forceinline void studio_render_t::override_material( material_t* const material, const int type ) {
		using fn_t = void( __thiscall* )( decltype( this ), material_t* const, const int, const int );

		return ( *reinterpret_cast< fn_t** >( this ) )[ 33u ]( this, material, type, 0 );
	}

	__forceinline bool studio_render_t::forced_material_override( ) {
		if ( m_override_material )
			return *reinterpret_cast< const std::uint32_t* >( m_override_material->name( ) + 4u ) == 'wolg';

		return m_override_type == 2 || m_override_type == 4;
	}

	__forceinline void c_model_render::draw_model(
		const std::uintptr_t context, const draw_model_state_t& state,
		const model_render_info_t& info, mat3x4_t* const bones
	) {
		using fn_t = void( __thiscall* )( decltype( this ),
			const std::uintptr_t, const draw_model_state_t&,
			const model_render_info_t&, mat3x4_t* const
		);

		return ( *reinterpret_cast< fn_t** >( this ) )[ 21u ]( this, context, state, info, bones );
	}

	__forceinline std::uintptr_t c_mdl_cache::lookup_hw_data( const std::uint16_t handle ) {
		using fn_t = std::uintptr_t( __thiscall* )( decltype( this ), const std::uint16_t );

		return ( *reinterpret_cast< fn_t** >( this ) )[ 15u ]( this, handle );
	}

	__forceinline std::uint16_t c_mat_system::first_material( ) {
		using fn_t = std::uint16_t( __thiscall* )( decltype( this ) );

		return ( *reinterpret_cast< fn_t** >( this ) )[ 86u ]( this );
	}

	__forceinline std::uint16_t c_mat_system::next_material( const std::uint16_t handle ) {
		using fn_t = std::uint16_t( __thiscall* )( decltype( this ), const std::uint16_t );

		return ( *reinterpret_cast< fn_t** >( this ) )[ 87u ]( this, handle );
	}

	__forceinline std::uint16_t c_mat_system::invalid_material( ) {
		using fn_t = std::uint16_t( __thiscall* )( decltype( this ) );

		return ( *reinterpret_cast< fn_t** >( this ) )[ 88u ]( this );
	}

	__forceinline material_t* c_mat_system::material( const std::uint16_t handle ) {
		using fn_t = material_t*( __thiscall* )( decltype( this ), const std::uint16_t );

		return ( *reinterpret_cast< fn_t** >( this ) )[ 89u ]( this, handle );
	}

	__forceinline material_t* c_mat_system::find_material(
		const char* name, const char* group, const bool complain, const char* complain_prefix
	) {
		using fn_t = material_t*( __thiscall* )( decltype( this ), const char*, const char*, const bool, const char* );

		return ( *reinterpret_cast< fn_t** >( this ) )[ 84u ]( this, name, group, complain, complain_prefix );
	}

	__forceinline material_t* c_mat_system::create_material( const char* name, const std::uintptr_t key_values ) {
		using fn_t = material_t*( __thiscall* )( decltype( this ), const char*, const std::uintptr_t );

		return ( *reinterpret_cast< fn_t** >( this ) )[ 83u ]( this, name, key_values );
	}

	__forceinline std::uintptr_t c_mat_system::render_context( ) {
		using fn_t = std::uintptr_t( __thiscall* )( decltype( this ) );

		return ( *reinterpret_cast< fn_t** >( this ) )[ 115u ]( this );
	}

	__forceinline void c_surface::start_drawing( ) {
		using fn_t = void( __thiscall* )( decltype( this ) );

		return reinterpret_cast< fn_t >( g_context->addresses( ).m_start_drawing )( this );
	}

	__forceinline void c_surface::finish_drawing( ) {
		using fn_t = void( __thiscall* )( decltype( this ) );

		return reinterpret_cast< fn_t >( g_context->addresses( ).m_finish_drawing )( this );
	}

	__forceinline void c_surface::set_text_font( const unsigned long font ) {
		using fn_t = void( __thiscall* )( decltype( this ), const unsigned long );

		return ( *reinterpret_cast< fn_t** >( this ) )[ 23u ]( this, font );
	}

	__forceinline void c_surface::set_text_pos( const vec2_t& pos ) {
		using fn_t = void( __thiscall* )( decltype( this ), const int, const int );

		return ( *reinterpret_cast< fn_t** >( this ) )[ 26u ](
			this, static_cast< int >( pos.x ), static_cast< int >( pos.y )
		);
	}

	__forceinline void c_surface::set_text_clr( const std::uint32_t clr ) {
		using fn_t = void( __thiscall* )( decltype( this ), const std::uint32_t );

		return ( *reinterpret_cast< fn_t** >( this ) )[ 24u ]( this, clr );
	}

	__forceinline void c_surface::add_text( const wchar_t* const str, const int len ) {
		using fn_t = void( __thiscall* )( decltype( this ), const wchar_t* const, const int, const int );

		return ( *reinterpret_cast< fn_t** >( this ) )[ 28u ]( this, str, len, 0 );
	}

	__forceinline void c_surface::set_clr( const std::uint32_t clr ) {
		using fn_t = void( __thiscall* )( decltype( this ), const std::uint32_t );

		return ( *reinterpret_cast< fn_t** >( this ) )[ 14u ]( this, clr );
	}

	__forceinline void c_surface::set_texture( const int id ) {
		using fn_t = void( __thiscall* )( decltype( this ), const int );

		return ( *reinterpret_cast< fn_t** >( this ) )[ 38u ]( this, id );
	}

	__forceinline unsigned long c_surface::create_font( ) {
		using fn_t = unsigned long( __thiscall* )( decltype( this ) );

		return ( *reinterpret_cast< fn_t** >( this ) )[ 71u ]( this );
	}

	__forceinline void c_surface::add_poly_line(
		int* const x_points, int* const y_points, const int points_count
	) {
		using fn_t = void( __thiscall* )( decltype( this ), int* const, int* const, const int );

		return ( *reinterpret_cast< fn_t** >( this ) )[ 20u ]( this, x_points, y_points, points_count );
	}

	__forceinline void c_surface::add_clred_text(
		const unsigned long font, const vec2_t& pos,
		const int r, const int g, const int b, const int a, const char* txt
	) {
		using fn_t = void( __thiscall* )(
			decltype( this ), const unsigned long, const int, int, const int,
			const int, const int, const int, const char*, const char*
		);

		return reinterpret_cast< fn_t >( g_context->addresses( ).m_add_clred_text )(
			this, font, static_cast< int >( pos.x ), static_cast< int >( pos.y ),
			r, g, b, a, txt, nullptr
		);
	}

	__forceinline void c_surface::add_line( const vec2_t& from, const vec2_t& to ) {
		using fn_t = void( __thiscall* )(
			decltype( this ), const int, const int, const int, const int
		);

		return ( *reinterpret_cast< fn_t** >( this ) )[ 19u ](
			this, static_cast< int >( from.x ), static_cast< int >( from.y ),
			static_cast< int >( to.x ), static_cast< int >( to.y )
		);
	}

	__forceinline void c_surface::add_rect( const vec2_t& min, const vec2_t& max ) {
		using fn_t = void( __thiscall* )(
			decltype( this ), const int, const int, const int, const int
		);

		return ( *reinterpret_cast< fn_t** >( this ) )[ 18u ](
			this, static_cast< int >( min.x ), static_cast< int >( min.y ),
			static_cast< int >( max.x ), static_cast< int >( max.y )
		);
	}

	__forceinline void c_surface::add_textured_rect( const vec2_t& min, const vec2_t& max ) {
		using fn_t = void( __thiscall* )(
			decltype( this ), const int, const int, const int, const int
		);

		return ( *reinterpret_cast< fn_t** >( this ) )[ 41u ](
			this, static_cast< int >( min.x ), static_cast< int >( min.y ),
			static_cast< int >( max.x ), static_cast< int >( max.y )
		);
	}

	__forceinline void c_surface::add_rect_filled( const vec2_t& min, const vec2_t& max ) {
		using fn_t = void( __thiscall* )(
			decltype( this ), const int, const int, const int, const int
		);

		return ( *reinterpret_cast< fn_t** >( this ) )[ 16u ](
			this, static_cast< int >( min.x ), static_cast< int >( min.y ),
			static_cast< int >( max.x ), static_cast< int >( max.y )
		);
	}

	__forceinline void c_surface::set_texture_rgba(
		const int id, const std::uint8_t* const clrs, const int width, const int height
	) {
		using fn_t = void( __thiscall* )(
			decltype( this ), const int, const std::uint8_t* const, const int, const int
		);

		return ( *reinterpret_cast< fn_t** >( this ) )[ 37u ]( this, id, clrs, width, height );
	}

	__forceinline int c_surface::create_new_texture( const bool procedural ) {
		using fn_t = int( __thiscall* )( decltype( this ), const bool );

		return ( *reinterpret_cast< fn_t** >( this ) )[ 43u ]( this, procedural );
	}

	__forceinline void c_surface::add_textured_polygon(
		const int vertices_count, const vertex_t* const vertices, const bool clip
	) {
		using fn_t = void( __thiscall* )( decltype( this ), const int, const vertex_t* const, const bool );

		return ( *reinterpret_cast< fn_t** >( this ) )[ 106u ]( this, vertices_count, vertices, clip );
	}

	__forceinline void c_surface::play_sound(const char* sample) {
		using fn_t = void(__thiscall*)(decltype(this), const char*);

		return (*reinterpret_cast<fn_t**>(this))[82u](this, sample);
	}

	__forceinline void c_font_mgr::calc_text_size( const unsigned long font, const wchar_t* str, int& width, int& height ) {
		using fn_t = void( __thiscall* )( decltype( this ), const unsigned long, const wchar_t*, int&, int& );

		return reinterpret_cast< fn_t >( g_context->addresses( ).m_calc_text_size )( this, font, str, width, height );
	}

	__forceinline void c_font_mgr::set_font_glyph_set(
		const unsigned long font, const char* name, const int tall, const int weight, const int blur,
		const int scanlines, const int flags, const int range_min, const int range_max
	) {
		using fn_t = void( __thiscall* )(
			decltype( this ), const unsigned long, const char*, const int, const int,
			const int, const int, const int, const int, const int
		);

		return reinterpret_cast< fn_t >( g_context->addresses( ).m_set_font_glyph_set )(
			this, font, name, tall, weight, blur, scanlines, flags, range_min, range_max
		);
	}

	__forceinline wchar_t* c_localize::find_safe( const char* token ) {
		using fn_t = wchar_t*( __thiscall* )( decltype( this ), const char* );

		return ( *reinterpret_cast< fn_t** >( this ) )[ 12u ]( this, token );
	}

	__forceinline int c_model_info::lookup_model_index( const char* name ) {
		using fn_t = int( __thiscall* )( decltype( this ), const char* );

		return ( *reinterpret_cast< fn_t** >( this ) )[ 2u ]( this, name );
	}

	__forceinline void c_beams::draw_beam( const std::uintptr_t beam ) {
		using fn_t = void( __thiscall* )( decltype( this ), const std::uintptr_t );

		return ( *reinterpret_cast< fn_t** >( this ) )[ 4u ]( this, beam );
	}

	__forceinline std::uintptr_t c_beams::create_beam_points( beam_info_t& info ) {
		using fn_t = std::uintptr_t( __thiscall* )( decltype( this ), beam_info_t& );

		return ( *reinterpret_cast< fn_t** >( this ) )[ 12u ]( this, info );
	}

	__forceinline std::uintptr_t c_beams::create_beam_ring_point( beam_info_t& info ) {
		using fn_t = std::uintptr_t( __thiscall* )( decltype( this ), beam_info_t& );

		return ( *reinterpret_cast< fn_t** >( this ) )[ 16u ]( this, info );
	}

	__forceinline void c_debug_overlay::add_box_overlay(
		const vec3_t& pos, const vec3_t& min, const vec3_t& max, const qangle_t& rotation,
		const int r, const int g, const int b, const int a, const float duration
	) {
		using fn_t = void( __thiscall* )( decltype( this ),
			const vec3_t&, const vec3_t&, const vec3_t&, const qangle_t&,
			const int, const int, const int, const int, const float
		);

		return ( *reinterpret_cast< fn_t** >( this ) )[ 1u ](
			this, pos, min, max, rotation, r, g, b, a, duration
		);
	}

	__forceinline void c_engine_sound::emit_ambient_sound(
		const char* sample, float volume, int pitch = 100, int flags = 0, float time = 0.f) {
		using fn_t = void(__thiscall*)(decltype(this), const char*, float, int, int, float);

		return (*reinterpret_cast<fn_t**>(this))[5u](this, sample, volume, pitch, flags, time);
	}

	__forceinline void c_engine_sound::get_active_sounds(utl_vec_t<sound_info_t>& sound_list) {
		using fn_t = void(__thiscall*)(decltype(this), utl_vec_t<sound_info_t>&);

		return (*reinterpret_cast<fn_t**>(this))[19u](this, sound_list);
	}

	__forceinline void c_panel::set_keyboard_input_enabled(unsigned int panel_id, bool state) {
		using fn_t = void(__thiscall*)(decltype(this), unsigned int, bool);

		return (*reinterpret_cast<fn_t**>(this))[31u](this, panel_id, state);
	}

	__forceinline void c_panel::set_mouse_input_enabled(unsigned int panel_id, bool state) {
		using fn_t = void(__thiscall*)(decltype(this), unsigned int, bool);

		return (*reinterpret_cast<fn_t**>(this))[31u](this, panel_id, state);
	}

	__forceinline const char* c_panel::get_panel_name(unsigned int panel_id) {
		using fn_t = const char* (__thiscall*)(decltype(this), unsigned int);

		return (*reinterpret_cast<fn_t**>(this))[36u](this, panel_id);
	}

	__forceinline int c_uipanel::get_child_count() {
		using fn_t = int(__thiscall*)(decltype(this));

		return (*reinterpret_cast<fn_t**>(this))[48u](this);
	}

	__forceinline c_uipanel* c_uipanel::get_child(int value) {
		using fn_t = c_uipanel * (__thiscall*)(decltype(this), int);

		return (*reinterpret_cast<fn_t**>(this))[49u](this, value);
	}

	__forceinline bool c_uipanel::has_class(const char* name) {
		using fn_t = bool(__thiscall*)(decltype(this), const char*);

		return (*reinterpret_cast<fn_t**>(this))[139u](this, name);
	}

	__forceinline void c_uipanel::set_attribute_float(const char* name, float value) {
		using fn_t = void(__thiscall*)(decltype(this), const char*, float);

		return (*reinterpret_cast<fn_t**>(this))[288u](this, name, value);
	}

	__forceinline int to_ticks( const float time ) {
		return static_cast< int >(
			time / g_global_vars->m_interval_per_tick + 0.5f
		);
	}

	__forceinline float to_time( const int ticks ) {
		return ticks * g_global_vars->m_interval_per_tick;
	}
}