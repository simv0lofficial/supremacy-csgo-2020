#pragma once

#include "../context.hpp"

namespace supremacy {
	__forceinline c_context::addresses_t& c_context::addresses( ) {
		return m_addresses;
	}

	__forceinline c_context::cvars_t& c_context::cvars( ) {
		return m_cvars;
	}

	__forceinline c_context::net_info_t& c_context::net_info( ) {
		return m_net_info;
	}

	__forceinline vec3_t& c_context::shoot_pos( ) {
		return m_shoot_pos;
	}

	__forceinline vec3_t& c_context::start_position() {
		return m_start_position;
	}

	__forceinline bool& c_context::console_drawn() {
		return m_console_drawn;
	}

	__forceinline bool& c_context::switch_lagcompensation_state() {
		return m_switch_lagcompensation_state;
	}

	__forceinline bool& c_context::should_return() {
		return m_should_return;
	}

	__forceinline int& c_context::last_sent_cmd_number( ) {
		return m_last_sent_cmd_number;
	}

	__forceinline int& c_context::last_cmd_number( ) {
		return m_last_cmd_number;
	}

	__forceinline bool& c_context::should_auto_buy() {
		return m_should_auto_buy;
	}

	__forceinline bool& c_context::allow_anim_update( ) {
		return m_allow_anim_update;
	}

	__forceinline bool& c_context::allow_setup_bones( ) {
		return m_allow_setup_bones;
	}

	__forceinline bool& c_context::force_bone_mask( ) {
		return m_force_bone_mask;
	}

	__forceinline std::vector< int >& c_context::sented_cmds( ) {
		return m_sented_cmds;
	}

	__forceinline e_context_flags& c_context::flags( ) {
		return m_flags;
	}

	__forceinline valve::c_weapon*& c_context::weapon( ) {
		return m_weapon;
	}

	__forceinline valve::weapon_data_t*& c_context::wpn_data( ) {
		return m_wpn_data;
	}

	__forceinline bool& c_context::freeze_time( ) {
		return m_freeze_time;
	}

	__forceinline bool& c_context::broke_lc( ) {
		return m_broke_lc;
	}

	__forceinline vec3_t& c_context::last_sent_origin( ) {
		return m_last_sent_origin;
	}

	__forceinline vec3_t& c_context::view_angles() {
		return m_view_angles;
	}
}