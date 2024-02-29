#pragma once

#include "../other.hpp"

namespace supremacy::valve {
	__forceinline const char* game_event_t::name( ) {
		using fn_t = const char*( __thiscall* )( decltype( this ) );

		return ( *reinterpret_cast< fn_t** >( this ) )[ 1u ]( this );
	}

	__forceinline float game_event_t::get_float( const char* name ) {
		using fn_t = float( __thiscall* )( decltype( this ), const char*, const float );

		return ( *reinterpret_cast< fn_t** >( this ) )[ 8u ]( this, name, 0.f );
	}

	__forceinline int game_event_t::get_int( const char* name ) {
		using fn_t = int( __thiscall* )( decltype( this ), const char*, const int );

		return ( *reinterpret_cast< fn_t** >( this ) )[ 6u ]( this, name, 0 );
	}

	__forceinline const char* game_event_t::get_str(const char* name) {
		using fn_t = const char* (__thiscall*)(decltype(this), const char*, const  char*);

		return (*reinterpret_cast<fn_t**>(this))[9u](this, name, xorstr_(""));
	}

	__forceinline void material_var_t::set_float( const float value ) {
		using fn_t = void( __thiscall* )( decltype( this ), const float );

		return ( *reinterpret_cast< fn_t** >( this ) )[ 4u ]( this, value );
	}

	__forceinline void material_var_t::set_int( const int value ) {
		using fn_t = void( __thiscall* )( decltype( this ), const int );

		return ( *reinterpret_cast< fn_t** >( this ) )[ 5u ]( this, value );
	}

	__forceinline void material_var_t::set_str( const char* value ) {
		using fn_t = void( __thiscall* )( decltype( this ), const char* );

		return ( *reinterpret_cast< fn_t** >( this ) )[ 6u ]( this, value );
	}

	__forceinline void material_var_t::set_vec( const float v0, const float v1, const float v2 ) {
		using fn_t = void( __thiscall* )( decltype( this ), const float, const float, const float );

		return ( *reinterpret_cast< fn_t** >( this ) )[ 11u ]( this, v0, v1, v2 );
	}

	__forceinline const char* material_t::name( ) {
		using fn_t = const char*( __thiscall* )( decltype( this ) );

		return ( *reinterpret_cast< fn_t** >( this ) )[ 0u ]( this );
	}

	__forceinline const char* material_t::texture_group( ) {
		using fn_t = const char*( __thiscall* )( decltype( this ) );

		return ( *reinterpret_cast< fn_t** >( this ) )[ 1u ]( this );
	}

	__forceinline void material_t::set_alpha( const float value ) {
		using fn_t = void( __thiscall* )( decltype( this ), const float );

		return ( *reinterpret_cast< fn_t** >( this ) )[ 27u ]( this, value );
	}

	__forceinline void material_t::set_clr( const float r, const float g, const float b ) {
		using fn_t = void( __thiscall* )( decltype( this ), const float, const float, const float );

		return ( *reinterpret_cast< fn_t** >( this ) )[ 28u ]( this, r, g, b );
	}

	__forceinline void material_t::set_flag( const int flag, const bool value ) {
		using fn_t = void( __thiscall* )( decltype( this ), const int, const bool );

		return ( *reinterpret_cast< fn_t** >( this ) )[ 29u ]( this, flag, value );
	}

	__forceinline material_var_t* material_t::find_var( const char* name, bool* const found, const bool complain ) {
		using fn_t = material_var_t*( __thiscall* )( decltype( this ), const char*, bool* const, const bool );

		return ( *reinterpret_cast< fn_t** >( this ) )[ 11u ]( this, name, found, complain );
	}

	__forceinline const char* net_channel_info_t::ip() {
		using fn_t = const char* (__thiscall*)(decltype(this));

		return (*reinterpret_cast<fn_t**>(this))[1u](this);
	}

	__forceinline bool net_channel_info_t::loopback() {
		using fn_t = bool(__thiscall*)(decltype(this));

		return (*reinterpret_cast<fn_t**>(this))[6u](this);
	}

	__forceinline float net_channel_info_t::latency( const int flow ) {
		using fn_t = float( __thiscall* )( decltype( this ), const int );

		return ( *reinterpret_cast< fn_t** >( this ) )[ 9u ]( this, flow );
	}

	__forceinline float net_channel_info_t::avg_latency( const int flow ) {
		using fn_t = float( __thiscall* )( decltype( this ), const int );

		return ( *reinterpret_cast< fn_t** >( this ) )[ 10u ]( this, flow );
	}

	__forceinline void trace_t::clear( ) {
		std::memset( this, 0, sizeof( trace_t ) );

		m_fraction = 1.f;
		m_surface.m_name = xorstr_( "**empty**" );
	}

	__forceinline bool trace_t::hit( ) const {
		return m_fraction < 1.f || m_all_solid || m_start_solid;
	}

	__forceinline void net_channel_t::send_datagram( const uintptr_t data ) {
		using fn_t = void( __thiscall* )( decltype( this ), const uintptr_t );

		return ( *reinterpret_cast< fn_t** >( this ) )[ 46u ]( this, data );
	}

	__forceinline void net_channel_t::send_net_msg(const uintptr_t msg, bool force_reliable, bool voice) {
		using fn_t = void(__thiscall*)(decltype(this), const uintptr_t, bool, bool);

		return (*reinterpret_cast<fn_t**>(this))[40u](this, msg, force_reliable, voice);
	}

	__forceinline std::uint32_t user_cmd_t::calc_checksum( ) {
		using fn_t = std::uint32_t( __thiscall* )( decltype( this ) );

		return reinterpret_cast< fn_t >( g_context->addresses( ).m_user_cmd_calc_checksum )( this );
	}

	__forceinline void cvar_t::set_int( const int value ) {
		using fn_t = void( __thiscall* )( decltype( this ), const int );

		return ( *reinterpret_cast< fn_t** >( this ) )[ 16u ]( this, value );
	}

	__forceinline void cvar_t::set_float( const float value ) {
		using fn_t = void( __thiscall* )( decltype( this ), const float );

		return ( *reinterpret_cast< fn_t** >( this ) )[ 15u ]( this, value );
	}

	__forceinline void cvar_t::set_str( const char* value ) {
		using fn_t = void( __thiscall* )( decltype( this ), const char* );

		return ( *reinterpret_cast< fn_t** >( this ) )[ 14u ]( this, value );
	}

	__forceinline float cvar_t::get_float( ) {
		using fn_t = float( __thiscall* )( decltype( this ) );

		return ( *reinterpret_cast< fn_t** >( this ) )[ 12u ]( this );
	}

	__forceinline const char* cvar_t::get_str( ) {
		using fn_t = const char*( __thiscall* )( decltype( this ) );

		return ( *reinterpret_cast< fn_t** >( this ) )[ 11u ]( this );
	}

	__forceinline int cvar_t::get_int( ) {
		using fn_t = int( __thiscall* )( decltype( this ) );

		return ( *reinterpret_cast< fn_t** >( this ) )[ 13u ]( this );
	}

	__forceinline bool cvar_t::get_bool( ) {
		return get_int( );
	}

	__forceinline studio_hitbox_t* studio_hitbox_set_t::hitbox( const int index ) {
		return reinterpret_cast< studio_hitbox_t* >(
			reinterpret_cast< std::uintptr_t >( this ) + m_hitbox_index
		) + index;
	}

	__forceinline studio_bone_t* studio_hdr_t::bone( const int index ) {
		return reinterpret_cast< studio_bone_t* >(
			reinterpret_cast< std::uintptr_t >( this ) + m_bone_index
		) + index;
	}

	__forceinline studio_hitbox_set_t* studio_hdr_t::hitbox_set( const int index ) {
		return reinterpret_cast< studio_hitbox_set_t* >(
			reinterpret_cast< std::uintptr_t >( this ) + m_hitbox_set_index
		) + index;
	}

	template < typename _value_t >
	__forceinline _value_t& utl_vec_t< _value_t >::at( const int i ) {
		return m_mem.m_ptr[ i ];
	}

	template < typename _value_t >
	__forceinline const _value_t& utl_vec_t< _value_t >::at( const int i ) const {
		return m_mem.m_ptr[ i ];
	}

	__forceinline void bf_write_t::write_user_cmd( user_cmd_t* const to, user_cmd_t* const from ) {
		using fn_t = void( __fastcall* )( decltype( this ), user_cmd_t* const, user_cmd_t* const );

		reinterpret_cast< fn_t >( g_context->addresses( ).m_write_user_cmd )( this, to, from );

		__asm add esp, 4
	}

	__forceinline void anim_state_t::reset( ) {
		reinterpret_cast< void( __thiscall* )( decltype( this ) ) >(
			g_context->addresses( ).m_reset_anim_state
		)( this );
	}

	__forceinline void anim_state_t::update(
		const float pitch, const float yaw
	) {
		reinterpret_cast< void( __vectorcall* )( decltype( this ), void*, float, float, float, void* ) >(
			g_context->addresses( ).m_update_anim_state
		)( this, nullptr, 0.f, yaw, pitch, nullptr );
	}
}