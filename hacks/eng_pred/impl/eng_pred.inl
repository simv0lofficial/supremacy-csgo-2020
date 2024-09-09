#pragma once

#include "../eng_pred.hpp"

namespace supremacy::hacks {
	__forceinline void c_eng_pred::net_vars_t::restore( const int cmd_number ) const {
		if ( cmd_number != m_cmd_number )
			return;

		if ( std::abs( valve::g_local_player->view_offset( ).z - m_view_offset_z ) <= 0.03125f )
			valve::g_local_player->view_offset( ).z = m_view_offset_z;

		const auto aim_punch_delta = valve::g_local_player->aim_punch( ) - m_aim_punch;
		if ( std::abs( aim_punch_delta.x ) <= 0.03125f
			&& std::abs( aim_punch_delta.y ) <= 0.03125f
			&& std::abs( aim_punch_delta.z ) <= 0.03125f )
			valve::g_local_player->aim_punch( ) = m_aim_punch;

		const auto aim_punch_vel_delta = valve::g_local_player->aim_punch_vel( ) - m_aim_punch_vel;
		if ( std::abs( aim_punch_vel_delta.x ) <= 0.03125f
			&& std::abs( aim_punch_vel_delta.y ) <= 0.03125f
			&& std::abs( aim_punch_vel_delta.z ) <= 0.03125f )
			valve::g_local_player->aim_punch_vel( ) = m_aim_punch_vel;
	}

	__forceinline void c_eng_pred::net_vars_t::store( const int cmd_number ) {
		m_cmd_number = cmd_number;

		m_view_offset_z = std::clamp( valve::g_local_player->view_offset( ).z, 0.f, 64.f );
		m_aim_punch = valve::g_local_player->aim_punch( );
		m_aim_punch_vel = valve::g_local_player->aim_punch_vel( );
	}

	__forceinline void c_eng_pred::local_data_t::init( const valve::user_cmd_t& user_cmd ) {
		std::memset( this, 0, sizeof( local_data_t ) );

		if ( const auto weapon = valve::g_local_player->weapon( ) ) {
			g_eng_pred->recoil_index( ) = weapon->recoil_index( );
			g_eng_pred->accuracy_penalty( ) = weapon->accuracy_penalty( );
		}

		m_spawn_time = valve::g_local_player->spawn_time( );
		m_tick_base = m_adjusted_tick_base = valve::g_local_player->tick_base( );

		m_user_cmd = user_cmd;

		store_anim( );

		m_net_vars.m_eye_angles = valve::g_local_player->eye_angles( );
		m_net_vars.m_aim_punch = valve::g_local_player->aim_punch( );
		m_net_vars.m_view_punch = valve::g_local_player->view_punch( );

		m_net_vars.m_origin = valve::g_local_player->origin( );
		m_net_vars.m_abs_origin = valve::g_local_player->abs_origin( );
		m_net_vars.m_obb_min = valve::g_local_player->obb_min( );
		m_net_vars.m_obb_max = valve::g_local_player->obb_max( );
		m_net_vars.m_velocity = valve::g_local_player->velocity( );
		m_net_vars.m_view_offset = valve::g_local_player->view_offset( );
		m_net_vars.m_aim_punch_vel = valve::g_local_player->aim_punch_vel( );
		m_net_vars.m_abs_velocity = valve::g_local_player->abs_velocity( );

		m_net_vars.m_strafing = valve::g_local_player->strafing( );
		m_net_vars.m_scoped = valve::g_local_player->scoped( );
		m_net_vars.m_walking = valve::g_local_player->walking( );

		m_net_vars.m_move_state = valve::g_local_player->move_state( );
		m_net_vars.m_tick_base = valve::g_local_player->tick_base( );

		m_net_vars.m_duck_amount = valve::g_local_player->duck_amount( );
		m_net_vars.m_duck_speed = valve::g_local_player->duck_speed( );
		m_net_vars.m_third_person_recoil = valve::g_local_player->third_person_recoil( );
		m_net_vars.m_lby = valve::g_local_player->lby( );

		m_net_vars.m_flags = valve::g_local_player->flags( );
		m_net_vars.m_move_type = valve::g_local_player->move_type( );

		m_pred_net_vars = m_net_vars;
	}

	__forceinline void c_eng_pred::local_data_t::save( const int cmd_number ) {
		if ( cmd_number != m_user_cmd.m_number )
			return;

		store_anim( );

		m_anim_state = *valve::g_local_player->anim_state( );

		m_pred_net_vars.m_eye_angles = valve::g_local_player->eye_angles( );
		m_pred_net_vars.m_aim_punch = valve::g_local_player->aim_punch( );
		m_pred_net_vars.m_view_punch = valve::g_local_player->view_punch( );

		m_pred_net_vars.m_origin = valve::g_local_player->origin( );
		m_pred_net_vars.m_abs_origin = valve::g_local_player->abs_origin( );
		m_pred_net_vars.m_obb_min = valve::g_local_player->obb_min( );
		m_pred_net_vars.m_obb_max = valve::g_local_player->obb_max( );
		m_pred_net_vars.m_velocity = valve::g_local_player->velocity( );
		m_pred_net_vars.m_view_offset = valve::g_local_player->view_offset( );
		m_pred_net_vars.m_aim_punch_vel = valve::g_local_player->aim_punch_vel( );
		m_pred_net_vars.m_abs_velocity = valve::g_local_player->abs_velocity( );

		m_pred_net_vars.m_strafing = valve::g_local_player->strafing( );
		m_pred_net_vars.m_scoped = valve::g_local_player->scoped( );
		m_pred_net_vars.m_walking = valve::g_local_player->walking( );

		m_pred_net_vars.m_move_state = valve::g_local_player->move_state( );
		m_pred_net_vars.m_tick_base = valve::g_local_player->tick_base( );

		m_pred_net_vars.m_duck_amount = valve::g_local_player->duck_amount( );
		m_pred_net_vars.m_duck_speed = valve::g_local_player->duck_speed( );
		m_pred_net_vars.m_third_person_recoil = valve::g_local_player->third_person_recoil( );
		m_pred_net_vars.m_lby = valve::g_local_player->lby( );

		m_pred_net_vars.m_flags = valve::g_local_player->flags( );
		m_pred_net_vars.m_move_type = valve::g_local_player->move_type( );
	}

	__forceinline void c_eng_pred::local_data_t::store_anim( ) {
		if ( const auto anim_state = valve::g_local_player->anim_state( ) ) {
			m_anim_state = *anim_state;
			m_abs_yaw = anim_state->m_foot_yaw;
		}

		m_anim_layers = valve::g_local_player->anim_layers( );
		m_pose_params = valve::g_local_player->pose_params( );
	}

	__forceinline void c_eng_pred::local_data_t::restore_anim( const bool layers ) const {
		if ( const auto anim_state = valve::g_local_player->anim_state( ) )
			*anim_state = m_anim_state;

		if ( layers )
			valve::g_local_player->anim_layers( ) = m_anim_layers;

		valve::g_local_player->pose_params( ) = m_pose_params;

		valve::g_local_player->set_abs_angles( { 0.f, m_abs_yaw, 0.f } );
	}

	__forceinline valve::e_frame_stage& c_eng_pred::last_frame_stage( ) {
		return m_last_frame_stage;
	}

	__forceinline std::array< c_eng_pred::net_vars_t, 150u >& c_eng_pred::net_vars( ) {
		return m_net_vars;
	}

	__forceinline std::array< c_eng_pred::local_data_t, 150u >& c_eng_pred::local_data( ) {
		return m_local_data;
	}

	__forceinline valve::user_cmd_t& c_eng_pred::old_user_cmd( ) {
		return m_old_user_cmd;
	}

	__forceinline valve::user_cmd_t& c_eng_pred::last_user_cmd( ) {
		return m_last_user_cmd;
	}

	__forceinline void c_eng_pred::reset_on_spawn( ) {
		m_net_velocity_modifier = 1.f;

		std::memset( m_local_data.data( ), 0, sizeof( local_data_t ) * m_local_data.size( ) );
	}

	__forceinline float& c_eng_pred::spread( ) {
		return m_spread;
	}

	__forceinline float& c_eng_pred::min_inaccuracy( ) {
		return m_min_inaccuracy;
	}

	__forceinline float& c_eng_pred::inaccuracy( ) {
		return m_inaccuracy;
	}

	__forceinline float& c_eng_pred::recoil_index( ) {
		return m_recoil_index;
	}

	__forceinline float& c_eng_pred::accuracy_penalty( ) {
		return m_accuracy_penalty;
	}

	__forceinline float& c_eng_pred::net_velocity_modifier( ) {
		return m_net_velocity_modifier;
	}

	__forceinline bool& c_eng_pred::r8_can_shoot( ) {
		return m_r8_can_shoot;
	}

	__forceinline float& c_eng_pred::postpone_fire_ready_time( ) {
		return m_postpone_fire_ready_time;
	}
}