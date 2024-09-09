#include "../../../supremacy.hpp"

namespace supremacy::hacks {
	/* super pasta */
	void c_sim_context::handle_context( extrapolation_data_t& data ) {
		data.m_sim_time += valve::g_global_vars->m_interval_per_tick;

		if ( !data.m_was_in_air ) {
			friction( data );
			data.m_velocity.z = 0.f;
		}

		check_velocity( data );

		if ( !data.m_was_in_air )
			walk_move( data );
		else
			air_move( data );

		data.m_was_in_air = !categorize_pos( data );

		check_velocity( data );

		if ( !data.m_was_in_air )
			data.m_velocity.z = 0.f;
	}

	/* AHAHAHHA WATAFAKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKK */
	void c_sim_context::air_move( extrapolation_data_t& data ) {
		float dir = math::to_deg( std::atan2f( data.m_velocity.y, data.m_velocity.x ) );

		vec3_t right{};
		vec3_t fwd{};
		vec3_t wish_vel{};
		math::angle_vectors( qangle_t( 0.f, dir, 0.f ), &fwd, &right );

		right.z = 0.f;
		fwd.z = 0.f;

		right.normalize( );
		fwd.normalize( );

		for ( int i{}; i < 2; ++i )
			wish_vel[ i ] = ( fwd[ i ] * data.m_velocity.x ) + ( right[ i ] * data.m_velocity.y );

		wish_vel.z = 0.f;

		auto wish_dir = wish_vel;
		auto wish_spd = wish_dir.normalize( );

		air_accelerate( data, wish_dir, wish_spd );

		try_player_move( data );
	}

	/* that's a pasta from moneybot and i really hope nave didn't fail anything here like in air_move o__o */
	void c_sim_context::try_touch_ground_in_quad( extrapolation_data_t& data, const vec3_t& start, const vec3_t& end, valve::trace_t* trace ) {
		vec3_t mins{}, maxs{};

		vec3_t mins_src = data.m_obb_min;
		vec3_t maxs_src = data.m_obb_max;

		float fraction = trace->m_fraction;
		vec3_t end_pos = trace->m_end_pos;

		mins = mins_src;
		maxs = vec3_t( std::min( 0.f, maxs_src.x ), std::min( 0.f, maxs_src.y ), maxs_src.z );

		try_touch_ground( start, end, mins, maxs, trace );
		if ( trace->m_hit_entity && trace->m_plane.m_normal.z >= 0.7f ) {
			trace->m_fraction = fraction;
			trace->m_end_pos = end_pos;
			return;
		}

		mins = vec3_t( std::max( 0.f, mins_src.x ), std::max( 0.f, mins_src.y ), mins_src.z );
		maxs = maxs_src;

		try_touch_ground( start, end, mins, maxs, trace );
		if ( trace->m_hit_entity && trace->m_plane.m_normal.z >= 0.7f ) {
			trace->m_fraction = fraction;
			trace->m_end_pos = end_pos;
			return;
		}

		mins = vec3_t( mins_src.x, std::max( 0.f, mins_src.y ), mins_src.z );
		maxs = vec3_t( std::min( 0.f, maxs_src.x ), maxs_src.y, maxs_src.z );

		try_touch_ground( start, end, mins, maxs, trace );
		if ( trace->m_hit_entity && trace->m_plane.m_normal.z >= 0.7f ) {
			trace->m_fraction = fraction;
			trace->m_end_pos = end_pos;
			return;
		}

		mins = vec3_t( std::max( 0.f, mins_src.x ), mins_src.y, mins_src.z );
		maxs = vec3_t( maxs_src.x, std::min( 0.f, maxs_src.y ), maxs_src.z );

		try_touch_ground( start, end, mins, maxs, trace );
		if ( trace->m_hit_entity && trace->m_plane.m_normal.z >= 0.7f ) {
			trace->m_fraction = fraction;
			trace->m_end_pos = end_pos;
			return;
		}

		trace->m_fraction = fraction;
		trace->m_end_pos = end_pos;
	}
}