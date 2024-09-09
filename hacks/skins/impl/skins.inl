#pragma once
#include "../skins.hpp"

namespace supremacy::hacks {
	__forceinline void c_sim_context::check_velocity( extrapolation_data_t& data ) const {
		static auto sv_max_vel = valve::g_cvar->find_var( xorstr_( "sv_maxvelocity" ) );

		for ( std::ptrdiff_t i{}; i < 3; ++i ) {
			if ( isnan( data.m_velocity[ i ] ) )
				data.m_velocity[ i ] = 0.f;

			if ( isnan( data.m_origin[ i ] ) )
				data.m_origin[ i ] = 0.f;

			if ( data.m_velocity[ i ] > sv_max_vel->get_float( ) )
				data.m_velocity[ i ] = sv_max_vel->get_float( );
			else if ( data.m_velocity[ i ] < -sv_max_vel->get_float( ) )
				data.m_velocity[ i ] = -sv_max_vel->get_float( );
		}
	}

	__forceinline void c_sim_context::trace_player_bbox( extrapolation_data_t& data, vec3_t& start, vec3_t& end, valve::trace_t* trace ) const {
		valve::ray_t ray = { start, end, data.m_obb_min, data.m_obb_max };
		valve::trace_filter_simple_t filter;

		filter.m_ignore_entity = valve::g_local_player;

		valve::g_engine_trace->trace_ray( ray, valve::e_mask::contents_solid, reinterpret_cast < valve::trace_filter_t* > ( &filter ), trace );
	}

	__forceinline void c_sim_context::friction( extrapolation_data_t& data ) const {
		static auto sv_friction = valve::g_cvar->find_var( xorstr_( "sv_friction" ) );
		static auto sv_stopspeed = valve::g_cvar->find_var( xorstr_( "sv_stopspeed" ) );
		const auto surf_friction = data.m_player->surface_friction( );
		auto speed = data.m_velocity.length_2d( );

		if ( speed < 0.1f ) {
			return;
		}

		float friction = sv_friction->get_float( ) * surf_friction;
		auto control = std::max( sv_stopspeed->get_float( ), speed );
		auto drop = control * friction * valve::g_global_vars->m_interval_per_tick;
		auto new_speed = speed - drop;

		if ( new_speed < 0.f )
			new_speed = 0.f;

		if ( new_speed != speed ) {
			new_speed /= speed;
			data.m_velocity *= new_speed;
		}
	}

	__forceinline void c_sim_context::air_accelerate( extrapolation_data_t& data, vec3_t& wish_dir, float wish_spd ) const {
		static auto sv_airaccelerate = valve::g_cvar->find_var( xorstr_( "sv_airaccelerate" ) );

		auto wish_niggas = wish_spd;

		if ( wish_niggas > 30.f )
			wish_niggas = 30.f;

		auto cur_spd = data.m_velocity.dot( wish_dir );
		auto add_spd = wish_niggas - cur_spd;

		if ( add_spd <= 0.f )
			return;

		auto accel_spd = sv_airaccelerate->get_float( ) * wish_niggas * valve::g_global_vars->m_interval_per_tick * data.m_player->surface_friction( );

		if ( accel_spd > add_spd )
			accel_spd = add_spd;

		for ( std::size_t i{}; i < 3u; ++i ) {
			data.m_velocity[ i ] += accel_spd * wish_dir[ i ];
		}
	}

	/* i think it needs improvements but idc for now p.s. it doesn't */
	__forceinline void c_sim_context::try_player_move( extrapolation_data_t& data ) const {
		valve::trace_t trace;
		vec3_t end_pos = data.m_origin + data.m_velocity * valve::g_global_vars->m_interval_per_tick;

		trace_player_bbox( data, data.m_origin, end_pos, &trace );

		if ( trace.m_fraction != 1.f ) {
			end_pos = trace.m_end_pos;
		}

		data.m_origin = end_pos;
	}

	__forceinline void c_sim_context::accelerate( extrapolation_data_t& data, vec3_t& wish_dir, float wish_spd, float accel ) const {
		auto cur_spd = data.m_velocity.dot( wish_dir );
		auto add_spd = wish_spd - cur_spd;

		if ( add_spd <= 0.f )
			return;

		const float accelerate_scale = std::max( 250.f, wish_spd );

		auto accel_spd = accel * valve::g_global_vars->m_interval_per_tick * accelerate_scale * data.m_player->surface_friction( );

		if ( accel_spd > add_spd )
			accel_spd = add_spd;

		for ( std::size_t i{}; i < 3u; ++i ) {
			data.m_velocity[ i ] += accel_spd * wish_dir[ i ];
		}
	}

	__forceinline void c_sim_context::walk_move( extrapolation_data_t& data ) const {
		float speed2d = data.m_velocity.length_2d( );
		static auto sv_stepsize = valve::g_cvar->find_var( xorstr_( "sv_stepsize" ) );
		valve::trace_t trace;
		static auto sv_accelerate = valve::g_cvar->find_var( xorstr_( "sv_accelerate" ) );

		float dir = math::to_deg( std::atan2f( data.m_velocity.y, data.m_velocity.x ) );

		vec3_t right{};
		vec3_t fwd{};
		vec3_t wish_vel{};
		math::angle_vectors( qangle_t( 0.f, dir, 0.f ), &fwd, &right );

		if ( fwd.z != 0.f ) {
			fwd.z = 0.f;
			fwd.normalize( );
		}

		if ( right.z != 0.f ) {
			right.z = 0.f;
			right.normalize( );
		}

		for ( int i{}; i < 2; ++i )
			wish_vel[ i ] = ( fwd[ i ] * data.m_velocity.x ) + ( right[ i ] * data.m_velocity.y );

		wish_vel.z = 0.f;

		auto wish_dir = wish_vel;
		auto wish_spd = wish_dir.normalize( );

		data.m_velocity.z = 0.f;
		accelerate( data, wish_dir, wish_spd, sv_accelerate->get_float( ) );
		data.m_velocity.z = 0.f;

		static auto sv_maxspeed = valve::g_cvar->find_var( xorstr_( "sv_maxspeed" ) );

		if ( data.m_velocity.length( ) > std::min( data.m_player->max_speed( ), sv_maxspeed->get_float( ) ) ) {
			data.m_velocity = data.m_velocity.normalized( ) * std::min( data.m_player->max_speed( ), sv_maxspeed->get_float( ) );
		}

		vec3_t dest{};

		dest[ 0 ] = data.m_origin[ 0 ] + data.m_velocity[ 0 ] * valve::g_global_vars->m_interval_per_tick;
		dest[ 1 ] = data.m_origin[ 1 ] + data.m_velocity[ 1 ] * valve::g_global_vars->m_interval_per_tick;
		dest[ 2 ] = data.m_origin[ 2 ];

		trace_player_bbox( data, data.m_origin, dest, &trace );

		if ( trace.m_fraction == 1.f ) {
			data.m_origin = trace.m_end_pos;

			valve::trace_t gt;
			vec3_t start{ data.m_origin };
			vec3_t end{ data.m_origin };

			start.z += 2.f;
			end.z -= sv_stepsize->get_float( );

			trace_player_bbox( data, data.m_origin, start, &gt );
			start = gt.m_end_pos;

			trace_player_bbox( data, start, end, &gt );

			if ( gt.m_fraction > 0.f && gt.m_fraction < 1.f && !gt.m_start_solid && gt.m_plane.m_normal.z >= 0.7f ) {
				float delta = std::abs( data.m_origin.z - gt.m_end_pos.z );

				if ( delta > 0.015625f )
					data.m_origin = gt.m_end_pos;
			}
		}
		else if ( !data.m_was_in_air ) {
			auto pos = data.m_origin;
			auto vel = data.m_velocity;

			try_player_move( data );

			auto down_pos = data.m_origin;
			auto down_vel = data.m_velocity;

			data.m_origin = pos;
			data.m_velocity = vel;

			vec3_t end_pos{};

			end_pos = data.m_origin;
			end_pos.z += sv_stepsize->get_float( ) + 0.03125f;

			valve::trace_t trace_;

			trace_player_bbox( data, data.m_origin, end_pos, &trace_ );

			if ( !trace_.m_start_solid && !trace_.m_all_solid )
				data.m_origin = trace_.m_end_pos;

			try_player_move( data );

			end_pos = data.m_origin;
			end_pos.z -= sv_stepsize->get_float( ) + 0.03125f;

			trace_player_bbox( data, data.m_origin, end_pos, &trace_ );

			if ( !trace_.m_start_solid && !trace_.m_all_solid )
				data.m_origin = trace_.m_end_pos;

			if ( trace_.m_plane.m_normal.z < 0.7f ) {
				data.m_origin = down_pos;
				data.m_velocity = down_vel;
			}
			else {
				if ( !trace.m_start_solid && !trace_.m_all_solid ) {
					data.m_origin = trace_.m_end_pos;
				}

				vec3_t up_pos{};
				up_pos = data.m_origin;

				float down_dist = ( down_pos.x - pos.x ) * ( down_pos.x - pos.x ) + ( down_pos.y - pos.y ) * ( down_pos.y - pos.y );
				float up_dist = ( up_pos.x - pos.x ) * ( up_pos.x - pos.x ) + ( up_pos.y - pos.y ) * ( up_pos.y - pos.y );
				if ( down_dist > up_dist ) {
					data.m_origin = down_pos;
					data.m_velocity = down_vel;
				}
				else {
					// copy z value from slide move
					data.m_velocity.z = down_vel.z;
				}

				valve::trace_t last_trace{};

				vec3_t start{ data.m_origin };
				vec3_t end{ data.m_origin };
				start.z += 2.f;
				end.z -= sv_stepsize->get_float( );

				trace_player_bbox( data, data.m_origin, start, &last_trace );

				start = last_trace.m_end_pos;

				trace_player_bbox( data, start, end, &last_trace );

				if ( last_trace.m_fraction > 0.f && last_trace.m_fraction < 1.f && !last_trace.m_start_solid && last_trace.m_plane.m_normal.z >= 0.7f ) {
					float delta = std::abs( data.m_origin.z - last_trace.m_end_pos.z );

					if ( delta > 0.015625f )
						data.m_origin = last_trace.m_end_pos;
				}
			}
		}
	}

	__forceinline bool c_sim_context::categorize_pos( extrapolation_data_t& data ) {
		vec3_t ground_point = data.m_origin;
		valve::trace_t trace;

		/* abs_origin.z - offset watafak */
		ground_point.z -= 2.f;

		trace_player_bbox( data, data.m_origin, ground_point, &trace );

		if ( !trace.m_hit_entity || ( trace.m_plane.m_normal.z < 0.7f && trace.m_fraction == 1.f ) ) {
			try_touch_ground_in_quad( data, data.m_origin, ground_point, &trace );

			if ( !trace.m_hit_entity || ( trace.m_plane.m_normal.z < 0.7f && trace.m_fraction == 1.f ) ) {
				return false;
			}
			else {
				return true;
			}
		}
		else {
			return true;
		}

		return true;
	}

	__forceinline void c_sim_context::try_touch_ground(
		const vec3_t& start, const vec3_t& end, const vec3_t& mins, const vec3_t& maxs, valve::trace_t* trace
	) {
		valve::ray_t ray = { start, end, mins, maxs };
		valve::trace_filter_simple_t filter;

		filter.m_ignore_entity = valve::g_local_player;

		valve::g_engine_trace->trace_ray( ray, valve::e_mask::contents_solid, reinterpret_cast < valve::trace_filter_t* > ( &filter ), trace );
	}
}