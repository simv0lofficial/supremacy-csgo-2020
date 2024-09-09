#pragma once

namespace supremacy::hacks {
	class c_sim_context {
	public:
		void handle_context( extrapolation_data_t& data );
		void air_move( extrapolation_data_t& data );
		void try_touch_ground_in_quad( extrapolation_data_t& data, const vec3_t& start, const vec3_t& end, valve::trace_t* trace );
	protected:
		__forceinline void check_velocity( extrapolation_data_t& data ) const;
		__forceinline void trace_player_bbox(
			extrapolation_data_t& data, vec3_t& start, vec3_t& end, valve::trace_t* trace ) const;
		__forceinline void friction( extrapolation_data_t& data ) const;
		__forceinline void air_accelerate( extrapolation_data_t& data, vec3_t& wish_dir, float wish_spd ) const;
		__forceinline void try_player_move( extrapolation_data_t& data ) const;
		__forceinline void walk_move( extrapolation_data_t& data ) const;
		__forceinline void accelerate( extrapolation_data_t& data, vec3_t& wish_dir, float wish_spd, float accel ) const;
		__forceinline bool categorize_pos( extrapolation_data_t& data );
		__forceinline void try_touch_ground(
			const vec3_t& start, const vec3_t& end, const vec3_t& mins, const vec3_t& maxs, valve::trace_t* trace );
	};

	const auto g_sim_ctx = std::make_unique < c_sim_context >( );
}

#include "impl/skins.inl"